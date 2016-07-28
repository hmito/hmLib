#ifndef HMLIB_SHAREDPTR_INC
#define HMLIB_SHAREDPTR_INC 101
#
/*
===shared_ptr===
v1_01/140412 hmIto
	_shared_array_elementを無名名前空間へ移動
	allocatorのrebindを用いた正しい記載へ変更
*/
#include<memory>
#include<hmLib/lab/rvalue_reference.hpp>
#include"default_deleter.hpp"
#include"unique_ptr.hpp"
namespace hmLib {
	namespace {
		template<typename T, typename D>
		struct _shared_ptr_element {
		public:
			typedef T* pointer;
			typedef D deleter;
			typedef _shared_ptr_element<T, D> my_type;
			typedef rvalue_reference<my_type> rvalue_reference;
		private:
			pointer ptr;
			deleter dlt;
			unsigned int cnt;
		public:
			_shared_ptr_element(pointer ptr_, deleter dlt_) :ptr(ptr_), dlt(dlt_), cnt(0) {
				if(ptr)++cnt;
			}
			_shared_ptr_element(rvalue_reference ref_) :ptr(ref_.ref.ptr), dlt(ref_.ref.dlt), cnt(ref_.ref.cnt) {
				ref_.ref.ptr=0;
				ref_.ref.cnt=0;
			}
			const _shared_ptr_element& operator=(rvalue_reference ref_) {
				if(ptr)dlt(ptr);
				prt=ref_.ref.ptr;
				dlt=ref_.ref.dlt;
				cnt=ref_.ref.cnt;
				ref_.ref.ptr=0;
				ref_.ref.cnt=0;
			}
			~_shared_ptr_element() {
				if(ptr)dlt(ptr);
			}
			my_type* attach() {
				++cnt;
				return this;
			}
			bool detach() {
				if(cnt>0)--cnt;
				return ptr!=0 && cnt==0;
			}
			pointer get()const { return ptr; }
			deleter get_deleter()const { return dlt; }
			unsigned int get_count()const { return cnt; }
			operator bool()const { ptr!=0; }
		private:
			_shared_ptr_element();
			_shared_ptr_element(const _shared_ptr_element&);
			const _shared_ptr_element& operator=(const _shared_ptr_element&);
		};
	}
	template<typename T, typename D=default_deleter<const T>, typename Alloc=std::allocator<T> >
	class shared_ptr {
	private:
		typedef shared_ptr<T, D, Alloc> my_type;
		typedef _shared_ptr_element<const T, D> my_ptr_element;
		typedef rvalue_reference<my_type> rvalue_reference;
		typedef hmLib::rvalue_reference<unique_ptr<T, D> > unique_ptr_rvalue_reference;
	public:
		typedef T* pointer;
		typedef T& reference;
		typedef D deleter; 
		typedef typename Alloc::template rebind<my_ptr_element>::other allocator;
	private:
		my_ptr_element* ptrh;
		allocator alloc;
	public:
		shared_ptr() :ptrh(0){}
		explicit shared_ptr(pointer ptr_) :ptrh(0) {
			construct(ptr_, deleter());
		}
		shared_ptr(pointer ptr_,deleter dlt_) :ptrh(0) {
			construct(ptr_, dlt_);
		}
		shared_ptr(const my_type& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		const my_type& operator=(const my_type& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		explicit shared_ptr(rvalue_reference mptr_):ptrh(0){
			swap(mptr_.ref);
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptrh)destroy();
				swap(mptr_.ref);
			}
			return *this;
		}
		template<typename U,typename UAlloc>
		shared_ptr(const shared_ptr<U,D,UAlloc>& my_) :ptrh(0) {
			my_.share_to(*this);
		}
		template<typename U, typename UAlloc>
		const my_type& operator=(const shared_ptr<U, D, UAlloc>& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				my_.share_to(*this);
			}
			return *this;
		}
		template<typename U, typename UAlloc>
		explicit shared_ptr(hmLib::rvalue_reference<shared_ptr<U, D, UAlloc>> mptr_) :ptrh(0) {
			mptr_.ref.share_to(*this);
			mptr_.ref.reset();
		}
		template<typename U, typename UAlloc>
		const my_type& operator=(hmLib::rvalue_reference<shared_ptr<U, D, UAlloc>> mptr_) {
			if(ptrh)destroy();
			mptr_.ref.share_to(*this);
			mptr_.ref.reset();
			return *this;
		}
		explicit shared_ptr(unique_ptr_rvalue_reference mptr_) :ptrh(0) {
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		const my_type& operator=(unique_ptr_rvalue_reference mptr_) {
			if(ptrh)destroy();
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
			return *this;
		}
		~shared_ptr() {
			if(ptrh)destroy();
		}
	public:
		operator bool() { return ptrh!=0; }
		pointer get() {
			if(ptrh)return return const_cast<T*>(ptrh->get());
			else return 0;
		}
		deleter get_deleter() { return ptrh->get_deleter(); }
		reference operator*() { return const_cast<T&>(*(ptrh->get())); }
		pointer operator->() { return const_cast<T*>(ptrh->get()); }
		unsigned int use_count()const {
			if(ptrh==0)return 0;
			return ptrh->get_count(); 
		}
		void reset(pointer ptr_=0) {
			if(ptrh)destroy();
			construct(ptr_, deleter());
		}
		void reset(pointer ptr_,deleter dlt_) {
			if(ptrh)destroy();
			construct(ptr_, dlt_)
		}
		void reset(my_ptr_element& rh_) {
			if(ptrh)destroy();
			share(&rh_);
		}
		void reset(unique_ptr_rvalue_reference mptr_) {
			if(ptrh)destroy();
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		void swap(my_type& My_) {
			my_ptr_element* ptrh_=My_.ptrh;
			allocator alloc_=My_.alloc;
			My_.ptrh=ptrh;
			My_.alloc=alloc;
			ptrh=ptrh_;
			alloc=alloc_;
		}
		template<typename U, typename UAlloc>
		void share_to(shared_ptr<U, D, UAlloc>& my_) {
			my_.reset(prth);
			
		}
	private://いずれの関数も、ptrhが存在している/空であることの確認はしていないので注意
		void construct(pointer ptr_, deleter dlt_) {
			ptrh=alloc.allocate(std::size_t(1));
			alloc.construct(ptrh, move(my_ptr_element(ptr_, dlt_)));
		}
		void destroy() {
			if(ptrh->detach()) {
				alloc.destroy(ptrh);
				alloc.deallocate(ptrh, std::size_t(1));
			}
			ptrh=0;
		}
		void share(my_ptr_element* ptrh_) {
			if(ptrh_) {
				ptrh=ptrh_->attach();
			}
		}
	};
	template<typename T, typename D, typename Alloc>
	class shared_ptr<T[], D, Alloc> {
		friend class shared_ptr<const T[], D, Alloc>;
	private:
		typedef shared_ptr<T[], D, Alloc> my_type;
		typedef _shared_ptr_element<const T, D> my_ptr_element;
		typedef rvalue_reference<my_type> rvalue_reference;
		typedef hmLib::rvalue_reference<unique_ptr<T[], D> > unique_ptr_rvalue_reference;
	public:
		typedef T* pointer;
		typedef T& reference;
		typedef D deleter;
		typedef typename Alloc::template rebind<my_ptr_element>::other allocator;
	private:
		my_ptr_element* ptrh;
		allocator alloc;
	public:
		shared_ptr() :ptrh(0) {}
		explicit shared_ptr(pointer ptr_) :ptrh(0) {
			construct(ptr_, deleter());
		}
		shared_ptr(pointer ptr_, deleter dlt_) :ptrh(0) {
			construct(ptr_, dlt_);
		}
		shared_ptr(const my_type& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		const my_type& operator=(const my_type& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		explicit shared_ptr(rvalue_reference mptr_) :ptrh(0) {
			swap(mtrh);
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptrh)destroy();
				swap(mtrh);
			}
			return *this;
		}
		template<typename U, typename UAlloc>
		shared_ptr(const shared_ptr<U, D, UAlloc>& my_) :ptrh(0) {
			share(my_.ptrh);
		}
		template<typename U, typename UAlloc>
		const my_type& operator=(const shared_ptr<U, D, UAlloc>& my_) {
			if(ptrh!=my_.ptrh) {
				if(ptrh)destroy();
				share(my_.ptrh);
			}
			return *this;
		}
		template<typename U, typename UAlloc>
		explicit shared_ptr(hmLib::rvalue_reference<shared_ptr<U, D, UAlloc>> mptr_) :ptrh(0) {
			swap(mptr_.ref.ptrh);
		}
		template<typename U, typename UAlloc>
		const my_type& operator=(hmLib::rvalue_reference<shared_ptr<U, D, UAlloc>> mptr_) {
			if(ptrh)destroy();
			swap(mptr_.ref.ptrh);
			return *this;
		}
		explicit shared_ptr(unique_ptr_rvalue_reference mptr_) :ptrh(0) {
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		const my_type& operator=(unique_ptr_rvalue_reference mptr_) {
			if(ptrh)destroy();
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
			return *this;
		}
		~shared_ptr() {
			if(ptrh)destroy();
		}
	public:
		operator bool() { return ptrh!=0; }
		pointer get() {
			if(ptrh)return return const_cast<T*>(ptrh->get());
			else return 0;
		}
		deleter get_deleter() { return ptrh->get_deleter(); }
		reference operator[](unsigned int pos) { return const_cast<T&>((ptrh->get())[pos]); }
		unsigned int use_count()const { return ptrh->get_count(); }
		void reset(pointer ptr_=0) {
			if(ptrh)destroy();
			construct(ptr_, deleter());
		}
		void reset(pointer ptr_, deleter dlt_) {
			if(ptrh)destroy();
			construct(ptr_, dlt_)
		}
		void reset(unique_ptr_rvalue_reference mptr_) {
			if(ptrh)destroy();
			construct(mptr_.ref.get(), mptr_.ref.get_deleter());
			mptr_.ref.release();
		}
		void swap(my_type& My_) {
			my_ptr_element* ptrh_=My_.ptrh;
			allocator alloc_=My_.alloc;
			My_.ptrh=ptrh;
			My_.alloc=alloc;
			ptrh=ptrh_;
			alloc=alloc_;
		}
	private://いずれの関数も、ptrhが存在している/空であることの確認はしていないので注意
		void construct(pointer ptr_, deleter dlt_) {
			ptrh=alloc.allocate(std::size_t(1));
			alloc.construct(ptrh, move(my_ptr_element(ptr_, dlt_)));
		}
		void destroy() {
			if(ptrh->detach()) {
				alloc.destroy(ptrh);
				alloc.deallocate(ptrh, std::size_t(1));
			}
			ptrh=0;
		}
		void share(my_ptr_element* ptrh_) {
			if(ptrh_) {
				ptrh=ptrh_->attach();
			}
		}
		void swap(my_ptr_element*& ptrh_) {
			my_ptr_element* tmp_prth=ptrh_;
			ptrh_=ptrh;
			ptrh=tmp_prth;
		}
	};
}
#
#endif
