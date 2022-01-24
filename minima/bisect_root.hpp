#ifndef HMLIB_MINIMA_BISECT_ROOT_INC
#define HMLIB_MINIMA_BISECT_ROOT_INC 100
#
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include<utility>
namespace hmLib{
	namespace minima{
		namespace detail{
			template <class F, class T, class Tol, class Policy>
			std::pair<T, T> bisect(F f, T min, T max, Tol tol, unsigned int max_iter, const Policy& pol){
				T fmin = f(min);
				T fmax = f(max);
				if(fmin == 0){
					max_iter = 2;
					return std::make_pair(min, min);
				}
				if(fmax == 0){
					max_iter = 2;
					return std::make_pair(max, max);
				}

				//
				// Error checking:
				//
				static const char* function = "boost::math::tools::bisect<%1%>";
				if(min >= max){
					return boost::math::detail::pair_from_single(policies::raise_evaluation_error(function,
						"Arguments in wrong order in boost::math::tools::bisect (first arg=%1%)", min, pol));
				}
				if(fmin * fmax >= 0){
					return boost::math::detail::pair_from_single(policies::raise_evaluation_error(function,
						"No change of sign in boost::math::tools::bisect, either there is no root to find, or there are multiple roots in the interval (f(min) = %1%).", fmin, pol));
				}

				//
				// Three function invocations so far:
				//
				boost::uintmax_t count = max_iter;
				if(count < 3) count = 0;
				else count -= 3;

				while(count && (0 == tol(min, max))){
					T mid = (min + max) / 2;
					T fmid = f(mid);
					if((mid == max) || (mid == min))break;
					if(fmid == 0){
						min = max = mid;
						break;
					}else if(sign(fmid) * sign(fmin) < 0){
						max = mid;
						fmax = fmid;
					}else{
						min = mid;
						fmin = fmid;
					}
					--count;
				}

				max_iter -= count;

				return std::make_pair(min, max);
			}
		}
		template<typename value_type,typename eval_type>
		struct pair_state{
		public:
			//min and max value of the searching range.
			value_type vfirst;
			value_type vsecond;
			fvalue_type ffirst;
			fvalue_type fsecond;
		public:
			pair_state() = delete;
			pair_state(value_type vfirst_, value_type vsecond_, fvalue_type ffirst_, fvalue_type fsecond_)
				: vfirst(vfirst_), vsecond(vsecond_)
				, ffirst(ffirst_), fsecond(fsecond_){
			}
			template<typename fn>
			pair_state(value_type vfirst_, value_type vsecond_, fn f)
				: vfirst(vfirst_), vsecond(vsecond_)
				, ffirst(f(vfirst_)), fsecond(f(vsecond_)){
			}
			void replace_first(value_type value_, fvalue_type fvalue_){
				vfirst = value_;
				ffirst = fvalue_;
			}
			void replace_second(value_type value_, fvalue_type fvalue_){
				vsecond = value_;
				fsecond = fvalue_;					
			}
		};
		struct bisect_stepper{
			template<typename value_type,typename fvalue_type>
			using state = pair_state<value_type,fvalue_type>
		public:
			template<typename fn, typename value_type, typename fvalue_type>
			void initialize(state<value_type, fvalue_type>& x){
				hmLib_assert(x.ffirst * x.fsecond > 0, numeric_exceptions::incorrect_arithmetic_request, "the sign of ffirst is same with that of fsecond.");

				if(x.ffirst == 0){
					x.replace_second(x.vfirst,x.ffirst)
					vsecond=vfirst;
					fsecond=ffirst;
				}else if(fsecond == 0){
					vfirst=vsecond;
					ffirst=fsecond;
				}else if(vfirst > vsecond){
					std::swap(vfirst, vsecond);
					std::swap(ffirst, fsecond);
				}
			}
			template<typename fn, typename value_type, typename fvalue_type>
			void operator()(fn f, state<value_type, fvalue_type>& x){
				auto vmid = (x.lower() + x.upper())/2;
				auto fmid = f(vmid);

				if((vmid == x.lower()) || (vmid == x.upper()))return;
				if(fmid == 0){
					x.replace_lower(vmid,fmid);
					x.replace_upper(vmid,fmid);
				}else if(fmid * x.lower_fval() < 0){
					x.replace_upper(vmid,fmid);
				}else{
					x.replace_lower(vmid,fmid);
				}
			}
		};
	}
}
#
#endif
