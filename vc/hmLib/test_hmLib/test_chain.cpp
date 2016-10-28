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
	}
	};
}