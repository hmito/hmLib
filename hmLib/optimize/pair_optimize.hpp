#ifndef HMLIB_OPTIMIZE_PAIROPTIMIZE_INC
#define HMLIB_OPTIMIZE_PAIROPTIMIZE_INC 100
#
#include<utility>
namespace hmLib{
	namespace optimize{
		namespace pair_breakers{
			template<typename state>
			struct close_breaker{
			private:
				double Dif;
				unsigned int MaxStep;
				unsigned int Step;
			public:
				close_breaker(double Dif_, unsigned int MaxStep_) :Dif(Dif_), MaxStep(MaxStep_), Step(0){}
				void clear(){ Step = 0; }
				unsigned int get_max_step()const{ return MaxStep; }
				unsigned int get_step()const{ return Step; }
				bool operator()(const state& First, const state& Second){
					if(Step >= MaxStep)return true;
					if(Second - First < Dif)return true;
					++Step;
					return false;
				}
			};
		}

		//four section search
		template<typename evaluate, typename pair_beaker>
		std::pair<double, double> four_section_search(evaluate&& Evaluate, double Min, double Max, pair_beaker&& Cond){
			double Q1 = (Min * 3 + Max) / 4;
			double Q3 = (Min + Max * 3) / 4;
			double EvalMin = Evaluate(Min);
			double EvalQ1 = Evaluate(Q1);
			double EvalC = Evaluate((Min + Max) / 2.0);
			double EvalQ3 = Evaluate(Q3);
			double EvalMax = Evaluate(Max);

			if(EvalMin >= EvalQ1 && EvalMin >= EvalC && EvalMin >= EvalQ3 && EvalMin >= EvalMax){
				Max = Q1;
				while(!Cond(Min, Max)){
					EvalC = Evaluate((Min + Max) / 2);

					if(EvalMin < EvalC)break;
					Max = (Min + Max) / 2;
				}
			} else if(EvalMax >= EvalQ1 && EvalMax >= EvalC && EvalMax >= EvalQ3){
				Min = Q3;
				while(!Cond(Min, Max)){
					EvalC = Evaluate((Min + Max) / 2);

					if(EvalMax < EvalC)break;
					Min = (Min + Max) / 2;
				}
			} else if(EvalC > EvalQ1 && EvalC > EvalQ3){
				Min = Q1;
				Max = Q3;
			} else if(EvalQ1>EvalQ3){
				Max = (Min + Max) / 2.0;
				EvalC = EvalQ1;
			} else{
				Min = (Min + Max) / 2.0;
				EvalC = EvalQ3;
			}

			while(!Cond(Min, Max)){
				Q1 = (Min * 3 + Max) / 4;
				Q3 = (Min + Max * 3) / 4;
				EvalQ1 = Evaluate(Q1);
				EvalQ3 = Evaluate(Q3);

				if(EvalC > EvalQ1 && EvalC > EvalQ3){
					Min = Q1;
					Max = Q3;
				} else if(EvalQ1>EvalQ3){
					Max = (Min + Max) / 2.0;
					EvalC = EvalQ1;
				} else{
					Min = (Min + Max) / 2.0;
					EvalC = EvalQ3;
				}
			}

			return std::make_pair(Min, Max);
		}
	}
}
#
#endif
