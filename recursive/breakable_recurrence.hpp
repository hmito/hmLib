#ifndef HMLIB_RECURSIVE_BREAKABLERECURRENCE_INC
#define HMLIB_RECURSIVE_BREAKABLERECURRENCE_INC 100
#
#include<utility>
namespace hmLib{
    namespace recursive{
		template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename breaker_type>
		std::pair<bool, size_t> recurrence(stepper_type&& Stepper, system_type& System, state_type& State, time_type BeginStep, time_type EndStep, breaker_type Breaker) {
			for (; BeginStep != EndStep; ++BeginStep) {
				if (Breaker(State, BeginStep)) {
					return std::make_pair(true, BeginStep);
				}
				Stepper(System, State, BeginStep);
			}
			return std::make_pair(Breaker(State, BeginStep), BeginStep);
		}
		template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename breaker_type, typename observer>
		std::pair<bool, size_t> recurrence(stepper_type&& Stepper, system_type& System, state_type& State, time_type BeginStep, time_type EndStep, breaker_type Breaker, observer Observer) {
			for (; BeginStep != EndStep; ++BeginStep) {
				if (Breaker(State, BeginStep)) {
					return std::make_pair(true, BeginStep);
				}
				Stepper(System, State, BeginStep);
				Observer(State, BeginStep);
			}
			return std::make_pair(Breaker(State, BeginStep), BeginStep);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type>
		std::pair<bool, size_t> recurrence_each(system_type& System, state_type& State, time_type BeginStep, time_type EndStep, breaker_type Breaker) {
			for (; BeginStep != EndStep; ++BeginStep) {
				if (Breaker(State, BeginStep)) {
					return std::make_pair(true, BeginStep);
				}
				System(State, BeginStep);
			}
			return std::make_pair(Breaker(State, BeginStep), BeginStep);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type, typename observer>
		std::pair<bool, size_t> recurrence_each(system_type& System, state_type& State, time_type BeginStep, time_type EndStep, breaker_type Breaker, observer Observer) {
			for (; BeginStep != EndStep; ++BeginStep) {
				if (Breaker(State, BeginStep)) {
					return std::make_pair(true, BeginStep);
				}
				System(State, BeginStep);
				Observer(State, BeginStep);
			}
			return std::make_pair(Breaker(State, BeginStep), BeginStep);
		}
    }
}
#
#endif
