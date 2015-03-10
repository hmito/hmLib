#ifndef INC_HMLIB_RSGATE
#define INC_HMLIB_RSGATE 101
#ifndef INC_HMLIB_GATESTREAM
#	include "hmLib_v2/gatestream.hpp"
#endif
#include "hmLib_v2/hmRSGate/winRS/winrs.h"
namespace hmstd{
	class RSgate:public virtual hmstd::basic_gate{
	private:
		WinRS* port;
		int portnum;
		int bps;
	public:
		RSgate(){
			port=0;
		}
		RSgate(int _portnum,int _bps){
			port=0;
			open(_portnum,_bps);
		}
		~RSgate(){close();}
		bool open(int _portnum,int _bps=9600){
			close();
			portnum =_portnum;
			bps=_bps;
			port = new WinRS(portnum,bps,ifLine::crlf,"8N1");

			if(!(*port))close();

			return gfail();
		}
		bool close(){
			if(port){
				delete port;
				port=0;
			}
			return false;
		}
		char getc(){
			if(port)return port->getc1();
			else return 0;
		}
		bool gfail(){
			if(port)return false;
			return true;
		}
		bool empty(){
			if(port)return (port->loc()==0);
			else return true;
		}
		void putc(char c){
			if(port)port->putc1(c);
		}
		void putuc(uchar c){
			if(port)port->putc1(static_cast<unsigned char>(c));
		}
		bool pfail(){
			if(port)return false;
			return true;
		}
	};
}
#endif
