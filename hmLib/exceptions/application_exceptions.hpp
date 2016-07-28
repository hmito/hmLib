#ifndef HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_APPLICATIONEXCEPTIONS_INC 100
#
#include"exception.hpp"
namespace hmLib{
	namespace application_exceptions {
		//applicationに関わる例外処理
		class application_exception :public exceptions::exception {
			typedef exceptions::exception base_type;
		public:
			explicit application_exception(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit application_exception(const char* Message_) :base_type(Message_) {}
		};
		//実行引数の数が異常
		class invalid_arguments_number :public application_exception {
			typedef application_exception base_type;
		public:
			explicit invalid_arguments_number(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit invalid_arguments_number(const char* Message_) :base_type(Message_) {}
		};
		//実行引数の値が異常
		class invalid_argument_value :public application_exception {
			typedef application_exception base_type;
		public:
			explicit invalid_argument_value(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit invalid_argument_value(const char* Message_) :base_type(Message_) {}
		};
	}
}
#
#endif
