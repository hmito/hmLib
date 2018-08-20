#include "stdafx.h"
#include "CppUnitTest.h"
#include"../../../lazy.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
TEST_CLASS(test_lazy) {
	struct hoge{
		int value;
		int type;
		hoge()noexcept:value(0),type(0) {}
		hoge(int value_)noexcept:value(value_), type(1) {}
		hoge(const hoge& other)noexcept:value(other.value), type(2) {}
		hoge& operator=(const hoge& other){
			value = other.value;
			type = 3;
			return *this;
		}
		hoge(hoge&& other)noexcept:value(other.value),type(4) {}
		hoge& operator=(hoge&& other) {
			value = other.value;
			type = 5;
			return *this;
		}
	};
public:
TEST_METHOD(make_from_later) {
	auto val = hmLib::later([](int i) {return hoge(i); }, 5);

	auto v1 = val.get();
	Assert::AreEqual(5, v1.value,L"");
	Assert::AreEqual(2, v1.type, L"");
}
};
}