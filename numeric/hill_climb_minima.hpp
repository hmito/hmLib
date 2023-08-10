#ifndef HMLIB_NUMERIC_HILLCLIMBMINIMA_INC
#define HMLIB_NUMERIC_HILLCLIMBMINIMA_INC 100
#
#include<random>
#include"evalue.hpp"
namespace hmLib {
	namespace numeric {
		//hill climing search
		template<typename evaluate, typename state_type, typename mutate, typename urbg, typename state_breaker>
		auto hill_climbing_minima(evaluate&& Evaluate, state_type& State, mutate&& Mutate, state_breaker&& Breaker, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state_type NState = State;

			while(!Breaker(State, Eval)){
				Mutate(NState, URBG);
				auto NEval = Evaluate(NState);

				if(Eval > NEval){
					Eval = NEval;
					State = NState;
				} else{
					NState = State;
				}
			}

			return Eval;
		}

		//simulated annealing search
		template<typename evaluate, typename state_type, typename mutate, typename temp, typename urbg, typename state_breaker>
		auto simulated_annealing_minima(evaluate&& Evaluate, state_type& State, mutate&& Mutate, temp&& Temp, state_breaker&& Breaker, unsigned int StepNum, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state_type NState = State;

			unsigned int StepCnt = 0;
			while(!Breaker(State, Eval)){
				Mutate(NState, URBG);
				auto NEval = Evaluate(NState);

				if(Eval > NEval || std::uniform_real_distribution<double>(0.0, 1.0)(URBG) < std::exp((Eval - NEval) / Temp(static_cast<double>(StepCnt) / StepNum))){
					Eval = NEval;
					State = NState;
				} else{
					NState = State;
				}

				++StepCnt;
			}

			return Eval;
		}
	}
}
#
#endif
