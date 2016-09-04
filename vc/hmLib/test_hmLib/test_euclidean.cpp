#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/euclidean.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_euclidean){
public:
	TEST_METHOD(construct){
		euclidean::point<double, 4> Element(0.0);
		Assert::AreEqual(0.0, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(construct_value){
		euclidean::point<double, 4> Element(0.4);
		Assert::AreEqual(0.4, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(construct_initialize_list){
		euclidean::point<double, 4> Element = {4.0,5.0,6.0,7.0};
		Assert::AreEqual(4.0, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(5.0, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(6.0, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(7.0, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(add){
		euclidean::point<double, 3> e1{1.2,2.4,3.6};
		euclidean::point<double, 3> e2{1.0,1.0,1.0};

		auto e3 = e1 + e2;
		Assert::AreEqual(2.2, e3[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(3.4, e3[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(4.6, e3[2], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(multiple){
		euclidean::point<double, 3> e1{1.2,2.4,3.6};

		auto e3 = e1 * 2;
		Assert::AreEqual(2.4, e3[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(4.8, e3[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(7.2, e3[2], 1.0e-10, L"ValueError");
	}
	};
}
