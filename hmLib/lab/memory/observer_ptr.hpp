#ifndef HMLIB_MEMORY_OBSERVEDPTR_INC
#define HMLIB_MEMORY_OBSERVEDPTR_INC 100
#
#include<type_traits>
#include<memory>
namespace hmLib {
	namespace memory{
		template <class W>
		class observer_ptr {
		public:
			// publish our template parameter and variations thereof
			using element_type = W;
			using pointer = std::add_pointer_t<W>; // exposition-only
			using reference = std::add_lvalue_reference_t<W>; // exposition-only
			// default c’tor
			observer_ptr() :Ptr(nullptr){}
			// pointer-accepting c’tors
			observer_ptr(nullptr_t):Ptr(nullptr){}
			explicit observer_ptr(pointer ptr_) :Ptr(ptr_){}
			// copying c’tors (in addition to compiler-generated copy c’tor)
			template <class W2> observer_ptr(const observer_ptr<W2>& optr_) : Ptr(optr_.Ptr){}
			// copying c’tors (in addition to compiler-generated copy c’tor)
			template <class W2> observer_ptr(const std::unique_ptr<W2>& uptr_) : Ptr(uptr_.get()){}
			// observers
			pointer get() const {return Ptr;}
			reference operator*() const{return *Ptr;}
			pointer operator->() const {return Ptr;}
			explicit operator bool() const {return Ptr!=nullptr;}
			// modifiers
			pointer release() {
				pointer ReleasedPtr=Ptr;
				Ptr=nullptr;
				return ReleasedPtr;
			}
			void reset(pointer ptr_= nullptr) {Ptr=ptr_;}
			// copying c’tors (in addition to compiler-generated copy c’tor)
			template <class W2> void reset(const std::unique_ptr<W2>& uptr_){ Ptr = uptr_.get(); }
			void swap(observer_ptr& optr_) { std::swap(Ptr, optr_.Ptr); }
		private:
			pointer Ptr;
		}; // observer_ptr<>
	}
}
#if defined(HMLIB_CPPVERSION_SUPPRIMENT) && HMLIB_CPPVERSION_SUPPRIMENT >= 17
#	if HMLIB_CPPVERSION_CURRENT >= 11
namespace std{
	template<typename W> 
	using observer_ptr = hmLib::memory::observer_ptr<W>;
}
#	endif
#endif
#
#endif
