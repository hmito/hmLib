#ifndef HMLIB_GATES_BUFGATE_INC
#define HMLIB_GATES_BUFGATE_INC 200
#
/*===bufgate===
バッファにデータを一時的にため、operator()で送受信するgateクラスです。
bufgate_v2_00/130422 hmIto
	limbufgateを廃止
	exceptionに対応
		開かれていないgateへの処理要求には、例外を返すように変更
bufgate v1_02/130329 hmIto
	一部変数名の記述ミスでコンパイルエラーが出ていた問題を修正
bufgate v1_01/130328 hmIto
	work関数をoperator()(unsigned int GetMaxNum,unsigned int PutMaxNum)に変更しました。
*/
#include"gate.hpp"
#include"exceptions.hpp"
#include <deque>
#include <vector>
namespace hmLib{
	//バッファGate
	class bufgate:public gate{
	private:
		gate* pGate;
		std::deque<char> oBuf;
		std::deque<char> iBuf;
	public:
		bufgate():pGate(0){return;}
		~bufgate(){close();}
		bool open(gate& rGate_){
			hmLib_assert(is_open(),gates::opened_exception,"bufgate have already been opened.");

			pGate=&rGate_;

			return false;
		}
		bool close(){
			hmLib_assert(is_open(),gates::not_opened_exception,"bufgate have not been opened yet.");

			pGate=0;

			return false;
		}
		bool is_open()const{return pGate!=0;}
		void operator()(void){
			hmLib_assert(is_open(),gates::not_opened_exception,"bufgate have not been opened yet.");
			try{
				if(pGate->can_getc()){
					while(pGate->can_getc()){
						gate::size_type size=1;
						char tmp;
						if(pGate->gets(&tmp,size))break;
						iBuf.push_back(tmp);
					}
				}else if(!oBuf.empty()){
					while(!oBuf.empty() && pGate->can_putc()){
						gate::size_type size=1;
						pGate->puts(&oBuf.front(),size);
						oBuf.pop_front();
					}
				}
			}catch(const gates::not_opened_exception& Except_){
				hmLib_thrownext(Except_,gates::not_opened_exception,"gate connected with bufgate is not opened.");
			}
		}
		void operator()(unsigned int GetMaxNum_,unsigned int PutMaxNum_){
			hmLib_assert(is_open(),gates::not_opened_exception,"bufgate have not been opened yet.");
			try{
				if(pGate->can_getc()){
					while( pGate->can_getc() && GetMaxNum_){
						gate::size_type size=1;
						char tmp;
						if(pGate->gets(&tmp,size))break;
						iBuf.push_back(tmp);
						--GetMaxNum_;
					}
				}else if(!oBuf.empty()){
					while(!oBuf.empty() && pGate->can_putc() && PutMaxNum_){
						gate::size_type size=1;
						pGate->puts(&oBuf.front(),size);
						oBuf.pop_front();
						--PutMaxNum_;
					}
				}
			}catch(const gates::not_opened_exception& Except_){
				hmLib_thrownext(Except_,gates::not_opened_exception,"gate connected with bufgate is not opened.");
			}		
		}
		unsigned int psize()const{return oBuf.size();}
		unsigned int gsize()const{return iBuf.size();}
		void pclear(){oBuf.clear();}
		void gclear(){iBuf.clear();}
	public://itfGate
		//受信待データ
		virtual bool empty(){return iBuf.empty();}
		//受信可能
		virtual bool can_get(){return !empty();}
		//複数byte受信
		virtual bool get(char* str_,size_type& size_){
			size_type max=size_;
			for(size_=0;size_<max;++size_){
				if(!can_get())return true;
				*str_=iBuf.front();
				iBuf.pop_front();
				++str_;
			}
			return false;
		}
		//送信待データ
		virtual bool full(){return false;}
		//送信可能
		virtual bool can_put(){return !full();}
		//複数byte送信
		virtual bool put(const char* str_,size_type& size_){
			size_type max=size_;
			for(size_=0;size_<max;++size_){
				if(!can_put())return true;
				oBuf.push_back(*str_);
				++str_;
			}
			return false;
		}
	};
}
#
#endif
