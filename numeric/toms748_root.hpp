#ifndef HMLIB_NUMERIC_TOMS748ROOT_INC
#define HMLIB_NUMERIC_TOMS748ROOT_INC 100
#
#include<utility>
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include "../math/math.hpp"
#include "../math/sign.hpp"
#include"../recur/breakable_recurse.hpp"
#include"../recur/stepper_category.hpp"
#include "detail/interpolate.hpp"
#include "evalue.hpp"
#include "breaker/erange_precision_breaker.hpp"
#include "numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type, typename eval_type=value_type>
		struct toms748_root_stepper{
			using stepper_category = hmLib::recur::naive_stepper_tag;
			using this_evalue = evalue<value_type,eval_type>;
			//system_type = fn;
			using state_type = erange<value_type,eval_type>;
		public:
			toms748_root_stepper():Stage(0){}
			template<typename fn>
			void initialize(fn, state_type&)noexcept{
				Stage = 0;
			}
			template<typename fn>
			void do_step(fn Fn, state_type& x){
				switch(Stage++){
				case 1:
					//init-second step
					bracket(x, detail::quadratic_interpolate(x.lower,x.upper,p,2), Fn, true);
					break;
				case 2:
					//loop-first step
					Dist = x.upper.v - x.lower.v;						
					bracket(x, detail::cubic_interpolate(x.lower,x.upper,p,q,2),Fn, true);
				case 3:
					//loop-second step
					bracket(x, detail::cubic_interpolate(x.lower,x.upper,p,q,3), Fn, false);
					break;
				case 4:
					//loop-third step
					bracket(x, detail::double_secant_interpolate(x.lower,x.upper),Fn,true);
					break;
				case 5:
					//loop-forth step
					if(x.upper.v - x.lower.v >= 0.5*Dist){
						// If convergence of above three processes are worse than bisect method, just try bisect
						bracket(x, detail::bisect_interpolate(x.lower,x.upper),Fn,true);
						Stage = 2;
					}else{
						//otherwise just try additional interpolate
						Dist = x.upper.v - x.lower.v;			
						bracket(x, detail::cubic_interpolate(x.lower,x.upper,p,q,2),Fn, true);
						Stage = 3;
					}
					break;
				default://start from here
					//init_first step
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748 root require lower <= upper.");
					hmLib_assert(hmLib::math::sign(x.lower.e)*hmLib::math::sign(x.upper.e) == hmLib::math::sign::negative, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748 root require fn(lower) * fn(upper) <= 0.");
					if(hmLib::math::sign(x.lower.e) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.e) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
					bracket(x, detail::secant_interpolate(x.lower,x.upper), Fn, false);
					Stage = 1; //reset stage number
					break;
				}
			}
		private:
			template<typename fn>
			void bracket(state_type& x, value_type zv, fn Fn, bool q_update){
				if(q_update) q = p;
					
				auto tol = std::numeric_limits<value_type>::epsilon() * 2;
				if((x.upper.v - x.lower.v) < 2 * tol * x.lower.v){
					zv = detail::bisect_interpolate(x.lower,x.upper);
				}else if(zv <= x.lower.v + std::abs(x.lower.v) * tol){
					zv = x.lower.v + std::abs(x.lower.v) * tol;
				}else if(zv >= x.upper.v - std::abs(x.upper.v) * tol){
					zv = x.upper.v - std::abs(x.upper.v) * tol;
				}

				auto z = this_evalue(zv).eval(Fn);

				if(hmLib::math::sign(z.e) == hmLib::math::sign::zero){
					p = this_evalue(0,0);
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(x.lower.e)*hmLib::math::sign(z.e) == hmLib::math::sign::negative){
					p = x.upper;
					x.upper = z;
				}else{
					p = x.lower;
					x.lower = z;
				}
			}
		private:
			int Stage;
			value_type Dist;
			this_evalue p;
			this_evalue q;
		};
		template<typename fn, typename value_type, typename breaker, typename observer>
		auto breakable_toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = toms748_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state_type = typename stepper::state_type;

			stepper Stepper;
			state_type State(Fn, lowerval,upperval);
			State.order();

			auto ans = hmLib::breakable_recurse(Stepper, Fn, State, maxitr, Brk, Obs);
			return std::make_pair(State, count_result(ans.first|Brk(State,ans.second),ans.second));
		}
		template<typename fn, typename value_type, typename breaker>
		auto breakable_toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			return breakable_toms748_root(Fn, lowerval, upperval, maxitr,Brk,hmLib::recur::null_observer());
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_toms748_root(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_toms748_root(Fn, lowerval, upperval, maxitr,erange_precision_breaker<error_type>(relerr,abserr));
		}
	}
}
#
#endif
