#ifndef HMLIB_DEBUGSTREAM_INC
#define HMLIB_DEBUGSTREAM_INC 101
#
/*===debugstream===
VSのデバッグ用出力窓に文字列を出力する事ができます。

debugstream v1_01/130328 hmIto
	streambuf_interfaceに対応
*/
#include <windows.h>
#include <iostream>
#ifndef HMLIB_STREAMBUFINTERFACE_INC
#	include <hmLib_v3_04/streambuf_interface.hpp>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/debugstream.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/debugstream.lib")
#	endif
#endif

namespace hmLib{
	class debugstreambuf:public streambuf_interface{
	public://nonbuf_streambuf_pattern
/*		virtual void putc(char c){if(c!=0)OutputDebugString(&c);}
		virtual void flush(){return;}
		virtual char getc(){return 0;}
		virtual void unget(char c){return;}
		virtual pos_type seek(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){return 0;}
		virtual pos_type seek(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){return 0;}
*/
	protected://common virtual functions
		pos_type iobuf_seekpos(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)override{return 0;}
		pos_type iobuf_seekoff(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)override{return 0;}
	protected://streambuf_interface ibuf
		char ibuf_overflow()override{return 0;}
		std::streamsize ibuf_gets(char* str,std::streamsize size)override{return 0;}
		bool ibuf_unget(char c)override{return false;}
		char ibuf_peek()override{return 0;}
	protected://streambuf_interface obuf
		bool obuf_overflow(char c)override{OutputDebugString(&c);return false;}
		std::streamsize obuf_puts(const char* str,std::streamsize size)override{
			for(std::streamsize cnt=0;cnt<size;++cnt)obuf_overflow(str[cnt]);
			return size;
		}
		bool obuf_flush()override{return false;}
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
