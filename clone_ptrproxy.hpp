#ifndef HMLIB_CLONEPTRPROXY_INC
#define HMLIB_CLONEPTRPROXY_INC 100
#
#include<type_traits>
#include<utility>
namespace hmLib {
	template<typename T>
	struct clone_ptrproxy {
	private:
		T val;
	public:
		clone_ptrproxy() = delete;
		explicit clone_ptrproxy(const T* val_):val(*val_) {}
		explicit clone_ptrproxy(const T& val_):val(val_) {}
		explicit clone_ptrproxy(T&& val_):val(std::move(val_)) {}
	public:
		T& operator*() { return val; }
		const T& operator*()const { return val; }
		T* operator->() { return &val; }
		const T* operator->()const { return &val; }
		T* get(){return &val;}
		const T* get()const {return &val;}
	};
	template<typename T>
	auto make_clone_ptrproxy(T&& v) {
		return clone_ptrproxy<
			typename std::conditional<
				std::is_const<typename std::remove_reference<T>::type>::value,
				const typename std::decay<T>::type, typename std::decay<T>::type
			>::type
		>(std::forward<T>(v));
	}
	template<typename T>
	auto make_const_clone_ptrproxy(T&& v) {
		return clone_ptrproxy<
			typename std::add_const<typename std::decay<T>::type>::type
		>(std::forward<T>(v));
	}
}
#
#endif
