#ifndef HMLIB_COMGATE_INC
#define HMLIB_COMGATE_INC 107
#
/*===comgate===
COMポートからのRS232c通信をサポート

comgate v1_07/130406 hmIto
	liste/talk関数では正常に取得できない場合があったので、getc1/putc1に変更
comgate v1_06/130324 hmIto
	size_typeがstd::streamsizeに変更になったのに対応
		get関数の内部変数をsize_typeからunsigned intに変更
		put関数の内部でstatic_castを導入
comgate v1_05/130310 hmIto
	full関数を引数なしに修正
*/
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/comgate.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/comgate.lib")
#	endif
#endif
#ifndef HMLIB_GATE_INC
#	include <hmLib_v3_04/gate.hpp>
#endif
#include "comgate/winRS/winrs.h"

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
			for(size_type cnt=0;cnt<size_;++cnt){
				*(str_++)=port->getc1();
			}
			return size_;
		}
		//送信待データの有無
		virtual bool full(){return false;}
		//送信可能状態
		virtual bool can_put(){return !full();}
		//複数byte送信
		virtual size_type put(const char* str_,const size_type& size_){
			for(size_type cnt=0;cnt<size_;++cnt){
				port->putc1(*(str_++));
			}
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
