#include "stdafx.h"
#include "CppUnitTest.h"
#include <array>
#include <algorithm>
#include <vector>
#include <list>
#include "../../../iterators.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_overflow_iterator) {
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
	TEST_CLASS(test_index_access_iterator) {
	public:
		TEST_METHOD(use_index_access_iterator_list) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::list<int> Index{ 2,3,5 };

			auto Range = make_index_access_range(Val.begin(), Index.begin(),Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
		}
		TEST_METHOD(use_index_access_iterator_vector) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_access_range(Val.begin(), Index.begin(),Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
			Assert::AreEqual(Index.size(), Range.size());
		}
	};
}
