#ifndef HMLIB_MATH_OPTIMIZE_INC
#define HMLIB_MATH_OPTIMIZE_INC 101
#
#include<random>
#include<utility>
#include"constants.hpp"
namespace hmLib{
	namespace optimize {
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
		/*!
		@brief	golden section search method
		Search the maximum value of Evaluate in the state [MinVal:MaxVal].
		@param [in]	Evaluate	Target function. The function need not to have a range where the slope becomes zero.
		@param [in]	MinVal	Minimum value of searching range.
		@param [in]	MaxVal	Maximum value of searching range.
		@param [in]	Error	Accuracy of the search. The return range will be less than Error.
		@return	Pair of the state (Min, Max) where Evaluate becomes maximum value.
				The distance of return pair Max-Min will be less than Error if the searching is sucessfully finished.
				If the searching of maximum value is failed, the return pair distance will be larger than Error.*/
		template<typename evaluate, typename state>
		std::pair<state, state> golden_section_search(evaluate&& Evaluate, state MinVal, state MaxVal, double Error) {
			auto MinEval = Evaluate(MinVal);
			auto MaxEval = Evaluate(MaxVal);

			//First division: MinSide
			auto LowerVal = (math::golden_ratio*MinVal + MaxVal) / (1 + math::golden_ratio);
			auto LowerEval = Evaluate(LowerVal);
			if (LowerEval < MinEval && LowerEval < MaxEval)return std::make_pair(MinVal, MaxVal);

			//Second division: MaxSide
			state UpperVal = (math::golden_ratio*LowerVal + MaxVal) / (1 + math::golden_ratio);
			auto UpperEval = Evaluate(UpperVal);

			while (MaxVal - MinVal > Error) {
				//Update Min 
				if (LowerEval < UpperEval) {
					MinVal = std::move(LowerVal);
					MinEval = std::move(LowerEval);

					LowerVal = std::move(UpperVal);
					LowerEval = std::move(UpperEval);

					UpperVal = (math::golden_ratio*LowerVal + MaxVal) / (1 + math::golden_ratio);
					UpperEval = Evaluate(UpperVal);
				}
				//Update Max
				else{
					MaxVal = std::move(UpperVal);
					MaxEval = std::move(UpperEval);

					UpperVal = std::move(LowerVal);
					UpperEval = std::move(LowerEval);

					LowerVal = (MinVal + math::golden_ratio*UpperVal) / (1 + math::golden_ratio);
					LowerEval = Evaluate(LowerVal);
				}
			}

			return std::make_pair(MinVal, MaxVal);
		}

		/*!
		@brief	golden section search method with small division method.
		Search the maximum value of Evaluate in the state [MinVal:MaxVal].
		@param [in]	Evaluate	Target function. The function can have a range where the slope becomes zero.
		@param [in]	MinVal	Minimum value of searching range.
		@param [in]	MaxVal	Maximum value of searching range.
		@param [in]	Error	Accuracy of the search. The return range will be less than Error.
		@return	Pair of the state (Min, Max) where Evaluate becomes maximum value.
				The distance of return pair Max-Min will be less than Error if the searching is sucessfully finished.
				If the searching of maximum value is failed, the return pair distance will be larger than Error.*/
		template<typename evaluate, typename state>
		std::pair<state, state> flatable_golden_section_search(evaluate&& Evaluate, state MinVal, state MaxVal, double Error) {
			auto MinEval = Evaluate(MinVal);
			auto MaxEval = Evaluate(MaxVal);

			//First division: MinSide
			auto LowerVal = (math::golden_ratio*MinVal + MaxVal) / (1 + math::golden_ratio);
			auto LowerEval = Evaluate(LowerVal);
			if (LowerEval < MinEval && LowerEval < MaxEval)return std::make_pair(MinVal, MaxVal);

			//Second division: MaxSide
			state UpperVal = (math::golden_ratio*LowerVal + MaxVal) / (1 + math::golden_ratio);
			auto UpperEval = Evaluate(UpperVal);

			while (MaxVal - MinVal > Error) {
				//Update Min 
				if (LowerEval < UpperEval) {
					MinVal = std::move(LowerVal);
					MinEval = std::move(LowerEval);

					LowerVal = std::move(UpperVal);
					LowerEval = std::move(UpperEval);

					UpperVal = (math::golden_ratio*LowerVal + MaxVal) / (1 + math::golden_ratio);
					UpperEval = Evaluate(UpperVal);
				}
				//Update Max
				else if (UpperEval < LowerEval) {
					MaxVal = std::move(UpperVal);
					MaxEval = std::move(UpperEval);

					UpperVal = std::move(LowerVal);
					UpperEval = std::move(LowerEval);

					LowerVal = (MinVal + math::golden_ratio*UpperVal) / (1 + math::golden_ratio);
					LowerEval = Evaluate(LowerVal);
				}
				//Same Case
				else {
					unsigned int Div = 11;

					unsigned int BestNoLower;
					unsigned int BestNoUpper;

					while (true) {
						BestNoLower = 0;
						BestNoUpper = 0;
						auto BestEval = MinEval;
						for (unsigned int i = 1; i < Div - 1; ++i) {
							state Val = (MinVal*(Div - i) + MaxVal*i) / Div;
							auto Eval = Evaluate(Val);

							if (BestEval < Eval) {
								BestNoLower = i;
								BestNoUpper = i;
								BestEval = Eval;
							} else if (BestEval == Eval) {
								BestNoUpper = i;
							}
						}
						if (BestEval < MaxEval) {
							BestNoLower = Div;
							BestNoUpper = Div;
							BestEval = MaxEval;
						} else if (BestEval == MaxEval) {
							BestNoUpper = Div;
						}

						if (BestNoLower != 0 || BestNoUpper != Div)break;

						if ((MaxVal - MinVal) / Div < Error) {
							return std::make_pair(MinVal, MaxVal);
						}
						Div = (Div - 1) * 10 + 1;
					}
					if (BestNoLower > 0)BestNoLower -= 1;
					if (BestNoUpper < Div)BestNoUpper += 1;
					auto NewMinVal = (MinVal*(Div - BestNoLower) + MaxVal*BestNoLower) / Div;
					auto NewMaxVal = (MinVal*(Div - BestNoUpper) + MaxVal*BestNoUpper) / Div;

					if (NewMinVal <= MinVal && NewMaxVal >= MaxVal) {
						return std::make_pair(MinVal, MaxVal);
					}
					MinVal = NewMinVal;
					MaxVal = NewMaxVal;
					MinEval = Evaluate(MinVal);
					MaxEval = Evaluate(MaxVal);

					//First division: MinSide
					auto LowerVal = (math::golden_ratio*MinVal + MaxVal) / (1 + math::golden_ratio);
					auto LowerEval = Evaluate(LowerVal);
					if (LowerEval < MinEval && LowerEval < MaxEval)return std::make_pair(MinVal, MaxVal);

					//Second division: MaxSide
					state UpperVal = (math::golden_ratio*LowerVal + MaxVal) / (1 + math::golden_ratio);
					auto UpperEval = Evaluate(UpperVal);
				}
			}

			return std::make_pair(MinVal, MaxVal);
		}
		
		//hill climing search
		template<typename evaluate, typename state, typename mutate, typename urbg, typename state_breaker>
		auto hill_climbing_search(evaluate&& Evaluate, state& State, mutate&& Mutate, state_breaker&& Breaker, urbg&& URBG)->decltype(Evaluate(State)){
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
		template<typename evaluate, typename state, typename mutate, typename temp, typename urbg, typename state_breaker>
		auto simulated_annealing_search(evaluate&& Evaluate, state& State, mutate&& Mutate, temp&& Temp, state_breaker&& Breaker, unsigned int StepNum, urbg&& URBG)->decltype(Evaluate(State)){
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
		template<typename evaluate, typename state, typename state_stepper>
		auto brute_force_search(evaluate&& Evaluate, state& State, state_stepper&& StateStepper)->decltype(Evaluate(State)){
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
