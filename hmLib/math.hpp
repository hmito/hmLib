#ifndef HMLIB_MATH_INC
#define HMLIB_MATH_INC 100
#
namespace hmLib{
	namespace math{
		constexpr double pi = 3.14159265358979323846;
		constexpr double napier = 2.71828182845904523536;
		
		inline unsigned long long factorial(unsigned long long val){
			return val>1 ? val*factorial(val - 1) : 1;
		}
	}
}
#
#endif
