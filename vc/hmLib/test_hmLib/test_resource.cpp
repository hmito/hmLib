#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include"../../../clone_ptrproxy.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{		
	TEST_CLASS(test_clone_ptrproxy){
	public:
		TEST_METHOD(make) {
			auto Ptr = make_clone_ptrproxy(5);
			Assert::AreEqual(5, *Ptr, L"");
			*Ptr = 4;
			Assert::AreEqual(4, *Ptr, L"");
		}
		TEST_METHOD(make_with_const) {
			const int i = 5;
			auto Ptr = make_clone_ptrproxy(i);
			Assert::AreEqual(5, *Ptr, L"");
			//*Ptr = 4;
			//Assert::AreEqual(4, *Ptr, L"");
		}
		TEST_METHOD(const_make) {
			auto Ptr = make_const_clone_ptrproxy(5);
			Assert::AreEqual(5, *Ptr, L"");
			//*Ptr = 4;
			//Assert::AreEqual(4, *Ptr, L"");
		}
		TEST_METHOD(make_string) {
			auto Ptr = make_clone_ptrproxy(std::string("test"));
			Assert::AreEqual(4u, Ptr->size(), L"");
			*Ptr = "const";
			Assert::AreEqual(5u, Ptr->size(), L"");
		}
	};
}