#ifndef HMLIB_MATHBIO_ADAPTIVEDYNAMICS_INC
#define HMLIB_MATHBIO_ADAPTIVEDYNAMICS_INC 100
#
#include<cmath>
#include<vector>
#include<utility>
#include<tuple>
#include<iterator>
#include<algorithm>
#include<numeric>
#include<utility>
#include<boost/numeric/ublas/matrix.hpp>
#include"../tuple.hpp"
#include"../algorithm.hpp"
#include"../exceptions.hpp"
#include"../ublas.hpp"
#include"../math/root.hpp"
#include"../math/constants.hpp"
#include"../math/differentiation.hpp"
namespace hmLib {
	/*
	=== pair game concept ===
	struct hoge_game{
		//define trait type
		using trait_type = ???;

		//return pair of payoff of the game between player p1 and player p2.
		std::pair<double,double> operator()(const trait_type& p1, const trait_type& p2);
	};
	*/
	namespace mathbio {
		namespace AD {
			template<typename pair_game, typename trait_input_iterator, typename frequency_output_iterator>
			frequency_output_iterator solveEqFreq_by_matrix(pair_game&& Game, trait_input_iterator sBeg, trait_input_iterator sEnd, frequency_output_iterator fOut) {
				namespace ub = boost::numeric::ublas;
				unsigned int len = static_cast<unsigned int>(std::distance(sBeg, sEnd));
				unsigned int snum = len;
				std::vector<trait_input_iterator> ExtinctSet;

				while(snum > 1) {
					//calculate Payoff Matrix
					boost::numeric::ublas::matrix<double> W(snum, snum);
					{
						unsigned int ino = 0;
						auto ieitr = ExtinctSet.begin();
						for(auto sItr = sBeg; sItr != sEnd; ++sItr) {
							if(ieitr != ExtinctSet.end() && sItr == *ieitr) {
								++ieitr;
								continue;
							}
							W(ino, ino) = Game(*sItr, *sItr).first;
							unsigned int jno = ino + 1;
							auto jeitr = ieitr;
							for(auto sItr2 = std::next(sItr); sItr2 != sEnd; ++sItr2) {
								if(jeitr != ExtinctSet.end() && sItr2 == *jeitr) {
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
					//calculate fitness from inverse matrix
					boost::numeric::ublas::matrix<double> IW;
					while(hmLib::ublas::invert(W, IW)) {
						for(unsigned int i = 0; i < snum; ++i) {
							W(i, i) *= 0.99;
						}
					}
					boost::numeric::ublas::vector<double> Freq = boost::numeric::ublas::prod(IW, boost::numeric::ublas::vector<double>(snum, 1));
					Freq /= std::accumulate(Freq.begin(), Freq.end(), 0.0);

					unsigned int min_pos = snum;
					double MinFreq = 1.0;
					for(unsigned int i = 0; i < snum; ++i) {
						if(Freq[i] < MinFreq) {
							MinFreq = Freq[i];
							min_pos = i;
						}
					}

					//exctinction not occur
					if(MinFreq >= 0.0) {
						auto ieitr = ExtinctSet.begin();
						unsigned int ino = 0;
						for(auto sItr = sBeg; sItr != sEnd; ++sItr) {
							if(ieitr != ExtinctSet.end() && sItr == *ieitr) {
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
					for(auto sItr = sBeg; sItr != sEnd; ++sItr) {
						if(ieitr != ExtinctSet.end() && sItr == *ieitr) {
							++ieitr;
						} else {
							if(ino == min_pos) {
								ExtinctSet.insert(ieitr, sItr);
								snum = len - static_cast<unsigned int>(ExtinctSet.size());
								break;
							}
							++ino;
						}
					}
				}

				auto eitr = ExtinctSet.begin();
				unsigned int ino = 0;
				for(auto sItr = sBeg; sItr != sEnd; ++sItr) {
					if(eitr != ExtinctSet.end() && sItr == *eitr) {
						*(fOut++) = 0.0;
						++eitr;
					} else {
						*(fOut++) = 1.0;
						++ino;
					}
				}

				return fOut;
			}

			template<typename pair_game_>
			struct circle_step_branch_dsystem {
				using pair_game = pair_game_;
				using trait_type = typename pair_game::trait_type;
				using state_type = std::vector < std::pair < double, trait_type> >;
			private:
				pair_game Game;
				double absstep;
				double relstep;
			public:
				circle_step_branch_dsystem(pair_game Game_, double absstep_, double relstep_):Game(std::move(Game_)), absstep(absstep_), relstep(relstep_) {}
				void operator()(state_type& s, unsigned int n)const {
					unsigned int ssize = static_cast<unsigned int>(s.size());

					//define plane_func_type for derivatives
					auto fnW = [&Game = this->Game, &s = s](double x, double y) {
						auto FreqRng = hmLib::make_get_range<0>(s.begin(), s.end());
						auto TraitRng = hmLib::make_get_range<1>(s.begin(), s.end());

						double W = 0.0;
						auto fBeg = FreqRng.begin();
						auto sBeg = TraitRng.begin();
						auto sEnd = TraitRng.end();
						for(; sBeg != sEnd; ++sBeg, ++fBeg) {
							W += *fBeg * Game(trait_type{ x,y }, *sBeg).first;
						}
						return W;
					};

					//update state based on the 2nd order selection gradient
					state_type ns;
					std::vector<trait_type> ndir;
					double maxrate = std::numeric_limits<double>::min();
					for(unsigned int i = 0; i < ssize; ++i) {
						double xstep = absstep + (relstep * (s[i].second)[0]);
						double ystep = absstep + (relstep * (s[i].second)[1]);
						double wx = 0;
						double wy = 0;
						double wxx = 0;
						double wxy = 0;
						double wyy = 0;
						hmLib::math::plane_derivative(fnW, (s[i].second)[0], (s[i].second)[1], absstep, relstep, wx, wy, wxx, wxy, wyy);

						auto fn = [=](double theta) {
							return -xstep * wx * std::sin(theta) + ystep * wy * std::cos(theta)
								+ ((ystep * ystep * wyy - xstep * xstep * wxx) / 2 * std::sin(2 * theta) + xstep * ystep * wxy * std::cos(2 * theta));
						};
						std::vector<double> theta;
						theta.reserve(2);
						hmLib::math::stable_root_toms748(fn, 0.0, 2 * hmLib::math::pi, 2 * hmLib::math::pi / 72, 2 * hmLib::math::pi / 7200, std::back_inserter(theta));
						hmLib_assert(theta.size() <= 2, hmLib::exceptions::numeric::incorrect_arithmetic_request<>, "detected more than two cross points.");

						std::vector<std::pair<double, double>> thetaw;
						for(auto v : theta) {
							double dx = xstep * std::cos(v);
							double dy = ystep * std::sin(v);
							double w = (wx * dx + wy * dy + wxx / 2 * dx * dx + wxy * dx * dy + wyy / 2 * dy * dy);
							if(w > 0) {
								thetaw.emplace_back(v, w);
							}
						}

						//branching occur if there are multiple stable theta.
						if(thetaw.size() == 0) {
							//no selection gradient
							ns.emplace_back(0.0, trait_type{ (s[i].second)[0], (s[i].second)[1] });
							ndir.push_back(trait_type{ 0.0, 0.0 });
						} else if(thetaw.size() == 1) {
							//single direction
							double dx = xstep * std::cos(thetaw[0].first);
							double dy = ystep * std::sin(thetaw[0].first);
							double rate = thetaw[0].second * s[i].first / std::sqrt(dx * dx + dy * dy);

							ns.emplace_back(0.0, trait_type{ (s[i].second)[0], (s[i].second)[1] });
							ndir.push_back(trait_type{ dx * rate, dy * rate });
							maxrate = std::max(rate, maxrate);
						} else {
							//multi direction := branching
							auto minwitr = hmLib::transform_min_element(thetaw.begin(), thetaw.end(), [](const std::pair<double, double>& v) {return v.second; }).second;

							for(auto itr = thetaw.begin(); itr != thetaw.end(); ++itr) {
								double v = itr->first;
								ns.emplace_back(0.0, trait_type{ s[i].second[0] + xstep * std::cos(v), s[i].second[1] + ystep * std::sin(v) });
								ndir.push_back(trait_type{ 0.0, 0.0 });
							}

							//std::ofstream fout("branch.csv", std::ios::app | std::ios::out);
							//fout << n << "," << s[i].second[0] << "," << s[i].second[1] << ","
							//	<< wx << "," << wy << "," << wxx << "," << wxy << "," << wyy << ","
							//	<< ns[ns.size() - 2][0] << "," << ns[ns.size() - 2][1] << "," << ns[ns.size() - 1][0] << "," << ns[ns.size() - 1][1] << std::endl;
						}
					}
					for(unsigned int i = 0; i < ns.size(); ++i) {
						ns[i].second += ndir[i] / maxrate;	//limiting maximum step size = step.
					}

					//boundary condition
					for(auto& v : ns) {
						v.second[0] = std::max(0.0, v.second[0]);
						v.second[1] = std::max(0.0, v.second[1]);
					}

					//remove too close strains
					{
						int cnt = 0;
						while(cnt < static_cast<int>(ns.size())) {
							unsigned int j = 0;
							for(j = cnt + 1; j < ns.size(); ++j) {
								//merge if distance is less than step/4
								if(hmLib::norm2(ns[cnt].second - ns[j].second) < hmLib::norm2(trait_type{ absstep,absstep } +trait_type{ relstep,relstep } *ns[cnt].second) / 16) {
									ns[cnt].second = (ns[cnt].second + ns[j].second) / 2.0;
									std::swap(ns[j], ns.back());
									ns.pop_back();
									cnt = -1;
									break;
								}
							}
							++cnt;
						}
					}

					//update fraction of s
					{
						auto TraitRng = hmLib::make_get_range<1>(ns.begin(), ns.end());
						auto FreqRng = hmLib::make_get_range<0>(ns.begin(), ns.end());
						solveEqFreq_by_matrix(Game, TraitRng.begin(), TraitRng.end(), FreqRng.begin());
						auto Itr = std::remove_if(ns.begin(), ns.end(), [](const std::pair<double, trait_type>& v) {return v.first <= 1e-10; });
						ns.erase(Itr, ns.end());
					}
					s = std::move(ns);
				}
			};
		}
	}
}
#
#endif
