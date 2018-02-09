#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../iterators.hpp"
#include "../../../math/axis.hpp"
#include "../../../math/constants.hpp"
#include "../../../math/factorial.hpp"
#include"../../../math/optimize.hpp"
#include"../../../math/root.hpp"
#include"../../../math/plane_root.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_math) {
public:
	TEST_METHOD(axis_use) {
		axis<double> Axis(0,10,100);
		Assert::AreSame(0.1, Axis.grid_width(), L"");
	}
	TEST_METHOD(plane_root_use) {
		std::vector<geometry::point<double>> Ans;
		hmLib::plane_root::grid_cross_search(
			[](double x, double y) {return x*x - y; },
			[](double x, double y) {return -x*x - y+10; },
			geometry::point<double>(-10.0,-10.0), geometry::point<double>(10.0, 10.0),
			101, geometry::point<double>(1e-3, 1e-3), geometry::point<double>(1e-6, 1e-6),back_inserter(Ans)
		);

		Assert::AreEqual(2u, Ans.size(), L"");
	}
	};
}
