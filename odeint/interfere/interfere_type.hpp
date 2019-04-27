#ifndef HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC 101
#
#include"../utility.hpp"
namespace hmLib {
	namespace odeint {
		enum struct interfere_request: unsigned int{
			none=0, 
			breakable=1,
			reset=2,
			initialize = 3
			//breakable: terminate integration (effective only in brakeable_intergare functions)
			//reset: try to initlize the stepper with newx, t and dt and resize.
			//initialize: try to initialize the stepper with newx, t and dt. (not adequate for resize)
		};
		namespace detail {
/*			template<typename stepper, typename sys, typename state_type, typename time_type>
			bool check_interfere(stepper& Stepper, sys& Sys, const state_type& State, time_type& Time, time_type& dT, state_type& NewState) {
				req = Sys.interfere(State, Time, dT, NewState);
				switch(req) {
				case interfere_request::breakable:
					return true;
				case interfere_request::reset:
					try_reset(Stepper);
					return false;
				case interfere_request::initialize:
					try_initialize(Stepper, Sys, Time, dT, NewState);
					return false;
				default:
					return false;
				}
			}*/
		}
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
