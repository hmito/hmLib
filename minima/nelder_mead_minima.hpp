#ifndef HMLIB_MINIMA_NELDERMEADMINIMA_INC
#define HMLIB_MINIMA_NELDERMEADMINIMA_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
#include<cmath>
#include"minima_result.hpp"
namespace hmLib{
	namespace minima{
		template<typename value_type,typename fvalue_type>
		struct nelder_mead_minima_stepper{
		private:
			using vertex_type = std::pair<fvalue_type, value_type>;
			struct vertex_compare{
				bool operator()(const vertex_type& v1,const vertex_type& v2){return v1.first < v2.first;}
			};
		public:
			struct state{
				friend struct nelder_mead_minima_stepper<value_type, fvalue_type>;
			private:
				std::vector<vertex_type> v;
			public:
				value_type value(const state& State){
					return std::min_element(State.begin(),State.end(),vertex_compare())->second;
				}
				fvalue_type fvalue(const state& State){
					return std::min_element(State.begin(),State.end(),vertex_compare())->first;
				}
			};
		public:
			nelder_mead_minima_stepper()
				: alpha(1)
				, gamma(2)
				, rho(0.5)
				, sigma(0.5){
			}
			nelder_mead_minima_stepper(value_type alpha_, value_type gamma_, value_type rho_, value_type sigma_)
				: alpha(alpha_)
				, gamma(gamma_)
				, rho(rho_)
				, sigma(sigma_){
			}
			template<typename fn, typename value_type, typename error_type>
			state make_state(fn Fn, const value_type& start, error_type rel_value=0.2, error_type abs_value=0.0001) {
				std::size_t size = std::distance(start.begin(),start.end());
				//initialize vertex
				std::vector<vertex_t> V(size+1, std::make_pair(Fn(start),start));
				for(std::size_t i=1; i < V.size(); ++i){
					auto itr = std::next(V[i].second.begin(),i-1);
					(*itr) += abs_value + rel_value * (*itr);
					V[i].first = Fn(V[i].second);
				}
				return V;
			}
			template<typename fn, typename error_type>
			void operator()(fn Fn, state& State) {
				std::size_t n = State.size()-1;

				//sort by the order of the function
				std::sort(State.begin(),State.end(),vertex_compare());

				//calculate centroid of all points except State[n]
				vertex_type Center = State[0];
				for(std::size_t i=0; i < n; ++i){
					for(std::size_t j = 1; j < n; ++j){
						Center.second[i] += State[j].second[i];
					}
					Center.second[i] /= n;
				}
				Center.first = Fn(Center.second);

				//calculate rerected point, which is located on the opposite side of the Center from the worst point
				auto Refrect = Center;
				for(std::size_t i=0; i < n; ++i){
					Refrect.second[i] += alpha*(Center.second[i] - State[n].second[i]);
				}
				Refrect.first = Fn(Refrect.second);
				
				if(Refrect.first < State[0].first){
					//if the rerected point is best, calculate expanded point,
					//	try another type of point: expanded point, which is farer than the refrected point from the center
					//	(gamma > 1 for this purpose)
					auto Expand = Center;
					for(std::size_t i=0; i < n; ++i){
						Expand.second[i] += gamma*(Refrect.second[i] - Center.second[i]);
					}
					Expand.first = Fn(Expand.second);

					if(Expand.first < Refrect.first){
						//if the expanded point is best,
						//	replace the worst point by the expanded point
						State[n] = Expand;
					}else{
						//otherwise, replace the worst point by the expanded point
						State[n] = Refrect;
					}
				}else if(State[0].first <= Refrect.first && Refrect.first < State[n-1].first){
					//if the refrected point is not best but better than worst point
					State[n] = Refrect;
				}else{
					//if the refrected point is the worst
					//	try another type of point: contracted point, which is located between the worst point and the center
					auto Contracted = Center;
					for(std::size_t i=0; i < n; ++i){
						Contracted.second[i] += rho*(State[n].second[i] - Center.second[i]);
					}				
					Contracted.first = Fn(Contracted.second);
					if(Contracted.first < State[n].first){
						//if the contracted point is better than the worst, replace
						State[n] = Contracted;
					}else{
						//otherwise, all points shrink to the best point.
						for(std::size_t j = 1; j < n+1; ++j){
							for(std::size_t i=0; i < n; ++i){
								State[j].second[i] = State[0].second[i] + sigma*(State[j].second[i] - State[0].second[i]);
							}
							State[j].first = Fn(State[j].second);
						}
					}
				}
			}
		private:
			value_type alpha;
			value_type gamma;
			value_type rho;
			value_type sigma;
		};
		template<typename fn, typename value_type, typename error_type, typename precision_breaker>
		auto nelder_mead_minima(fn Fn, value_type ini, precision_breaker pbrk, unsigned int maxitr, error_type rel_value = 0.2, error_type abs_value=0.0001){
			nelder_mead_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, ini, rel_value, abs_value);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(pbrk(State))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State);
			}

			return make_minima_result(pbrk(State), maxitr,State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename error_type>
		auto nelder_mead_minima(fn Fn, value_type lower, value_type upper, unsigned int maxitr, error_type relerr, error_type abserr){
			return nelder_mead_minima(Fn, lower, upper, maxitr,simplex_precision_breaker<error_type>(relerr,abserr));
		}
	}
}
#
#endif
