#ifndef HMLIB_MINIMA_GOLDENSECTIONMINIMA_INC
#define HMLIB_MINIMA_GOLDENSECTIONMINIMA_INC 100
#
#include<cmath>
#include"minima_result.hpp"
#include"range_precision_breaker.hpp"
namespace hmLib{
    namespace minima{
		template<typename value_type,typename fvalue_type = value_type>
		struct golden_section_minima_stepper{
			struct state{
				friend struct golden_section_minima_stepper<value_type>;
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
			public:
				state() = default;
				template<typename fn>
				state(fn f, value_type lowerval_, value_type upperval_, value_type inival_)
					: lowerval(lowerval_)
					, upperval(upperval_)
					, val1(inival_)
					, val2(inival_)
					, val3(inival_)
					, fval1(f(inival_))
					, fval2(f(inival_))
					, fval3(f(inival_)){
				}
				const value_type& value()const{return val1;}
				const fvalue_type& fvalue()const{return fval1;}
				const value_type& lower()const{return lowerval;}
				const value_type& upper()const{return upperval;}
			};
			template<typename fn>
			state make_state(fn f, value_type lowerval, value_type upperval, value_type inival)const{
				return state(f, lowerval, upperval, inival);
			}
			template<typename fn,typename error_type>
			void operator()(fn f, state& x, error_type precision)const {
				static const value_type golden_ratio = 0.3819660113;

				// midpoint
				value_type midval = (x.upperval + x.lowerval) / 2;

				// golden section
				value_type delta = golden_ratio * ((x.val1 >= midval) ? x.lowerval - x.val1: x.upperval - x.val1);

				// okey, let's try another point.
				value_type tryval = x.val1;
				if(std::abs(delta) >= precision){
					tryval += delta;
				}else if(delta > 0){
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
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, precision_breaker Brk){
			golden_section_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, lowerval, upperval, inival);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State,Brk.precision(State));
			}

			return make_minima_result(pbrk(State,maxitr), maxitr, State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename precision_breaker, typename observer>
		auto breakable_golden_section_minima(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, precision_breaker Brk, observer Obs){
			golden_section_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, lowerval, upperval, inival);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State,Brk.precision(State));
				Obs(State,i);
			}

			return make_minima_result(pbrk(State,maxitr), maxitr, State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename error_type>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_golden_section_minima(Fn, lowerval, upperval, inival, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto golden_section_minima(fn Fn, value_type lowerval, value_type upperval, value_type inival, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_golden_section_minima(Fn, lowerval, upperval, inival, maxitr,range_precision_breaker<error_type>(relerr,abserr), Obs);
		}
   }
}
#
#endif
