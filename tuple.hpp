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
		template<unsigned int n, bool is_end>
		struct tuple_reduce_impl {
			template<typename Tuple, typename T, typename Fn>
			T operator()(Tuple&& t, T Ini, Fn&& f)const {
				Ini = f(Ini, std::get<n>(t));
				return tuple_reduce_impl<n + 1, std::tuple_size<typename std::decay<Tuple>::type>::value == n + 1>()(
					std::forward<Tuple>(t), Ini, std::forward<Fn>(f)
				);
			}
		};
		template<unsigned int n>
		struct tuple_reduce_impl<n, true> {
			template<typename Tuple, typename T, typename Fn>
			T operator()(Tuple&& t, T Ini, Fn&& f)const {
				return Ini;
			}
		};
	}
	template<typename Tuple, typename T, typename Fn>
	T tuple_reduce(Tuple&& t, T Ini, Fn&& f) {
		return detail::tuple_reduce_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value == 0>()(
			std::forward<Tuple>(t),Ini,std::forward<Fn>(f)
		);
	}

	namespace detail {
		template<unsigned int n, bool is_end>
		struct tuple_foreach_impl {
			template<typename Tuple, typename Fn>
			void operator()(Tuple&& t, Fn&& f)const {
				f(std::get<n>(t));
				tuple_foreach_impl<n + 1, std::tuple_size<typename std::decay<Tuple>::type>::value == n + 1>()(std::forward<Tuple>(t), std::forward<Fn>(f));
			}
		};
		template<unsigned int n>
		struct tuple_foreach_impl<n, true> {
			template<typename Tuple, typename Fn>
			void operator()(Tuple&& t, Fn&& f)const {
				return;
			}
		};
	}
	template<typename Tuple, typename Fn>
	void tuple_foreach(Tuple&& t, Fn&& f) {
		return detail::tuple_foreach_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value == 0>()(
			std::forward<Tuple>(t),
			std::forward<Fn>(f)
		);
	}

	namespace detail {
		template<size_t n, bool is_end>
		struct tuple_transform_impl {
			template<typename Tuple, typename Fn>
			auto operator()(Tuple&& t, Fn&& f)const {
				std::tuple<decltype((f(std::get<n>(t))))> eval(f(std::get<n>(t)));
				return std::tuple_cat(
					std::move(eval),
					tuple_transform_impl<n + 1, std::tuple_size<typename std::decay<Tuple>::type>::value == n + 1>()(
						std::forward<Tuple>(t),
						std::forward<Fn>(f)
					)
				);
			}
			template<typename Tuple1, typename Tuple2, typename Fn>
			auto operator()(Tuple1&& t1, Tuple2&& t2, Fn&& f)const {
				std::tuple<decltype((f(std::get<n>(t1), std::get<n>(t2))))> eval(f(std::get<n>(t1), std::get<n>(t2)));
				return std::tuple_cat(
					eval,
					tuple_transform_impl<n + 1, std::tuple_size<typename std::decay<Tuple1>::type>::value == n + 1>()(
						std::forward<Tuple1>(t1),
						std::forward<Tuple2>(t2),
						std::forward<Fn>(f)
					)
				);
			}
		};
		template<size_t n>
		struct tuple_transform_impl<n, true> {
			template<typename Tuple, typename Fn>
			auto operator()(Tuple&& t, Fn&& f)const {
				return std::tuple<>();
			}
			template<typename Tuple1, typename Tuple2, typename Fn>
			auto operator()(Tuple1&& t1, Tuple2&& t2, Fn&& f)const {
				return std::tuple<>();
			}
		};
	}
	template<typename Tuple, typename Fn>
	auto tuple_transform(Tuple&& t, Fn&& f) {
		return detail::tuple_transform_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value == 0>()(
			std::forward<Tuple>(t),
			std::forward<Fn>(f)
		);
	}
	template<typename Tuple1, typename Tuple2, typename Fn>
	auto tuple_transform(Tuple1&& t1, Tuple2&& t2, Fn&& f) {
		return detail::tuple_transform_impl<0, std::tuple_size<typename std::decay<Tuple1>::type>::value == 0>()(
			std::forward<Tuple1>(t1),
			std::forward<Tuple2>(t2),
			std::forward<Fn>(f)
		);
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
		template<size_t n, bool is_end>
		struct tuple_invoke_impl {
			template<typename Fn, typename Tuple, typename... OtherTuples>
			auto operator()(Fn&& f, Tuple&& t, OtherTuples&&... ot) {
				std::tuple<decltype((f(std::get<n>(t),std::get<n>(ot)...)))> eval(f(std::get<n>(t), std::get<n>(ot)...));
				return std::tuple_cat(
					std::move(eval),
					tuple_invoke_impl<n + 1, std::tuple_size<typename std::decay<Tuple>::type>::value == n + 1>()(
						std::forward<Fn>(f), std::forward<Tuple>(t), std::forward<Tuple>(ot)...
					)
				);
			}
		};
		template<size_t n>
		struct tuple_invoke_impl<n, true> {
			template<typename Fn, typename Tuple, typename... OtherTuples>
			auto operator()(Fn&& f, Tuple&& t, OtherTuples&&... ot) {
				return std::tuple<>();
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	auto tuple_invoke(Fn&& f, Tuple&& t, OtherTuples&& ... ot) {
		return detail::tuple_invoke_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value == 0>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}

	namespace detail {
		template<unsigned int n, bool is_end>
		struct tuple_invoke_at_impl {
			template<typename Fn, typename Tuple, typename... OtherTuples>
			decltype(auto) operator()(unsigned int pos, Fn&& f, Tuple&& t, OtherTuples&&... ot) {
				if (n == pos) {
					return f(std::get<n>(t),std::get<n>(ot)...);
				}
				return tuple_invoke_at_impl<n + 1, std::tuple_size<typename std::decay<Tuple>::type>::value == n + 1>()(
					pos, std::forward<Fn>(f), std::forward<Tuple>(t), std::forward<OtherTuples>(ot)...);
			}
		};
		template<unsigned int n>
		struct tuple_invoke_at_impl<n, true> {
			template<typename Fn, typename Tuple, typename... OtherTuples>
			decltype(auto) operator()(unsigned int pos, Fn&& f, Tuple&& t, OtherTuples&&... ot) {
				hmLib_throw(hmLib::access_exceptions::out_of_range_access, "tuple_invoke_at index is out of range.");
				return f(std::get<0>(t), std::get<0>(ot)...);
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	decltype(auto) tuple_invoke_at(unsigned int n, Fn&& f, Tuple&& t, OtherTuples&&... ot) {
		return detail::tuple_invoke_at_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value==0>()(n,
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
		struct tuple_array_impl<T, 0> {
			auto operator()(const T& ini) { return std::tuple<>(); }
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
		struct tuple_generate_impl<T, 0> {
			template<typename Fn>
			auto operator()(Fn&& f) { return std::tuple<>(); }
		};
	}
	template<size_t n, typename Fn>
	auto tuple_generate(Fn&& f) {
		return detail::tuple_generate_impl<decltype(f()), n>()(std::forward<Fn>(f));
	}

	template<typename... T>
	struct hash< std::tuple<T...> > {
		using result_type = std::size_t;
		using argument_type = std::tuple<T...>;
		result_type operator()(const argument_type& key)const {
			return tuple_reduce(key, result_type(0), [](result_type seed, auto v) {return merge_hash(seed, hash<typename std::decay<decltype(v)>::type>()(v)); });
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
