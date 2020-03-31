#ifndef HMLIB_MATHBIO_PAIRGAME_INC
#define HMLIB_MATHBIO_PAIRGAME_INC 100
#
#include<iterator>
#include<utility>
#include<numeric>
#include<vector>
#include<boost/numeric/odeint.hpp>
#include<boost/numeric/ublas/matrix.hpp>
#include<boost/optional.hpp>
#include"../ublas.hpp"
namespace hmLib {
	namespace mathbio {
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
			//return mean payoff if success to calculate fitness, none if extinction occur or fail to calculate inverse matrix
			template<typename frequency_output_iterator>
			boost::optional<double> solve_freq_by_matrix(const payoff_matrix& Mx, freq_vector& Freq) {
				//check monomorphic case
				if (Mx.size1() == 1) {
					Freq = boost::numeric::ublas::vector<double>(1, 1.0);
					return Mx(0, 0);
				}

				//calculate fitness from inverse matrix
				boost::numeric::ublas::matrix<double> IW;
				if (hmLib::ublas::invert(Mx, IW)) {
					//fail to calculate inverce matrix
					return boost::none;
				}
				Freq = boost::numeric::ublas::prod(IW, boost::numeric::ublas::vector<double>(Mx.size1(), 1.0));

				double meanw = std::accumulate(Freq.begin(), Freq.end(), 0.0);
				Freq /= meanw;
				return meanw;
			}
			template<typename frequency_output_iterator>
			boost::optional<double> solve_freq_by_matrix(const payoff_matrix& Mx, freq_vector& Freq, double MinFreq, bool Recursive = false) {
				std::vector<unsigned int> Exist;
				{
					auto Ans = solve_freq_by_matrix(Mx, Freq);
					if (!Ans)return boost::none;

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
							SubMx(i, j) = Mx(Exist[j], Exist[i]);
						}
					}

					freq_vector SubFreq(SubMx.size1(), 0.0);
					auto Ans = solve_freq_by_matrix(SubMx, SubFreq);
					if (!Ans)return boost::none;

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
					if (!Recursive)return boost::none;

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
			template<typename pair_game, typename trait_input_iterator, typename frequency_forward_iterator>
			double solve_freq_by_ode(const payoff_matrix& Mx, freq_vector& Freq){

			}
		}

		template<typename pair_game_>
		struct pairgame_strainfitness {
			using game = pair_game_;
			using trait_type = typename game::trait_type;
		private:
			game Game;
		public:
			explicit pairgame_strainfitness(game Game_) :Game(std::move(Game_)) {}
			template<typename trait_iterator, typename frac_iterator>
			double solve(trait_iterator xbeg, trait_iterator xend, frac_iterator fbeg, frac_iterator fend) {
				auto Result = hmLib::mathbio::pairgame::solve_freq_by_matrix(Game, xbeg, xend, fbeg);
				if(!std::get<0>(Result)) {
					//success to calculate fraction from matrix
					return std::get<1>(Result);
				}
				return hmLib::mathbio::pairgame::solve_freq_by_ode(Game, xbeg, xend, fbeg);
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
	}
}
#
#endif
