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
}
#
#endif
