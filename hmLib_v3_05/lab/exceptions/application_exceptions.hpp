#ifndef HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC 100
#
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		//applicationに関わる例外処理
		class applicatiib_exception:public virtual exception{
			typedef exception excp_type;
		public:
			explicit applicatiib_exception(const std::string& Message_):excp_type(Message_.c_str()){}
			explicit applicatiib_exception(const char* Message_):excp_type(Message_){}
		};
		//実行引数の数が異常
		class invalid_arguments_number:public applicatiib_exception{
			typedef exception excp_type;
			typedef applicatiib_exception base_type;
		public:
			explicit invalid_arguments_number(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_arguments_number(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//実行引数の値が異常
		class invalid_argument_value:public applicatiib_exception{
			typedef exception excp_type;
			typedef applicatiib_exception base_type;
		public:
			explicit invalid_argument_value(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_argument_value(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
	}
}
#
#endif
