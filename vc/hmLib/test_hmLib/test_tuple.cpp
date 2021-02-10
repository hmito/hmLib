#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../tuple.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	namespace {
		struct NotCopy {
			NotCopy() = default;
			NotCopy(const NotCopy&) = delete;
			NotCopy(NotCopy&&) = default;
		};
		NotCopy getNotCopy() { return NotCopy(); }
	}
	TEST_CLASS(test_tuple) {
		TEST_METHOD(tuple_reduce) {
			Assert::AreEqual(6.5, hmLib::tuple_reduce(std::make_tuple(1, 2.5, 3.0), 0.0, [](double v, auto tv) {return v + tv; }), 1e-5);
		}
	};
}