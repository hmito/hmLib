#ifndef HMLIB_DEBUGSTREAM_INC
#define HMLIB_DEBUGSTREAM_INC 100
#include <windows.h>
#include <iostream>
#ifndef HMLIB_STREAMBUFPATTERN_INC
#	include "streambuf_pattern.hpp"
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_03/lib/Debug/debugstream.lib")
#	else
#		pragma comment(lib,"hmLib_v3_03/lib/Release/debugstream.lib")
#	endif
#endif

namespace hmLib{
	class debugstreambuf:public pattern::nonbuf_streambuf_pattern{
	public://nonbuf_streambuf_pattern
		virtual void putc(char c){if(c!=0)OutputDebugString(&c);}
		virtual void flush(){return;}
		virtual char getc(){return 0;}
		virtual void unget(char c){return;}
		virtual pos_type seek(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){return 0;}
		virtual pos_type seek(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){return 0;}
	};
	class debugstream : public std::ostream{
	private:
		debugstreambuf* m_buf;
	public:
		debugstream():std::ostream(m_buf=new debugstreambuf()){return;}
		~debugstream(){delete m_buf;}
	};
	extern debugstream debugout;
}
#endif
