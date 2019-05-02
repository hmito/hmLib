#ifndef HMLIB_FUNCTIONAL_SHARE_INC
#define HMLIB_FUNCTIONAL_SHARE_INC 100
#
#include<type_traits>
#include<memory>
namespace hmLib {
	template<typename T, bool use_shared_ptr_ = !(std::is_pointer<T>::value || std::is_function<T>::value)>
	struct shared_wrapper {
	private:
		std::shared_ptr<T> Ptr;
	public:
		shared_wrapper() = delete;
		shared_wrapper(const T& val):Ptr(std::make_shared<T>(val)) {}
		shared_wrapper(T&& val):Ptr(std::make_shared<T>(std::move(val))) {}
	public:
		T& get()const noexcept { return *Ptr.get(); }
		operator T&()const noexcept { return *Ptr.get(); }
		template<typename... args>
		decltype(auto) operator()(args... Args)const {
			return (*Ptr)(std::forward<args>(Args)...);
		}
		static constexpr bool use_shared_ptr() { return true; }
	};
	template<typename T>
	struct shared_wrapper<T, false> {
		using type = typename std::conditional<std::is_function<T>::value, T*, T>::type;
	private:
		type Ptr;
	public:
		shared_wrapper() = delete;
		shared_wrapper(type Ptr_):Ptr(Ptr_) {}
	public:
		T& get()const noexcept { return *Ptr; }
		operator T&()const noexcept { return *Ptr; }
		template<typename... args>
		decltype(auto) operator()(args... Args)const {
			return (*Ptr)(std::forward<args>(Args)...);
		}
		static constexpr bool use_shared_ptr() { return false; }
	};
	template<typename T>
	auto share(T&& v) {
		return shared_wrapper<typename std::remove_reference<T>::type>(std::forward<T>(v));
	}
	template<typename T>
	shared_wrapper<T> share(shared_wrapper<T> v) {
		return v;
	}
}
#
#endif
