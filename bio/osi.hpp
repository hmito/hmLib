#ifndef HMLIB_BIO_OSI_INC
#define HMLIB_BIO_OSI_INC 100
#
#include<utility>
#include<vector>
#include<cmath>
#include<boost/optional.hpp>
#include"../tuple.hpp"
#include"../algorithm/sampling.hpp"
#include"../random.hpp"
#include"../exceptions.hpp"
#include"pairgame.hpp"
namespace hmLib {
	namespace bio {
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
						//if trial number is enough small, just try trial times.
						//	this is because approximation by normal distribution is less precious in small trial number.
						if (f <= 0) {
							//fail to invade
							for (unsigned int i = 0; i < trial; ++i) {
								dt += -std::log(hmLib::random::uniform_real(0.0, 1.0));
							}
						} else {
							for (unsigned int i = 0; i < trial; ++i) {
								//might success to invade (not always)
								dt += -std::log(hmLib::random::uniform_real(0.0, 1.0));
								if (f > hmLib::random::uniform_real(0.0, 1.0)) {
									Success = true;
									break;
								}
							}
						}
					} else {
						//aproximate by normal distribution
						if (f <= 0 || std::pow(1 - f, trial) > hmLib::random::uniform_real(0.0, 1.0)) {
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
			double dt;
			boost::optional<trait> branch;
			osi_step_result() = default;
			osi_step_result(bool success_, double dt_) :success(success_), dt(dt_), branch(boost::none) {}
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
			//return value is osi_step_result with the focal trait type
			//[fbeg,fend) and meanw is changed inside of the function
			template<typename strainfitness, typename mutate, typename trait_iterator, typename frac_iterator,typename URBG>
			auto operator()(strainfitness&& Fitness, mutate&& Mutate, trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend, double& meanw, URBG&& Engine) {
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
					auto sxitr = Sampler(Engine);

					//create mutant
					mutant = Mutate(*sxitr, Engine);
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
					double nmeanw = Fitness.solve(xbeg, xend, fbeg, fend);

					//calculate fitness for pre-resident
					double nw = Fitness(mutant, xbeg, xend, fbeg, fend);

					//reverse invasion is possible
					if (OSIPolicy.can_invade(nw, nmeanw)) {
						//swap trait value again (so mutant is now back to mutant)
						std::swap(*sxitr, mutant);
						meanw = Fitness.solve(xbeg, xend, fbeg, fend);
						return osi_step_result<decltype(mutant)>{ true, dt, mutant };
					} else {
						meanw = nmeanw;
						return osi_step_result<decltype(mutant)>{ true, dt };
					}
				}
				return osi_step_result<decltype(mutant)>{ false, dt };
			}
		};


		/*strain fitness 
		struct strainfitness_pattern{
			using trait_type = hoge;
		public:
			template<typename trait_iterator, typename frac_iterator>
			double solve(trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend);
			template<typename trait_iterator, typename frac_iterator>
			double operator()(const trait_type& x, trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend);
		};
		*/
		template<typename trait_type>
		struct osi_state {
			//trait, frequency, strain_no
			std::vector<std::tuple<trait_type, double, unsigned int>> strains;
			double meanw;
		};
		template<typename strainfitness_, typename mutate_, typename osi_policy_>
		struct osi_dsystem {
			using strainfitness = strainfitness_;
			using trait_type = typename strainfitness::trait_type;
			using mutate = mutate_;
			using osi_policy = osi_policy_;
			using state_type = osi_state<trait_type>;
			using this_type = osi_dsystem<strainfitness, mutate, osi_policy>;
			struct failtrial_breaker {
			private:
				const this_type& Ref;
			public:
				failtrial_breaker(const this_type& Ref_) :Ref(Ref_) {}
				bool operator()(const state_type&, double t)const {return Ref.FailTrial;}
			};
		private:
			strainfitness Fitness;
			mutate Mutate;
			osi_stepper<osi_policy_> Stepper;
			unsigned int StrainNo;
			double ThrFreq;
			bool FailTrial;
		public:
			osi_dsystem(strainfitness Fitness_, mutate Mutate_, osi_policy OSIPolicy_, unsigned int MaxTrial_ = 10000, double ThrFreq_ = 1e-6)
				: Fitness(std::move(Fitness_)), Mutate(std::move(Mutate_)), Stepper(OSIPolicy_, MaxTrial_), StrainNo(0), ThrFreq(ThrFreq_), FailTrial(false){
			}
			void operator()(state_type& s, double& t) {
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());

				auto Result = Stepper(Fitness, Mutate, xrange.begin(), xrange.end(), frange.begin(), frange.end(), s.meanw, hmLib::random::default_engine());
				t += Result.dt;
				FailTrial = !Result.success;

				//add new strain
				if (Result.branch) {
					s.strains.emplace_back(std::move(Result.branch).value(), ThrFreq*2, StrainNo++);
					auto xrange2 = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
					auto frange2 = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());

					s.meanw = Fitness.solve(xrange2.begin(), xrange2.end(), frange2.begin(), frange2.end());
				}

				//remove extinction
				auto ssend = std::remove_if(s.strains.begin(), s.strains.end(), [=](const auto& v) {return std::get<1>(v) < ThrFreq; });
				s.strains.erase(ssend, s.strains.end());
			}
			void reset() { Stepper.reset(); StrainNo = 0; }
			template<typename trait_iterator>
			state_type make_state(trait_iterator Beg, trait_iterator End) {
				state_type s;
				for (; Beg != End; ++Beg){
					s.strains.emplace_back(*Beg, 0.0, StrainNo++);
				}
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());
				s.meanw = Fitness.solve(xrange.begin(), xrange.end(), frange.begin(), frange.end());
				return s;
			}
			state_type make_state(trait_type Trait) {
				state_type s;
				s.strains.emplace_back(std::move(Trait), 1.0, StrainNo++);
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());
				s.meanw = Fitness.solve(xrange.begin(), xrange.end(), frange.begin(), frange.end());
				return s;
			}
			failtrial_breaker make_failtrial_breaker()const {return failtrial_breaker(*this);}
		};
	}
}
#
#endif
