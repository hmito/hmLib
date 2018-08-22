#ifndef HMLIB_MONAD_FMAP_INC
#define HMLIB_MONAD_FMAP_INC 100
#
#include<utility>
#include<type_traits>
#include"monad_traits.hpp"
//fmap
namespace hmLib {
	namespace monad {
		template<typename T>
		using default_fmap_target = is_not_monad<T>;

		namespace detail {
			template<typename T, template<typename> typename is_target, bool IsTarget = is_target<T>::value>
			struct fmap_target_type_impl {
				using type = T;
			};
			template<typename T, template<typename> typename is_target>
			struct fmap_target_type_impl<T, is_target, false> {
				using type = typename fmap_target_type_impl<typename monad_traits<T>::value_type, is_target>::type;
			};
		}
		template<typename fn>
		struct fmap_target {
		private:
			template<typename T, typename efn, typename V = typename efn::template fmap_target<std::nullptr_t>>
			static auto check_impl(efn)->typename efn::template fmap_target<T>;
			template<typename T>
			static auto check_impl(...)->default_fmap_target<T>;
		public:
			template<typename T>
			using is_target = decltype(check_impl<T>(std::declval<typename std::decay<fn>::type>()));
			template<typename T>
			static constexpr bool check() { return is_target<T>::value; }

			template<typename T>
			static constexpr bool check(const T&) { return is_target<T>::value; }

			template<typename T>
			using target_type = typename detail::fmap_target_type_impl<T, is_target>::type;
		};

		namespace detail {
			template<template<typename> typename context_, typename fn>
			struct for_context_wrapper {
				template<typename T>
				using fmap_target = context_<T>;
				const fn& Fn;
			public:
				for_context_wrapper(const fn& Fn_) :Fn(Fn_) {}
				template<typename T>
				auto operator()(T&& Val) const { return Fn(std::forward<T>(Val)); }
			};
		}
		template<template<typename> typename context, typename fn>
		auto for_context(fn&& Fn) {
			return detail::for_context_wrapper<context, typename std::decay<fn>::type>(std::forward<fn>(Fn));
		}

		namespace detail {
			template<typename T>
			using for_top_context = std::true_type;
		}
		template<typename fn>
		auto for_top(fn&& Fn) { return for_context<detail::for_top_context>(Fn); }

		namespace detail {
			template<typename eT, typename efn, bool IsTarget = fmap_target<efn>::check<eT>(), bool IsMonad = is_monad<eT>::value>
			struct fmap_impl {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return Fn(std::forward<T>(val)); }
			};
			template<typename eT, typename efn>
			struct fmap_impl<eT, efn, false, true> {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return val.apply(std::forward<fn>(Fn)); }
			};
			template<typename eT, typename efn>
			struct fmap_impl<eT, efn, false, false> {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return std::forward<T>(val); }
			};
		}
		template<typename T, typename fn>
		auto fmap(T&& val, fn&& Func) {
			return detail::fmap_impl<typename std::decay<T>::type, typename std::decay<fn>::type>()(std::forward<T>(val), std::forward<fn>(Func));
		}

	}
}
#
#endif
