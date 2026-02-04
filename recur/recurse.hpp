#ifndef HMLIB_RECUR_RECURSE_INC
#define HMLIB_RECUR_RECURSE_INC 200
#
#include<concepts>
#include<type_traits>
#include"observer/null_observer.hpp"
#include"concept.hpp"
#include"stepper.hpp"
namespace hmLib{
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type,hmLib::recur::observer<state_type,time_type> obs>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, time_type EndTime, duration_type dTime, obs Observer) {
		while (StartTime < EndTime) {
			Stepper.do_step(System, State, StartTime, dTime);
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, time_type EndTime, duration_type dTime) {
		return recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, dTime, hmLib::recur::null_observer());
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type,hmLib::recur::observer<state_type,time_type> obs>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, unsigned int StepNum, duration_type dTime, obs Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			Stepper.do_step(System, State, StartTime, dTime);
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<typename stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State,
		time_type StartTime, unsigned int StepNum, duration_type dTime) {
		return recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, dTime, hmLib::recur::null_observer());
	}
	template<typename system_type, typename state_type, hmLib::recur::observer<state_type,unsigned int> obs>
	unsigned int recurse(system_type&& System, state_type& State,
		unsigned int StepNum, obs Observer) {
		return recurse_n(recur_stepper(), std::forward<system_type>(System), State, 0u, StepNum, 1u, Observer);
	}
	template<typename system_type, typename state_type>
	unsigned int recurse(system_type&& System, state_type& State,
		unsigned int StepNum) {
		return recurse(std::forward<system_type>(System), State, StepNum, hmLib::recur::null_observer());
	}
}
#
#endif
