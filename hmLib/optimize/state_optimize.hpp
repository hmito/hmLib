#ifndef HMLIB_OPTIMIZE_STATEOPTIMIZE_INC
#define HMLIB_OPTIMIZE_STATEOPTIMIZE_INC 100
#
#include<random>
#include<utility>
namespace hmLib{
	namespace optimize{
		namespace state_breakers{
			template<typename state, typename eval_type>
			struct none_breaker{
			public:
				void clear(){}
				bool operator()(const state& State, eval_type Eval){ return false; }
			};
			template<typename state, typename eval_type>
			struct const_step_breaker{
			private:
				unsigned int Num;
				unsigned int Cnt;
			public:
				const_step_breaker(unsigned int StepNum_) :Num(StepNum_), Cnt(0){}
				void clear(){ Cnt = 0; }
				unsigned int get_step()const{ return Cnt; }
				unsigned int get_maxstep()const{ return Num; }
				bool operator()(const state& State, eval_type Eval){
					return Cnt++ >= Num;
				}
			};
			template<typename state, typename eval_type>
			struct const_stable_breaker{
			private:
				unsigned int Num;
				unsigned int Cnt;
				eval_type Best;
			public:
				const_stable_breaker(unsigned int StepNum_) :Num(StepNum_){ clear(); }
				void clear(){ Cnt = 0; }
				unsigned int get_stable_step()const{ return Cnt; }
				unsigned int get_stable_maxstep()const{ return Num; }
				eval_type get_best_eval()const{ return Best; }
				bool operator()(const state& State, eval_type Eval){
					if(Cnt == 0){
						Best = Eval;
						Cnt = 1;
					} else if(Best < Eval){
						Best = Eval;
						Cnt = 1;
					} else{
						++Cnt;
					}
					return Cnt >= Num;
				}
			};
			template<typename state, typename eval_type>
			struct limited_const_stable_breaker{
			private:
				unsigned int Num;
				unsigned int Cnt;
				eval_type Best;
				unsigned int StepCnt;
				unsigned int StepMax;
			public:
				limited_const_stable_breaker(unsigned int StepNum_, unsigned int StepMax_) :Num(StepNum_), StepMax(StepMax_){ clear(); }
				void clear(){ Cnt = 0; StepCnt = 0; }
				unsigned int get_stable_step()const{ return Cnt; }
				unsigned int get_stable_maxstep()const{ return Num; }
				unsigned int get_step()const{ return StepCnt; }
				unsigned int get_maxstep()const{ return StepMax; }
				eval_type get_best_eval()const{ return Best; }
				bool operator()(const state& State, eval_type Eval){
					if(Cnt == 0){
						Best = Eval;
						Cnt = 1;
					} else if(Best < Eval){
						Best = Eval;
						Cnt = 1;
					} else{
						++Cnt;
					}
					return Cnt >= Num || StepCnt++ >= StepMax;
				}
			};
		}

		//hill climing search
		template<typename state, typename evaluate, typename mutate, typename urbg, typename state_breaker>
		auto hill_climbing_search(state& State, evaluate&& Evaluate, mutate&& Mutate, state_breaker&& Breaker, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state NState = State;

			while(!Breaker(State, Eval)){
				Mutate(NState, URBG);
				auto NEval = Evaluate(NState);

				if(Eval < NEval){
					Eval = NEval;
					State = NState;
				} else{
					NState = State;
				}
			}

			return Eval;
		}

		//simulated annealing search
		template<typename state, typename evaluate, typename mutate, typename temp, typename urbg, typename state_breaker>
		auto simulated_annealing_search(state& State, evaluate&& Evaluate, mutate&& Mutate, temp&& Temp, state_breaker&& Breaker, unsigned int StepNum, urbg&& URBG)->decltype(Evaluate(State)){
			auto Eval = Evaluate(State);
			state NState = State;

			unsigned int StepCnt = 0;
			while(!Breaker(State, Eval)){
				Mutate(NState, URBG);
				auto NEval = Evaluate(NState);

				if(Eval < NEval || std::uniform_real_distribution<double>(0.0, 1.0)(URBG) < std::exp((NEval - Eval) / Temp(static_cast<double>(StepCnt) / StepNum))){
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
		template<typename state, typename evaluate, typename state_stepper>
		auto brute_force_search(state& State, evaluate&& Evaluate, state_stepper&& StateStepper)->decltype(Evaluate(State)){
			StateStepper.initialize(State);

			auto Eval = Evaluate(State);
			state NState = State;

			while(!StateStepper(NState)){
				auto NEval = Evaluate(NState);
				if(Eval < NEval){
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
