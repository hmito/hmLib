#include<cmath>
#include<vector>
#include<iterator>
#include<algorithm>
#include<numeric>
#include<optional>
#include<utility>
#include"hmLib/mathbio/adaptive_dynamics.hpp"
#include"hmLib/ublas.hpp"
#include"hmLib/varray.hpp"
#include"hmLib/odeint.hpp"
#include"hmLib/recursive.hpp"
#include"hmLib/exceptions.hpp"
#include"hmLib/random.hpp"
#include"hmLib/algorithm.hpp"
#include"hmLib/math/root.hpp"
#include"hmLib/math/constants.hpp"
namespace hmLib {
	namespace mathbio {
		template<typename pair_game, typename trait_input_iterator, typename frequency_output_iterator>
		void pairgame_payoff_matrix(boost::numeric::ublas::matrix<double>& WMx, pair_game&& Game, trait_input_iterator xbeg, trait_input_iterator xend) {
			namespace ub = boost::numeric::ublas;
			unsigned int len = static_cast<unsigned int>(std::distance(xbeg, xend));

			boost::numeric::ublas::matrix<double> W(len, len);
			unsigned int ino = 0;
			for (auto xitr = xbeg; xitr != xend; ++xitr) {
				W(ino, ino) = Game(*xitr, *xitr).first;
				unsigned int jno = ino + 1;
				for (auto xitr2 = std::next(xitr); xitr2 != xend; ++xitr2) {
					auto val = Game(*xitr, *xitr2);
					W(ino, jno) = val.first;
					W(jno, ino) = val.second;
					++jno;
				}
				++ino;
			}
			return W;
		}
		/*	template<typename frequency_output_iterator>
			std::pair<bool, double> eqfreq_from_payoff_matrix(const boost::numeric::ublas::matrix<double>& WMx, frequency_output_iterator fOut) {
				namespace ub = boost::numeric::ublas;
				unsigned int len = static_cast<unsigned int>(std::distance(sBeg, sEnd));
				std::vector<unsigned int> exist(len);
				std::iota(exist.begin(), exist.end(), 0);

				while (snum > 1) {
					//calculate Payoff Matrix
					boost::numeric::ublas::matrix<double> W(exist.size(), exist.size());
					for (unsigned int i = 0; i < exist.size(); ++i) {
						for (unsigned int j = 0; j < exist.size(); ++j) {
							W(i, j) = WMx(exist[i], exist[j]);
						}
					}

					//calculate fitness from inverse matrix
					boost::numeric::ublas::matrix<double> IW;
					if (hmLib::ublas::invert(W, IW)) {
						for (unsigned int i = 0; i < snum; ++i) {
							W(i, i) *= 0.99;
						}
					}
					boost::numeric::ublas::vector<double> Freq = boost::numeric::ublas::prod(IW, boost::numeric::ublas::vector<double>(snum, 1));
					Freq /= std::accumulate(Freq.begin(), Freq.end(), 0.0);

					unsigned int min_pos = snum;
					double MinFreq = 1.0;
					for (unsigned int i = 0; i < snum; ++i) {
						if (Freq[i] < MinFreq) {
							MinFreq = Freq[i];
							min_pos = i;
						}
					}

					//exctinction not occur
					if (MinFreq >= 0.0) {
						auto ieitr = ExtinctSet.begin();
						unsigned int ino = 0;
						for (auto sItr = sBeg; sItr != sEnd; ++sItr) {
							if (ieitr != ExtinctSet.end() && sItr == *ieitr) {
								*(fOut++) = 0.0;
								++ieitr;
							} else {
								*(fOut++) = Freq[ino];
								++ino;
							}
						}
						return fOut;
					}

					//exctinction occur
					auto ieitr = ExtinctSet.begin();
					unsigned int ino = 0;
					for (auto sItr = sBeg; sItr != sEnd; ++sItr) {
						if (ieitr != ExtinctSet.end() && sItr == *ieitr) {
							++ieitr;
						} else {
							if (ino == min_pos) {
								ExtinctSet.insert(ieitr, sItr);
								snum = len - static_cast<unsigned int>(ExtinctSet.size());
								break;
							}
							++ino;
						}
					}
					{
						unsigned int ino = 0;
						auto ieitr = ExtinctSet.begin();
						for (auto sItr = sBeg; sItr != sEnd; ++sItr) {
							if (ieitr != ExtinctSet.end() && sItr == *ieitr) {
								++ieitr;
								continue;
							}
							W(ino, ino) = Game(*sItr, *sItr).first;
							unsigned int jno = ino + 1;
							auto jeitr = ieitr;
							for (auto sItr2 = std::next(sItr); sItr2 != sEnd; ++sItr2) {
								if (jeitr != ExtinctSet.end() && sItr2 == *jeitr) {
									++jeitr;
									continue;
								}
								auto val = Game(*sItr, *sItr2);
								W(ino, jno) = val.first;
								W(jno, ino) = val.second;
								++jno;
							}
							++ino;
						}
					}
				}

				auto eitr = ExtinctSet.begin();
				unsigned int ino = 0;
				for (auto sItr = sBeg; sItr != sEnd; ++sItr) {
					if (eitr != ExtinctSet.end() && sItr == *eitr) {
						*(fOut++) = 0.0;
						++eitr;
					} else {
						*(fOut++) = 1.0;
						++ino;
					}
				}

				return fOut;
			}
		*/
	}
}
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

			//return type of invasion policy
			template<typename trait>
			struct invasion_result {
				bool success;
				double dt;
				std::optional<trait> branch;
				invasion_result() = default;
				invasion_result(bool success_, double dt_) :success(success_), dt(dt_), branch(std::nullopt) {}
				invasion_result(bool success_, double dt_, trait x) :success(success_), dt(dt_), branch(std::move(x)) {}
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

		template<typename osi_policy_>
		struct osi_solver {
			using osi_policy = osi_policy_;	//doubel operator()(double w, double meanw);	return invasion probability
		private:
			osi_policy OSIPolicy;
			unsigned int MaxMutationTrial;
		public:
			osi_solver(osi_policy OSIPolicy_, unsigned int MaxMutationTrial_ = 10000)
				: OSIPolicy(std::move(OSIPolicy_)), MaxMutationTrial(MaxMutationTrial_) {
			}
			void reset() { OSIPolicy.reset(); }
			template<typename strainfitness, typename mutate, typename trait_iterator, typename frac_iterator>
			auto operator()(strainfitness&& Fitness, mutate&& Mutate, trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend, double& meanw) {
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
						return osi::invasion_result<decltype(mutant)>{ true, dt, mutant };
					} else {
						return osi::invasion_result<decltype(mutant)>{ true, dt };
					}
				}
				return osi::invasion_result<decltype(mutant)>{ false, dt };
			}
		};

		template<typename pair_game_>
		struct pairgame_strainfitness {
			using game = pair_game_;
			using trait_type = typename game::trait_type;
		private:
			game Game;
		public:
			explicit pairgame_strainfitness(game Game_) :Game(std::move(Game_)) {}
			template<typename trait_iterator, typename frac_iterator>
			std::optional<double> solve(trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend) {
				if (hmLib::mathbio::AD::solveEqFreq_by_matrix(Game, xbeg, xend, fbeg))return std::nullopt;
				return std::optional<double>(operator()(xbeg, xend, fbeg, fend));
			}
			template<typename trait_iterator, typename frac_iterator>
			double operator()(const trait_type& x, trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend) {
				double w = 0.0;
				double fsum = 0.0;
				for (; fbeg != fend; ++fbeg, ++xbeg) {
					w += *fbeg * Game(x, *xbeg).first;
					fsum += *fbeg;
				}
				return w / fsum;
			}
			template<typename trait_iterator, typename frac_iterator>
			double operator()(trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend) {
				double meanw = 0.0;
				double fsum = 0.0;
				auto xitr1 = xbeg;
				for (auto fitr1 = fbeg; fitr1 != fend; ++fitr1, ++xitr1) {
					auto xitr2 = xbeg;
					for (auto fitr2 = fbeg; fitr2 != fend; ++fitr2, ++xitr2) {
						meanw += *fitr1 * *fitr2 * Game(*xitr1, *xitr2).first;
						fsum += *fitr1 * *fitr2;
					}
				}
				return meanw / fsum;
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
			//using osi_policy = osi::clt_osi_policy<osi::birth_exp_invasion_fitness>;
		private:
			strainfitness Fitness;
			mutate Mutate;
			osi_solver<osi_policy_> Solver;
			double Threshold;
		public:
			pairgame_osi_dsystem(pair_game Game_, mutate Mutate_, osi_policy OSIPolicy_, double Threshold_ = 1e-6)
				: Fitness(std::move(Game_)), Mutate(std::move(Mutate_)), Solver(OSIPolicy_), Threshold(Threshold_) {
			}
			void operator()(state_type& s, double& t) {
				auto xrange = hmLib::make_get_range<0>(s.strains.begin(), s.strains.end());
				auto frange = hmLib::make_get_range<1>(s.strains.begin(), s.strains.end());
				auto Result = Solver(Fitness, Mutate, xrange.begin(), xrange.end(), frange.begin(), frange.end(), s.meanw);

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