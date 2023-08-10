#ifndef HMLIB_RECUR_BREAKABLERECURSE_INC
#define HMLIB_RECUR_BREAKABLERECURSE_INC 100
#
#include<concepts>
#include<utility>
#include"recurse.hpp"
namespace hmLib{
	template<hmLib::recur::stepper stepper_type, typename system_type, typename state_type, typename time_type, typename breaker, typename observer>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, breaker Breaker, observer Observer) {
		while (StartTime < EndTime) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<hmLib::recur::stepper stepper_type, typename system_type, typename state_type, typename time_type, typename breaker>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, breaker Breaker) {
		return breakable_recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, Breaker,  hmLib::recur::null_observer());
	}
	template<hmLib::recur::stepper stepper_type, typename system_type, typename state_type, typename time_type, typename breaker, typename observer>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, breaker Breaker, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<hmLib::recur::stepper stepper_type, typename system_type, typename state_type, typename time_type, typename breaker>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, breaker Breaker) {
		return breakable_recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, Breaker,  hmLib::recur::null_observer());
	}

	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, typename breaker, typename observer>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime, breaker Breaker, observer Observer) {
		while (StartTime < EndTime) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State);
			StartTime += dTime;
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, typename breaker>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime, breaker Breaker) {
		return breakable_recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, dTime, Breaker, hmLib::recur::null_observer());
	}
	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, typename breaker, typename observer>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, duration_type dTime,breaker Breaker, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State);
			StartTime += dTime;
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, typename breaker>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum,duration_type dTime, breaker Breaker) {
		return breakable_recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, dTime, Breaker, hmLib::recur::null_observer());
	}
	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename breaker, typename observer>
	std::pair<bool, unsigned int> breakable_recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum, breaker Breaker, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StepNum)) {
				return std::make_pair(true, Cnt);
			}
			Stepper.do_step(System, State);
			Observer(State, Cnt);
		}
		return std::make_pair(false, StepNum);
	}
	template<hmLib::recur::naive_stepper stepper_type, typename system_type, typename state_type, typename breaker>
	std::pair<bool, unsigned int> breakable_recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum, breaker Breaker) {
		return breakable_recurse(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StepNum, Breaker, hmLib::recur::null_observer());
	}
}
#
#endif
