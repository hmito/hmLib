#ifndef HMLIB_LATTICES_EXCEPTIONS_INC
#define HMLIB_LATTICES_EXCEPTIONS_INC 100
#
#include"../exceptions.hpp"
namespace hmLib{
	namespace lattices{
		struct exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
		struct out_of_dim_exception_tag {};
		using out_of_dim = hmLib::exceptions::exception_pattern<out_of_dim_exception_tag, exception>;
		struct invalid_view_range_exception_tag{};
		using invalid_view_range = hmLib::exceptions::exception_pattern<invalid_view_range_exception_tag, exception>;
	}
}
#
#endif
