#ifndef HMLIB_OPTIMIZE_VALUEOPTIMIZE_INC
#define HMLIB_OPTIMIZE_VALUEOPTIMIZE_INC 100
#
namespace hmLib{
	namespace optimize{
		template<typename input_iterator, typename evaluator>
		input_iterator max_return_element(input_iterator Beg, input_iterator End, evaluator&& Eval){
			if(Beg == End)return End;

			auto Max = Eval(*Beg);
			input_iterator MaxItr = Beg;
			for(++Beg; Beg != End; ++Beg){
				auto Val = Eval(*Beg);
				if(Val > Max){
					MaxItr = Beg;
					Max = std::move(Val);
				}
			}

			return MaxItr;
		}
	}
}
#
#endif
