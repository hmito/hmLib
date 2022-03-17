#ifndef HMLIB_MATH_MATH_INC
#define HMLIB_MATH_MATH_INC 100
#
namespace hmLib{
	template<typename T>
	T ipow(T base, unsigned int exp){
		T result = 1;

		if (exp & 1)result *= base;
		exp >>= 1;

		while(exp){
			base *= base;
			if (exp & 1)result *= base;
			exp >>= 1;
		}
		return result;
	}
	namespace math{namespace detail{
		template <class T, bool is_integral = std::is_integral<T>::value>
		struct div_or_impl{
			inline T operator()(T numerator, T denominator, T otherwise)const noexcept {
				if(std::abs(denominator) < 1 && std::abs(denominator * tools::max_value<T>()) <= std::abs(numerator)){
					return otherwise;
				}
				return numerator / denominator;
			}
		};
		template <class T>
		struct div_or_impl<T,true>{
			inline T operator()(T numerator, T denominator, T otherwise)const noexcept {
				if(denominator==0){
					return otherwise;
				}
				return numerator / denominator;
			}
		};
	}}
	template <class T, bool is_int = std::is_integral<T>::value>
	inline T div_or(T numerator, T denominator, T otherwise){
		return math::detail::div_or_impl<T>(numerator,denominator,otherwise);
	}
}
#
#endif
