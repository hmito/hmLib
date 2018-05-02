#ifndef HMLIB_CLONEPTR_INC
#define HMLIB_CLONEPTR_INC 100
#
#include<type_traits>
#include<utility>
namespace hmLib {
	template<typename T>
	struct clone_ptr {
	private:
		T val;
	public:
		clone_ptr() = delete;
		explicit clone_ptr(const T& val_):val(val_) {}
		explicit clone_ptr(T&& val_):val(std::move(val_)) {}
	public:
		T& operator*() { return val; }
		const T& operator*()const { return val; }
		T* operator->() { return &val; }
		const T* operator->()const { return &val; }
		T* get(){return &val;}
		const T* get()const {return &val;}
	};
	template<typename T>
	auto make_clone_ptr(T&& v) {
		return clone_ptr<
			typename std::conditional<
				std::is_const<typename std::remove_reference<T>::type>::value,
				const typename std::decay<T>::type, typename std::decay<T>::type
			>::type
		>(std::forward<T>(v));
	}
	template<typename T>
	auto make_const_clone_ptr(T&& v) {
		return clone_ptr<
			typename std::add_const<typename std::decay<T>::type>::type
		>(std::forward<T>(v));
	}
}
#
#endif
