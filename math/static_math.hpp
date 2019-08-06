#ifndef HMLIB_MATH_STATICMATH_INC
#define HMLIB_MATH_STATICMATH_INC 100
#
namespace hmLib{
	namespace math{
		namespace detail {
			template<unsigned int p>
			struct static_pow_impl {
				template<typename T>
				static constexpr T calc(const T& v) {
					return v*static_pow_impl<p-1>::calc(v);
				}
			};
			template<>
			struct static_pow_impl<0> {
				template<typename T>
				static constexpr T calc(const T& v) {
					return 1;
				}
			};
		}
	}
	template<unsigned int p, typename T>
	constexpr T static_pow(T v){
		return math::detail::static_pow_impl<p>::calc(v);
	}
}
#
#endif
