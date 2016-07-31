#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/circular.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_circular){
public:
	TEST_METHOD(construct){
		circular<int, 10> Cir;
		Assert::IsFalse(Cir.full(),L"full function error.");
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual(0u, Cir.size(), L"size function error.");
		Assert::AreEqual(10u, Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
	}
	TEST_METHOD(push_back){
		circular<unsigned int, 10> Cir;
		Cir.push_back(0);
		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(10-i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Cir.push_back(i);
		}
		Assert::IsTrue(Cir.full(), L"full function error.");
		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(10), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(0), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");

		for(unsigned int i = 0; i < Cir.size(); ++i){
			Assert::AreEqual(i, Cir.at(i), L"push_back value error");
		}
	}
	TEST_METHOD(pop_back){
		circular<int, 10> Cir;

		for(unsigned int i = 0; i < Cir.max_size(); ++i){
			Cir.push_back(i);
		}

		Cir.pop_back();

		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(10 - i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Cir.pop_back();
		}
		Assert::IsFalse(Cir.full(), L"full function error.");
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(0), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(10), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
	}
	};
}
