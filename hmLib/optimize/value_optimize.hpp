#ifndef HMLIB_OPTIMIZE_VALUEOPTIMIZE_INC
#define HMLIB_OPTIMIZE_VALUEOPTIMIZE_INC 100
#
namespace hmLib{
	namespace optimize{
		template<typename input_iterator, typename evaluator>
		input_iterator max_return_element(input_iterator Beg, input_iterator End, evaluator&& Eval){
			input_iterator MaxItr;
			auto Max = Eval(*Beg);
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
