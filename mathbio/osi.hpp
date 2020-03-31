#ifndef HMLIB_MATHBIO_OSI_INC
#define HMLIB_MATHBIO_OSI_INC 100
#
#include<utility>
#include<vector>
#include<cmath>
#include<optional>
#include"../tuple.hpp"
#include"../algorithm/sampling.hpp"
#include"../random.hpp"
#include"../exceptions.hpp"
#include"pairgame.hpp"
namespace hmLib {
	namespace mathbio {
		namespace osi {
			// birth rate: b = rate*(w/meanw)
			// daeth rate: d = rate
			// w > 0 and meanw > 0 are requried.
			struct birth_ratio_invasion_fitness {
				// birth rate: b = rate*(w/meanw)
				// daeth rate: d = rate
				// mutation rate: m = mutation_frequency*birth_rate = TimeStep
				// invasion rate: f = (b - d)/b = 1 - meanw/w
				//Note: w and meanw should be positive
			public:
				double operator()(double w, double meanw)const {
					return 1 - meanw / w;
				}
			};

			// birth rate: b = rate*exp(alpha*(w-meanw))
			// daeth rate: d = rate
			// w and meanw can be negative.
			struct birth_exp_invasion_fitness {
				// birth rate: b = rate*exp(alpha*(w-meanw))
				// daeth rate: d = rate
				// mutation rate: m = mutation_frequency*birth_rate = TimeStep
				// invasion rate: f = (b - d)/b = 1 - exp(-alpha*(w-meanw))
				// alpha determines the strength of the selection.
				//Note: w and/or meanw can be negative
			private:
				double alpha;
			public:
				birth_exp_invasion_fitness()noexcept : alpha(1.0) {}
				explicit birth_exp_invasion_fitness(double alpha_)noexcept : alpha(alpha_) {}
				double operator()(double w, double meanw)const {
					return 1 - std::exp(-alpha * (w - meanw));
				}
			};

			// invasion control (strict trial)
			template<typename invasion_fitness>
			struct simple_osi_policy {
			private:
				invasion_fitness Invasion;
			public:
				simple_osi_policy(invasion_fitness Invasion_ = invasion_fitness()) :Invasion(std::move(Invasion_)) {}
				void reset() {}
				std::pair<bool, double> try_invade(double w, double meanw) {
					double f = Invasion(w, meanw);
					return std::make_pair(f < hmLib::random::uniform_real(0.0, 1.0), -std::log(hmLib::random::uniform_real(0.0, 1.0)));
				}
				bool can_invade(double w, double meanw) { return Invasion(w, meanw) > 0; }
			};

			// invasion control (approximate by normal distribution based on central limit theorem)
			template<typename invasion_fitness>
			struct clt_osi_policy {
				//average time consumption invasion trial 
				//sum of log(uniform_real(0.0,1.0)) for N times seems to be apploximated by a normal distribution with
				//	mean := -N
				//	var  :=  N
				//i.e., hmLib::random::normal(-1.0*N, std::sqrt(N))
			private:
				invasion_fitness Invasion;
				double target;	//[0:1]: target "mean probability" that an optimal mutant can successfully invade.
				double decay;	//[0:1]: reduction rate of the old data (1: no reduction, 0: forget immidiately).
				double overrate;//[0: ]: rate of the over-estimation of the maximum probability from current probability. (0: no over-estimation 1: twice as large as f can appear)
				unsigned int max_trial;
			private:
				double maxf;
			public:
				clt_osi_policy(double target_, double decay_, double overrate_, unsigned int max_trial_ = 5u, invasion_fitness Invasion_ = invasion_fitness())
					: Invasion(std::move(Invasion_)), target(target_), decay(decay_), overrate(overrate_), max_trial(max_trial_), maxf(target) {
				}
				void reset() { maxf = target; }
				std::pair<bool, double> try_invade(double w, double meanw) {
					double f = Invasion(w, meanw);

					unsigned int trial = std::max<unsigned int>(1, static_cast<unsigned int>(std::floor(std::log(1 - target) / std::log(1 - maxf))));
					maxf = std::max(f * (1.0 + overrate), (1.0 - decay) * maxf);

					bool Success = false;
					double dt = 0;

					//Success of invasion flag
					if (trial < max_trial) {
						//if trial number is enough small, just try 5 times.
						//	this is because approximation by normal distribution is less precious in small trial number.
						if (f <= 0) {
							for (unsigned int i = 0; i < trial; ++i) {
								dt += -std::log(hmLib::random::uniform_real(0.0, 1.0));
							}
						} else {
							for (unsigned int i = 0; i < trial; ++i) {
								dt += -std::log(hmLib::random::uniform_real(0.0, 1.0));
								if (f > hmLib::random::uniform_real(0.0, 1.0)) {
									Success = true;
									break;
								}
							}
						}
					} else {
						//aproximate by normal distribution
						if (f < 0 || std::pow(1 - f, trial) > hmLib::random::uniform_real(0.0, 1.0)) {
							//fail to invade
							do {
								dt = -hmLib::random::normal(-1.0 * trial, std::sqrt(trial));
							} while (dt <= 0);
						} else {
							//success to invade
							Success = true;

							//check mean time for invasion
							//	mean trial = 1*p + 2*p*(1-p)+ ... N*p*(1-p)^(N-1) with N = invasion_trial
							//             = ( (1 - (1-p)^N)/p - N*(1-p)^N )/( 1 - (1-p)^N )
							double miss_prob = std::pow(1 - f, trial);
							double mean_trial = ((1 - miss_prob) / f - trial * miss_prob) / (1 - miss_prob);

							//hmLib::random::normal(-1.0*N, std::sqrt(N))
							do {
								dt = -hmLib::random::normal(-mean_trial, std::sqrt(mean_trial));
							} while (dt <= 0);
						}
					}
					return std::make_pair(Success, dt);
				}
				bool can_invade(double w, double meanw) { return Invasion(w, meanw) > 0; }
			};
		}

		//return type of invasion policy
		template<typename trait>
		struct osi_step_result {
			bool success;
			double meanw;
			double dt;
			std::optional<trait> branch;
			osi_step_result() = default;
			osi_step_result(bool success_, double dt_) :success(success_), dt(dt_), branch(std::nullopt) {}
			osi_step_result(bool success_, double dt_, trait x) :success(success_), dt(dt_), branch(std::move(x)) {}
		};

		template<typename osi_policy_>
		struct osi_stepper {
			using osi_policy = osi_policy_;	//control invasion trial
		private:
			osi_policy OSIPolicy;
			unsigned int MaxMutationTrial;
		public:
			osi_stepper(osi_policy OSIPolicy_, unsigned int MaxMutationTrial_ = 10000)
				: OSIPolicy(std::move(OSIPolicy_)), MaxMutationTrial(MaxMutationTrial_) {
			}
			void reset() { OSIPolicy.reset(); }
			template<typename strainfitness, typename mutate, typename trait_iterator, typename frac_iterator>
			osi_step_result<decltype(*std::declval<trait_iterator>())> operator()(strainfitness&& Fitness, mutate&& Mutate, trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend, double& meanw) {
				hmLib_assert(std::distance(xbeg, xend) == std::distance(fbeg, fend), hmLib::numeric_exceptions::incorrect_arithmetic_request, "distance of two iterator pairs are different.");

				//make sampler following frequency of each strain
				auto Sampler = hmLib::make_roulette_sampler(xbeg, xend, fbeg, fend);

				//consumed time
				double dt = 0.0;

				//create trait variable for mutant
				auto mutant = (*xbeg);

				//try to find successful invasion
				for (unsigned int MTrial = 0; MTrial < MaxMutationTrial; ++MTrial) {
					//select strain producing mutant
					auto sxitr = Sampler(hmLib::random::default_engine());

					//create mutant
					Mutate(*sxitr, mutant);
					double w = Fitness(mutant, xbeg, xend, fbeg, fend);

					//calc current trial num with updating maxf
					//  std::tuple<bool[Success], double[dt]>
					auto Invasion = OSIPolicy.try_invade(w, meanw);
					dt += std::get<1>(Invasion);

					//if fail to invade, just continue loop.
					if (!std::get<0>(Invasion))continue;

					//swap trait value
					std::swap(*sxitr, mutant);

					//Update fraction with mutant
					auto optmeanw = Fitness.solve(xbeg, xend, fbeg, fend);
					if (!optmeanw.has_value())continue;
					meanw = *optmeanw;

					//calculate fitness for pre-resident
					w = Fitness(mutant, xbeg, xend, fbeg, fend);

					//reverse invasion is possible
					if (OSIPolicy.can_invade(w, meanw)) {
						//swap trait value again (so mutant is now back to mutant)
						std::swap(*sxitr, mutant);
						meanw = (Fitness.solve(xbeg, xend, fbeg, fend)).value();
						return osi_step_result<decltype(mutant)>{ true, dt, mutant };
					} else {
						return osi_step_result<decltype(mutant)>{ true, dt };
					}
				}
				return osi_step_result<decltype(mutant)>{ false, dt };
			}
		};

		template<typename pair_game_, typename mutate_, typename osi_policy_>
		struct pairgame_osi_dsystem {
			using pair_game = pair_game_;
			using strainfitness = pairgame_strainfitness<pair_game>;
			using trait_type = typename pair_game::trait_type;
			using mutate = mutate_;
			using osi_policy = osi_policy_;
			struct state_type {
				std::vector<std::pair<trait_type, double>> strains;
				double meanw;
			};
		private:
			strainfitness Fitness;
			mutate Mutate;
			osi_stepper<osi_policy_> Stepper;
			double Threshold;
		public:
			pairgame_osi_dsystem(pair_game Game_, mutate Mutate_, osi_policy OSIPolicy_, double Threshold_ = 1e-6)
				: Fitness(std::move(Game_)), Mutate(std::move(Mutate_)), Stepper(OSIPolicy_), Threshold(Threshold_) {
			}
			void operator()(state_type& s, double& t) {
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());
				auto Result = Stepper(Fitness, Mutate, xrange.begin(), xrange.end(), frange.begin(), frange.end(), s.meanw);

				if (!Result.success) {
					s.meanw = (Fitness.solve(xrange.begin(), xrange.end(), frange.begin(), frange.end())).value();
				}

				t += Result.dt;

				//add new strain
				if (Result.branch) {
					s.strains.emplace_back(std::move(Result.branch).value(), Threshold);
					auto xrange2 = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
					auto frange2 = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());

					auto optmeanw = Fitness.solve(xrange2.begin(), xrange2.end(), frange2.begin(), frange2.end());
					if (!optmeanw.has_value()) {
						s.strains.pop_back();
					} else {
						s.meanw = *optmeanw;
					}
				}

				//remove extinction
				auto ssend = std::remove_if(s.strains.begin(), s.strains.end(), [=](const auto& v) {return v.second < Threshold; });
				s.strains.erase(ssend, s.strains.end());
			}
			void reset() { Solver.reset(); }
			void initialize_state(state_type& s, bool solve_fraction = false) {
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());
				if (solve_fraction) {
					s.meanw = (Fitness.solve(xrange.begin(), xrange.end(), frange.begin(), frange.end())).value();
				} else {
					s.meanw = Fitness(xrange.begin(), xrange.end(), frange.begin(), frange.end());
				}
			}
		};
	}
}
#
#endif