#ifndef HMLIB_NUMERIC_HILLCLIMBMINIMA_INC
#define HMLIB_NUMERIC_HILLCLIMBMINIMA_INC 100
#
#include"../recur/breakable_recurse.hpp"
#include"../recur/stepper_category.hpp"
#include<cmath>
#include"../recur/breakable_recurse.hpp"
#include"evalue.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type,typename eval_type,typename urbg>
		struct hill_climbing_minima_stepper{
			//system_type = pair<Fn,Mutate>;
			using state_type = evalue<value_type,eval_type>;
			using stepper_category = hmLib::recur::naive_stepper_tag;
		public:
			hill_climbing_minima_stepper() = delete;
			explicit hill_climbing_minima_stepper(urbg URBG_):URBG(URBG_){}
			template<typename fnpair>
			void do_step(fnpair FnPair,  state_type& State)const{
				NState.v = State.v;
				FnPair.second(NState.v, URBG);
				NState.eval(FnPair.first);
				if(State.e > NState.e){
					State = std::move(NState);
				}
			}
		private:
			state_type NState;
			urbg URBG;
		};
		template<typename value_type,typename eval_type,typename urbg>
		struct simulated_annealing_minima_stepper{
			//system_type = pair<Fn,Mutate>;
			using state_type = evalue<value_type,eval_type>;
			using stepper_category = hmLib::recur::stepper_tag;
			using time_type = unsigned int;
		public:
			simulated_annealing_minima_stepper() = delete;
			explicit simulated_annealing_minima_stepper(unsigned int StepNum_, double MaxTemp_, double MinTemp_, urbg URBG_)
				:StepNum(StepNum_)
				,MaxTemp(MaxTemp_)
				,MinTemp(MinTemp_)
				,URBG(URBG_){
			}
			template<typename fnpair>
			void do_step(fnpair FnPair,  state_type& State, time_type& t)const{
				NState.v = State.v;
				FnPair.second(NState.v, URBG);
				NState.eval(FnPair.first);
				if(State.e > NState.e || std::uniform_real_distribution<double>(0.0, 1.0)(URBG) < std::exp((State.e - NState.e) / (1e-10 +  MinTemp + (MaxTemp-MinTemp)*(1.0 - static_cast<double>(t) / StepNum)))){
					State = std::move(NState);
				}
				++t;
			}
		private:
			state_type NState;
			urbg URBG;
			unsigned int StepNum;
			double MinTemp;
			double MaxTemp;
		};
		template<typename evaluate, typename mutate, typename state_type, typename breaker, typename observer>
		auto breakable_hill_climbing_minima(evaluate&& Fn, mutate&& Mutate, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = bisect_root_stepper<std::decay_t<value_type>,decltype(Fn(lowerval))>;
			using state_type = typename stepper::state_type;

			stepper Stepper;
			state_type State(Fn, lowerval, upperval);
			State.order();

			auto ans = hmLib::breakable_recurse(Stepper, Fn, State, maxitr, Brk, Obs);
			return std::make_pair(State, count_result(ans.first|Brk(State,ans.second),ans.second));
		}
		template<typename fn, typename value_type, typename breaker>
		auto breakable_bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, breaker Brk){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, Brk, hmLib::recur::null_observer());
		}
 		template<typename fn, typename value_type, typename error_type,typename observer>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr, observer Obs){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, erange_precision_breaker<error_type>(relerr,abserr), Obs);
		}
		template<typename fn, typename value_type, typename error_type>
		auto bisect_root(fn Fn, value_type lowerval, value_type upperval, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_bisect_root(Fn, lowerval, upperval, maxitr, erange_precision_breaker<error_type>(relerr,abserr));
		}

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
