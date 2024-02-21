#ifndef HMLIB_NUMERIC_NELDERMEADMINIMA_INC
#define HMLIB_NUMERIC_NELDERMEADMINIMA_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
#include<type_traits>
#include<cmath>
#include<ranges>
#include<iostream>
#include"../recur/breakable_recurse.hpp"
#include"evalue.hpp"
#include"esimplex.hpp"
#include"numeric_result.hpp"
#include"breaker/esimplex_precision_breaker.hpp"
#include"../random.hpp"
namespace hmLib{
	namespace numeric{
		template<typename elem_type_,typename eval_type_>
		struct nelder_mead_minima_stepper{
			using elem_type = elem_type_;
			using eval_type = eval_type_;
			using state_type = esimplex<elem_type,eval_type>;
			using evalue_type = typename state_type::evalue_type;
		private:
			//system_type = fn;
			using this_type = nelder_mead_minima_stepper<elem_type,eval_type>;
		public:
			nelder_mead_minima_stepper()
				: alpha(1)
				, gamma(2)
				, rho(0.5)
				, sigma(0.5){
			}
			nelder_mead_minima_stepper(elem_type alpha_, elem_type gamma_, elem_type rho_, elem_type sigma_)
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
				evalue_type Center = State[0];
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
		template<typename fn, typename value_type, typename elem_type, typename breaker,typename observer>
		auto breakable_nelder_mead_minima(fn Fn, value_type&& Range, elem_type relval, elem_type absval, unsigned int maxitr, breaker Brk, observer Obs){
//			using stepper = brent_minima_stepper<std::decay_t<elem_type>,decltype(Fn(lowerval))>;
			using stepper = nelder_mead_minima_stepper<std::decay_t<decltype(*(Range.begin()))>, std::decay_t<decltype(Fn(Range))>>;
			using state_type = typename stepper::state_type;

			stepper Stepper;
			state_type State(Fn, std::begin(Range), std::end(Range), relval, absval, hmLib::random::default_engine());

			auto ans = hmLib::breakable_recurse(Stepper, Fn, State, maxitr, Brk, Obs);
			if(!(ans.first|Brk(State,ans.second))){
				return make_step_result(ans.second,State);
			}else{
				return make_step_result(ans.second,State,State.minima()->v);
			}
		}
		template<typename fn, typename value_type, typename elem_type, typename breaker>
		auto breakable_nelder_mead_minima(fn Fn, value_type&& Range, elem_type relval, elem_type absval, unsigned int maxitr, breaker Brk){
			return breakable_nelder_mead_minima(Fn,  std::forward<value_type>(Range), relval, absval, maxitr, Brk, hmLib::recur::null_observer());
		}
		template<typename fn, typename value_type, typename elem_type, typename error_type,typename observer>
		auto nelder_mead_minima(fn Fn, value_type&& Range, elem_type relval, elem_type absval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_nelder_mead_minima(Fn, std::forward<value_type>(Range), relval, absval, maxitr,esimplex_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename value_type, typename elem_type, typename error_type>
		auto nelder_mead_minima(fn Fn, value_type&& Range, elem_type relval, elem_type absval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_nelder_mead_minima(Fn,  std::forward<value_type>(Range), relval, absval, maxitr,esimplex_precision_breaker<error_type>(relerr,abserr));
		}
	}
}
#
#endif
