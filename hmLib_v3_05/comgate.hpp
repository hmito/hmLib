#ifndef HMLIB_COMGATE_INC
#define HMLIB_COMGATE_INC 108
#
/*===comgate===
COMポートからのRS232c通信をサポート

comgate v1_08/130622 hmIto
	can_get/can_put関数がempty/fullを参照しないように変更
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
#		pragma comment(lib,"hmLib_v3_05/lib/Debug/comgate.lib")
#	else
#		pragma comment(lib,"hmLib_v3_05/lib/Release/comgate.lib")
#	endif
#endif
#ifndef HMLIB_GATE_INC
#	include <hmLib_v3_05/gate.hpp>
#endif
#ifndef HMLIB_GATEEXCEPTIONS_INC
#	include <hmLib_v3_05/gate_exceptions.hpp>
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
		virtual bool empty()override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return (port->loc()==0);
		}
		//受信可能状態
		virtual bool can_get()override{
//			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return is_open();
		}
		//複数byte受信
		virtual size_type get(char* str_,const size_type& size_)override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			for(size_type cnt=0;cnt<size_;++cnt){
				*(str_++)=port->getc1();
			}
			return size_;
		}
		//送信待データの有無
		virtual bool full()override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return false;
		}
		//送信可能状態
		virtual bool can_put()override{
//			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return is_open();
		}
		//複数byte送信
		virtual size_type put(const char* str_,const size_type& size_)override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			for(size_type cnt=0;cnt<size_;++cnt){
				port->putc1(*(str_++));
			}
			return size_;
		}
	public:
		comgate():port(0){return;}
		comgate(int _portnum,int _bps):port(0){open(_portnum,_bps);}
		virtual ~comgate(){if(is_open())close();}
	private:
	public:
		bool open(int _portnum,int _bps){
			hmLib_assert(!is_open(),gate_opened_exception,"comgate have already been opened.");
			portnum =_portnum;
			bps=_bps;
			port = new WinRS(portnum,bps,ifLine::crlf,"8N1");

			if(!(*port))close();

			return !(is_open());
		}
		bool is_open()const{return port!=0;}
		bool close(){
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");

			delete port;
			port=0;

			return false;
		}
	};
}
#endif
