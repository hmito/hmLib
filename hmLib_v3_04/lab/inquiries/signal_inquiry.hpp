#ifndef HMLIB_INQUIRIES_SIGNALINQUIRY_INC
#define HMLIB_INQUIRIES_SIGNALINQUIRY_INC 101
#
/*===inquiries::signal_inquiry===
operator()(void)で、取得した値を返さずに、Signalを発信するinquiry

inquiries::signal_inquiry v1_01/130329 hmIto
	inquiryで値取得した結果、前回の値と変わりない場合はsignalを発信する、changed_signal_inquiryを追加
	コピーコンストラクタ/代入演算子を明示的にprivate化
	グローバル関数にconnect関数を追加
inquiries::signal_inquiry v1_00/130328 hmIto
	新規作成
*/
#include<boost/signals2.hpp>
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		template<typename T>
		class signal_inquiry:public inquiry_abstruct{
			typedef signal_inquiry<T> my_type;
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
		private:
			const T* ptr;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			signal_inquiry():ptr(nullptr),locked(false),pmx(){}
			signal_inquiry(my_type&& My_):ptr(nullptr),locked(false),pmx(){
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),"signal_inquiry have already connected");
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
				return *this;
			}
		private:
			signal_inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(const T& t_){
				hmLib_assert(!is_connect(),"signal_inquiry have already connected");
				ptr=&t_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const T& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"signal_inquiry have already connected");
				ptr=&t_;
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			void operator()(void)const{
				hmLib_assert(is_connect(),"signal_inquiry have not connected yet");
				if(pmx){
					std::lock_guard<mutex_proxy> Lock(*pmx);
					Signal(*ptr);
				}else{
					Signal(*ptr);
				}
			}
		};
		template<typename Value>
		inquiries::connection connect(signal_inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(signal_inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
		template<typename T>
		class changed_signal_inquiry:public inquiry_abstruct{
			typedef changed_signal_inquiry<T> my_type;
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
		private:
			T value;
			const T* ptr;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			changed_signal_inquiry():value(),ptr(nullptr),locked(false),pmx(){}
			changed_signal_inquiry(my_type&& My_):value(),ptr(nullptr),locked(false),pmx(){
				std::swap(value,My_.value);
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),"signal_inquiry have already connected");
				std::swap(value,My_.value);
				std::swap(ptr,My_.ptr);
				std::swap(locked,My_.locked);
				std::swap(pmx,My_.pmx);
				return *this;
			}
		private:
			changed_signal_inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(const T& t_){
				hmLib_assert(!is_connect(),"changed_signal_inquiry have already connected");
				ptr=&t_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const T& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"changed_signal_inquiry have already connected");
				ptr=&t_;
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			void operator()(void)const{
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx){
					std::lock_guard<mutex_proxy> Lock(*pmx);
					if(*ptr!=value){
						value=*ptr;
						Signal(value);
					}
				}else{
					if(*ptr!=value){
						value=*ptr;
						Signal(value);
					}
				}
			}
		};
		template<typename Value>
		inquiries::connection connect(changed_signal_inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(changed_signal_inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
	}
}
#
#endif
