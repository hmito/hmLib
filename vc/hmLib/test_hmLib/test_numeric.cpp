#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include"../../../numeric.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_numeric) {
	public:
	TEST_METHOD(bisect_root) {
		//auto fn = [](double x) {return (x - 0.222)* (x - 0.222)-0.5; }
		auto fn = [](double x) {return (x - 0.222) * (x + 0.333); };

		auto ans = hmLib::numeric::bisect_root(fn, 0.0, 1.0, 1000, 1e-4, 1e-5);

		Assert::AreEqual(0.222, ans.first.upper.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.lower.v, 1e-4);
	}
	TEST_METHOD(toms748_root) {
		//auto fn = [](double x) {return (x - 0.222)* (x - 0.222)-0.5; }
		auto fn = [](double x) {return (x - 0.222) * (x + 0.333); };

		auto ans = hmLib::numeric::toms748_root(fn, 0.0, 1.0, 1000, 1e-4, 1e-5);

		Assert::AreEqual(0.222, ans.first.upper.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.lower.v, 1e-4);
	}
	TEST_METHOD(golden_section_minima) {
		auto fn = [](double x) {return (x - 0.222) * (x - 0.222); };
		//auto fn = [](double x) {return (x - 0.222) * (x + 0.333); };

		auto ans = hmLib::numeric::golden_section_minima(fn, 0.0, 1.0, 1000, 1e-4, 1e-5);

		Assert::AreEqual(0.222, ans.first.upper.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.guess.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.lower.v, 1e-4);
	}
	TEST_METHOD(brent_minima) {
		auto fn = [](double x) {return (x - 0.222) * (x - 0.222); };
		//auto fn = [](double x) {return (x - 0.222) * (x + 0.333); };

		auto ans = hmLib::numeric::brent_minima(fn, 0.0, 1.0, 1000, 1e-4, 1e-5);

		Assert::AreEqual(0.222, ans.first.upper.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.guess.v, 1e-4);
		Assert::AreEqual(0.222, ans.first.lower.v, 1e-4);
	}
	};
}
