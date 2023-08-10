#ifndef HMLIB_RECUR_BREAKRETURNTYPE_INC
#define HMLIB_RECUR_BREAKRETURNTYPE_INC 100
#
namespace hmLib{
	template<typename time_type>
	struct break_return_type{
		bool is_break;
		time_type t;
		unsigned int c;
		break_return_type()=delete;
		break_return_type(bool is_break_, time_type t_, size_t c_)
			:is_break(is_break_)
			,t(t_)
			,c(c_){
		}
	};
}
#
#endif
