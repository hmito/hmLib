#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../hmLib/factorial.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{		
	TEST_CLASS(test_factorial){
	public:
		TEST_METHOD(consturct){
			hmLib::factorial Factorial(5);
			Assert::AreEqual(static_cast<unsigned int>(5 * 4 * 3 * 2), Factorial.evaluate());
		}
		TEST_METHOD(consturct_complex){
			hmLib::factorial_complex Factorial(5);
			Assert::AreEqual<double>(static_cast<double>(5 * 4 * 3 * 2), Factorial.evaluate());
		}
		TEST_METHOD(multi){
			double val = hmLib::factorial(5) * hmLib::factorial(3);
			Assert::AreEqual<double>(static_cast<double>(5 * 4*3*2*3*2), val);
		}
		TEST_METHOD(div){
			double val = hmLib::factorial(5)/ hmLib::factorial(3);
			Assert::AreEqual<double>(static_cast<double>(5 * 4), val);
		}
		TEST_METHOD(multi2){
			double val = hmLib::factorial(5) * hmLib::factorial(3)/ hmLib::factorial(4);
			Assert::AreEqual<double>(static_cast<double>(5 * 3 * 2 ), val);
		}
		TEST_METHOD(div2){
			double val = hmLib::factorial(3) / hmLib::factorial(5) * hmLib::factorial(7);
			Assert::AreEqual<double>(static_cast<double>(3*2*7*6), val);
		}
		TEST_METHOD(div3){
			double val = hmLib::factorial(5) / hmLib::factorial(4) / hmLib::factorial(7);
			Assert::AreEqual<double>(1/static_cast<double>(7*6*4*3*2), val);
		}
		TEST_METHOD(plus){
			double val = hmLib::factorial(5) + hmLib::factorial(7);
			Assert::AreEqual<double>(static_cast<double>(7 * 6 * 5 * 4 * 3 * 2 + 5*4*3*2), val);
		}
	};
}
