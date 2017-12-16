#ifndef HMLIB_TUPLE_INC
#define HMLIB_TUPLE_INC 100
#
#include<tuple>
namespace hmLib {
	template<std::size_t n, typename Fn, typename... Tuple>
	auto tuple_apply_at(Fn f, Tuple... t) {
		return f(std::get<n>(t)...);
	}
	namespace detail {
		template<std::size_t r, std::size_t n, typename Fn, typename... Tuple>
		struct tuple_for_each_impl{
			auto operator()(Fn f, Tuple... t) {
				return std::tuple_cat(
					std::make_tuple(f(std::get<n>(t)...)),
					tuple_for_each_impl<r - 1, n + 1, Fn, Tuple...>()(f, t...)
				);
			}
		};
		template<std::size_t n, typename Fn, typename... Tuple>
		struct tuple_for_each_impl<1,n,Fn,Tuple...>{
			auto operator()(Fn f, Tuple... t) {
				return std::make_tuple(
					f(std::get<n>(t)...)
				);
			}
		};
	}
	template<typename Fn, typename Tuple, typename... OtherTuples>
	auto tuple_for_each(Fn f, Tuple t, OtherTuples... ot) {
		return detail::tuple_for_each_impl<std::tuple_size<Tuple>::value, 0, Fn, Tuple, OtherTuples...>()(
			std::forward<Fn>(f),
			std::forward<Tuple>(t),
			std::forward<OtherTuples>(ot)...
		);
	}
}
#
#endif
