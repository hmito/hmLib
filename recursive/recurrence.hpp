#ifndef HMLIB_RECURSIVE_RECURRENCE_INC
#define HMLIB_RECURSIVE_RECURRENCE_INC 100
#
namespace hmLib{
	namespace recursive {
		template<typename stepper_type, typename system_type, typename state_type, typename time_type>
		void recurrence(stepper_type&& Stepper, system_type& System, state_type& State, time_type BeginStep, time_type EndStep) {
			while(BeginStep < EndStep) {
				Stepper(System, State, BeginStep);
			}
		}
		template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename observer>
		void recurrence(stepper_type&& Stepper, system_type& System, state_type& State, time_type BeginStep, time_type EndStep, observer Observer) {
			while (BeginStep < EndStep) {
				Stepper(System, State, BeginStep);
				Observer(State, BeginStep);
			}
		}
		template<typename system_type, typename state_type, typename time_type>
		void recurrence_each(system_type& System, state_type& State, time_type BeginStep, time_type EndStep) {
			for(; BeginStep != EndStep; ++BeginStep) {
				System(State, BeginStep);
			}
		}
		template<typename system_type, typename state_type, typename time_type, typename observer>
		void recurrence_each(system_type& System, state_type& State, time_type BeginStep, time_type EndStep, observer Observer) {
			for(; BeginStep != EndStep; ++BeginStep) {
				System(State, BeginStep);
				Observer(State, BeginStep);
			}
		}
	}
}
#
#endif
