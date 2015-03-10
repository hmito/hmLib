#ifndef HMLIB_EXCEPTIONS_ACCESSEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_ACCESSEXCEPTIONS_INC 100
#
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		//アクセスしようとしているオブジェクトがのときに投げられる
		class invalid_access:public virtual exception{
			typedef exception excp_type;
		public:
			explicit invalid_access(const std::string& Message_):excp_type(Message_.c_str()){}
			explicit invalid_access(const char* Message_):excp_type(Message_){}
		};
		//配列要素外にアクセスしようとしたときに投げられる
		class out_of_range_access:public invalid_access{
			typedef exception excp_type;
			typedef invalid_access base_type;
		public:
			explicit out_of_range_access(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit out_of_range_access(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//nullptrにアクセスしようとしたときに投げられる
		class nullptr_dereferemce:public invalid_access{
			typedef exception excp_type;
			typedef invalid_access base_type;
		public:
			explicit nullptr_dereferemce(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit nullptr_dereferemce(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//不正な参照にアクセスしようとしている時
		class invalid_reference:public invalid_access{
			typedef exception excp_type;
			typedef invalid_access base_type;
		public:
			explicit invalid_reference(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_reference(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
	}
}
#
#endif
