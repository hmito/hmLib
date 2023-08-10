#ifndef HMLIB_RECUR_RECURSE_INC
#define HMLIB_RECUR_RECURSE_INC 200
#
#include<concepts>
#include"observer/null_observer.hpp"
#include"stepper_category.hpp"
namespace hmLib{
	template<stepper stepper_type, typename system_type, typename state_type, typename time_type, typename observer>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, observer Observer) {
		while (StartTime < EndTime) {
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<stepper stepper_type, typename system_type, typename state_type, typename time_type>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime) {
		return recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, null_observer());
	}
	template<stepper stepper_type, typename system_type, typename state_type, typename time_type, typename observer>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			Stepper.do_step(System, State, StartTime);
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<stepper stepper_type, typename system_type, typename state_type, typename time_type>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, unsigned int StepNum) {
		return recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, null_observer());
	}

	template<naive_stepper stepper_type, stepper system_type, typename state_type, typename time_type, typename duration_type, typename observer>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime, observer Observer) {
		while (StartTime < EndTime) {
			Stepper.do_step(System, State);
			StartTime += dTime;
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<naive_stepper stepper_type, typename system_type, typename state_type, typename time_type, typename duration_type>
	time_type recurse_until(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime, time_type EndTime, duration_type dTime) {
		return recurse_until(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, EndTime, dTime, null_observer());
	}
	template<naive_stepper stepper_type, stepper system_type, typename state_type, typename time_type, typename duration_type, typename observer>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime,unsigned int StepNum, duration_type dTime, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			Stepper.do_step(System, State);
			StartTime += dTime;
			Observer(State, StartTime);
		}
		return StartTime;
	}
	template<naive_stepper stepper_type, stepper system_type, typename state_type, std::integral time_type, typename duration_type>
	time_type recurse_n(stepper_type&& Stepper, system_type&& System, state_type& State, time_type StartTime,unsigned int StepNum, duration_type dTime) {
		return recurse_n(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StartTime, StepNum, dTime, null_observer());
	}
	template<naive_stepper stepper_type, stepper system_type, typename state_type, typename observer>
	unsigned int recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum, observer Observer) {
		for (unsigned int Cnt = 0; Cnt < StepNum; ++Cnt) {
			Stepper.do_step(System, State);
			Observer(State, Cnt);
		}
		return StepNum;
	}
	template<naive_stepper stepper_type, stepper system_type, typename state_type>
	unsigned int recurse(stepper_type&& Stepper, system_type&& System, state_type& State, unsigned int StepNum) {
		return recurse(std::forward<stepper_type>(Stepper), std::forward<system_type>(System), State, StepNum, null_observer());
	}
}
#
#endif
