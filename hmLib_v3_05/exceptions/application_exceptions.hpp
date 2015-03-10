#ifndef HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC 100
#
#include"core_exceptions.hpp"
namespace hmLib{
	namespace exceptions{
		//applicationに関わる例外処理
		class applicatiib_exception:public virtual exception{};
		//実行引数の数が異常
		class invalid_arguments_number:public bad_exception,public applicatiib_exception{
			typedef exception excp_type;
			typedef bad_exception base_type;
		public:
			explicit invalid_arguments_number(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_arguments_number(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//実行引数の値が異常
		class invalid_argument_value:public virtual bad_exception{
			typedef exception excp_type;
			typedef bad_exception base_type;
		public:
			explicit invalid_argument_value(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_argument_value(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
	}
}
#
#endif
