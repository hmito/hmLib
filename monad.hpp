#ifndef HMLIB_MONAD_INC
#define HMLIB_MONAD_INC 100
#
#include<type_traits>
#include<vector>
#include"type_traits.hpp"
#include"lazy.hpp"
namespace hmLib {
	template<typename T>
	struct monad_traits {
		using value_type = T;
		using base_type = T;
		static T flatten(const T& v) { return v; }
		template<typename fn>
		static decltype(auto) map(fn&& Fn, const T& v) { return Fn(v); }
		template<typename fn>
		static decltype(auto) fmap(fn&& Fn, const T& v) { return flatten(Fn(v)); }
	};

	template<typename T>
	decltype(auto) flatten(T&& v) {
		using traits = monad_traits<typename std::decay<T>::type>;
		return traits::flatten(std::forward<T>(v));
	}
	template<typename fn, typename T>
	decltype(auto) map(fn&& Fn, T&& v) {
		using traits = monad_traits<typename std::decay<T>::type>;
		return traits::map(std::forward<fn>(Fn), std::forward<T>(v));
	}
	template<typename fn, typename T>
	decltype(auto) fmap(fn&& Fn, T&& v) {
		using traits = monad_traits<typename std::decay<T>::type>;
		return traits::fmap(std::forward<fn>(Fn), std::forward<T>(v));
	}

	template<typename fn>
	struct map_applier {
		fn& Fn;
		map_applier(fn& Fn_):Fn(Fn_) {}
		template<typename T>
		decltype(auto) operator()(T&& value) {
			return map(Fn, std::forward<T>(value));
		}
	};
	template<typename fn>
	struct fmap_applier {
		fn& Fn;
		fmap_applier(fn& Fn_):Fn(Fn_) {}
		template<typename T>
		decltype(auto) operator()(T&& value) {
			return fmap(Fn, std::forward<T>(value));
		}
	};

	template<template<typename> typename container>
	struct back_insertable_contaienr_monad {
		template<typename U>
		using this_monad = container<U>;

		template<typename fn, typename T>
		static decltype(auto) apply(fn&& Fn, const std::vector<T>& v) {
			using return_type = typename std::decay<decltype(Fn(std::declval<T>()))>::type;
			this_monad<return_type> Ans;
			Ans.reserve(v.size());
			for(const auto& val:v) {
				Ans.emplace_back(Fn(val));
			}
			return Ans;
		}
		template<typename fn, typename T>
		static decltype(auto) flatten_apply(fn&& Fn, const std::vector<T>& v) {
			using return_type = typename std::decay<decltype(Fn(std::declval<T>()))>::type;
			this_monad<return_type> Ans;
			for(const auto& val:v) {
				for(const auto& each:Fn(val)) {
					Ans.emplace_back(each);
				}
			}
			return Ans;
		}


		template<typename fn, typename T>
		static decltype(auto) map(fn&& Fn, const std::vector<T>& v) {
			return apply(map_applier<fn>(Fn), v);
		}
		template<typename fn, typename T>
		static decltype(auto) fmap(fn&& Fn, const std::vector<T>& v) {
			return apply(fmap_applier<fn>(Fn), v);
		}
		template<typename T>
		static decltype(auto) flatten(const std::vector<T>& v) {
			return flatten_apply(return_fn<T>(), const std::vector<T>& v);
		}
	};

	template<typename T>
	struct monad_traits<std::vector<T>> {
	private:
		template<typename U>
		using vector_container = std::vector<U>;
	public:
		using monad_type = back_insertable_contaienr_monad<vector_container>;
		using value_type = typename monad_traits<T>::value_type;
		using base_type = T;
	};

}
#
#endif
