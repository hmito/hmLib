#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#define HMLIB_EXCEPTIONS_EXCEPTION_INC 100
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
/*===exceptions::exception===
基本的なマクロ宣言
*/
#include <exception>
#include <string>

namespace hmLib{
	namespace exceptions{
		typedef std::exception exception;
		template<typename identifier_,typename base_type_=exception>
		struct exception_pattern :public base_type_ {
			typedef base_type_ base_type;
		public:
			explicit exception_pattern(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit exception_pattern(const char* Message_) :base_type(Message_) {}
		};
	}
}
#ifndef  HMLIB_NOEXCEPT
#	define _hmLib_exceptions_to_str(num) #num
#	define hmLib_exceptions_to_str(num) _hmLib_exceptions_to_str(num)
#	define hmLib_throwformat(str) (std::string(__FILE__)+"("+hmLib_exceptions_to_str(__LINE__)+") in "+__FUNCTION__+": "+str)
#	define hmLib_throw(except,str) throw except((hmLib_throwformat(str)).c_str())
#	define hmLib_thrownext(except,nextexcept,str) throw nextexcept((std::string(except.what())+"\n"+hmLib_throwformat(str)).c_str())
#	define hmLib_assert(condition,except,str) if(!(condition))hmLib_throw(except,str)
#	define hmLib_assert_any(condition,str) hmLib_assert(condition,hmLib::exceptions::exception,str)
#else
#	define _hmLib_exceptions_to_str(num)
#	define hmLib_exceptions_to_str(num)
#	define hmLib_throwformat(str)
#	define hmLib_throw(except,str)
#	define hmLib_thrownext(except,nextexcept,str)
#	define hmLib_assert(condition,except,str)
#	define hmLib_assert_any(condition,str)
#endif

#
#endif
