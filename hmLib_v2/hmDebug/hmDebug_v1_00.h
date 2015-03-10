#ifndef INC_HMLIB_HMDEBUG
#define INC_HMLIB_HMDEBUG 100
#ifndef INC_HMLIB_HMSTREAMBUF
#	include <hmstreambuf.hpp>
#endif
#include <windows.h>
#include <iostream>
namespace hmstd{
	class debugstreambuf : public hmstd::hmNBstreambuf{
	public:
		virtual void putc(char c){
			if(c!=0)OutputDebugString(&c);
			return;
		}
		virtual char getc(){return 0;}
		virtual void unget(char c){return;}
	};
	class debugstream : public std::ostream{
	private:
		debugstreambuf m_buf;
	public:
		debugstream(void):std::ostream(&m_buf){return;}
	};
	extern debugstream dout;
	template<class OStream>
	class debugflow_{
		friend debugflow_<OStream>& endl(debugflow_<OStream>& debugf);
	private:
		OStream* osp;
	protected:
		void copy(const debugflow_<OStream>& _debugf){osp=_debugf.osp;}
	public:
		debugflow_<OStream>(){osp=0;}
		debugflow_<OStream>(OStream& _os){open(_os);}
		debugflow_<OStream>(const debugflow_<OStream>& _debugf){copy(_debugf);}
		virtual ~debugflow_<OStream>(){close();}
		void open(OStream& _os){osp=&_os;}
		void close(){osp=0;}
		bool is_open()const{return osp==0;}
		const debugflow_<OStream>& operator=(const debugflow_<OStream>& _debugf){
			if(this!=&_debugf)copy(_debugf);
			return *this;
		}
		template<class T>
		void put(const T& _t){
			#ifdef _DEBUG
				(*osp)<<_t;
			#endif
		}
		template<class T>
		debugflow_<OStream>& operator<<(const T& _t){
			put(_t);
			return *this;
		}
		debugflow_<OStream>& operator<<(debugflow_<OStream>& (*p_debugflow_MF)(debugflow_<OStream>&)){return (*p_debugflow_MF)(*this);}
	};
	typedef debugflow_<std::ostream> debugflow;
}
namespace hm{
	extern hmstd::debugflow d_dout;
	extern hmstd::debugflow d_cout;
}
namespace std{
	template<class OStream>
	hmstd::debugflow_<OStream>& endl(hmstd::debugflow_<OStream>& debugf){
		debugf.put("\n");
		return debugf;
	}
}
#endif
