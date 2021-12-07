#ifndef HMLIB_MINIMA_BRENTMINIMA_INC
#define HMLIB_MINIMA_BRENTMINIMA_INC 100
#
#include<cmath>
#include"minima_result.hpp"
#include"range_precision_breaker.hpp"
namespace hmLib{
    namespace minima{
		template<typename value_type,typename fvalue_type = value_type>
		struct brent_minima_stepper{
			struct state{
				friend struct brent_minima_stepper<value_type>;
			private:
				//min and max value of the searching range.
				value_type lowerval;
				value_type upperval;
				value_type val1;
				value_type val2;
				value_type val3;
				fvalue_type fval1;
				fvalue_type fval2;
				fvalue_type fval3;
				value_type delta;  // The distance moved in the last step
				value_type pdelta; // The distance moved in the step before last
			public:
				state() = default;
				template<typename fn>
				state(fn f, value_type lowerval_, value_type upperval_)
					: lowerval(lowerval_)
					, upperval(upperval_)
					, val1(upperval)
					, val2(upperval)
					, val3(upperval)
					, fval1(f(upperval))
					, fval2(f(upperval))
					, fval3(f(upperval))
					, delta(0)
					, pdelta(0){
				}
				template<typename fn>
				state(fn f, value_type lowerval_, value_type upperval_, value_type inival_)
					: lowerval(lowerval_)
					, upperval(upperval_)
					, val1(inival_)
					, val2(inival_)
					, val3(inival_)
					, fval1(f(inival_))
					, fval2(f(inival_))
					, fval3(f(inival_))
					, delta(0)
					, pdelta(0){
				}
				const value_type& value()const{return val1;}
				const fvalue_type& fvalue()const{return fval1;}
				const value_type& lower()const{return lowerval;}
				const value_type& upper()const{return upperval;}
			};
			template<typename fn>
			state make_state(fn f, value_type lowerval, value_type upperval)const{
				return state(f, lowerval, upperval);
			}
			template<typename fn>
			state make_state(fn f, value_type lowerval, value_type upperval, value_type inival)const{
				return state(f, lowerval, upperval, inival);
			}
			template<typename fn,typename error_type>
			void operator()(fn f, state& x, error_type precision)const {
				static const value_type golden_ratio = 0.3819660113;

				// midpoint
				value_type midval = (x.upperval + x.lowerval) / 2;

				//whether we should try parabolic fit or not?
				if(std::abs(x.pdelta) > precision){
					value_type r = (x.val1 - x.val2) * (x.fval1 - x.fval3);
					value_type q = (x.val1 - x.val3) * (x.fval1 - x.fval2);
					value_type p = (x.val1 - x.val3) * q - (x.val1 - x.val2) * r;
					q = 2 * (q - r);

					if(q > 0) p = -p;
					else q = -q;

					// check the accuracy of parabolic fit
					if((std::abs(p) >= std::abs(q * x.pdelta / 2)) || (p <= q * (x.lowerval - x.val1)) || (p >= q * (x.upperval - x.val1))){
						// bad; golden section instead
						x.pdelta = (x.val1 >= midval) ? midval - x.val1 : x.upperval - x.val1;
						x.delta = golden_ratio * x.pdelta;
					}else{
						// good; paraboratic fit
						x.pdelta = x.delta;
						x.delta = p / q;

						value_type newval = x.val1 + x.delta;
						if((newval - x.lowerval) < 2*precision || (x.upperval- newval) < 2*precision){
							x.delta = (midval - x.val1) < 0 ? static_cast<value_type>(-std::abs(precision)) : static_cast<value_type>(std::abs(precision));
						}
					}
				}else{
					// golden section
					x.pdelta = (x.val1 >= midval) ? x.lowerval - x.val1: x.upperval - x.val1;
					x.delta = golden_ratio * x.pdelta;
				}

				// okey, let's try another positition.
				value_type tryval = x.val1;
				if(std::abs(x.delta) >= precision){
					tryval += x.delta;
				}else if(x.delta > 0){
					tryval += precision;
				}else{
					tryval += -precision;
				}
				fvalue_type ftryval = f(tryval);

				if(ftryval <= x.fval1){
					//if tried point is the best

					//update min/max by the previous best
					if(tryval >= x.val1) x.lowerval = x.val1;
					else x.upperval = x.val1;

					// update holding points
					x.val3 = x.val2;
					x.val2 = x.val1;
					x.val1 = tryval;
					x.fval3 = x.fval2;
					x.fval2 = x.fval1;
					x.fval1 = ftryval;
				}else{
					//if the tried point is not the best (but should be better than one of vals)
					
					//update min/max by the tryval
					if(tryval < x.val1) x.lowerval = tryval;
					else x.upperval = tryval;

					if((ftryval <= x.fval2) || (x.val2 == x.val1)){
						// tried point is the second best
						x.val3 = x.val2;
						x.val2 = tryval;
						x.fval3 = x.fval2;
						x.fval2 = ftryval;
					} else if((ftryval <= x.fval3) || (x.val3 == x.val1) || (x.val3  == x.val2)){
						// tried point is the third best
						x.val3 = tryval;
						x.fval3 = ftryval;
					}
				}
			}
		};
		template<typename fn, typename value_type, typename precision_breaker>
		auto brent_minima_breaker(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, precision_breaker pbrk){
			brent_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, lowerval, upperval);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(pbrk(State))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State,pbrk.precision(State));
			}

			return make_minima_result(pbrk(State), maxitr, State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename precision_breaker>
		auto brent_minima_breaker(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, precision_breaker pbrk){
			brent_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, lowerval, upperval, inival);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(pbrk(State))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State,pbrk.precision(State));
			}

			return make_minima_result(pbrk(State), maxitr, State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename error_type>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return brent_minima_breaker(Fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
		template<typename fn, typename value_type, typename error_type>
		auto brent_minima(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, error_type relerr, error_type abserr){
			return brent_minima_breaker(Fn, lowerval, upperval, inival, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
    }
}
#
#endif
