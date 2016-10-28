#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/chain.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_chain){
public:
	TEST_METHOD(element){
		using my_chain = chain<double>;

		my_chain::element Elm1;
		Assert::IsTrue(Elm1, L"Elm cannot be written.");

		*Elm1 = 5.2;

		Assert::AreEqual(5.2, *Elm1);
	}
	TEST_METHOD(construct){
		chain<double> Cir;
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual(0u, Cir.size(), L"size function error.");
	}
	TEST_METHOD(push_back){
		using my_chain = chain<double>;
		my_chain::element Elm1;
		*Elm1 = 3.5;

		my_chain Cir;
		Cir.push_back(Elm1);
		Assert::IsFalse(Elm1, L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(),L"fron function error");
		
		my_chain::element Elm2;
		*Elm2 = 4.5;
		Cir.push_back(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(), L"front function error");
		Assert::AreEqual(4.5, Cir.back(), L"back function error");
	}
	TEST_METHOD(push_front){
		using my_chain = chain<double>;
		my_chain::element Elm1;
		*Elm1 = 3.5;

		my_chain Cir;
		Cir.push_front(Elm1);
		Assert::IsFalse(Elm1, L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(), L"fron function error");
		Assert::AreEqual(3.5, Cir.back(), L"back function error");

		my_chain::element Elm2;
		*Elm2 = 4.5;
		Cir.push_front(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(4.5, Cir.front(), L"back function error");
		Assert::AreEqual(3.5, Cir.back(), L"front function error");
	}
	TEST_METHOD(insert){
		using my_chain = chain<int>;
		my_chain Cir;

		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
			Cir.push_back(Elm[i]);
		}

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(10u, Cir.size(), L"size function error.");

		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(i * 5, *Itr, L"");
			++Itr;
		}
		Assert::IsTrue(Itr==Cir.end(), L"itr == error");
	}
	};
}