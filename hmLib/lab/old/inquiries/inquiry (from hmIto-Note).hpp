#ifndef HMLIB_INQUIRIES_INQUIRY_INC
#define HMLIB_INQUIRIES_INQUIRY_INC 200
#
/*===inquiries::inquiry===
inquiryクラス
	あらゆるオブジェクトと接続することができる。
	operator()(void)で、接続したオブジェクトをconstで取得することができる。
	lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。

inquiries::inquiry_v2_00/130415 hmito
	const参照を戻り値とする関数を保持するオブジェクトとして再定義
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
#include<functional>
#ifndef HMLIB_MUTEXPROXY_INC
#	include<hmLib/mutex_proxy.hpp>
#endif
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib/exception.hpp>
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
			typedef std::function<const T(void)> func_type;
			typedef inquiry<T> my_type;
		private:
			func_type func;
			bool locked;
			mutex_proxy mxprx;
//			std::unique_ptr<mutex_proxy> pmx;
		public:
			inquiry():ptr(nullptr),locked(false),mxprx(){}
			inquiry(my_type&& My_):ptr(nullptr),locked(false),mxprx(){
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(mxprx,My_.mxprx);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(mxprx,My_.mxprx);
				return *this;
			}
		private:
			inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(const func_type& func_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				func=func_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const func_type& func_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"inquiry have already connected");
				func=func_;
				mxprx=std::move(mutex_proxy(mx_));
				return create_connection();
			}
			const T operator()(void)const{
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				return func();
			}
			void lock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(mxprx.is_proxy())mxprx.lock();
				locked=true;
			}
			void unlock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(mxprx.is_proxy())mxprx.unlock();
				locked=false;
			}
			bool try_lock(){
				hmLib_assert(is_connect(),"inquiry have not connected yet");
				if(!mxprx.is_proxy() || mxprx.try_lock())locked=true;
				return locked;
			}
		};
		template<typename Value>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
	}
}
#
#endif
