#ifndef HMLIB_RECUR_STEPPER_INC
#define HMLIB_RECUR_STEPPER_INC 100
#
#include"concept.hpp"
namespace hmLib{
	struct recur_stepper{
		template<typename system_type, typename state_type, typename time_type, typename duration_type>
		void do_step(system_type& sys, state_type& x, time_type& t, duration_type dt){
			if constexpr (recur::invariant_system<system_type, state_type>) {
				sys(x);
				t += dt;
			}
			else if constexpr (recur::adaptive_step_system<system_type, state_type, time_type, duration_type>) {
				double actual_dt = sys(x, t, dt);
				t += actual_dt;
			}
			else if constexpr (recur::const_step_system<system_type, state_type, time_type, duration_type>) {
				sys(x, t, dt);
				t += dt;
			} else {
				static_assert(false, "Unsupported system type");
			}
		}
	};
}
#
#endif
