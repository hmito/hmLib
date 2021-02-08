#ifndef HMLIB_ODEINT_RECURSIVE_RECURRENCE_INC
#define HMLIB_ODEINT_RECURSIVE_RECURRENCE_INC 200
#
#include"time_update_checker.hpp"
namespace hmLib{
	namespace odeint {
		template<typename dsystem_type, typename state_type, typename time_type>
		size_t recurrence(dsystem_type& System, state_type& State, time_type StartTime, time_type EndTime) {
			size_t Cnt = 0;
			detail::time_update_checker<time_type> Checker(StartTime);
			while(StartTime < EndTime) {
				Checker(StartTime);
				System(State, StartTime);
				++Cnt;
			}
			return Cnt;
		}
		template<typename dsystem_type, typename state_type, typename time_type, typename observer>
		size_t recurrence(dsystem_type& System, state_type& State, time_type StartTime, time_type EndTime, observer Observer) {
			size_t Cnt = 0;
			detail::time_update_checker<time_type> Checker(StartTime);
			while (StartTime < EndTime) {
				Checker(StartTime);
				System(State, StartTime);
				Observer(State, StartTime);
				++Cnt;
			}
			return Cnt;
		}
		template<typename dsystem_type, typename state_type, typename time_type>
		time_type recurrence_nstep(dsystem_type& System, state_type& State, time_type StartTime, size_t StepNum) {
			for (size_t Cnt = 0; Cnt < StepNum; ++Cnt) {
				System(State, StartTime);
			}
			return StartTime;
		}
		template<typename dsystem_type, typename state_type, typename time_type, typename observer>
		time_type recurrence_nstep(dsystem_type& System, state_type& State, time_type StartTime, size_t StepNum, observer Observer) {
			for (size_t Cnt = 0; Cnt < StepNum; ++Cnt) {
				System(State, StartTime);
				Observer(State, StartTime);
			}
			return StartTime;
		}
		template<typename dsystem_type, typename state_type, typename time_type>
		void recurrence_increment(dsystem_type& System, state_type& State, time_type StartTime, time_type EndTime) {
			while(StartTime < EndTime) {
				System(State, StartTime++);
			}
		}
		template<typename dsystem_type, typename state_type, typename time_type, typename observer>
		void recurrence_increment(dsystem_type& System, state_type& State, time_type StartTime, time_type EndTime, observer Observer) {
			while (StartTime < EndTime) {
				System(State, StartTime++);
				Observer(State, StartTime);
			}
		}
	}
}
#
#endif
