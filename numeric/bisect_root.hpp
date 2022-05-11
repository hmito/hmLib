#ifndef HMLIB_NUMERIC_BISECTROOT_INC
#define HMLIB_NUMERIC_BISECTROOT_INC 100
#
#include<utility>
#include"../math/sign.hpp"
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include"evalpair.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type, typename evalue_type=value_type>
		struct bisect_root_stepper{
			using pair = evalpair<value_type,evalue_type>;
			using state =evalrange<value_type,evalue_type>;
		public:
			bisect_root_stepper():IsFirst(true){}
			template<typename fn>
			void initialize(fn, state&)noexcept{
				IsFirst = true;
			}
			template<typename fn>
			void operator()(fn Fn, state& x){
				if(std::exchange(IsFirst,false)){
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "bisect root require lower <= upper.");
					if(hmLib::math::sign(x.lower.e) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.e) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
				}

				pair z(Fn,detail::bisect_interpolate(x.lower,x.upper));

				if(hmLib::math::sign(z.e) == hmLib::math::sign::zero){
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(a.e)*hmLib::math::sign(z.e) == hmLib::math::sign::negative){
					x.upper = std::move(z);
				}else{
					x.lower = std::move(z);
				}
			}
		private:
			bool IsFirst;
		};
		template<typename fn, typename value_type, typename breaker>
		auto breakable_bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			using stepper = bisect_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, lowerval, upperval);
			//State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename breaker, typename observer>
		auto breakable_bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = bisect_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, lowerval, upperval);
			//State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename error_type>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
	}
}
#
#endif
