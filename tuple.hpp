#ifndef HMLIB_TUPLE_INC
#define HMLIB_TUPLE_INC 100
#
#include<tuple>
#include<utility>
#include"exceptions.hpp"
#include"hash.hpp"
namespace hmLib {
	namespace detail {
		template<unsigned int n, unsigned int r>
		struct tuple_for_each_impl{
			template<typename Fn, typename... Tuple>
			auto operator()(Fn&& f, Tuple&&... t) {
				return std::tuple_cat(
					std::tuple<decltype((f(std::get<n>(t)...)))>(f(std::get<n>(t)...)),
					tuple_for_each_impl<n+1,r-1>()(f, t...)
				);
			}
		};
		template<unsigned int n>
		struct tuple_for_each_impl<n,1>{
			template<typename Fn, typename... Tuple>
			auto operator()(Fn&& f, Tuple&&... t) {
				return std::tuple<decltype((f(std::get<n>(t)...)))>(f(std::get<n>(t)...));
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	auto tuple_for_each(Fn&& f, Tuple&& t, OtherTuples&&... ot) {
		return detail::tuple_for_each_impl<0, std::tuple_size<typename std::decay<Tuple>::type>::value>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}

	namespace detail {
		template<typename ans_type, unsigned int n, unsigned int r>
		struct tuple_apply_at_impl {
			template<typename Fn, typename... Tuple>
			ans_type operator()(unsigned int pos, Fn&& f, Tuple&&... v) {
				if(n==pos) {
					return f(std::get<n>(v)...);
				}
				return tuple_apply_at_impl<ans_type, n+1,r-1>()(pos, std::forward<Fn>(f), std::forward<Tuple>(v)...);
			}
		};
		template<typename ans_type, unsigned int n>
		struct tuple_apply_at_impl<ans_type, n,0>{
			template<typename Fn, typename... Tuple>
			ans_type operator()(unsigned int pos, Fn&& f, Tuple&&... v) {
				hmLib_throw(hmLib::access_exceptions::out_of_range_access, "tuple access is out of range.");
			}
		};
	}
	template<typename ans_type, typename Fn, typename Tuple, typename... OtherTuples>
	ans_type tuple_apply_at(unsigned int n, Fn&& f, Tuple&& t, OtherTuples&&... ot){
		return detail::tuple_apply_at_impl<ans_type, 0,std::tuple_size<typename std::decay<Tuple>::type>::value>()(n,
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}
	template<std::size_t n, typename Fn, typename... Tuple>
	auto tuple_apply_at(Fn f, Tuple... t) {
		return f(std::get<n>(t)...);
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
}
#
#endif
