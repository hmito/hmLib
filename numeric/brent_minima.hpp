#ifndef HMLIB_NUMERIC_BRENTMINIMA_INC
#define HMLIB_NUMERIC_BRENTMINIMA_INC 100
#
#include<cmath>
#include<utility>
#include"../math/constants.hpp"
#include"../math/sign.hpp"
#include"evalpair.hpp"
#include"numeric_result.hpp"
#include"range_precision_breaker.hpp"
namespace hmLib{
    namespace numeric{
		template<typename T>
		struct brent_minima_stepper{
//			using stepper_tag = precision_stepper;
			using state = guess_evalrange<T>;
		public:
			brent_minima_stepper():IsFirst(true){}
			template<typename F>
			void initialize(F, state&)noexcept{
				IsFirst = true;
			}
			template<typename fn,typename error_type>
			void operator()(fn f, state& x, error_type precision) {
				using std::abs;
				constexpr T epsilon(1.0/(math::golden_ratio<T>+1.0));

				if(std::exchange(IsFirst,false)){
					hmLib_assert(x.lower.v <= x.guess.v && x.guess.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "brent minima require lower <= guess <= upper.");
					delta = 0;
					pdelta = 0;
					best2nd = x.guess;
					best3rd = x.guess;
				}

				// midpoint
				T midval = (x.upper.v + x.lower.v) / 2;

				//whether we should try parabolic fit or not?
				if(std::abs(pdelta) > precision){
					T r = (x.guess.v - best2nd.v) * (x.guess.f - best3rd.f);
					T q = (x.guess.v - best3rd.v) * (x.guess.f - best2nd.f);
					T p = (x.guess.v - best3rd.v) * q - (x.guess.v - best2nd.v) * r;

					q = 2 * (q - r);

					if(q > 0) p = -p;
					else q = -q;

					// check the accuracy of parabolic fit
					if((abs(p) >= abs(q * pdelta / 2)) || (p <= q * (x.lower.v - x.guess.v)) || (p >= q * (x.upper.v - x.guess.v))){
						// bad; golden section instead
						pdelta = (x.guess.v >= midval) ? midval - x.guess.v : x.upper.v - x.guess.v;
						delta = epsilon * pdelta;
					}else{
						// good; paraboratic fit
						pdelta = delta;
						delta = p / q;

						T newval = x.guess.v + delta;
						if((newval - x.lower.v) < 2*precision || (x.upper.v- newval) < 2*precision){
							delta = static_cast<T>((midval - x.guess.v) < 0 ? -abs(precision) : abs(precision));
						}
					}
				}else{
					// golden section
					pdelta = (x.guess.v >= midval) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v;
					delta = epsilon * pdelta;
				}

				// okey, let's try another positition.
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
					// update holding points
					best3rd = best2nd;
					best2nd = x.guess;

					//update min/max by the previous best
					if(trial.v >= x.guess.v){
						x.lower = x.guess;
						x.guess = trial;
					}else{
						x.upper = x.guess;
						x.guess = trial;
					}
				}else{
					// update holding points
					if((trial.f <= best2nd.f) || (best2nd.v == x.guess.v)){
						// tried point is the second best
						best3rd = best2nd;
						best2nd = trial;
					} else if((trial.f <= best3rd.f) || (best3rd.v == x.guess.v) || (best3rd.v  == best2nd.v)){
						// tried point is the third best
						best3rd = trial;
					}

					//if the tried point is not the best; update only either border
					if(trial.v < x.guess.v){
						x.lower = trial;
					}else{
						x.upper = trial;
					}
				}
			}
		private:
			bool IsFirst;
			T delta;  // The distance moved in the last step
			T pdelta; // The distance moved in the step before last
			evalpair<T> best2nd;
			evalpair<T> best3rd;
		};
		template<typename fn, typename value_type, typename precision_breaker, typename observer>
		auto breakable_brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, precision_breaker Brk, observer Obs){
			using stepper = toms748_root_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, (lowerval+upperval)/2.0, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State,Brk.precision(State));
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename precision_breaker>
		auto breakable_brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, precision_breaker Brk){
			using stepper = toms748_root_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, (lowerval+upperval)/2.0, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State,Brk.precision(State));
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename error_type, typename observer>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_brent_minima(Fn, lowerval, upperval, maxitr,make_range_precision_breaker(relerr,abserr),Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_brent_minima(Fn, lowerval, upperval, maxitr,make_range_precision_breaker(relerr,abserr));
		}
    }
}
#
#endif
