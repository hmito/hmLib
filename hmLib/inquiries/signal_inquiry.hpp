#ifndef HMLIB_INQUIRIES_SIGNALINQUIRY_INC
#define HMLIB_INQUIRIES_SIGNALINQUIRY_INC 102
#
/*===inquiries::signal_inquiry===
operator()(void)で、取得した値を返さずに、Signalを発信するinquiry

inquiries::signal_inquiry v1_02/130421 hmIto
	関数型に対応
	exceptionsに対応
inquiries::signal_inquiry v1_01/130329 hmIto
	inquiryで値取得した結果、前回の値と変わりない場合はsignalを発信する、changed_signal_inquiryを追加
	コピーコンストラクタ/代入演算子を明示的にprivate化
	グローバル関数にconnect関数を追加
inquiries::signal_inquiry v1_00/130328 hmIto
	新規作成
*/
#include<boost/signals2.hpp>
#ifndef HMLIB_EXCEPTIONS_INC
#	include"../exceptions.hpp"
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		template<typename T>
		class signal_inquiry:public inquiry_abstruct{
			typedef signal_inquiry<T> my_type;
			typedef std::function<const T(void)> func_type;
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
		private:
			func_type func;
			bool locked;
			mutex_proxy mx;
		public:
			signal_inquiry():func(),locked(false),mx(){}
			signal_inquiry(my_type&& My_):func(),locked(false),mx(){
				std::swap(func,My_.func);
				std::swap(locked,My_.locked);
				std::swap(mx,My_.mx);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),connected_exception,"signal_inquiry have already connected");
				std::swap(func,My_.func);
				std::swap(locked,My_.locked);
				std::swap(mx,My_.mx);
				return *this;
			}
		private:
			signal_inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(func_type&& func_){
				hmLib_assert(!is_connect(),connected_exception,"signal_inquiry have already connected");
				func=func_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(func_type&& func_,Mutex_& mx_){
				hmLib_assert(!is_connect(),connected_exception,"signal_inquiry have already connected");
				func=func_;
				mx=mutex_proxy(mx_);
				return create_connection();
			}
			void operator()(void)const{
				hmLib_assert(is_connect(),unconnected_exception,"signal_inquiry have not connected yet");
				if(mx.is_proxy()){
					std::lock_guard<mutex_proxy> Lock(mx);
					Signal(func_());
				}else{
					Signal(func_());
				}
			}
		};
		template<typename T>
		class revise_signal_inquiry:public inquiry_abstruct{
			typedef revise_signal_inquiry<T> my_type;
			typedef std::function<const T(void)> func_type;
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
		private:
			bool Active;
			T last_val;
			func_type func;
			bool locked;
			mutex_proxy mx;
		public:
			revise_signal_inquiry():func(),locked(false),mx(),Active(false){}
			revise_signal_inquiry(my_type&& My_):func(),locked(false),mx(),Active(false){
				std::swap(func,My_.func);
				std::swap(locked,My_.locked);
				std::swap(mx,My_.mx);
				std::swap(Active,My_.Active);
				std::swap(last_val,My_.last_val);
			}
			const my_type& operator=(my_type&& My_){
				hmLib_assert(!is_connect(),connected_exception,"revise_signal_inquiry have already connected");
				std::swap(func,My_.func);
				std::swap(locked,My_.locked);
				std::swap(mx,My_.mx);
				std::swap(Active,My_.Active);
				std::swap(last_val,My_.last_val);
				return *this;
			}
		private:
			revise_signal_inquiry(const my_type& My_);
			const my_type& operator=(const my_type& My_);
		public:
			connection connect(func_type&& func_){
				hmLib_assert(!is_connect(),connected_exception,"revise_signal_inquiry have already connected");
				func=func_;
				Active=false;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(func_type&& func_,Mutex_& mx_){
				hmLib_assert(!is_connect(),connected_exception,"revise_signal_inquiry have already connected");
				func=func_;
				Active=false;
				mx=mutex_proxy(mx_);
				return create_connection();
			}
			void operator()(void)const{
				hmLib_assert(is_connect(),unconnected_exception,"revise_signal_inquiry have not connected yet");
				if(mx.is_proxy()){
					std::lock_guard<mutex_proxy> Lock(mx);
					T tmp=func_();
					if(Active==false || tmp!=last_val){
						Active=true;
						val=std::move(tmp);
						Signal(val);
					}
				}else{
					T tmp=func_();
					if(Active==false || tmp!=last_val){
						Active=true;
						val=std::move(tmp);
						Signal(val);
					}
				}
			}
		};
	}
}
#
#endif
