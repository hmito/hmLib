#ifndef HMLIB_SHAREDMEMORY_INC
#define HMLIB_SHAREDMEMORY_INC 100
#
#include <boost/pool/object_pool.hpp>
#include <vector>
namespace hmLib{
	template<class T>
	class shared_memory{
		typedef unsigned int size_type;
		static boost::object_pool<T> Pool;
	private:
		T* Ptr;
	public:
		shared_memory()
			:Ptr(Pool.construct()){
		}
		shared_memory(const T& t_):Ptr(Pool.construct(t_)){}
		shared_memory(const shared_memory& My_);
		~shared_memory(){Pool.destroy(Ptr);}
		const shared_memory& operator=(const shared_memory& My_);
		operator T*()const{return Ptr;}
		T* operator ->()const{return Ptr;}
		void destroy(){
			if(Ptr!=NULL)Pool.destroy(Ptr);
			Ptr=NULL;
		}
	};
	template<class T>
	boost::object_pool<T> shared_memory<T>::Pool;
	template<class T>
	class shared_array_memory{
		typedef unsigned int size_type;
		static boost::pool<> Pool;
	private:
		array_type* Ptr;
	public:
		shared_array_memory():Ptr(Pool.construct()){}
		shared_array_memory(size_type Size_):Ptr(Pool.construct(array_type(Size_))){}
		shared_array_memory(size_type Size_,const T& t_):Ptr(Pool.construct(array_type(Size_,t_))){}
		~shared_array_memory(){Pool.destroy(Ptr);}
		operator T*()const{return Ptr;}
		T* operator ->()const{return Ptr;}
		void destroy(){
			if(Ptr!=NULL)Pool.destroy(Ptr);
			Ptr=NULL;
		}
	};
}
#
#endif
