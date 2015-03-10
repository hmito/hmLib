#ifndef HMLIB_MUTEXPROXY_INC
#define HMLIB_MUTEXPROXY_INC 100
#
/*===mutex_proxy===
汎用Mutexを受け取って代理的に機能する

mutex_proxy:v1_00/130308 hmIto
	汎用mutex参照クラス、mutex_proxyを追加
*/
#include<functional>
namespace hmLib{
	class mutex_proxy{
	private:
		std::function<void(void)> FnLock;
		std::function<void(void)> FnUnlock;
		std::function<bool(void)> FnTryLock;
	public:
		template<typename mutex_>
		mutex_proxy(mutex_& Mx_)
			:FnLock(std::bind(std::mem_fn(&mutex_::lock),std::ref(Mx_)))
			,FnUnlock(std::bind(std::mem_fn(&mutex_::unlock),std::ref(Mx_)))
			,FnTryLock(std::bind(std::mem_fn(&mutex_::try_lock),std::ref(Mx_))){
		}
	public:
		void lock(){FnLock();}
		void unlock(){FnUnlock();}
		bool try_locki(){return FnTryLock();}
	};
}
#
#endif
