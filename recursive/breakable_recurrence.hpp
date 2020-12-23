#ifndef HMLIB_RECURSIVE_BREAKABLERECURRENCE_INC
#define HMLIB_RECURSIVE_BREAKABLERECURRENCE_INC 100
#
#include<utility>
#include<tuple>
namespace hmLib{
    namespace recursive{
		template<typename system_type, typename state_type, typename time_type, typename breaker_type>
		std::tuple<bool, time_type, size_t> breakable_recurrence(system_type& System, state_type& State, time_type StartTime, time_type EndTime, breaker_type Breaker) {
			size_t Step=0;
			detail::time_checker<time_type> Checker;
			while (StartTime < EndTime) {
				Checker(StartTime);
				if (Breaker(State, StartTime)) {
					return std::make_tuple(true, StartTime, Step);
				}
				System(State, StartTime);
				++Step;
			}
			return std::make_tuple(Breaker(State, StartTime), StartTime, Step);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type, typename observer>
		std::tuple<bool, time_type, size_t> breakable_recurrence(system_type& System, state_type& State, time_type StartTime, time_type EndTime, breaker_type Breaker, observer Observer) {
			size_t Step = 0;
			detail::time_checker<time_type> Checker;
			while (StartTime < EndTime) {
				Checker(StartTime);
				if (Breaker(State, StartTime)) {
					return std::make_tuple(true, StartTime, Step);
				}
				System(State, StartTime);
				Observer(State, StartTime);
				++Step;
			}
			return std::make_pair(Breaker(State, StartTime), StartTime, Step);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type>
		std::tuple<bool, time_type, size_t> breakable_recurrence_nstep(system_type& System, state_type& State, time_type StartTime, size_t StepNum, breaker_type Breaker) {
			size_t Step = 0;
			for (; Step < StepNum; ++Step) {
				if (Breaker(State, StartTime)) {
					return std::make_tuple(true, StartTime, Step);
				}
				System(State, StartTime);
			}
			return std::make_tuple(Breaker(State, StartTime), StartTime, Step);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type, typename observer>
		std::tuple<bool, time_type, size_t> breakable_recurrence_nstep(system_type& System, state_type& State, time_type StartTime, size_t StepNum, breaker_type Breaker, observer Observer) {
			size_t Step = 0;
			for (; Step < StepNum; ++Step) {
				if (Breaker(State, StartTime)) {
					return std::make_tuple(true, StartTime, Step);
				}
				System(State, StartTime);
				Observer(State, StartTime);
			}
			return std::make_tuple(Breaker(State, StartTime), StartTime, Step);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type>
		std::pair<bool, size_t> breakable_recurrence_increment(system_type& System, state_type& State, time_type StartTime, time_type EndTime, breaker_type Breaker) {
			while(StartTime < EndTime) {
				if (Breaker(State, StartTime)) {
					return std::make_pair(true, StartTime);
				}
				System(State, StartTime++);
			}
			return std::make_pair(Breaker(State, StartTime), StartTime);
		}
		template<typename system_type, typename state_type, typename time_type, typename breaker_type, typename observer>
		std::pair<bool, size_t> breakable_recurrence_increment(system_type& System, state_type& State, time_type StartTime, time_type EndTime, breaker_type Breaker, observer Observer) {
			while (StartTime < EndTime) {
				if (Breaker(State, StartTime)) {
					return std::make_pair(true, StartTime);
				}
				System(State, StartTime++);
				Observer(State, StartTime);
			}
			return std::make_pair(Breaker(State, StartTime), StartTime);
		}
    }
}
#
#endif
