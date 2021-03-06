﻿#ifndef HMLIB_LATTICES_EXCEPTIONS_INC
#define HMLIB_LATTICES_EXCEPTIONS_INC 100
#
#include"../exceptions.hpp"
namespace hmLib{
	namespace lattices{
		struct exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
		struct invalid_range_exception_tag {};
		using invalid_range = hmLib::exceptions::exception_pattern<invalid_range_exception_tag, exception>;
	}
}
#
#endif
