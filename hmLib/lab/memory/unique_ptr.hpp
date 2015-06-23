#ifndef HMLIB_MEMORY_UNIQUEPTR_INC
#define HMLIB_MEMORY_UNIQUEPTR_INC 100
#
#include<hmLib/lab/rvalue_reference.hpp>
#include"default_deleter.hpp"
namespace hmLib{
	template<typename T,typename D = default_deleter<const T>>
	class unique_ptr{
	private:
		typedef T* pointer;
		typedef D deleter;
		typedef unique_ptr<T, D> my_type;
		typedef rvalue_reference<my_type> rvalue_reference;
	private:
		pointer ptr;
		deleter dlt;
	public:
		unique_ptr()
			:ptr(0)
			,dlt(){
		}
		explicit unique_ptr(pointer ptr_)
			:ptr(ptr_)
			,dlt(){
		}
		unique_ptr(pointer ptr_,deleter dlt_)
			:ptr(ptr_)
			,dlt(dlt_){
		}
		explicit unique_ptr(rvalue_reference mptr_)
			:ptr(mptr_.ref.get())
			,dlt(mptr_.ref.get_deleter()) {
			mptr_.ref.release();
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptr)dlt(ptr);
				ptr=mptr_.ref.get();
				dlt=mptr_.ref.get_deleter();
				mptr_.ref.release();
			}
			return *this;
		}
		template<typename U>
		explicit unique_ptr(hmLib::rvalue_reference<unique_ptr<U, D> > mptr_)
			: ptr(mptr_.ref.get())
			, dlt(mptr_.ref.get_deleter()) {
			mptr_.ref.release();
		}
		template<typename U>
		const my_type& operator=(hmLib::rvalue_reference<unique_ptr<U, D> > mptr_) {
			if(ptr!=mptr_.ref.get()) {
				if(ptr)dlt(ptr);
				ptr=mptr_.ref.get();
				dlt=mptr_.ref.get_deleter();
				mptr_.ref.release();
			}
			return *this;
		}
		~unique_ptr() {
			if(ptr)dlt(ptr);
		}
		void swap(my_type& My_) {
			if(this==&My_)return;

			pointer tmp_ptr=ptr;
			deleter tmp_dlt=dlt;
			ptr=My_.ptr;
			dlt=My_.dlt;
			My_.ptr=tmp_ptr;
			My_.dlt=tmp_dlt;
		}
	private:
		//copy disable
		unique_ptr(const my_type&);
		const my_type& operator=(const my_type& My_);
	public:
		pointer get()const { return ptr; }
		deleter get_deleter()const { return dlt; }
		pointer release() {
			pointer ans=ptr;
			ptr=0;
			return ptr;
		}
		void reset(pointer ptr_=0) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
			dlt=deleter();
		}
		void reset(pointer ptr_, deleter dlt_) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
			dlt=dlt_;
		}
		operator bool() { return ptr!=0; }
		T& operator*() { return *ptr; }
		T* operator->() { return ptr; }
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return ptr; }
	};
	template<typename T, typename D>
	class unique_ptr<T[],D>{
	private:
		typedef T* pointer;
		typedef D deleter;
		typedef unique_ptr<T[], D> my_type;
		typedef rvalue_reference<my_type> rvalue_reference;
	private:
		pointer ptr;
		deleter dlt;
	public:
		unique_ptr()
			:ptr(0)
			, dlt() {
		}
		explicit unique_ptr(pointer ptr_)
			:ptr(ptr_)
			, dlt() {
		}
		unique_ptr(pointer ptr_, deleter dlt_)
			:ptr(ptr_)
			, dlt(dlt_) {
		}
		explicit unique_ptr(rvalue_reference mptr_)
			:ptr(mptr_.ref.ptr)
			, dlt(mptr_.ref.dlt) {
			mptr_.ref.release();
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptr)dlt(ptr);
				ptr=mptr_.ref.get();
				dlt=mptr_.ref.get_deleter();
				mptr_.ref.release();
			}
			return *this;
		}
		~unique_ptr() {
			if(ptr)dlt(ptr);
		}
		void swap(my_type& My_) {
			if(this==&My_)return;

			pointer tmp_ptr=ptr;
			deleter tmp_dlt=dlt;
			ptr=My_.ptr;
			dlt=My_.dlt;
			My_.ptr=tmp_ptr;
			My_.dlt=tmp_dlt;
		}
	private:
		//copy disable
		unique_ptr(const my_type&);
		const my_type& operator=(const my_type& My_);
	public:
		pointer get()const { return ptr; }
		deleter get_deleter()const { return dlt; }
		pointer release() {
			pointer ans=ptr;
			ptr=0;
			return ptr;
		}
		void reset(pointer ptr_=0) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
		}
		void reset(pointer ptr_, deleter dlt_) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
			dlt=dlt_;
		}
		operator bool() { return ptr!=0; }
		T& operator[](unsigned int pos) { return ptr[pos]; }
	};
}
#
#endif
