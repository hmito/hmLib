#ifndef HMLIB_EXCEPTIONS_IOEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_IOEXCEPTIONS_INC 100
#
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		//バッファがoverflowした場合
		class io_exception:public virtual exception{
			typedef exception base_type;
		public:
			explicit io_exception(const std::string& Message_):excp_type(Message_.c_str()){}
			explicit io_exception(const char* Message_):excp_type(Message_){}
		};
		//バッファがoverflowした場合
		class io_overflow:public io_exception{
			typedef exception excp_type;
			typedef invalid_status base_type;
		public:
			explicit io_overflow(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit io_overflow(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//バッファがeofに達した場合
		class io_end_of_file:public io_exception{
			typedef exception excp_type;
			typedef invalid_status base_type;
		public:
			explicit io_end_of_file(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit io_end_of_file(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//streamが開かれていない場合
		class io_not_opened:public io_exception{
			typedef exception excp_type;
			typedef invalid_request base_type;
		public:
			explicit io_not_opened(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit io_not_opened(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//streamが開かれている場合
		class io_opened:public io_exception{
			typedef exception excp_type;
			typedef invalid_request base_type;
		public:
			explicit io_opened(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit io_opened(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
	}
}
#
#endif
