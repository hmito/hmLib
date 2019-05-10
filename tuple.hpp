#ifndef HMLIB_TUPLE_INC
#define HMLIB_TUPLE_INC 100
#
#include<tuple>
#include<utility>
#include"exceptions.hpp"
#include"iterators/transform_iterator.hpp"
#include"hash.hpp"
namespace hmLib {
	namespace detail {
		template<size_t n, size_t r>
		struct tuple_transform_impl {
			template<typename Fn, typename... Tuple>
			auto operator()(Fn&& f, Tuple&&... t) {
				return std::tuple_cat(
					std::tuple<decltype((f(std::get<n>(t)...)))>(f(std::get<n>(t)...)),
					tuple_transform_impl<n+1,r-1>()(f, t...)
				);
			}
		};
		template<size_t n>
		struct tuple_transform_impl<n,1>{
			template<typename Fn, typename... Tuple>
			auto operator()(Fn&& f, Tuple&&... t) {
				return std::tuple<decltype((f(std::get<n>(t)...)))>(f(std::get<n>(t)...));
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	auto tuple_transform(Fn&& f, Tuple&& t, OtherTuples&& ... ot) {
		return detail::tuple_transform_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}

	//old definition
	template<typename Fn, typename Tuple, typename... OtherTuples>
	auto tuple_for_each(Fn&& f, Tuple&& t, OtherTuples&&... ot) {
		return detail::tuple_transform_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}

	namespace detail {
		template<typename T, size_t n>
		struct tuple_array_impl {
			auto operator()(const T& ini) {
				return std::tuple_cat(
					std::tuple<T>(ini),
					tuple_array_impl<T, n-1>()(ini)
				);
			}
		};
		template<typename T>
		struct tuple_array_impl<T, 1> {
			auto operator()(const T& ini) { return std::tuple<T>(ini); }
		};
	}
	template<typename T, size_t n>
	auto tuple_array(const T& ini) {
		return detail::tuple_array_impl<T, n>()(ini);
	}

	namespace detail {
		template<typename T, size_t n>
		struct tuple_generate_impl {
			template<typename Fn>
			auto operator()(Fn&& f) {
				return std::tuple_cat(
					std::tuple<T>(f()),
					tuple_generate_impl<T, n - 1>()(f)
				);
			}
		};
		template<typename T>
		struct tuple_generate_impl<T, 1> {
			template<typename Fn>
			auto operator()(Fn&& f) { return std::tuple<T>(f()); }
		};
	}
	template<size_t n, typename Fn>
	auto tuple_generate(Fn&& f) {
		return detail::tuple_generate_impl<decltype(f()), n>()(std::forward<Fn>(f));
	}

	namespace detail {
		template<typename ans_type, unsigned int n, unsigned int r>
		struct tuple_apply_at_impl {
			template<typename Fn, typename... Tuple>
			decltype(auto) operator()(unsigned int pos, Fn&& f, Tuple&&... v) {
				if(n==pos) {
					return f(std::get<n>(v)...);
				}
				return tuple_apply_at_impl<n+1,r-1>()(pos, std::forward<Fn>(f), std::forward<Tuple>(v)...);
			}
		};
		template<unsigned int n>
		struct tuple_apply_at_impl<n,0>{
			template<typename Fn, typename... Tuple>
			decltype(auto) operator()(unsigned int pos, Fn&& f, Tuple&&... v) {
				hmLib_throw(hmLib::access_exceptions::out_of_range_access, "tuple access is out of range.");
				return f(std::get<0>(v)...);
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	decltype(auto) tuple_apply_at(unsigned int n, Fn&& f, Tuple&& t, OtherTuples&&... ot){
		return detail::tuple_apply_at_impl<0,std::tuple_size<typename std::decay<Tuple>::type>::value>()(n,
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}

	template<std::size_t n, typename Fn, typename... Tuple>
	decltype(auto) tuple_apply_at(Fn f, Tuple... t) {
		return f(std::get<n>(t)...);
	}

	namespace detail {
		template <class F, class Tuple, std::size_t... I>
		decltype(auto) tuple_apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
			return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
		}
	}
	template <class F, class Tuple>
	decltype(auto) tuple_apply(F&& f, Tuple&& t) {
		return detail::tuple_apply_impl(
			std::forward<F>(f), std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>{}
		);
	}

	namespace detail {
		template<unsigned int n, unsigned int r>
		struct tuple_reduce_impl {
			template<typename Fn, typename Tuple, typename T>
			auto operator()(Fn&& f, Tuple&& t, T Ini) {
				return tuple_reduce_impl<n+1, r-1>()(f, t, f(Ini, std::get<n>(t)));
			}
		};
		template<unsigned int n>
		struct tuple_reduce_impl<n, 1> {
			template<typename Fn, typename Tuple, typename T>
			auto operator()(Fn&& f, Tuple&& t, T Ini) {
				return f(Ini, std::get<n>(t));
			}
		};
	}
	template<typename Fn, typename Tuple,typename T>
	auto tuple_reduce(Fn&& f, Tuple&& t, T Ini) {
		return detail::tuple_reduce_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			Ini
		);
	}

	template<typename... T>
	struct hash< std::tuple<T...> > {
		using result_type = std::size_t;
		using argument_type = std::tuple<T...>;
		result_type operator()(const argument_type& key)const {
			return tuple_reduce([](result_type seed, auto v) {return merge_hash(seed, hash<typename std::decay<decltype(v)>::type>()(v)); }, key, result_type(0));
		}
	};

	template<std::size_t n>
	struct getter{
		template<typename T>
		decltype(auto) operator()(T&& val)const{ return std::get<n>(std::forward<T>(val)); }
	};

	template<std::size_t n, typename iterator>
	decltype(auto) make_get_range(iterator Beg, iterator End) {
		return hmLib::make_transform_range(Beg, End, getter<n>());
	}
}
#
#endif
