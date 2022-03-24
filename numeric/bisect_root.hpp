#ifndef HMLIB_NUMERIC_BISECTROOT_INC
#define HMLIB_NUMERIC_BISECTROOT_INC 100
#
#include<utility>
#include"../math/sign.hpp"
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename T>
		struct bisect_root_stepper{
			using pair = evalpair<T>;
			using state =evalrange<T>;
		public:
			bisect_root_stepper():IsFirst(true){}
			template<typename F>
			void initialize(F, state&)noexcept{
				IsFirst = true;
			}
			template<typename F>
			void operator()(F fn, state& x){
				if(std::exchange(IsFirst,false)){
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "bisect root require lower <= upper.");
					if(hmLib::math::sign(x.lower.f) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.f) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
				}

				pair z(fn,detail::bisect_interpolate(x.lower,x.upper));

				if(hmLib::math::sign(z.f) == hmLib::math::sign::zero){
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(a.f)*hmLib::math::sign(z.f) == hmLib::math::sign::negative){
					x.upper = z;
				}else{
					x.lower = z;
				}
			}
		private:
			bool IsFirst;
		};
		template<typename F, typename T, typename breaker>
		auto breakable_bisect_root(F fn, T lowerval, T upperval, unsigned int maxitr, breaker Brk){
			using stepper = bisect_root_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(fn,State);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename F, typename T, typename breaker, typename observer>
		auto breakable_bisect_root(F fn, T lowerval, T upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = bisect_root_stepper<T>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(fn, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(fn,State);
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename F, typename T, typename error_type>
		auto bisect_root(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_bisect_root(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename F, typename T, typename error_type,typename observer>
		auto bisect_root(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_bisect_root(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr), Obs);
		}
	}
}
#
#endif
