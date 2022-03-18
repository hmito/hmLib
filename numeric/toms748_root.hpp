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
		template<typename T>
		struct toms748_root_stepper{
			using pair = evalpair<T>;
			using state =evalrange<T>;
		public:
			toms748_root_stepper():Stage(0){}
			template<typename F>
			void initialize(F, state&)noexcept{
				Stage = 0;
			}
			template<typename F>
			void operator()(F fn, state& x){
				switch(Stage++){
				case 1:
					//init-second step
					bracket(x, detail::quadratic_interpolate(x.lower,x.upper,p,2), fn, true);
					break;
				case 2:
					//loop-first step
					Dist = x.upper.v - x.lower.v;						
					bracket(x, detail::cubic_interpolate(x.lower,x.upper,p,q,2),fn, true);
				case 3:
					//loop-second step
					bracket(x, detail::cubic_interpolate(x.lower,x.upper,p,q,3), fn, false);
					break;
				case 4:
					//loop-third step
					bracket(x, detail::double_secant_interpolate(x.lower,x.upper),fn,true);
					break;
				case 5:
					//loop-forth step
					if(x.upper.v - x.lower.v >= 0.5*Dist){
						// If convergence of above three processes are worse than bisect method, just try bisect
						bracket(x, detail::bisect_interpolate(x.lower,x.upper),fn,true);
						Stage = 2;
					}else{
						//otherwise just try additional interpolate
						Dist = x.upper.v - x.lower.v;			
						bracket(x, detail::cubic_interpolate(x.lower,x.upper,2),fn, true);
						Stage = 3;
					}
					break;
				default://start from here
					//init_first step
					hmLib_assert(x.lower.v <= x.upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748 root require lower <= upper.");
					if(hmLib::math::sign(x.lower.f) == hmLib::math::sign::zero){
						x.upper=x.lower;
						return;
					}
					if(hmLib::math::sign(x.upper.f) == hmLib::math::sign::zero){
						x.lower=x.upper;
						return;
					}
					bracket(x, secant_interpolate(x.lower,x.upper), fn, false);
					Stage = 1; //reset stage number
					break;
				}
			}
		private:
			template<typename F>
			void bracket(state& x, T zv, F fn, bool q_update){
				if(q_update) q = p;
					
				auto tol = std::numeric_limits<T>::epsilon() * 2;
				if((x.upper.v - x.lower.v) < 2 * tol * x.lower.v){
					zv = detail::bisect_interpolate(x.lower,x.upper);
				}else if(z <= x.lower.v + std::abs(x.lower.v) * tol){
					zv = x.lower.v + std::abs(x.lower.v) * tol;
				}else if(z >= x.upper.v - std::abs(x.upper.v) * tol){
					zv = x.upper.v - std::abs(x.upper.v) * tol;
				}

				pair z(fn,zv);

				if(hmLib::math::sign(z.f) == hmLib::math::sign::zero){
					p = pair(0,0);
					x.lower = z;
					x.upper = z;
				}else if(hmLib::math::sign(a.f)*hmLib::math::sign(z.f) == hmLib::math::sign::negative){
					p = x.upper;
					x.upper = z;
				}else{
					p = x.lower;
					x.lower = z;
				}
			}
		private:
			int Stage;
			T Dist;
			pair p;
			pair q;
		};
		template<typename F, typename T, typename breaker>
		auto breakable_toms748_root(F fn, T lowerval, T upperval, unsigned int maxitr, breaker Brk){
			using stepper = toms748_root_stepper<T>;
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
		auto breakable_toms748_root(F fn, T lowerval, T upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = toms748_root_stepper<T>;
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
		auto toms748_root(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_toms748_root(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr));
		}
 		template<typename F, typename T, typename error_type,typename observer>
		auto toms748_root(F fn, T lowerval, T upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_toms748_root(fn, lowerval, upperval, maxitr,range_precision_breaker<error_type>(relerr,abserr), Obs);
		}
	}
}
#
#endif
