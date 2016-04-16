#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace lattices{
		struct exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
	}
}
