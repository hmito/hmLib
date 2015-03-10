#ifndef HMLIB_INQUIRY_INC
#define HMLIB_INQUIRY_INC 100
#include<memory>
#include<mutex>
#include<functional>
#include<vector>
#include"mutex_proxy.hpp"
#include"../exception.hpp"

/*===inquiry===
値を能動的に参照取得するための共通インターフェースを提供する
	安全のため、いずれのinquiryクラスパターンでもconstな制御しか許されていない。
	接続相手の値を能動的に制御するためには、signal/slotパターンの利用を推奨している。
	高頻度のsignal/slot発信を回避する目的であれば、inquiry_signalの利用を推奨している。
	
提供クラス
	inquiryクラス
		あらゆるオブジェクトと接続することができる。
		operator()(void)で、接続したオブジェクトをconstで取得することができる。
		lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。
	range_inquiryクラス
		begin/end関数を利用できるオブジェクトと接続することができる。
		begin/end関数で、iterator_を取得できる。
		lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。
	unique_contactsクラス
		コンタクトを複数保持し、destructor呼び出し時にdisconnectを呼び出すようなクラス。
inquiry:v1_00/130328 hmIto
	inquiryの定義を変更
		能動型signalとしてのinquiryから、const参照値取得クラスとしてのinquiryへ定義変更。
	inquiry追加
		operator()で値参照機能を提供するクラス
		mutexインターフェースを備える
	range_inquiry追加
		指定のiteratorのbegin/endを提供するクラス。
		mutexインターフェースを備える
	unique_connections追加
		connectionクラスを保持し、destructorでdisconnectを呼び出す。
inquiry:v0_03/130324 hmIto
	inquiryにdefault_valueを追加
inquiry:v0_02/130224 hmIto
	inquiry_astruct::connection(inquiry_abstruct*)の変数名が間違っていた致命的バグを修正
inquiry:v0_01/130224 hmIto
	inquiryについては提供開始
	range_inquiryは、iterator_proxy待ち
*/
namespace hmLib{
	namespace inquiries{
		//connectionシステムを提供する機能
		class inquiry_abstruct{
		public:
			class connection{
			private:
				inquiry_abstruct* ptr;
			private:
				connection();
				connection(const connection& My_);
				const connection& operator=(const connection& My_);
			public:
				connection(inquiry_abstruct* ptr_):ptr(ptr_){}
				connection(connection&& My_):ptr(nullptr){
					std::swap(ptr,My_.ptr);
					ptr->ptr=this;
				}
				const connection& operator=(connection&& My_){
					hmLib_assert(ptr==nullptr,"connection already have a pointer");
					ptr=My_.ptr;
					My_.ptr=nullptr;
				}
				~connection(){if(ptr)ptr->ptr=nullptr;}
				bool is_connect()const{return ptr!=nullptr;}
				void disconnect(){if(ptr)ptr->disconnect();}
				friend class inquiry_abstruct;
			};
		private:
			bool IsConnect;
			connection* ptr;
		protected:
			connection create_connection(void){
				hmLib_assert(!is_connect(),"inquiry_abstruct have already connected");
				IsConnect=true;
				return connection(this);
			}
			virtual void disconnect(void){
				hmLib_assert(is_connect(),"inquiry_abstruct have no connection");
				if(ptr)ptr->ptr=nullptr;
				ptr=nullptr;
				IsConnect=false;
			}
			bool is_connect(void)const{return IsConnect;}
		public:
			inquiry_abstruct():ptr(nullptr),IsConnect(false){}
			inquiry_abstruct(inquiry_abstruct&& My_):ptr(My_.ptr),IsConnect(My_.IsConnect){
				My_.ptr=nullptr;
				My_.IsConnect=false;
			}
			const inquiry_abstruct& operator=(inquiry_abstruct&& My_){
				IsConnect=My_.IsConnect;
				ptr=My_.ptr;
				My_.IsConnect=false;
				My_.ptr=nullptr;
			}
			virtual ~inquiry_abstruct(){if(is_connect())disconnect();}
		private:
			inquiry_abstruct(const inquiry_abstruct& My_);
			const inquiry_abstruct& operator=(const inquiry_abstruct& My_);
		};
		typedef inquiry_abstruct::connection connection;
		//変数への参照inquiry
		template<typename T>
		class inquiry:public inquiry_abstruct{
		private:
			const T* ptr;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			connection connect(const T& t_){
				hmLib_assert(!is_connect(),"have connected yet");
				ptr=&t_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const T& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"have connected yet");
				ptr=&t_;
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			const T& operator()(void)const{
				hmLib_assert(is_connect(),"have not connected yet");
				return *ptr;
			}
			void lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)	pmx->lock();
				locked=true;
			}
			void unlock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)pmx->unlock();
				locked=false;
			}
			bool try_lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx && pmx->try_lcok())locked=true;
				return locked;
			}
		};
		//コンテナへの参照inquiry
		template<typename iterator_>
		class range_inquiry:public inquiry_abstruct{
		private:
			std::function<iterator_(void)> FnBegin;
			std::function<iterator_(void)> FnEnd;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			template<typename Containor_>
			connection connect(const Containor_& t_){
				hmLib_assert(!is_connect(),"have connected yet");
				FnBegin=[&](void)->iterator_{return std::begin(t_);};
				FnEnd=[&](void)->iterator_{return std::end(t_);};
				return create_connection();
			}
			template<typename Containor_,typename Mutex_>
			connection connect(const Containor_& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"have connected yet");
				FnBegin=[&](void)->iterator_{return std::begin(t_);};
				FnEnd=[&](void)->iterator_{return std::end(t_);};
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			iterator_ begin()const{
				hmLib_assert(is_connect(),"have not connected yet");
				return FnBegin();
			}
			iterator_ end()const{
				hmLib_assert(is_connect(),"have not connected yet");
				return FnEnd();
			}
			void lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)	pmx->lock();
				locked=true;
			}
			void unlock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)pmx->unlock();
				locked=false;
			}
			bool try_lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx && pmx->try_lcok())locked=true;
				return locked;
			}
		};
		//connection管理用クラス
		class unique_connections{
		private:
			typedef unique_connections my_type;
		private:
			std::vector<connection> Connections;
		public:
			unique_connections(){}
			unique_connections(my_type&& My_){Connections.swap(My_.Connections);}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(Connections.size()==0,"Connections already have a connection");
				Connections.swap(My_.Connections);
				return *this;
			}
			~unique_connections(){
				for(auto itr=std::begin(Connections);itr!=std::end(Connections);++itr)itr->disconnect();
			}
		private:
			unique_connections(const my_type&);
			const my_type& operator=(const my_type& My_);
		public:
			void operator()(connection&& Connection_){Connections.push_back(std::move(Connection_));}
			void disconnect(){
				for(auto itr=std::begin(Connections);itr!=std::end(Connections);++itr)itr->disconnect();
				Connections.clear();
			}
		};
	}
}
#
#endif
