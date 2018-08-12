#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../type_traits.hpp"
#include "../../../tuple.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_type_traits) {
	public:
		TEST_METHOD(const_iterator_check) {
//			static_assert(hmLib::is_const_iterator<typename std::vector<double>::const_iterator>::value);
//			static_assert(!hmLib::is_const_iterator<typename std::vector<double>::iterator>::value);
		}
	};
	TEST_CLASS(test_tuple) {
		TEST_METHOD(apply_at){
			auto t = std::make_tuple(0, 5.0, 'c');
			auto fn = [](auto v)->bool {return v>0; };

			Assert::IsFalse(tuple_apply_at<bool>(0, fn, t));
			Assert::IsTrue(tuple_apply_at<bool>(1, fn, t));
			Assert::IsTrue(tuple_apply_at<bool>(2, fn, t));
		}
	};
}
