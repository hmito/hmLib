#ifndef HMLIB_DXLIB_EXCEPTIONS_INC
#define HMLIB_DXLIB_EXCEPTIONS_INC 100
#
#ifndef HMLIB_EXCEPTIONS_EXCEPTION
#	include<hmLib/exceptions.hpp>
#endif
namespace hmLib{
	namespace dxlib{
		namespace exceptions{
			struct dxlib_exception :public virtual hmLib::exceptions::exception{
				typedef exception excp_type;
			public:
				explicit dxlib_exception(const std::string& Message_) :excp_type(Message_.c_str()){}
				explicit dxlib_exception(const char* Message_) :excp_type(Message_){}
			};
		}
	}
}
#
#endif
