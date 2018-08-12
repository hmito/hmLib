#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#define HMLIB_EXCEPTIONS_EXCEPTION_INC 100
#
#ifndef HMLIB_CONFIG_INC
#	include "../hmLib_config.h"
#endif
/*===exceptions::exception===
基本的なマクロ宣言
*/
#include"../static_buffer.hpp"
#include <exception>
#include <string>

namespace hmLib{
	namespace exceptions{
		namespace details{
			struct exception_identifier{};
			using buffer = static_buffer<char, 4096, exception_identifier>;
		}
		struct exception : public std::exception{
		private:
			details::buffer Buffer;
		public:
			exception()noexcept{}
			explicit exception(const char* What_)noexcept{
				auto Itr = Buffer.begin();
				while(*What_ != 0 && Itr != Buffer.end()){
					*(Itr++) = *(What_++);
				}
			}
			exception(const exception&) = default;
			exception& operator=(const exception&) = default;
		public:
			const char* what() const noexcept override{ return Buffer.cbegin(); }
		};
		template<typename identifier_,typename base_type_=exception>
		struct exception_pattern :public base_type_ {
			typedef base_type_ base_type;
		public:
			explicit exception_pattern(const std::string& What_)noexcept :base_type(What_.c_str()) {}
			explicit exception_pattern(const char* What_)noexcept :base_type(What_) {}
		};
	}
}
#define _hmLib_exceptions_to_str(num) #num
#define hmLib_exceptions_to_str(num) _hmLib_exceptions_to_str(num)
#define hmLib_throwformat(str) (std::string(__FILE__)+"("+hmLib_exceptions_to_str(__LINE__)+") in "+__FUNCTION__+": "+str)
#define hmLib_throw(except,str) throw except((hmLib_throwformat(str)).c_str())
#define hmLib_thrownext(except,nextexcept,str) throw nextexcept((std::string(except.what())+"\n"+hmLib_throwformat(str)).c_str())
#ifndef  HMLIB_NOEXCEPT
#	define hmLib_assert(condition,except,str) if(!(condition))hmLib_throw(except,str)
#	define hmLib_assert_any(condition,str) hmLib_assert(condition,hmLib::exceptions::exception,str)
#else
#	define hmLib_assert(condition,except,str)
#	define hmLib_assert_any(condition,str)
#endif

#
#endif
