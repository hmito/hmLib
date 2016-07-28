#ifndef HMLIB_VCOM_VIRTUALCOMGATE_INC
#define HMLIB_VCOM_VIRTUALCOMGATE_INC 100
#
/*===virtual_comgate===
スレッドを使って仮想的にcom通信を再現するクラス群
*/
#include<deque>
#include<mutex>
#include"virtual_com_abstruct.hpp"
#include"virtual_comgate_abstruct.hpp"
namespace hmLib{
	namespace vcom{
		//仮想comgate
		class virtual_comgate:public virtual_comgate_abstruct{
			typedef std::mutex mutex;
			typedef std::lock_guard<mutex> lock;
		public:
			virtual ~virtual_comgate(){
				if(is_open())close();
			}
		private:
			std::mutex Mx;
			std::deque<char> ibuf;
			std::deque<char> obuf;
		private:
			virtual_com_abstruct* ptr;
		public:
			bool is_open()const{return ptr!=nullptr;}
			void open(virtual_com_abstruct& VCom_){
				hmLib_assert(!is_open(),gate_opened_exception,"virtua_com have already been opened.");
				ptr=&VCom_;
				ptr->connect(this);
			}
			void close(){
				hmLib_assert(is_open(),gate_opened_exception,"virtua_com have not been opened.");
				ptr->disconnect(this);
				ptr=nullptr;
			}
		public: //gate
			//受信可能状態かの確認
			bool can_get()override{return is_open();}
			//受信可能データの有無
			bool empty()override{return ibuf.size()==0;}
			//複数byte受信　受信文字アドレスと、受信文字数が引数　実際の受信文字数が戻り値
			size_type get(char* str_,const size_type& size_)override{
				lock Lock(Mx);
				for(size_type i=0;i<size_;++i){
					if(ibuf.size()==0)return i;
					str_[i]=ibuf.front();
					ibuf.pop_front();
				}
				return size_;	
			}
			//送信可能状態かの確認
			bool can_put()override{return is_open();}
			//送信可能データの有無
			bool full()override{return false;}
			//複数byte送信　送信文字アドレスと、送信文字数が引数　実際の送信文字数が戻り値
			size_type put(const char* str_,const size_type& size_)override{
				lock Lock(Mx);
				for(size_type i=0;i<size_;++i){
					obuf.push_back(str_[i]);
				}
				return size_;
			}
		public: //virtaul_comgate_abstruct
			//comとして送信するデータを取得する
			size_type com_send(char* ptr_, size_type size_){
				lock Lock(Mx);
				for(size_type i=0;i<size_;++i){
					if(obuf.size()==0)return i;
					ptr_[i]=obuf.front();
					obuf.pop_front();
				}
				return size_;
			}
			//comとして受信させるデータを与える
			size_type com_recv(const char* ptr_, size_type size_){
				lock Lock(Mx);
				for(size_type i=0;i<size_;++i){
					ibuf.push_back(ptr_[i]);
				}
				return size_;
			}
		};
	}
}
#
#endif
