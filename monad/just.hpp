#ifndef HMLIB_MONAD_JUST_INC
#define HMLIB_MONAD_JUST_INC 100
#
#include<utility>
#include"monad_traits.hpp"
#include"monad_mixin.hpp"
namespace hmLib {
	namespace monad {
		template<typename T>
		struct just :public monad_mixin<just<T>, T, monad_categories::omittable_monad_tag> {
		private:
			using this_type = just<T>;
		private:
			T val;
		public:
			just() = default;
			just(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			just(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//for monad
			template<typename U>
			struct rebind { using type = just<U>; };
			explicit just(const T& val_) :val(val_) {}
			explicit just(T&& val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				return omitted_wrap<this_type>(hmLib::monad::fmap(val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			static just<T> flatten(just<just<T>> m_) { return just(m_.get().get()); }
		public:
			operator T&() { return val; }
			operator const T&()const { return val; }
			T& get() { return val; }
			const T& get()const { return val; }
			void set(T val_) { val = std::move(val_); }
		};

		template<typename T>
		auto make_just(T&& val) { return just<typename std::decay<T>::type>(std::forward<T>(val)); }

		template<>
		struct monadic_define<bool> {
			auto operator()(bool v)const noexcept { return just<bool>(v); }
		};
		template<>
		struct monadic_define<signed char> {
			auto operator()(signed char v)const noexcept { return just<signed char >(v); }
		};
		template<>
		struct monadic_define<unsigned char> {
			auto operator()(unsigned char v)const noexcept { return just<unsigned char>(v); }
		};
		template<>
		struct monadic_define<signed short> {
			auto operator()(signed short v)const noexcept { return just<signed short>(v); }
		};
		template<>
		struct monadic_define<unsigned short> {
			auto operator()(unsigned short v)const noexcept { return just<unsigned short >(v); }
		};
		template<>
		struct monadic_define<signed int> {
			auto operator()(signed int v)const noexcept { return just<signed int>(v); }
		};
		template<>
		struct monadic_define<unsigned int> {
			auto operator()(unsigned int v)const noexcept { return just<unsigned int >(v); }
		};
		template<>
		struct monadic_define<signed long> {
			auto operator()(signed long v)const noexcept { return just<signed long>(v); }
		};
		template<>
		struct monadic_define<unsigned long> {
			auto operator()(unsigned long v)const noexcept { return just<unsigned long >(v); }
		};
		template<>
		struct monadic_define<signed long long> {
			auto operator()(signed long long v)const noexcept { return just<signed long long>(v); }
		};
		template<>
		struct monadic_define<unsigned long long> {
			auto operator()(unsigned long long v)const noexcept { return just<unsigned long long >(v); }
		};
		template<>
		struct monadic_define<float> {
			auto operator()(float v)const noexcept { return just<float>(v); }
		};
		template<>
		struct monadic_define<double> {
			auto operator()(double v)const noexcept { return just<double>(v); }
		};
		template<>
		struct monadic_define<long double> {
			auto operator()(long double v)const noexcept { return just<long double>(v); }
		};
	}
}
#
#endif
