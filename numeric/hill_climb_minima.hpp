#ifndef HMLIB_MINIMA_INC
#define HMLIB_MINIMA_INC 100
#
#include<random>
#include<utility>
#include"math/constants.hpp"
#include"minima/golden_section_minima.hpp"
#include"minima/brent_minima.hpp"
#include"minima/nelder_mead_minima.hpp"
namespace hmLib {
	namespace minima {
		//hill climing search
		template<typename evaluate, typename state, typename mutate, typename urbg, typename state_breaker>
		auto hill_climbing_minima(evaluate&& Evaluate, state& State, mutate&& Mutate, state_breaker&& Breaker, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state NState = State;

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
		template<typename evaluate, typename state, typename mutate, typename temp, typename urbg, typename state_breaker>
		auto simulated_annealing_minima(evaluate&& Evaluate, state& State, mutate&& Mutate, temp&& Temp, state_breaker&& Breaker, unsigned int StepNum, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state NState = State;

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

		//brute-forece search
		template<typename evaluate, typename state, typename state_stepper>
		auto brute_force_minima(evaluate&& Evaluate, state& State, state_stepper&& StateStepper)->decltype(Evaluate(State)){
			StateStepper.initialize(State);

			auto Eval = Evaluate(State);
			state NState = State;

			while(!StateStepper(NState)){
				auto NEval = Evaluate(NState);
				if(Eval > NEval){
					Eval = NEval;
					State = NState;
				}
			}

			return Eval;
		}
	}
}
#
#endif
