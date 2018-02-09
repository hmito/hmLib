#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../geometry/point.hpp"
#include "../../../geometry/segment.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_geometry) {
public:
	TEST_METHOD(is_convertible) {
		using namespace hmLib::geometry;

		pint p1(0, 0);
		pdouble p2(0u, 0u);
		pdouble p3(1.0,1.0);

		const pint& p4 = p3;

		const pint& p5 = static_cast<pint>(p3);

		p3+= p1;
	}
	};
}
