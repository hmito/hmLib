#ifndef HMLIB_BIO_PAIRGAME_INC
#define HMLIB_BIO_PAIRGAME_INC 100
#
#include<iterator>
#include<utility>
#include<numeric>
#include<vector>
#include<optional>
#include<boost/numeric/odeint.hpp>
#include<boost/numeric/ublas/matrix.hpp>
#include"../exceptions/exception.hpp"
#include"../ublas.hpp"
#include"../odeint/validate.hpp"
#include"../odeint/integrate.hpp"
#include"../odeint/breaker/overflow_breaker.hpp"
namespace hmLib {
	namespace bio {
		/*
		struct pair_game_pattern{
			//payoff of pair game between x1 and x2 (first: for x1, second: for x2)
			std::pair<double,double> operator()(trait x1, trait x2);
		};
		*/
		namespace pairgame {
			using payoff_matrix = boost::numeric::ublas::matrix<double>;
			using freq_vector = boost::numeric::ublas::vector < double>;

			template<typename pair_game, typename trait_input_iterator>
			void calc_payoff_matrix(payoff_matrix& Mx, pair_game&& Game, trait_input_iterator sBeg, trait_input_iterator sEnd) {
				unsigned int len = static_cast<unsigned int>(std::distance(sBeg, sEnd));
				//resize when matrix size is different from number of strains
				if (len != Mx.size1() || len != Mx.size2()) {
					Mx.resize(len, len, false);
				}

				unsigned int ix = 0;
				for (auto sItr1 = sBeg; sItr1 != sEnd; ++sItr1, ++ix) {
					Mx(ix,ix) = Game(*sItr1, *sItr1).first;
					unsigned int iy = ix + 1;
					for (auto sItr2 = std::next(sItr1); sItr2 != sEnd; ++sItr2, ++iy) {
						auto Pair = Game(*sItr1, *sItr2);
						Mx(ix, iy) = Pair.first;
						Mx(iy, ix) = Pair.second;
					}
				}
			}
			//return mean payoff if success to calculate fitness, none if fail to calculate inverse matrix
			//ignore even if derived fraction is negative
			std::optional<double> solve_freq_by_matrix(const payoff_matrix& Mx, freq_vector& Freq) {
				//check monomorphic case
				if (Mx.size1() == 1) {
					Freq = boost::numeric::ublas::vector<double>(1, 1.0);
					return Mx(0, 0);
				}

				//calculate fitness from inverse matrix
				boost::numeric::ublas::matrix<double> IW;
				if (hmLib::ublas::invert(Mx, IW)) {
					//fail to calculate inverce matrix
					return std::nullopt;
				}
				Freq = boost::numeric::ublas::prod(IW, boost::numeric::ublas::vector<double>(Mx.size1(), 1.0));

				double meanw = 1.0/std::accumulate(Freq.begin(), Freq.end(), 0.0);
				Freq *= meanw;
				return meanw;
			}
			//return mean payoff if success to calculate fitness, none if multiple extinction occur or fail to calculate inverse matrix
			//fraction is kept larger than MinFreq
			//Recursive flag allow extinction of multiple strains
			//Note: recursive = true case might sometimes cause unnecessary extinction.
			std::optional<double> solve_freq_by_matrix(const payoff_matrix& Mx, freq_vector& Freq, double MinFreq, bool Recursive = false) {
				std::vector<unsigned int> Exist;
				{
					auto Ans = solve_freq_by_matrix(Mx, Freq);
					if (!Ans)return std::nullopt;

					auto MinItr = std::min_element(Freq.begin(), Freq.end());
					if (*MinItr >= MinFreq) {
						return Ans;
					}

					//initialize Exist vector
					Exist.assign(Freq.size(), 0);
					std::iota(Exist.begin(), Exist.end(), 0);
					//remove minimum strain
					Exist[std::distance(Freq.begin(), MinItr)] = Exist.back();
					Exist.pop_back();
				}

				while (Exist.size() > 1) {
					//calculate subset of Payoff Matrix
					payoff_matrix SubMx(Exist.size(), Exist.size());

					for (unsigned int i = 0; i < Exist.size(); ++i) {
						SubMx(i, i) = Mx(Exist[i], Exist[i]);

						for (unsigned int j = i+1; j < Exist.size(); ++j) {
							SubMx(i, j) = Mx(Exist[i], Exist[j]);
							SubMx(j, i) = Mx(Exist[j], Exist[i]);
						}
					}

					freq_vector SubFreq(SubMx.size1(), 0.0);
					auto Ans = solve_freq_by_matrix(SubMx, SubFreq);
					if (!Ans)return std::nullopt;

					auto MinItr = std::min_element(SubFreq.begin(), SubFreq.end());
					if (*MinItr >= MinFreq) {
						Freq.resize(Mx.size1(), false);
						std::fill(Freq.begin(), Freq.end(), 0.0);

						for (unsigned int i = 0; i < Exist.size();++i) {
							Freq[Exist[i]] = SubFreq[i];
						}

						return Ans;
					}

					//multiple strain extinction
					if (!Recursive)return std::nullopt;

					//remove minimum strain
					Exist[std::distance(SubFreq.begin(), MinItr)] = Exist.back();
					Exist.pop_back();
				}

				//monomorphic
				Freq.resize(Mx.size1(), false);
				std::fill(Freq.begin(), Freq.end(), 0.0);
				Freq[Exist[0]] = 1.0;
				return Mx(Exist[0], Exist[0]);
			}

			namespace detail {
				struct freq_csys {
					using state_type = freq_vector;
					using time_type = double;
					using validate_result = hmLib::odeint::validate_result;
				private:
					const payoff_matrix& Mx;
					double ThrFreq;
				public:
					freq_csys(const payoff_matrix& Mx_,double ThrFreq_) :Mx(Mx_), ThrFreq(ThrFreq_){}
					void operator()(const state_type& f, state_type& df, time_type t) {
						state_type w = boost::numeric::ublas::prod(Mx, f);
						double sumf = std::accumulate(f.begin(), f.end(), 0.0);
						double meanw = 0.0;
						for (unsigned int i = 0; i < f.size(); ++i) {
							meanw += w[i] * f[i] / sumf;
						}

						df = state_type(f.size(), 0.0);
						for (unsigned int i = 0; i < f.size(); ++i) {
							df[i] = (w[i] - meanw) * f[i] / sumf;
						}
					}
					bool is_invalid_step(const state_type& x, time_type t) {
						return std::any_of(x.begin(), x.end(), [=](double v) {return v < 0.0; });
					}
					validate_result validate(const state_type& x, time_type t, state_type& nx) {
						if (std::any_of(x.begin(), x.end(), [=](double v) {return v < ThrFreq; })) {
							nx = x;
							for (auto& v : nx) {
								if (v < ThrFreq)v = 0.0;
							}
							nx /= std::accumulate(nx.begin(), nx.end(), 0.0);
							return validate_result::assigned;
						}
						if (std::abs(std::accumulate(x.begin(), x.end(), 0.0) - 1.0) > 0.001) {
							nx = x;
							nx /= std::accumulate(nx.begin(), nx.end(), 0.0);
							return validate_result::assigned;
						}
						return validate_result::none;
					}
					validate_result validate(const state_type& x1, const state_type& x2, time_type t, state_type& nx) {
						nx = x2;
						for (auto& v : nx) {
							if (v < ThrFreq)v = 0.0;
						}
						nx /= std::accumulate(nx.begin(), nx.end(), 0.0);
						return validate_result::assigned;
					}
				};
			}
			double solve_freq_by_ode(const payoff_matrix& Mx, freq_vector& Freq, size_t stepnum, double ThrFreq){
				if (Freq.size() != Mx.size1()) {
					Freq = freq_vector(Mx.size1(), 1.0 / Mx.size1());
				}

				auto Stepper = hmLib::odeint::make_step_validate<void>(1e-2, ThrFreq/10, 1e-2, 100, boost::numeric::odeint::make_dense_output(ThrFreq/100, 1e-3, boost::numeric::odeint::runge_kutta_dopri5<freq_vector>()));

				detail::freq_csys Sys(Mx,ThrFreq);

				hmLib::odeint::time_overflow_breaker<> Brk(1e50);
				hmLib::odeint::breakable_integrate_adaptive_n(Stepper, Sys, Freq, 0.0, 0.001, stepnum, Brk);

				for (auto& v : Freq) {
					if (v < ThrFreq)v = 0.0;
				}
				Freq /= std::accumulate(Freq.begin(), Freq.end(), 0.0);

				freq_vector w = boost::numeric::ublas::prod(Mx, Freq);
				double meanw = 0.0;
				for (unsigned int i = 0; i < w.size(); ++i) {
					meanw += w[i] * Freq[i];
				}
				return meanw;
			}
		}

		template<typename pair_game_>
		struct pairgame_strainfitness {
			using game = pair_game_;
			using trait_type = typename game::trait_type;
		private:
			game Game;
			double ThrFreq;
			unsigned int StepNum;
		public:
			explicit pairgame_strainfitness(game Game_, double ThrFreq_, unsigned int TryStepNum_)
				:Game(std::move(Game_)), ThrFreq(ThrFreq_), StepNum(TryStepNum_){
			}
			template<typename trait_iterator, typename frac_iterator>
			double solve(trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend) {
				pairgame::freq_vector Freq;
				pairgame::payoff_matrix Mx;
				pairgame::calc_payoff_matrix(Mx, Game, xbeg, xend);
				auto Result = pairgame::solve_freq_by_matrix(Mx, Freq);			
				if (Result && std::all_of(Freq.begin(), Freq.end(), [](double v) {return v >= 0.0; })) {
					std::copy(Freq.begin(), Freq.end(), fbeg);
					return *Result;
				}// else {
				//	hmLib_throw(hmLib::exceptions::exception,"fail to calc inverse matrix");
				//}
				
				Freq.resize(Mx.size1(), false);
				std::copy(fbeg, fend, Freq.begin());
				double meanw = pairgame::solve_freq_by_ode(Mx, Freq, StepNum, ThrFreq);
				std::copy(Freq.begin(), Freq.end(), fbeg);
				return meanw;
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
		};
	}
}
#
#endif
