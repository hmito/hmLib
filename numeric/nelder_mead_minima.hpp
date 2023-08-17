#ifndef HMLIB_NUMERIC_NELDERMEADMINIMA_INC
#define HMLIB_NUMERIC_NELDERMEADMINIMA_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
#include<type_traits>
#include<cmath>
#include<iostream>
#include"../recur/breakable_recurse.hpp"
#include"../recur/stepper_category.hpp"
#include"evalue.hpp"
#include"esimplex.hpp"
#include"breaker/esimplex_precision_breaker.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type,typename eval_type>
		struct nelder_mead_minima_stepper{
			using stepper_category = hmLib::recur::naive_stepper_tag;
			using state_type = esimplex<value_type,eval_type>;
			using vertex_type = typename state_type::vertex;
		private:
			//system_type = fn;
			using this_type = nelder_mead_minima_stepper<value_type,eval_type>;
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
			template<typename fn>
			void do_step(fn Fn, state_type& State)const {
				std::size_t n = std::distance(State.begin(),State.end())-1;

				//sort by the order of the function
				std::sort(State.begin(),State.end());

				//calculate centroid of all points except State[n]
				vertex_type Center = State[0];
				for(std::size_t i=0; i < n; ++i){
					for(std::size_t j = 1; j < n; ++j){
						Center.v[i] += State[j].v[i];
					}
					Center.v[i] /= n;
				}
				Center.e = Fn(Center.v);

				//calculate rerected point, which is located on the opposite side of the Center from the worst point
				auto Refrect = Center;
				for(std::size_t i=0; i < n; ++i){
					Refrect.v[i] += alpha*(Center.v[i] - State[n].v[i]);
				}
				Refrect.e = Fn(Refrect.v);
				
				if(Refrect.e < State[0].e){
					//if the rerected point is best, calculate expanded point,
					//	try another type of point: expanded point, which is farer than the refrected point from the center
					//	(gamma > 1 for this purpose)
					auto Expand = Center;
					for(std::size_t i=0; i < n; ++i){
						Expand.v[i] += gamma*(Refrect.v[i] - Center.v[i]);
					}
					Expand.e = Fn(Expand.v);

					if(Expand.e < Refrect.e){
						//if the expanded point is best,
						//	replace the worst point by the expanded point
						State[n] = Expand;
					}else{
						//otherwise, replace the worst point by the expanded point
						State[n] = Refrect;
					}
				}else if(State[0].e <= Refrect.e && Refrect.e < State[n-1].e){
					//if the refrected point is not best but better than worst point
					State[n] = Refrect;
				}else{
					//if the refrected point is the worst
					//	try another type of point: contracted point, which is located between the worst point and the center
					auto Contracted = Center;
					for(std::size_t i=0; i < n; ++i){
						Contracted.v[i] += rho*(State[n].v[i] - Center.v[i]);
					}				
					Contracted.e = Fn(Contracted.v);
					if(Contracted.e < State[n].e){
						//if the contracted point is better than the worst, replace
						State[n] = Contracted;
					}else{
						//otherwise, all points shrink to the best point.
						for(std::size_t j = 1; j < n+1; ++j){
							for(std::size_t i=0; i < n; ++i){
								State[j].v[i] = State[0].v[i] + sigma*(State[j].v[i] - State[0].v[i]);
							}
							State[j].e = Fn(State[j].v);
						}
					}
				}
			}
		private:
			double alpha;
			double gamma;
			double rho;
			double sigma;
		};
		template<typename fn, typename state_type, typename breaker,typename observer>
		auto breakable_nelder_mead_minima(fn Fn, state_type&& State, unsigned int maxitr, breaker Brk, observer Obs){
//			using stepper = brent_minima_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using stepper = nelder_mead_minima_stepper<typename std::decay_t<state_type>::value_type, typename state_type::eval_type>;

			stepper Stepper;
			state_type ThisState = std::forward<state_type>(State);

			auto ans = hmLib::breakable_recurse(Stepper, Fn, ThisState, maxitr, Brk, Obs);
			return std::pair<state_type,count_result>(std::move(ThisState), count_result(ans.first|Brk(ThisState,ans.second),ans.second));
		}
		template<typename fn, typename state_type, typename breaker>
		auto breakable_nelder_mead_minima(fn Fn, state_type&& State, unsigned int maxitr, breaker Brk){
			return breakable_nelder_mead_minima(Fn, std::forward<state_type>(State), maxitr, Brk, hmLib::recur::null_observer());
		}
		template<typename fn, typename state_type, typename error_type,typename observer>
		auto nelder_mead_minima(fn Fn, state_type&& State, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_nelder_mead_minima(Fn, std::forward<state_type>(State), maxitr,esimplex_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename state_type, typename error_type>
		auto nelder_mead_minima(fn Fn, state_type&& State, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_nelder_mead_minima(Fn, std::forward<state_type>(State), maxitr,esimplex_precision_breaker<error_type>(relerr,abserr));
		}
	}
}
#
#endif
