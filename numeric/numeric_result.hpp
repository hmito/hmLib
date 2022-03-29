#ifndef HMLIB_NUMERIC_NUMERICRESULT_INC
#define HMLIB_NUMERIC_NUMERICRESULT_INC 100
#
namespace hmLib{
	namespace numeric{
		struct count_result{
			bool success;
			unsigned int count;
			count_result() = default;
			count_result(bool success_,unsigned int count_):success(success_),count(count_){}
		};
	}
}
#
#endif
