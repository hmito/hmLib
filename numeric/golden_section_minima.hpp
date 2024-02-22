#ifndef HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC
#define HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC 100
#
#include<cmath>
#include"../math/constants.hpp"
#include"../math/sign.hpp"
#include"../recur/breakable_recurse.hpp"
#include"evalue.hpp"
#include"numeric_result.hpp"
#include"numeric_system.hpp"
#include"breaker/erange_precision_breaker.hpp"
namespace hmLib{
    namespace numeric{
		template<typename fn_type, typename precision_type>
		using golden_section_minima_system = precision_system<fn_type,precision_type>;
		template<typename value_type,typename eval_type=value_type>
		struct golden_section_minima_stepper{
			using this_evalue = evalue<value_type,eval_type>;
			//system_type = pair<fn, precison_fn>;
			using state_type = etrirange<value_type,eval_type>;
			template<typename fn_type, typename precision_type>
			void do_step(golden_section_minima_system<fn_type,precision_type> Sys, state_type& x)const {
				using std::abs;
				constexpr value_type epsilon(1.0/(math::golden_ratio<value_type>+1.0));

				auto precision = Sys.precision(x);

				// golden section point
				value_type delta = epsilon * ((x.guess.v * 2 >= x.upper.v + x.lower.v) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v);

				// check whether golden section point can be used
				this_evalue trial(x.guess);
				if(abs(delta) >= precision){
					trial.v += delta;
				}else if(hmLib::math::sign(delta) == hmLib::math::sign::positive){
					trial.v += precision;
				}else{
					trial.v -= precision;
				}
				trial.eval(Sys.fn);

				if(trial.e <= x.guess.e){
					//if tried point is the best; update guess
					if(trial.v >= x.guess.v){
						x.lower = x.guess;
						x.guess = trial;
					}else{
						x.upper = x.guess;
						x.guess = trial;
					}
				}else{
					//if the tried point is not the best; update only either border
					if(trial.v < x.guess.v){
						x.lower = trial;
					}else{
						x.upper = trial;
					}
				}
			}
		};
		template<typename fn, typename value_type, typename breaker,typename observer>
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = golden_section_minima_stepper<std::decay_t<value_type>,std::decay_t<decltype(Fn(lowerval))>>;
			using state_type = typename stepper::state_type;

			stepper Stepper;
			state_type State(Fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			auto ans = hmLib::breakable_recurse(Stepper, make_precision_system(Fn,[&Brk](const state_type& x){return Brk.precision(x);}), State, maxitr, Brk, Obs);
			if(!(ans.first||Brk(State,ans.second))){
				return(make_step_result(ans.second,State));
			}else{
				return(make_step_result(ans.second,State,State.guess));
			}
		}
		template<typename fn, typename value_type, typename breaker>
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			return breakable_golden_section_minima(Fn, lowerval, upperval, maxitr,Brk,hmLib::recur::null_observer());
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_golden_section_minima(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_golden_section_minima(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr));
		}
   }
}
#
#endif
