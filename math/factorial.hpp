#ifndef HMLIB_MATH_FACTORIAL_INC
#define HMLIB_MATH_FACTORIAL_INC 100
#
namespace hmLib {
	template<typename integer_type, typename integer_max_type = unsigned long long>
	constexpr integer_max_type factorial(integer_type val) {
		return val>1 ? val*factorial(val - 1) : 1;
	}
	template<typename integer_type, typename integer_max_type = unsigned long long>
	integer_max_type nPr(integer_type n, integer_type r) {
		integer_max_type Ans = 1;
		for(integer_type i = 1; i<=r; ++i) {
			Ans *= (n - i + 1);
		}
		return Ans;
	}
	template<typename integer_type, typename integer_max_type = unsigned long long>
	integer_max_type nCr(integer_type n, integer_type r) {
		if(r * 2 > n) r = n - r;
		integer_max_type dividend = 1;
		integer_max_type divisor = 1;
		for(integer_type i = 1; i <= r; ++i) {
			dividend *= (n - i + 1);
			divisor *= i;
		}
		return dividend / divisor;
	}
	template<typename integer_type, typename integer_max_type = unsigned long long>
	integer_max_type nHr(integer_type n, integer_type r){
		return nCr<integer_type,integer_max_type>(n + r - 1, r);
	}
}
#
#endif
