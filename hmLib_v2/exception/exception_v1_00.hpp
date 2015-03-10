#ifndef INC_HMLIB_EXCEPTION
#define INC_HMLIB_EXCEPTION 100
#
#include <exception>
#include <string>
//#define throw_exception(EXCEPTION_TYPE, message) \
//	throw EXCEPTION_TYPE(message, __FILE__,__LINE__)
//#define throw_normal_exception(message) \
//	throw hmLib::exception::normal_exception(message, __FILE__, __LINE__)((void)0)
#ifdef  HMLIB_ASSERT
#	define hmLib_assert_normal(boolian,message) \
		if(!(boolian))throw hmLib::exception::normal_exception(message, __FILE__, __LINE__)
#else
#	define hmLib_assert_normal(boolian,message)
#endif
#ifdef  HMLIB_ASSERT
#	define hmLib_throw_normal(message) \
		throw hmLib::exception::normal_exception(message, __FILE__, __LINE__)
#else
#	define hmLib_throw_normal(message)
#endif

namespace hmLib{
	namespace exception{
		class normal_exception : public std::exception {
		private:
			std::string Message;
			const char* File;
			int Line;
		public:
			normal_exception(const std::string& Message_):Message(Message_){}
			normal_exception(const std::string& Message_, const char* File_,const int Line_)
				:Message(Message_)
				,File(File_)
				,Line(Line_){
			}
			virtual ~normal_exception() throw(){}
		public:
			virtual const char* what() const throw(){return Message.c_str();}
			const char* file() const{return File;}
			const int line() const{return Line;}
		};
	}
}
#
#endif
