#ifndef HMLIB_INQUIRIES_INQUIRY_INC
#define HMLIB_INQUIRIES_INQUIRY_INC 102
#
/*===inquiries::inquiry===
inquiryクラス
	あらゆるオブジェクトと接続することができる。
	operator()(void)で、接続したオブジェクトをconstで取得することができる。
	lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。

inquiries::inquiry v1_02/130412 hmIto
	try_lock関数の呼び出しで呼び出し名が間違っていた問題を修正
	try_lockでmutexを保持していない場合にロックに失敗しているように見えていた問題を修正
inquiries::inquiry v1_01/130329 hmIto
	コピーコンストラクタ/代入演算子を明示的にprivate化
	グローバル関数にconnect関数を追加
inquiries::inquiry v1_00/130328 hmIto
	inquiryから分離
*/
#include<memory>
#ifndef HMLIB_MUTEXPROXY_INC
#	include<hmLib_v3_04/mutex_proxy.hpp>
#endif
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		//変数への参照inquiry
		template<typename T>
		class inquiry:public inquiry_abstruct{
		private:
			typedef inquiry<T> my_type;
		private:
			const T* ptr;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			inquiry():ptr(nullptr),locked(false),pmx(){}
			inquiry(my_type&& My_):ptr(nullptr),locked(false),pmx(){
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
				return *this;
			}
		private:
			inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(const T& t_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				ptr=&t_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const T& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				ptr=&t_;
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			const T& operator()(void)const{
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				return *ptr;
			}
			void lock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(pmx)	pmx->lock();
				locked=true;
			}
			void unlock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(pmx)pmx->unlock();
				locked=false;
			}
			bool try_lock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(!pmx || pmx->try_lock())locked=true;
				return locked;
			}
		};
		template<typename Value>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
	}
}
#
#endif
