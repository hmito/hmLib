#ifndef HMLIB_NUMERIC_NUMERICRESULT_INC
#define HMLIB_NUMERIC_NUMERICRESULT_INC 100
#
namespace hmLib{
	namespace numeric{
		template<typename state_type>
		struct count_result{
			bool success;
			unsigned int count;
			state_type state;
			count_result() = default;
			count_result(bool success_,unsigned int count_,state_type&& state_):success(success_),count(count_),state(std::move(state_)){}
			count_result(bool success_,unsigned int count_,const state_type& state_):success(success_),count(count_),state(state_){}
		};
	}
}
#
#endif
