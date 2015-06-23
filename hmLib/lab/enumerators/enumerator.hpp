#ifndef HMLIB_ENUMERATORS_ENUMERATOR_INC
#define HMLIB_ENUMERATORS_ENUMERATOR_INC 100
#
#include<boost/pool/pool_alloc.hpp>
#include"enumerator_interface.hpp"
namespace hmLib {
	//enumerator
	template<typename T>
	struct enumerator : public enumerator_interaface<T>{
	private:
		struct basic_enumerator_adaptor {
		public:
			struct deleter { void operator()(basic_enumerator_adaptor* ptr)const { ptr->destroy(); } };
		public:
			virtual basic_enumerator_adaptor* clone()const=0;
			virtual void destroy()=0;
			virtual bool equal(const basic_enumerator_adaptor& itr)const=0;
			virtual void increment()=0;
			virtual const value_type& dereference()const=0;
		public:
			template<typename enumerator_>
			static basic_enumerator_adaptor* construct(enumerator_ itr);
		};
		template<typename enumerator_>
		struct enumerator_adaptor :public basic_enumerator_adaptor {
			typedef enumerator_adaptor<enumerator_> adaptor;
		private:
			enumerator_ itr;
		public:
			basic_enumerator_adaptor* clone()const override { return adaptor::construct(itr); }
			void destroy() override { adaptor::destruct(this); }
			bool equal(const basic_enumerator_adaptor& itr_)const override {
				const adaptor* adp=static_cast<const adaptor*>(&itr_);
				if(adp==nullptr)return false;
				return itr == adp->itr;
			}
			void increment() override { ++itr; }
			const value_type& dereference()const override { return *itr; }
		public:
			enumerator_adaptor(enumerator_ itr_) :itr(itr_) {}
		private:
			static boost::fast_pool_allocator<adaptor> allocator;
		public:
			static void destruct(adaptor* ptr) {
				allocator.destroy(ptr);
				allocator.deallocate(ptr);
			}
			static basic_enumerator_adaptor* construct(enumerator_ titr_) {
				adaptor* ans=allocator.allocate();
				allocator.construct(ans, adaptor(titr_));
				return ans;
			}
		};
	private:
		std::unique_ptr<basic_enumerator_adaptor, typename basic_enumerator_adaptor::deleter> ptr;
	public:
		//return true if the object can be accessed to.
		virtual operator bool()const { return (*ptr; }
		//access to object.
		virtual T& operator*() {return ptr->operator*();}
		//move to next object.
		virtual void operator++() { return ptr->operator ++; }
		//restart from the initial object.
		virtual void reset()=0;
	public://for conctructor
		enumerator() :ptr(nullptr) {}
		enumerator(const my_type& My_) :ptr(nullptr) { if(My_.ptr)ptr.reset(My_.ptr->clone()); }
		const my_type& operator=(const my_type& My_) {
			if(this!=&My_)ptr.reset(My_.ptr->clone());
			return *this;
		}
		enumerator(my_type&& My_) :ptr(std::move(My_.ptr)) {}
		const my_type& operator=(my_type&& My_) {
			if(this!=&My_)ptr=std::move(My_.ptr);
			return *this;
		}
		template<typename enumerator_>
		enumerator(enumerator_ itr_):ptr(basic_enumerator_adaptor::construct(itr_)) {}
		template<typename enumerator_>
		const my_type& operator=(enumerator_ itr_) {
			ptr.reset(basic_enumerator_adaptor::construct(itr));
			return *this;
		}
	};
	template<typename value_type, typename difference_type>
	template<typename enumerator_>
	static typename enumerator<value_type, difference_type>::basic_enumerator_adaptor* enumerator<value_type, difference_type>::basic_enumerator_adaptor::construct(enumerator_ itr_) { return enumerator_adaptor<enumerator_>::construct(itr_); }
	template<typename value_type, typename difference_type>
	template<typename enumerator_>
	boost::fast_pool_allocator<typename enumerator<value_type, difference_type>::enumerator_adaptor<enumerator_>> enumerator<value_type, difference_type>::enumerator_adaptor<enumerator_>::allocator;

}
#
#endif
