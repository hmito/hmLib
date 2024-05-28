#ifndef HMLIB_RECUR_BREAKABLERECURSE_INC
#define HMLIB_RECUR_BREAKABLERECURSE_INC 100
#
#include<concepts>
#include<utility>
#include"recurse.hpp"
#include"concept.hpp"
namespace hmLib{
	template<typename system_type, typename state_type, typename time_type, 
		hmLib::recur::stepper<system_type, state_type, time_type> stepper_type, 
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, brk Breaker, obs Observer) {
		while (StartTime < EndTime) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<typename system_type, typename state_type, typename time_type, 
		hmLib::recur::stepper<system_type, state_type, time_type> stepper_type, 
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, brk Breaker) {
		return breakable_recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, Breaker,  hmLib::recur::null_observer());
	}
	template<typename system_type, typename state_type, typename time_type, 
		hmLib::recur::stepper<system_type, state_type, time_type> stepper_type, 
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, brk Breaker, obs Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<typename system_type, typename state_type, typename time_type, 
		hmLib::recur::stepper<system_type, state_type, time_type> stepper_type, 
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, brk Breaker) {
		return breakable_recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, Breaker,  hmLib::recur::null_observer());
	}

	template<typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime, brk Breaker, obs Observer) {
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
	template<typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime, brk Breaker) {
		return breakable_recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, dTime, Breaker, hmLib::recur::null_observer());
	}
	template<typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, duration_type dTime,brk Breaker, obs Observer) {
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
	template<typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum,duration_type dTime, brk Breaker) {
		return breakable_recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, dTime, Breaker, hmLib::recur::null_observer());
	}
	template<typename system_type, typename state_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,unsigned int> brk,
		hmLib::recur::observer<state_type,unsigned int> obs>
	std::pair<bool, unsigned int> breakable_recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum, brk Breaker, obs Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StepNum)) {
				return std::make_pair(true, Cnt);
			}
			Stepper.do_step(System, State);
			Observer(State, Cnt);
		}
		return std::make_pair(false, StepNum);
	}
	template<typename system_type, typename state_type,
		hmLib::recur::naive_stepper<system_type, state_type> stepper_type,
		hmLib::recur::breaker<state_type,unsigned int> brk>
	std::pair<bool, unsigned int> breakable_recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum, brk Breaker) {
		return breakable_recurse(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StepNum, Breaker, hmLib::recur::null_observer());
	}
}
#
#endif
