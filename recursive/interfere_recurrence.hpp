#ifndef HMLIB_RECURSIVE_INTERFERERECURRENCE_INC
#define HMLIB_RECURSIVE_INTERFERERECURRENCE_INC 100
#
namespace hmLib{
    namespace recursive{
		enum class interfere_request {
			none, terminate
		};
		template<typename stepper_t, typename interfere_system_t, typename state_t, typename time_t>
		time_t interfere_recurrence(stepper_t&& Stepper, interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep) {
			while(BeginStep != EndStep) {
				if(Stepper(System, State, BeginStep++) == interfere_request::terminate)break;
			}
			return BeginStep;
		}
		template<typename stepper_t, typename interfere_system_t, typename state_t, typename time_t, typename observer>
		time_t interfere_recurrence(stepper_t&& Stepper, interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep, observer Observer) {
			while(BeginStep != EndStep) {
				if(Stepper(System, State, BeginStep++) == interfere_request::terminate){
					Observer(State, BeginStep);
					break;
				}
				Observer(State, BeginStep);
			}
			return BeginStep;
		}
		template<typename interfere_system_t, typename state_t, typename time_t>
		time_t interfere_recurrence_each(interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep) {
			while(BeginStep != EndStep) {
				if(System(State, BeginStep++) == interfere_request::terminate)break;
			}
			return BeginStep;
		}
		template<typename interfere_system_t, typename state_t, typename time_t, typename observer>
		time_t interfere_recurrence_each(interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep, observer Observer) {
			while(BeginStep != EndStep) {
				if(System(State, BeginStep++) == interfere_request::terminate){
					Observer(State, BeginStep);
					break;
				}
				Observer(State, BeginStep);
			}
			return BeginStep;
		}
    }
}
#
#endif
