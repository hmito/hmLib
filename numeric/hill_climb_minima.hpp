#ifndef HMLIB_NUMERIC_HILLCLIMBMINIMA_INC
#define HMLIB_NUMERIC_HILLCLIMBMINIMA_INC 100
#
#include<cmath>
#include"../recur/breakable_recurse.hpp"
#include"evalue.hpp"
#include"numeric_result.hpp"
namespace hmLib {
	namespace numeric {
		struct hill_climb_stepper{
			template<typename fnpair>
			bool try_step(fnpair FnPair, state_type& x)const {
				using std::abs;
				constexpr value_type epsilon(1.0/(math::golden_ratio<value_type>+1.0));

				auto precision = FnPair.second(x);

				// golden section point
				value_type delta = epsilon * ((x.guess.v * 2 >= x.upper.v + x.lower.v) ? x.lower.v - x.guess.v: x.upper.v - x.guess.v);

				// check whether golden section point can be used
				this_evalue trial(x.guess);
				if(abs(delta) >= precision){
					trial.v += delta;
				}else if(hmLib::math::sign(delta) == hmLib::math::sign::positive){
					trial.v += precision;
				}else{
					trial.v -= precision;
				}
				trial.eval(FnPair.first);

				if(trial.e <= x.guess.e){
					//if tried point is the best; update guess
					if(trial.v >= x.guess.v){
						x.lower = x.guess;
						x.guess = trial;
					}else{
						x.upper = x.guess;
						x.guess = trial;
					}
				}else{
					//if the tried point is not the best; update only either border
					if(trial.v < x.guess.v){
						x.lower = trial;
					}else{
						x.upper = trial;
					}
				}
			}


		};
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
