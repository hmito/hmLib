#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../geometry/point.hpp"
#include "../../../geometry/segment.hpp"
#include "../../../math/axis.hpp"
#include "../../../math/constants.hpp"
#include "../../../math/factorial.hpp"
#include"../../../math/optimize.hpp"
#include"../../../math/root.hpp"
//#include"../../../math/plane_root.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_geometry) {
public:
	TEST_METHOD(is_convertible) {
		using namespace hmLib::plane_geometry;

		pint p1(0, 0);
		pdouble p2(0u, 0u);
		pdouble p3(1.0,1.0);

//		pint p6 = p2;

//		const pint& p4 = p3;

		const pint& p5 = static_cast<pint>(p3);

		p3+= p1;
	}
	};
}
