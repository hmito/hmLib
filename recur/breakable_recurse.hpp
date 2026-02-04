#ifndef HMLIB_RECUR_BREAKABLERECURSE_INC
#define HMLIB_RECUR_BREAKABLERECURSE_INC 100
#
#include<concepts>
#include<utility>
#include"observer/null_observer.hpp"
#include"concept.hpp"
#include"stepper.hpp"
namespace hmLib{
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, 
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, time_type EndTime, duration_type dTime, brk Breaker, obs Observer) {
		while (StartTime < EndTime) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime, dTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type, 
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, time_type EndTime, duration_type dTime, brk Breaker) {
		return breakable_recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, dTime, Breaker,  hmLib::recur::null_observer());
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::breaker<state_type,time_type> brk,
		hmLib::recur::observer<state_type,time_type> obs>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, 
		time_type StartTime, unsigned int StepNum, duration_type dTime, brk Breaker, obs Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			if (Breaker(State, StartTime)) {
				return std::make_pair(true, StartTime);
			}
			Stepper.do_step(System, State, StartTime, dTime);
			Observer(State, StartTime);
		}
		return std::make_pair(false, StartTime);
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type,
		hmLib::recur::breaker<state_type,time_type> brk>
	std::pair<bool, time_type> breakable_recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, 
		time_type StartTime, unsigned int StepNum, duration_type dTime, brk Breaker) {
		return breakable_recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, dTime, Breaker,  hmLib::recur::null_observer());
	}
	template<typename system_type, typename state_type,
		hmLib::recur::breaker<state_type,unsigned int> brk,
		hmLib::recur::observer<state_type,unsigned int> obs>
	std::pair<bool, unsigned int> breakable_recurse(system_type&& System, state_type& State, 
		unsigned int StepNum, brk Breaker, obs Observer) {
		return breakable_recurse_n(recur_stepper(), std::forward<system_type>(System), State, 0u, StepNum, 1u, Breaker, Observer);
	}
	template<typename system_type, typename state_type,
		hmLib::recur::breaker<state_type,unsigned int> brk>
	std::pair<bool, unsigned int> breakable_recurse(system_type&& System, state_type& State, 
		unsigned int StepNum, brk Breaker) {
		return breakable_recurse(std::forward<system_type>(System), State, StepNum, Breaker, hmLib::recur::null_observer());
	}
}
#
#endif
