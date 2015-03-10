#ifndef HMLIB_SIGNALINQUIRY_INC
#define HMLIB_SIGNALINQUIRY_INC 100
#
#include<boost/signals2.hpp>
#include"inquiry.hpp"
namespace hmLib{
	namespace inquiries{
		template<typename T>
		class signal_inquiry:public inquiry_abstruct{
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
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
			void operator()(void)const{
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx){
					std::lock_guard<mutex_proxy> Lock(*pmx);
					Signal(*ptr);
				}else{
					Signal(*ptr);
				}
			}
		};
	}
}
#
#endif
