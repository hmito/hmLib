#include "stdafx.h"
#include "CppUnitTest.h"
#include <array>
#include <algorithm>
#include <list>
#include "../../../iterators.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_iterators) {
	public:
		TEST_METHOD(use_overflow_iterator) {
			std::array<int, 3> Arr;

			auto Itr = hmLib::make_overflow_iterator(Arr.begin(), Arr.end());
			*(Itr++) = 0;
			Assert::IsFalse(Itr.overflow());
			Assert::IsFalse(Itr.empty());
			*(Itr++) = 1;
			Assert::IsFalse(Itr.overflow());
			Assert::IsFalse(Itr.empty());
			*(Itr++) = 2;
			Assert::IsFalse(Itr.overflow());
			Assert::IsTrue(Itr.empty());
			*(Itr++) = 3;
			Assert::IsTrue(Itr.overflow());
			Assert::IsTrue(Itr.empty());

			for(int i = 0; i < 3; ++i) {
				Assert::AreEqual(i, Arr[i]);
			}
		}
	};
}
