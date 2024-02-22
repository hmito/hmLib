#ifndef HMLIB_NUMERIC_BISECTROOT_INC
#define HMLIB_NUMERIC_BISECTROOT_INC 100
#
#include<cmath>
#include<utility>
#include"../math/sign.hpp"
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include"../recur/breakable_recurse.hpp"
#include"detail/interpolate.hpp"
#include"evalue.hpp"
#include"breaker/erange_precision_breaker.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type, typename eval_type=value_type>
		struct bisect_root_stepper{
			using this_evalue = evalue<value_type,eval_type>;
			//system_type = fn;
			using state_type = erange<value_type,eval_type>;
		public:
			bisect_root_stepper():IsFirst(true){}
			template<typename fn>
			void initialize(fn, state_type&)noexcept{
				IsFirst = true;
			}
			template<typename fn>
			void do_step(fn Fn, state_type& x){
				if(std::exchange(IsFirst,false)){
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "bisect root require lower <= upper.");
					hmLib_assert(hmLib::math::sign(x.lower.e)*hmLib::math::sign(x.upper.e) == hmLib::math::sign::negative, hmLib::numeric_exceptions::incorrect_arithmetic_request, "bisect root require fn(lower) * fn(upper) <= 0.");
					if(hmLib::math::sign(x.lower.e) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.e) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
				}

				this_evalue z = this_evalue(detail::bisect_interpolate(x.lower,x.upper)).eval(Fn);

				if(hmLib::math::sign(z.e) == hmLib::math::sign::zero){
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(x.lower.e)*hmLib::math::sign(z.e) == hmLib::math::sign::negative){
					x.upper = std::move(z);
				}else{
					x.lower = std::move(z);
				}
			}
		private:
			bool IsFirst;
		};
		template<typename fn, typename value_type, typename breaker, typename observer>
		auto breakable_bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = bisect_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state_type = typename stepper::state_type;
			using evalue_type = typename state_type::evalue_type;

			stepper Stepper;
			state_type State(Fn, lowerval, upperval);
			State.order();

			auto ans = hmLib::breakable_recurse(Stepper, Fn, State, maxitr, Brk, Obs);
			if(!(ans.first||Brk(State,ans.second))){
				return make_step_result(ans.second,State);
			}else{
				return make_step_result(ans.second,State,
					evalue_type(detail::secant_interpolate(State.lower,State.upper)).eval(Fn)
				);
			}
//			return std::make_pair(State, (ans.first|Brk(State,ans.second),ans.second));
		}
		template<typename fn, typename value_type, typename breaker>
		auto breakable_bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, Brk, hmLib::recur::null_observer());
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, erange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, erange_precision_breaker<error_type>(relerr,abserr));
		}
	}
}
#
#endif
