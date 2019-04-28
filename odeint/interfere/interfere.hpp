#ifndef HMLIB_ODEINT_INTERFERE_INTERFERE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFERE_INC 101
#
#include"../utility.hpp"
namespace hmLib {
	namespace odeint {
		enum struct interfere_request: unsigned int{
			none=0, 
			breakable=1, 	//terminate integration (effective only in brakeable_intergare functions)
			reset=2,		//try to initlize the stepper with newx, t and dt and resize.
			initialize = 3	//try to initialize the stepper with newx, t and dt (not adequate for resize).
		};
		/*=== interfere_systme ===
		template<typename state_type,typename time_type>
		struct interfere_system {
			interfere_request interfere(const state_type& x, time_type& t, time_type& dt, state_type& newx);
		};
		*/
	}
}
#
#endif
