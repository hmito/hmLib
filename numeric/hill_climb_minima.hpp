#ifndef HMLIB_NUMERIC_HILLCLIMBMINIMA_INC
#define HMLIB_NUMERIC_HILLCLIMBMINIMA_INC 100
#
#include<cmath>
#include"../recur/breakable_recurse.hpp"
#include"evalue.hpp"
#include"numeric_result.hpp"
#include"numeric_system.hpp"
#include"breaker/minima_stagnant_breaker.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type,typename eval_type,typename urbg>
		struct hill_climbing_minima_stepper{
			//system_type = pair<Fn,Mutate>;
			using state_type = evalue<value_type,eval_type>;
		public:
			hill_climbing_minima_stepper() = delete;
			explicit hill_climbing_minima_stepper(urbg URBG_):URBG(URBG_){}
			template<typename fn_type,typename generator_type>
			void do_step(generator_system<fn_type,generator_type> Sys, state_type& State){
				state_type NState = State;
				Sys.generator(NState.v, URBG);
				NState.eval(Sys.fn);
				if(State.e > NState.e){
					State = std::move(NState);
				}
			}
		private:
			urbg URBG;
		};
		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename breaker, typename observer>
		auto breakable_hill_climbing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, urbg URBG, breaker Brk, observer Obs){
			using stepper = hill_climbing_minima_stepper<std::decay_t<value_type>,decltype(Fn(inival)),urbg>;
			using state_type = typename stepper::state_type;
			using system_type = generator_system<std::decay_t<evaluate>,std::decay_t<mutate>>;
			stepper Stepper(URBG);
			state_type State(inival,Fn(inival));
			system_type Sys(std::forward<evaluate>(Fn),std::forward<mutate>(Mutate)); 

			auto ans = hmLib::breakable_recurse(Stepper, Sys, State, maxitr, Brk, Obs);
			if(!(ans.first||Brk(State,ans.second))){
				return(make_step_result(ans.second,State));
			}else{
				return(make_step_result(ans.second,State,State));
			}
		}
		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename breaker>
		auto breakable_hill_climbing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, urbg URBG, breaker Brk){
			return breakable_hill_climbing_minima(Fn, Mutate, inival, maxitr, URBG, Brk, hmLib::recur::null_observer());
		}
 		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename observer>
		auto hill_climbing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, urbg URBG, unsigned int stableitr, observer Obs){
			return breakable_hill_climbing_minima(Fn, Mutate, inival, maxitr, URBG, make_minima_stagnant_breaker(Fn(inival),stableitr), Obs);
		}
		template<typename evaluate, typename mutate, typename value_type, typename urbg>
		auto hill_climbing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, urbg URBG, unsigned int stableitr){
			return breakable_hill_climbing_minima(Fn, Mutate, inival, maxitr, URBG, make_minima_stagnant_breaker(Fn(inival),stableitr));
		}

		template<typename value_type,typename eval_type,typename urbg>
		struct simulated_annealing_minima_stepper{
			using state_type = evalue<value_type,eval_type>;
			using time_type = unsigned int;
		public:
			simulated_annealing_minima_stepper() = delete;
			explicit simulated_annealing_minima_stepper(unsigned int StepNum_, double MaxTemp_, double MinTemp_, urbg URBG_)
				: StepNum(StepNum_)
				, MaxTemp(MaxTemp_)
				, MinTemp(MinTemp_)
				, URBG(URBG_){
			}
			template<typename fn_type,typename generator_type>
			void do_step(generator_system<fn_type,generator_type> Sys, state_type& State, time_type& t){
				state_type NState = State;
				Sys.generator(NState.v, URBG);
				NState.eval(Sys.fn);
				if(State.e > NState.e 
					|| std::uniform_real_distribution<double>(0.0, 1.0)(URBG) < std::exp((State.e - NState.e) / (1e-10 +  MinTemp + (MaxTemp-MinTemp)*(1.0 - static_cast<double>(t) / StepNum)))){
					State = std::move(NState);
				}
				++t;
			}
		private:
			urbg URBG;
			unsigned int StepNum;
			double MinTemp;
			double MaxTemp;
		};
		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename breaker, typename observer>
		auto breakable_simulated_annealing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, unsigned int StepNum, double MaxTemp, double MinTemp, urbg URBG, breaker Brk, observer Obs){
			using stepper = hill_climbing_minima_stepper<std::decay_t<value_type>,decltype(Fn(inival)),urbg>;
			using state_type = typename stepper::state_type;
			using system_type = generator_system<std::decay_t<evaluate>,std::decay_t<mutate>>;
			stepper Stepper(StepNum,MaxTemp,MinTemp,URBG);
			state_type State(inival,Fn(inival));
			system_type Sys(std::forward<evaluate>(Fn),std::forward<mutate>(Mutate)); 

			auto ans = hmLib::breakable_recurse(Stepper, Sys, State, maxitr, Brk, Obs);
			if(!(ans.first||Brk(State,ans.second))){
				return(make_step_result(ans.second,State));
			}else{
				return(make_step_result(ans.second,State,State));
			}
		}
		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename breaker>
		auto breakable_simulated_annealing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, unsigned int StepNum, double MaxTemp, double MinTemp, urbg URBG, breaker Brk){
			return breakable_simulated_annealing_minima(Fn, Mutate, inival, maxitr, StepNum,MaxTemp,MinTemp,URBG, Brk, hmLib::recur::null_observer());
		}
		template<typename evaluate, typename mutate, typename value_type, typename urbg, typename observer>
		auto simulated_annealing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr,unsigned int StepNum, double MaxTemp, double MinTemp,  urbg URBG, unsigned int stableitr, observer Obs){
			auto Brk = make_minima_stagnant_breaker(Fn(inival),stableitr);
			return breakable_simulated_annealing_minima(Fn, Mutate, inival, maxitr, StepNum,MaxTemp,MinTemp,URBG, Brk, Obs);
		}
		template<typename evaluate, typename mutate, typename value_type, typename urbg>
		auto simulated_annealing_minima(evaluate&& Fn, mutate&& Mutate, value_type inival, unsigned int maxitr, unsigned int StepNum, double MaxTemp, double MinTemp, urbg URBG, unsigned int stableitr){
			auto Brk = make_minima_stagnant_breaker(Fn(inival),stableitr);
			return breakable_simulated_annealing_minima(Fn, Mutate, inival, maxitr, StepNum,MaxTemp,MinTemp,URBG, Brk);
		}
	}
}
#
#endif
