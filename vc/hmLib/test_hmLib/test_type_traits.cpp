#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../type_traits.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_type_traits) {
public:
	TEST_METHOD(const_iterator_check) {
		static_assert(hmLib::is_const_iterator<typename std::vector<double>::const_iterator>::value);
		static_assert(!hmLib::is_const_iterator<typename std::vector<double>::iterator>::value);
	}
	};
}
