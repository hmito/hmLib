#ifndef HMLIB_ANYMUTEX_INC
#define HMLIB_ANTMUTEX_INC 100
#
/*===any_mutex===
様々なmutex型を受け取る方
any_mutex:v1_00/130327 hmIto
	リリース
*/
#include<memory>
#include<boost/pool/pool_alloc.hpp>
namespace hmLib{
	struct any_mutex{
	private:
		typedef any_mutex my_type;
	private:
		struct basic_mutex_adaptor{
		public:
			struct deleter{void operator()(basic_mutex_adaptor* ptr)const{ptr->destroy();}};
		public:
			virtual void destroy()=0;
			virtual void lock()=0;
			virtual void unlock()=0;
			virtual bool try_lock()=0;
		public:
			template<typename mutex_>
			static basic_mutex_adaptor* construct(mutex_& mx);
		};
		template<typename mutex_>
		struct mutex_adaptor:public basic_mutex_adaptor{
		private:
			typedef mutex_adaptor<mutex_> adaptor;
		private:
			mutex_& mx;
		public:
			void destroy()override{}
			void lock()override{mx.lock();}
			bool try_lock()override{return mx.try_lock();}
			void unlock()override{mx.unlock();}
		public:
			mutex_adaptor(mutex_& mx_):mx(mx_){}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr){
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_mutex_adaptor* construct(mutex_& mx_){
				adaptor* ans=allocator.allocate();
				allocator.construct(ans,adaptor(mx_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_mutex_adaptor,typename basic_mutex_adaptor::deleter> ptr;
	public://for conctructor
		any_mutex():ptr(nullptr){}
		any_mutex(const my_type& My_);
		const my_type& operator=(const my_type& My_);
		template<typename mutex_>
		any_mutex(mutex_& mx_):ptr(basic_mutex_adaptor::construct(mx_)){}
		template<typename iterator_>
		const my_type& operator=(iterator_ itr_){
			ptr.reset(basic_mutex_adaptor::construct(itr));
			return *this;
		}
	public://for mutex
		void lock(){ptr->lock();}
		bool try_lock(){return ptr->try_lock();}
		void unlock(){ptr->unlock();}
	};
	template<typename mutex_>
	static any_mutex::basic_mutex_adaptor* any_mutex::basic_mutex_adaptor::construct(mutex_& mx_){return mutex_adaptor<mutex_>::construct(mx_);}
	template<typename mutex_>
	boost::fast_pool_allocator<typename any_mutex::mutex_adaptor<mutex_>> any_mutex::mutex_adaptor<mutex_>::allocator;
}
#
#endif
