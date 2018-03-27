#ifndef HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFERETYPE_INC 101
#
#include"../utility.hpp"
namespace hmLib {
	namespace odeint {
		enum struct interfere_request: unsigned int{
			none=0, 
			terminate=1,
			initialize=2,
			resize_and_initialize=3,
			restep = 4, 
			restep_with_dt = 5
		};
		namespace interferes {
			namespace detail {
				inline bool is_termination_requested(interfere_request req) {
					return req == interfere_request::terminate;
				}
				inline bool is_initializing_requested(interfere_request req) {
					return req == interfere_request::initialize || req==interfere_request::resize_and_initialize;
				}
				inline bool is_restep_requested(interfere_request req) {
					return req==interfere_request::restep || req==interfere_request::restep_with_dt;
				}
				inline bool should_use_dt_on_restep(interfere_request req) {
					return req==interfere_request::restep_with_dt;
				}
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void interfere_excute(interfere_request& req, stepper& Stepper, sys& Sys, state_type& State, time_type& Time, time_type& dT, state_type& NewState) {
					req = Sys.interfere(State, Time, dT, NewState);
					switch(req) {
					case interfere_request::none:
						return false;
					case interfere_request::terminate:
						return true;
					case interfere_request::initialize:
						State = NewState;
						try_initialize(Stepper, Sys, NewState, Time, dT);
						return false;
					case interfere_request::resize_and_initialize:
						State = NewState;
						try_adjust_size(Stepper, State);
						try_initialize(Stepper, Sys, NewState, Time, dT);
						return false;
					default:
						return false;
					}
				}
			}
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
