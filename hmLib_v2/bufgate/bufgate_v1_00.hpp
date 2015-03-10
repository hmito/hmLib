#ifndef INC_HMLIB_BUFGATE
#define INC_HMLIB_BUFGATE 100
#
#ifndef INC_HMLIB_GATE
#	include "hmLib_v2/gate.hpp"
#endif
#ifndef INC_HMLIB_CIRCULAR
#	include "hmLib_v2/circular.hpp"
#endif
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
		bool open(itfGate& rGate_){
			//すでに開かれていたら、エラーを返す
			if(is_open())return true;

			pGate=&rGate_;

			return false;
		}
		bool close(){
			//すでに閉じられていたら、エラーを返す
			if(!is_open())return true;

			pGate=0;

			return false;
		}
		bool is_open()const{return pGate!=0;}
		void work(){
			if(!pGate->empty()){
				while(!pGate->empty() && pGate->can_get()){
					itfGate::size_type size=1;
					char tmp;
					if(pGate->get(&tmp,size))break;
					iBuf.push_back(tmp);
				}
			}else if(!oBuf.empty()){
				while(!oBuf.empty() && pGate->can_put()){
					itfGate::size_type size=1;
					pGate->put(oBuf.front(),size);
					oBuf.pop_front();
				}
			}
		}
		void work(int num_){
			if(!pGate->empty()){
				while(!pGate->empty() && pGate->can_get() && num_){
					itfGate::size_type size=1;
					char tmp;
					if(pGate->get(&tmp,size))break;
					iBuf.push_back(tmp);
					--num_;
				}
			}else if(!oBuf.empty()){
				while(!oBuf.empty() && pGate->can_put() && num_){
					itfGate::size_type size=1;
					pGate->put(oBuf.front(),size);
					oBuf.pop_front();
					--num_;
				}
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
				iBuf.front(*str_);
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
	//有限バッファGate
	class limbufgate:public itfGate{
	private:
		itfGate* pGate;
		hmLib::bicircular<char> oBuf;
		hmLib::bicircular<char> iBuf;
	public:
		limbufgate():pGate(0){return;}
		~limbufgate(){close();}
		bool open(itfGate& rGate_,unsigned int oBufSize,unsigned int iBufSize){
			//すでに開かれていたら、エラーを返す
			if(is_open())return true;

			pGate=&rGate_;

			//Buf確保
			oBuf.create(oBufSize);
			iBuf.create(iBufSize);

			return false;
		}
		bool close(){
			//すでに閉じられていたら、エラーを返す
			if(!is_open())return true;

			pGate=0;

			oBuf.release();
			iBuf.release();

			return false;
		}
		bool is_open()const{return pGate!=0;}
		void work(){
			if(!pGate->empty()){
				while(!pGate->empty()){
					iBuf.push_back(pGate->getc());
				}
			}else if(!oBuf.empty()){
				while(!oBuf.empty()){
					pGate->putc(oBuf.front());
					oBuf.pop_front();
				}
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
				iBuf.front(*str_);
				iBuf.pop_front();
				++str_;
			}
			return false;
		}
		//送信待データ
		virtual bool full(){return oBuf.full();}
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
