#ifndef INC_HMLIB_COM_GATE
#define INC_HMLIB_COM_GATE 100
#ifndef INC_HMLIB_GATE
#	include "hmLib_v2/gate.hpp"
#endif
#include "hmLib_v2/comgate/winRS/winrs.h"
namespace hmLib{
	class comgate:public virtual gate{
	private:
		WinRS* port;
		int portnum;
		int bps;
	public://gate
		//受信待データの有無
		virtual bool empty(){return (port->loc()==0);}
		//受信可能状態
		virtual bool can_get(){return !empty();}
		//複数byte受信
		virtual size_type get(char* str_,const size_type& size_){
			size_type ans;
			if(port->listen(str_,ans)!=0)return 0;
			return ans;
		}
		//送信待データの有無
		virtual bool full(const size_type& size_=1){return false;}
		//送信可能状態
		virtual bool can_put(){return !full();}
		//複数byte送信
		virtual size_type put(const char* str_,const size_type& size_){
			if(port->putN(str_,size_)!=0)return 0;
			return size_;
		}
	public:
		comgate():port(0){return;}
		comgate(int _portnum,int _bps):port(0){open(_portnum,_bps);}
		~comgate(){close();}
		bool open(int _portnum,int _bps){
			close();
			portnum =_portnum;
			bps=_bps;
			port = new WinRS(portnum,bps,ifLine::crlf,"8N1");

			if(!(*port))close();

			return !(is_open());
		}
		bool is_open()const{return port!=0;}
		bool close(){
			if(port){
				delete port;
				port=0;
			}
			return false;
		}
	};
}
#endif
