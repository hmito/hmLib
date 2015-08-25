#ifndef HMLIB_COMGATE_INC
#define HMLIB_COMGATE_INC 201
#
/*===comgate===
COMポートからのRS232c通信をサポート

v2_00/131222 amby
	次世代型comgate	
	複数文字列を扱うget/putを廃止し、単数文字列に特化
	ただし、gets/putsで効率の問題で使いたい場合の手段は残す
	新たに、flowing/flush/is_openを追加
	flowing: !empty()に対応
	flush: 実装なし
	
comgate v1_09/131222 amby
	Flow制御の設定ができるように、open関数を変更
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
#if !(defined(HMLIB_NOLIB) || defined(HMLIB_COMGATE_NOLIB))
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib/lib/Debug/comgate.lib")
#	else
#		pragma comment(lib,"hmLib/lib/Release/comgate.lib")
#	endif
#endif
#ifndef HMLIB_GATE_INC
#	include <hmLib/gate.hpp>
#endif
#include "hmLib/comgate/winRS/winrs.h"

namespace hmLib{
	class comgate:public virtual gate{
	private:
		WinRS* port;
		int portnum;
		int bps;
	public://gate
		// ゲートが開いているかの確認
		virtual bool is_open() override{ return port != 0; }
		
		//受信可能状対かの確認
		virtual bool can_getc()override{

			hmLib_assert(is_open(), gate_not_opened_exception, "comgate have not been opened yet.");
			return (port->loc() != 0 && is_open());
		}
		// 受信が継続しているかの確認
		virtual bool flowing()override{
			hmLib_assert(is_open(), gate_not_opened_exception, "comgate have not been opened yet.");
			return (port->loc() != 0);
		}
		//1byte受信
		virtual char getc()override{
			hmLib_assert(is_open(), gate_not_opened_exception, "comgate have not been opened yet.");
			return port->getc1();
		}


		//送信可能状態かの確認
		virtual bool can_putc()override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return is_open();
		}
		//送信を無理やりやる。。コマンドなし
		virtual void flush()override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			return;
		}
		//1byte送信
		virtual void  putc(char c)override{
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");
			port->putc1(c);
			return;
		}

	public:
		comgate():port(0){return;}
		comgate(int _portnum,int _bps):port(0){open(_portnum,_bps);}
		comgate(int _portnum, int _bps, bool use_flowControl_) :port(0){ open(_portnum, _bps, use_flowControl_);}
		virtual ~comgate(){if(is_open())close();}
	private:
	public:
		bool open(int _portnum,int _bps, bool use_flowControl_ = false){
			hmLib_assert(!is_open(),gate_opened_exception,"comgate have already been opened.");
			portnum =_portnum;
			bps=_bps;
			port = new WinRS(portnum, bps, ifLine::crlf, "8N1", use_flowControl_);

			if(!(*port))close();

			return !(is_open());
		}

		bool close(){
			hmLib_assert(is_open(),gate_not_opened_exception,"comgate have not been opened yet.");

			delete port;
			port=0;

			return false;
		}
	};
}
#endif
