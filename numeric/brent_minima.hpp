#ifndef HMLIB_NUMERIC_BRENTMINIMA_INC
#define HMLIB_NUMERIC_BRENTMINIMA_INC 100
#
#include<cmath>
#include<utility>
#include"../exceptions.hpp"
#include"../math/constants.hpp"
#include"../math/sign.hpp"
#include"../recur/breakable_recurse.hpp"
#include"../recur/stepper_category.hpp"
#include"evalue.hpp"
#include"breaker/erange_precision_breaker.hpp"
#include"numeric_result.hpp"
namespace hmLib{
    namespace numeric{
		template<typename value_type, typename eval_type=value_type>
		struct brent_minima_stepper{
			using stepper_category = hmLib::recur::naive_stepper_tag;
			using this_evalue = evalue<value_type,eval_type>;
			//system_type = pair<fn, precison_fn>;
			using state_type = etrio<value_type,eval_type>;
		public:
			brent_minima_stepper():IsFirst(true),delta(0),pdelta(0),best2nd(),best3rd(){}
			template<typename fnpair>
			void initialize(fnpair, state_type&)noexcept{
				IsFirst = true;
			}
			template<typename fnpair>
			void do_step(fnpair FnPair, state_type& x) {
				using std::abs;
				constexpr value_type epsilon(1.0/(math::golden_ratio<value_type>+1.0));

				auto precision = FnPair.second(x);

				if(std::exchange(IsFirst,false)){
					hmLib_assert(x.lower.v <= x.guess.v && x.guess.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "brent minima require lower <= guess <= upper.");
					delta = 0;
					pdelta = 0;
					best2nd = x.guess;
					best3rd = x.guess;
				}

				// midpoint
				auto midval = (x.upper.v + x.lower.v) / 2;

				//whether we should try parabolic fit or not?
				if(std::abs(pdelta) > precision){
					auto r = (x.guess.v - best2nd.v) * (x.guess.e - best3rd.e);
					auto q = (x.guess.v - best3rd.v) * (x.guess.e - best2nd.e);
					auto p = (x.guess.v - best3rd.v) * q - (x.guess.v - best2nd.v) * r;

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

						auto newval = x.guess.v + delta;
						if((newval - x.lower.v) < 2*precision || (x.upper.v- newval) < 2*precision){
							delta = static_cast<value_type>((midval - x.guess.v) < 0 ? -abs(precision) : abs(precision));
						}
					}
				}else{
					// golden section
					pdelta = (x.guess.v >= midval) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v;
					delta = epsilon * pdelta;
				}

				// okey, let's try another positition.
				this_evalue trial(x.guess);
				if(abs(delta) >= precision){
					trial.v += delta;
				}else if(hmLib::math::sign(delta) == hmLib::math::sign::positive){
					trial.v += precision;
				}else{
					trial.v -= precision;
				}
				trial.eval(FnPair.first);

				if(trial.e <= x.guess.e){
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
					if((trial.e <= best2nd.e) || (best2nd.v == x.guess.v)){
						// tried point is the second best
						best3rd = best2nd;
						best2nd = trial;
					} else if((trial.e <= best3rd.e) || (best3rd.v == x.guess.v) || (best3rd.v  == best2nd.v)){
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
			value_type delta;  // The distance moved in the last step
			value_type pdelta; // The distance moved in the step before last
			this_evalue best2nd;
			this_evalue best3rd;
		};
		template<typename fn, typename value_type, typename precision_breaker, typename observer>
		auto breakable_brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, precision_breaker Brk, observer Obs){
			using stepper = brent_minima_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state_type = typename stepper::state_type;

			stepper Stepper;
			state_type State(Fn, (lowerval+upperval)/2.0, lowerval, upperval);
			State.order();

			auto ans = hmLib::breakable_recurse(Stepper, std::make_pair(Fn,[&Brk](const state_type& x){return Brk.precision(x);}), State, maxitr, Brk, Obs);
			return std::make_pair(State, count_result(ans.first|Brk(State,ans.second),ans.second));
		}
		template<typename fn, typename value_type, typename precision_breaker>
		auto breakable_brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, precision_breaker Brk){
			return breakable_brent_minima(Fn, lowerval, upperval, maxitr,Brk,hmLib::recur::null_observer());
		}
		template<typename fn, typename value_type, typename error_type, typename observer>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_brent_minima(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr),Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_brent_minima(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr));
		}
    }
}
#
#endif
