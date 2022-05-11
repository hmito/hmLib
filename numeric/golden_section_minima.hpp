#ifndef HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC
#define HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC 100
#
#include<cmath>
#include"../math/constants.hpp"
#include"../math/sign.hpp"
#include"evalpair.hpp"
#include"numeric_result.hpp"
namespace hmLib{
    namespace numeric{
		template<typename value_type,typename evalue_type=value_type>
		struct golden_section_minima_stepper{
			using stepper_tag = precision_stepper;
			using pair = evalpair<value_type,evalue_type>;
			using state = guess_evalrange<value_type,evalue_type>;
			template<typename fn,typename error_type>
			void operator()(fn Fn, state& x, error_type precision)const {
				using std::abs;
				constexpr value_type epsilon(1.0/(math::golden_ratio<value_type>+1.0));

				// golden section point
				value_type delta = epsilon * ((x.guess.v * 2 >= x.upper.v + x.lower.v) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v);

				// check whether golden section point can be used
				pair trial(x.guess);
				if(abs(delta) >= precision){
					trial.v += delta;
				}else if(hmLib::math::sign(delta) == hmLib::math::sign::positive){
					trial.v += precision;
				}else{
					trial.v -= precision;
				}
				trial.eval(Fn);

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
		template<typename fn, typename value_type, typename breaker>
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			using stepper = golden_section_minima_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename breaker, typename observer>
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = golden_section_minima_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename error_type>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_golden_section_minima(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_golden_section_minima(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
   }
}
#
#endif
