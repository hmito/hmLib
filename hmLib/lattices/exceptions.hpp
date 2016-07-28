#ifndef HMLIB_LATTICES_EXCEPTIONS_INC
#define HMLIB_LATTICES_EXCEPTIONS_INC 100
#
#include"../exceptions.hpp"
namespace hmLib{
	namespace lattices{
		struct exception_tag{};
		struct out_of_dim_exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
		using out_of_dim = hmLib::exceptions::exception_pattern<out_of_dim_exception_tag, exception>;
	}
}
#
#endif
