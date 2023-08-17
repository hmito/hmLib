#ifndef HMLIB_RECUR_NULLSTEPPER_INC
#define HMLIB_RECUR_NULLSTEPPER_INC 100
#
#include<concepts>
namespace hmLib{
	namespace recur{
		struct null_naive_stepper{
			template<typename system_type, typename state_type>
			void do_step(system_type&& sys, state_type& x){
				sys(x);
			}
		};
		struct increment_null_stepper{
			template<typename system_type, typename state_type, std::integral time_type>
			void do_step(system_type&& sys, state_type& x, time_type& t){
				sys(x,t++);
			}
		};
		struct null_stepper{
			template<typename system_type, typename state_type, typename time_type>
			void do_step(system_type&& sys, state_type& x, time_type& t){
				sys(x,t);
			}
		};
	}
}
#
#endif
