#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../tuple.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	namespace {
		struct NotCopy {
			NotCopy() = default;
			explicit NotCopy(int x_) :x(x_) {}
			NotCopy(const NotCopy&) = delete;
			NotCopy(NotCopy&& v)noexcept{
				x = v.x;
				v.x = -1;
			}
			NotCopy& operator=(const NotCopy&) = delete;
			NotCopy& operator=(NotCopy&& v)noexcept {
				if (this != &v) {
					x = v.x;
					v.x = -1;
				}
			}
			int x;
			int& ref() { return x; }
		};
		NotCopy getNotCopy() { return NotCopy(); }
	}
	TEST_CLASS(test_tuple) {
		TEST_METHOD(tuple_reduce) {
			Assert::AreEqual(6.5, hmLib::tuple_reduce(std::make_tuple(1, 2.5, 3.0), 0.0, [](double v, auto tv) {return v + tv; }), 1e-5);
			Assert::AreEqual(6, hmLib::tuple_reduce(std::make_tuple(NotCopy(1), NotCopy(2), NotCopy(3)), 0, [](int v, auto& tv) {return v + tv.x; }));
		}
		TEST_METHOD(tuple_transform_ref) {
			auto t = std::make_tuple(NotCopy(1), NotCopy(2), NotCopy(3));
			auto x = hmLib::tuple_transform(t, [](auto& v)->int& {return v.ref(); });
			std::get<0>(x) = 4;
			std::get<1>(x) = 5;
			std::get<2>(x) = 6;
			Assert::AreEqual(4, std::get<0>(t).x);
			Assert::AreEqual(5, std::get<1>(t).x);
			Assert::AreEqual(6, std::get<2>(t).x);
		}
		TEST_METHOD(tuple_transform_move) {
			auto p = std::make_tuple(NotCopy(1), NotCopy(2), NotCopy(3));
			auto q = hmLib::tuple_transform(p, [](auto& v)->auto {if (v.x % 2 == 0)return NotCopy(10);  return std::move(v); });
			Assert::AreEqual(-1, std::get<0>(p).x);
			Assert::AreEqual(2, std::get<1>(p).x);
			Assert::AreEqual(-1, std::get<2>(p).x);
			Assert::AreEqual(1, std::get<0>(q).x);
			Assert::AreEqual(10, std::get<1>(q).x);
			Assert::AreEqual(3, std::get<2>(q).x);
		}

	};
}
