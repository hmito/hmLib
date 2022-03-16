#ifndef HMLIB_MINIMA_BISECT_ROOT_INC
#define HMLIB_MINIMA_BISECT_ROOT_INC 100
#
#include"../exceptions/exception.hpp"
#include"../exceptions/numeric_exceptions.hpp"
#include<utility>
namespace hmLib{
	namespace minima{
		template<typename value_type,typename fvalue_type = value_type>
		struct bisect_stepper{
			struct state{
			public:
				//min and max value of the searching range.
				value_type val1;
				value_type val2;
				fvalue_type fval1;
				fvalue_type fval2;
			public:
				state() = delete;
				state(value_type val1_, value_type val2_, fvalue_type fval1_, fvalue_type fval2_)
					: val1(val1_), val2(val2_)
					, fval1(fval1_), fval2(fval2_){
				}
				template<typename fn>
				state(fn f, value_type val1_, value_type val2_)
					: val1(val1_), val2(val2_)
					, fval1(f(val1_)), fval2(f(val2_)){
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
