#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#define HMLIB_EXCEPTIONS_EXCEPTION_INC 100
#
/*===exceptions::exception===
基本的なマクロ宣言
*/
#include <exception>
#include <string>

namespace hmLib{
	namespace exceptions{
		typedef std::exception exception;
	}
}
#ifndef  HMLIB_NOEXCEPT
#	define _hmLib_exceptions_to_str(num) #num
#	define hmLib_exceptions_to_str(num) _hmLib_exceptions_to_str(num)
#	define hmLib_throwformat(str) (std::string(__FILE__)+"("hmLib_exceptions_to_str(__LINE__)") in "__FUNCTION__": "+str)
#	define hmLib_throw(category,str) throw category((hmLib_throwformat(str)).c_str())
#	define hmLib_thrownext(except,category,str) throw category((std::string(except.what())+"\n"+hmLib_throwformat(str)).c_str())
#	define hmLib_assert(condition,category,str) if(!(condition))hmLib_throw(category,str)
#	define hmLib_assert_any(condition,str) hmLib_assert(condition,hmLib::exceptions::exception,str)
#else
#	define _hmLib_exceptions_to_str(num)
#	define hmLib_exceptions_to_str(num)
#	define hmLib_throwformat(str)
#	define hmLib_throw(category,str)
#	define hmLib_thrownext(except,category,str)
#	define hmLib_assert(condition,category,str)
#	define hmLib_assert_any(condition,str)
#endif

#
#endif
