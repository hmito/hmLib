#ifndef HMLIB_EXCEPTIONS_COREEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_COREEXCEPTIONS_INC 100
#
#include<string>
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		//何らかのリセット処理がないと再起不可能な例外
		class bad_exception:public virtual exception{
			typedef exception base_type;
		public:
			explicit bad_exception(const std::string& Message_):base_type(Message_.c_str()){}
			explicit bad_exception(const char* Message_):base_type(Message_){}
		};
		//要求がおかしいため失敗したが、オブジェクトは正常
		class invalid_request:public virtual exception{
			typedef exception base_type;
		public:
			explicit invalid_request(const std::string& Message_):base_type(Message_.c_str()){}
			explicit invalid_request(const char* Message_):base_type(Message_){}
		};
		//結果がおかしいため失敗したが、オブジェクトは正常
		class invalid_status:public virtual exception{
			typedef exception base_type;
		public:
			explicit invalid_status(const std::string& Message_):base_type(Message_.c_str()){}
			explicit invalid_status(const char* Message_):base_type(Message_){}
		};
	}
}

#
#endif
