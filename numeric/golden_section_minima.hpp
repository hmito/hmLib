#ifndef HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC
#define HMLIB_NUMERIC_GOLDENSECTIONMINIMA_INC 100
#
#include<cmath>
#include"../math/constants.hpp"
#include"../math/sign.hpp"
#include"evalpair.hpp"
#include"numeric_result.hpp"
#include"range_precision_breaker.hpp"
namespace hmLib{
    namespace numeric{
		template<typename T>
		struct golden_section_minima_stepper{
//			using stepper_tag = precision_stepper;
			using state = guess_evalrange<T>;
			template<typename F,typename error_type>
			void operator()(F fn, state& x, error_type precision)const {
				using std::abs;
				constexpr T epsilon(1.0/(math::golden_ratio<T>+1.0));

				// golden section point
				T delta = epsilon * ((x.guess.v * 2 >= x.upper.v + x.lower.v) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v);

				// check whether golden section point can be used
				evalpair trial(x.guess);
				if(abs(delta) >= precision){
					trial.v += delta;
				}else if(hmLib::math::sign(delta) == hmLib::math::sign::positive){
					trial.v += precision;
				}else{
					trial.v -= precision;
				}
				trial.eval(fn);

				if(trial.f <= x.guess.f){
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
		template<typename F, typename T, typename precision_breaker>
		auto breakable_golden_section_minima(F fn, T lowerval, T upperval, unsigned int maxitr, precision_breaker Brk){
			using stepper = golden_section_minima_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(fn,State, Brk.precision(State));
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename F, typename T, typename precision_breaker, typename observer>
		auto breakable_golden_section_minima(F fn, T lowerval, T upperval, unsigned int maxitr, precision_breaker Brk, observer Obs){
			using stepper = golden_section_minima_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(fn,State, Brk.precision(State));
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename F, typename T, typename error_type>
		auto golden_section_minima(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_golden_section_minima(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename F, typename T, typename error_type,typename observer>
		auto golden_section_minima(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_golden_section_minima(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr), Obs);
		}
   }
}
#
#endif
