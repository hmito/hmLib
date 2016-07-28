#ifndef HMLIB_VCOM_THREADVIRTUALCOM_INC
#define HMLIB_VCOM_THREADVIRTUALCOM_INC 100
#
/*===thread_virtual_com===
スレッドを使って仮想的にcom通信を再現するクラス群
*/
#include<chrono>
#include<mutex>
#include<thread>
#include"virtual_comgate_abstruct.hpp"
#include"virtual_com_abstruct.hpp"
namespace hmLib{
	namespace vcom{
		//スレッドベース仮想com
		class fdx_virtual_com:public virtual_com_abstruct{
			typedef std::mutex mutex;
			typedef std::lock_guard<mutex> lock;
		private:
			class engine{
				typedef std::mutex mutex;
				typedef std::lock_guard<mutex> lock;
				typedef std::chrono::system_clock clock;
				typedef std::chrono::milliseconds milliseconds;
			private:
				mutex JoinMx;
				bool IsRequestJoin;
				unsigned int bps;
				fdx_virtual_com* ptr;
			private:
				bool is_request_join(void){
					lock Lock(JoinMx);
					return IsRequestJoin;
				}
			public:
				engine(fdx_virtual_com* ptr_,unsigned int bps_)
					:ptr(ptr_)
					,bps(bps_)
					,IsRequestJoin(false){
				}
				int operator()(void){
					clock::time_point time=clock::now();
					unsigned int SendNum=static_cast<unsigned int>(std::chrono::duration_cast<milliseconds>(clock::now()-time).count())*8*1000/bps;
					unsigned int SendPos=SendNum;
					char Buf[1024];

					while(true){
						//終了要求が行われていれば、終了
						if(is_request_join())break;

						//接続が行われていなければ、最新状態にして待機
						if(!ptr->is_connect()){
							SendPos=SendNum;
							continue;
						}

						//時間から送信可能な数を調べる
						SendNum=static_cast<unsigned int>(std::chrono::duration_cast<milliseconds>(clock::now()-time).count())*bps/8/1000;
						//送信可能な時間がたっていれば、その時間分だけ送信
						if(SendNum>SendPos){
							lock Lock(ptr->ComMx);
							unsigned int BufNum;
							BufNum=static_cast<unsigned int>(ptr->com1->com_send(Buf,SendNum-SendPos));
							ptr->com2->com_recv(Buf,BufNum);
							BufNum=static_cast<unsigned int>(ptr->com2->com_send(Buf,SendNum-SendPos));
							ptr->com1->com_recv(Buf,BufNum);
							SendPos=SendNum;
						}
					}
					return 0;
				}
				void request_join(void){
					lock Lock(JoinMx);
					IsRequestJoin=true;
				}
			};
		private:
			std::thread Thread;
			engine* Engine;
			mutex ComMx;
			virtual_comgate_abstruct* com1;
			virtual_comgate_abstruct* com2;
		public:
			fdx_virtual_com()
				:Engine(nullptr)
				,com1(nullptr)
				,com2(nullptr){
			}
			virtual ~fdx_virtual_com(){
				if(is_start())stop();
				if(is_connect())disconnect();
			}
			void start(unsigned int bps_){
				if(is_start())return;
				Engine=new engine(this,bps_);
				Thread=std::thread(std::ref(*Engine));
			}
			void stop(){
				if(!is_start())return;
				Engine->request_join();
				delete Engine;
				Engine=nullptr;
			}
			bool is_start()const{return Engine!=nullptr;}
		public: //virtual_com_abstruct
			//virtual_comgateを接続する
			void connect(virtual_comgate_abstruct* ptr)override{
				lock Lock(ComMx);
				if(com1==nullptr)com1=ptr;
				else if(com2==nullptr)com2=ptr;
			}
			//virtual_comgateを切断する
			void disconnect(virtual_comgate_abstruct* ptr=nullptr)override{
				lock Lock(ComMx);
				if(ptr==nullptr){
					com1=nullptr;
					com2=nullptr;
				}else{
					if(com1==ptr)com1=nullptr;
					else if(com2==ptr)com2=nullptr;
				}
			}
			//virtual_comgateが接続されているかを確認する
			bool is_connect(virtual_comgate_abstruct* ptr=nullptr)override{
				lock Lock(ComMx);
				if(ptr==nullptr){
					return(com1!=nullptr && com2!=nullptr);
				}else{
					return(com1==ptr || com2==ptr);
				}
			}
		};
	}
}
#
#endif
