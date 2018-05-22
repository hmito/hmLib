#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../varray.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_varray) {
public:
	TEST_METHOD(construct) {
		varray<double, 4> Element(0.0);
		Assert::AreEqual(0.0, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.0, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(construct_value) {
		varray<double, 4> Element(0.4);
		Assert::AreEqual(0.4, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(0.4, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(construct_initialize_list) {
		varray<double, 4> Element = { 4.0,5.0,6.0,7.0 };
		Assert::AreEqual(4.0, Element[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(5.0, Element[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(6.0, Element[2], 1.0e-10, L"ValueError");
		Assert::AreEqual(7.0, Element[3], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(add) {
		varray<double, 3> e1{ 1.2,2.4,3.6 };
		varray<double, 3> e2{ 1.0,1.0,1.0 };

		auto e3 = e1 + e2;
		Assert::AreEqual(2.2, e3[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(3.4, e3[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(4.6, e3[2], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(multiple) {
		varray<double, 3> e1{ 1.2,2.4,3.6 };

		auto e3 = e1 * 2;
		Assert::AreEqual(2.4, e3[0], 1.0e-10, L"ValueError");
		Assert::AreEqual(4.8, e3[1], 1.0e-10, L"ValueError");
		Assert::AreEqual(7.2, e3[2], 1.0e-10, L"ValueError");
	}
	TEST_METHOD(add_scalar) {
		varray<int, 3> ei{ 1,2,3 };

		auto eii = ei + 2;
		Assert::IsTrue(std::is_same_v<decltype(eii), varray<int, 3>>, L"add int+int = int");

		auto eif = ei + 2.0;
		Assert::IsTrue(std::is_same_v<decltype(eif), varray<double, 3>>, L"add int+double= double");

		auto eeii = ei+ei;
		Assert::IsTrue(std::is_same_v<decltype(eeii), varray<int, 3>>, L"add double+double= double");

		varray<double, 3> ef{ 1.0,2.0,3.0 };

		auto efi = ef + 2;
		Assert::IsTrue(std::is_same_v<decltype(efi), varray<double, 3>>, L"add double+int = double");

		auto eff = ef + 2.0;
		Assert::IsTrue(std::is_same_v<decltype(eff), varray<double, 3>>, L"add double+double= double");

		auto eeff = ei+ef;
		Assert::IsTrue(std::is_same_v<decltype(eeff), varray<double, 3>>, L"add double+double= double");
	}
	TEST_METHOD(divmod) {
		//varray<double, 3> e1{ 1.2,2.4,3.6 };
		//auto e1a = e1%3;
		//compile error

		varray<int, 3> e2{ 5,6,7 };
		auto e2a = e2%3;
		Assert::AreEqual(2, e2a[0]);
		Assert::AreEqual(0, e2a[1]);
		Assert::AreEqual(1, e2a[2]);
	}
	};
}
