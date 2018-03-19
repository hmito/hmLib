#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../iterators.hpp"
#include "../../../math/root.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_math) {
	public:
		TEST_METHOD(root_finding_toms748) {
		auto f = [](double x) {return (x - 0.3)*(x - 0.4549)*(x - 0.8991); };

		std::vector<double> Ans;
		hmLib::root::root_toms748(f, 0.0, 1.0, 0.001, 1e-5, hmLib::back_inserter(Ans));

		Assert::AreEqual(3u, Ans.size(), L"Ans Num Error");
		Assert::AreEqual(0.3, Ans.at(0), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.4549, Ans.at(1), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.8991, Ans.at(2), 1e-5, L"Ans Num Error");
		}
		TEST_METHOD(root_finding_bisect) {
		auto f = [](double x) {return (x - 0.3)*(x - 0.4549)*(x - 0.8991); };

		std::vector<double> Ans;
		hmLib::root::root_bisect(f, 0.0, 1.0, 0.001, 1e-5, hmLib::back_inserter(Ans));

		Assert::AreEqual(3u, Ans.size(), L"Ans Num Error");
		Assert::AreEqual(0.3, Ans.at(0), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.4549, Ans.at(1), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.8991, Ans.at(2), 1e-5, L"Ans Num Error");
		}
	};
}
