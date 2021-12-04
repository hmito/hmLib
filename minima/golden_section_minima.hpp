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
				value_type lower;
				value_type upper;
				value_type val1;
				value_type val2;
				value_type val3;
				fvalue_type fval1;
				fvalue_type fval2;
				fvalue_type fval3;
			public:
				state() = default;
				template<typename fn>
				state(fn f, value_type lower_, value_type upper_)
					: lower(lower_)
					, upper(upper_)
					, val1(upper)
					, val2(upper)
					, val3(upper)
					, fval1(f(upper))
					, fval2(f(upper))
					, fval3(f(upper)){
				}
				const value_type& value()const{return val1;}
				const fvalue_type& fvalue()const{return fval1;}
				const value_type& lower()const{return lower;}
				const value_type& upper()const{return upper;}
			};
			template<typename fn>
			state make_state(fn f, value_type lower, value_type upper)const{
				return state(f, lower, upper)
			}
			template<typename fn,typename error_type>
			void operator()(fn f, state& x, error_type precision){
				static const value_type golden_ratio = 0.3819660113;

				// midpoint
				value_type midval = (x.upper + x.lower) / 2;

				// golden section
				value_type delta = golden_ratio * ((x.val1 >= midval) ? x.lower - x.val1: x.upper - x.val1);

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
					if(tryval >= x.val1) x.lower = x.val1;
					else x.upper = x.val1;

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
					if(tryval < x.val1) x.lower = tryval;
					else x.upper = tryval;

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
		auto golden_section_minima(fn Fn, value_type lower, value_type upper, unsigned int maxitr, precision_breaker pbrk){
			golden_section_minima_stepper<value_type,decltype(std::declval<fn>()(std::declval<value_type>()))> Stepper;
			auto State = Stepper.make_state(Fn, lower, upper);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(pbrk(State))return make_minima_result(true, i, State.value(),State.fvalue());
				Stepper(Fn,State,pbrk.precision(State.value()));
			}

			return make_minima_result(pbrk(State), maxitr, State.value(),State.fvalue());
		}
		template<typename fn, typename value_type, typename error_type>
		auto golden_section_minima(fn Fn, value_type lower, value_type upper, unsigned int maxitr, error_type relerr, error_type abserr){
			return golden_section_minima(Fn, lower, upper, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
    }
}
#
#endif
