#ifndef HMLIB_EXCEPTION_INC
#define HMLIB_EXCEPTION_INC 102
#
#include <exception>
#include <string>
//#define throw_exception(EXCEPTION_TYPE, message) \
//	throw EXCEPTION_TYPE(message, __FILE__,__LINE__)
//#define throw_normal_exception(message) \
//	throw hmLib::exception::normal_exception(message, __FILE__, __LINE__)((void)0)
#ifndef  HMLIB_NOEXCEPT
#	define hmLib_assert(boolian,message) \
		if(!(boolian))throw hmLib::exception::exception(message, __FILE__, __LINE__)
#else
#	define hmLib_assert(boolian,message)
#endif
#ifndef  HMLIB_NOEXCEPT
#	define hmLib_throw(message) \
		throw hmLib::exception::exception(message, __FILE__, __LINE__)
#else
#	define hmLib_throw(message)
#endif

namespace hmLib{
	namespace exception{
		class exception : public std::exception {
		private:
			std::string Message;
			const char* File;
			int Line;
		public:
			exception(const std::string& Message_):Message(Message_){}
			exception(const std::string& Message_, const char* File_,const int Line_)
				:Message(Message_)
				,File(File_)
				,Line(Line_){
			}
			virtual ~exception() throw(){}
		public:
			virtual const char* what() const throw(){return Message.c_str();}
			const char* file() const{return File;}
			const int line() const{return Line;}
		};
	}
}
#
#endif
