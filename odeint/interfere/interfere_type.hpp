#ifndef HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC 100
#
namespace hmLib {
	namespace odeint {
		enum struct interfere_request{none, initialize, terminate};
		/*=== interfere_systme ===
		template<typename state_type,typename time_type>
		struct interfere_system {
			template<typename stepper>
			interfere_type ready(stepper& Stepper, const state_type& x, time_type t, time_type dt, state_type& nx);
			bool valid_step(const state_type& x, time_type t);						//valid_step should return true for current state after ready function
			bool validate(onst state_type& x, time_type t, state_type& vx);			//validate should return true when it change state
		};
		*/
	}
}
#
#endif
