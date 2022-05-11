#ifndef HMLIB_NUMERIC_TOMS748ROOT_INC
#define HMLIB_NUMERIC_TOMS748ROOT_INC 100
#
#include<utility>
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include "../math/math.hpp"
#include "../math/sign.hpp"
#include "evalpair.hpp"
#include "interpolate.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type, typename evalue_type=value_type>
		struct toms748_root_stepper{
			using pair = evalpair<value_type,evalue_type>;
			using state =evalrange<value_type.evalue_type>;
		public:
			toms748_root_stepper():Stage(0){}
			template<typename fn>
			void initialize(fn, state&)noexcept{
				Stage = 0;
			}
			template<typename fn>
			void operator()(fn Fn, state& x){
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
						bracket(x, detail::cubic_interpolate(x.lower,x.upper,2),Fn, true);
						Stage = 3;
					}
					break;
				default://start from here
					//init_first step
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748 root require lower <= upper.");
					if(hmLib::math::sign(x.lower.e) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.e) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
					bracket(x, secant_interpolate(x.lower,x.upper), Fn, false);
					Stage = 1; //reset stage number
					break;
				}
			}
		private:
			template<typename fn>
			void bracket(state& x, value_type zv, fn Fn, bool q_update){
				if(q_update) q = p;
					
				auto tol = std::numeric_limits<value_type>::epsilon() * 2;
				if((x.upper.v - x.lower.v) < 2 * tol * x.lower.v){
					zv = detail::bisect_interpolate(x.lower,x.upper);
				}else if(z <= x.lower.v + std::abs(x.lower.v) * tol){
					zv = x.lower.v + std::abs(x.lower.v) * tol;
				}else if(z >= x.upper.v - std::abs(x.upper.v) * tol){
					zv = x.upper.v - std::abs(x.upper.v) * tol;
				}

				pair z(Fn,zv);

				if(hmLib::math::sign(z.e) == hmLib::math::sign::zero){
					p = pair(0,0);
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(a.e)*hmLib::math::sign(z.e) == hmLib::math::sign::negative){
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
			pair p;
			pair q;
		};
		template<typename fn, typename value_type, typename breaker>
		auto breakable_toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			using stepper = toms748_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename breaker, typename observer>
		auto breakable_toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = toms748_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state = typename stepper::state;
			stepper Stepper;
			state State(Fn, lowerval,upperval);
			State.order();

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State,i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename value_type, typename error_type>
		auto toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_toms748_root(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto toms748_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_toms748_root(Fn, lowerval, upperval, maxitr,evalrange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
	}
}
#
#endif
