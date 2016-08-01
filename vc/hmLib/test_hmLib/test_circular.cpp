#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/circular.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_circular){
		struct value_t{
		private:
			static unsigned int Cnt;
		public:
			static unsigned int cnt(){ return Cnt; }
		private:
			int Val;
		public:
			value_t() :Val(0){ ++Cnt; }
			value_t(int Val_) :Val(Val_){ ++Cnt; }
			value_t(const value_t& val) :Val(val.Val){ ++Cnt; }
			value_t& operator=(const value_t&) = default;
			~value_t(){ --Cnt; }
			int value()const{ return Val; }
			operator int()const{ return Val; }
		};
public:
	TEST_METHOD(construct){
		circular<value_t, 10> Cir;
		Assert::IsFalse(Cir.full(),L"full function error.");
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual(0u, Cir.size(), L"size function error.");
		Assert::AreEqual(10u, Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
	}
	TEST_METHOD(push_back){
		{
			value_t Val;
			Assert::AreEqual(1u, value_t::cnt(), L"construct number error");
		}

		circular<value_t, 10> Cir;
		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");

		Cir.push_back(0);

		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(10-i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Assert::AreEqual(i, value_t::cnt(), L"construct number error");
			Cir.push_back(i);
		}
		Assert::IsTrue(Cir.full(), L"full function error.");
		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(10), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(0), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
		Assert::AreEqual(10u, value_t::cnt(), L"construct number error");

		for(unsigned int i = 0; i < Cir.size(); ++i){
			Assert::AreEqual<int>(i, Cir.at(i), L"push_back value error");
		}
	}
	TEST_METHOD(pop_back){
		circular<value_t, 10> Cir;

		for(unsigned int i = 0; i < Cir.max_size(); ++i){
			Cir.push_back(i);
		}
		Assert::AreEqual(10u, value_t::cnt(), L"construct number error");

		Cir.pop_back();

		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(10 - i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Assert::AreEqual(10u-i, value_t::cnt(), L"construct number error");
			Cir.pop_back();
		}
		Assert::IsFalse(Cir.full(), L"full function error.");
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(0), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(10), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");
	}
	TEST_METHOD(push_front){
		circular<value_t, 10> Cir;
		Cir.push_front(0);
		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(10 - i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Assert::AreEqual(i, value_t::cnt(), L"construct number error");
			Cir.push_front(i);
		}
		Assert::IsTrue(Cir.full(), L"full function error.");
		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(10), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(0), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
		Assert::AreEqual(10u, value_t::cnt(), L"construct number error");

		for(unsigned int i = 0; i < Cir.size(); ++i){
			Assert::AreEqual<int>(i, Cir.at(Cir.size()-i-1), L"push_back value error");
		}
	}
	TEST_METHOD(pop_front){
		circular<value_t, 10> Cir;

		for(unsigned int i = 0; i < Cir.max_size(); ++i){
			Cir.push_front(i);
		}

		Assert::AreEqual<int>(int(Cir.size()-1), Cir.front(), L"value error");
		Assert::AreEqual(10u, value_t::cnt(), L"construct number error");
		Cir.pop_front();

		for(unsigned int i = 1; i < Cir.max_size(); ++i){
			Assert::IsFalse(Cir.full(), L"full function error.");
			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual((unsigned int)(10 - i), Cir.size(), L"size function error.");
			Assert::AreEqual((unsigned int)(i), Cir.remain(), L"remain function error.");
			Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
			Assert::AreEqual<int>(int(Cir.max_size() - 1-i), Cir.front(), L"value error");
			Assert::AreEqual(10u-i, value_t::cnt(), L"construct number error");
			Cir.pop_front();
		}
		Assert::IsFalse(Cir.full(), L"full function error.");
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual((unsigned int)(0), Cir.size(), L"size function error.");
		Assert::AreEqual((unsigned int)(10), Cir.remain(), L"remain function error.");
		Assert::AreEqual(10u, Cir.max_size(), L"max_size function error.");
		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");
	}
	TEST_METHOD(push_and_pop1){
		circular<value_t, 5> Cir;

		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");

		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");

		Cir.push_back(0);
		Cir.push_back(1);
		Cir.push_back(2);
		Cir.push_back(3);
		Assert::AreEqual<int>(0, Cir.at(0), L"push value error");
		Assert::AreEqual<int>(1, Cir.at(1), L"push value error");
		Assert::AreEqual<int>(2, Cir.at(2), L"push value error");
		Assert::AreEqual<int>(3, Cir.at(3), L"push value error");
		Assert::AreEqual(4u, Cir.size(), L"push size error");
		Assert::AreEqual(4u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(1, Cir.front());
		Assert::AreEqual<int>(3, Cir.back());
		Assert::AreEqual(3u, Cir.size(), L"push size error");
		Assert::AreEqual(3u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(2, Cir.front());
		Assert::AreEqual<int>(3, Cir.back());
		Assert::AreEqual(2u, Cir.size(), L"push size error");
		Assert::AreEqual(2u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(3, Cir.front());
		Assert::AreEqual<int>(3, Cir.back());
		Assert::AreEqual(1u, Cir.size(), L"push size error");
		Assert::AreEqual(1u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty error");
	}
	TEST_METHOD(push_and_pop2){
		circular<value_t, 5> Cir;

		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");

		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");

		Cir.push_front(0);
		Cir.push_front(1);
		Cir.push_front(2);
		Cir.push_front(3);
		Assert::AreEqual<int>(3, Cir.at(0), L"push value error");
		Assert::AreEqual<int>(2, Cir.at(1), L"push value error");
		Assert::AreEqual<int>(1, Cir.at(2), L"push value error");
		Assert::AreEqual<int>(0, Cir.at(3), L"push value error");
		Assert::AreEqual(4u, Cir.size(), L"push size error");
		Assert::AreEqual(4u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(2, Cir.front());
		Assert::AreEqual<int>(0, Cir.back());
		Assert::AreEqual(3u, Cir.size(), L"push size error");
		Assert::AreEqual(3u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(1, Cir.front());
		Assert::AreEqual<int>(0, Cir.back());
		Assert::AreEqual(2u, Cir.size(), L"push size error");
		Assert::AreEqual(2u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::AreEqual<int>(0, Cir.front());
		Assert::AreEqual<int>(0, Cir.back());
		Assert::AreEqual(1u, Cir.size(), L"push size error");
		Assert::AreEqual(1u, value_t::cnt(), L"construct number error");

		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty error");
	}
	TEST_METHOD(iterator_0){
		circular<value_t, 5> Cir;

		Cir.push_back(0);
		Cir.push_back(1);
		Cir.push_back(2);
		Cir.push_back(3);
		Assert::AreEqual(4, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(0, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(3, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(2, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(1, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}
		Cir.pop_front();
		Assert::AreEqual(0, Cir.end() - Cir.begin(), L"iterator diff error");
		Assert::IsTrue(Cir.begin()==Cir.end(), L"empty error");
	}
	TEST_METHOD(iterator_1){
		circular<value_t, 5> Cir;

		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");

		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");

		Cir.push_back(0);
		Cir.push_back(1);
		Cir.push_back(2);
		Cir.push_back(3);
		Assert::AreEqual(4, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(0, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(3, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(2, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_front();
		Assert::AreEqual(1, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}
		Cir.pop_front();
		Assert::AreEqual(0, Cir.end() - Cir.begin(), L"iterator diff error");
		Assert::IsTrue(Cir.begin() == Cir.end(), L"empty error");
	}
	TEST_METHOD(iterator_2){
		circular<value_t, 5> Cir;

		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Cir.push_back(0);
		Cir.pop_front();
		Assert::IsTrue(Cir.empty(), L"empty function error.");

		Assert::AreEqual(0u, value_t::cnt(), L"construct number error");

		Cir.push_front(0);
		Cir.push_front(1);
		Cir.push_front(2);
		Cir.push_front(3);
		Assert::AreEqual(4, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(0, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_back();
		Assert::AreEqual(3, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(1, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_back();
		Assert::AreEqual(2, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(2, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}

		Cir.pop_back();
		Assert::AreEqual(1, Cir.end() - Cir.begin(), L"iterator diff error");
		{
			auto Itr = Cir.begin();
			Assert::IsFalse(Itr == Cir.end(), L"End itr error");
			Assert::AreEqual<int>(3, *Itr, L"value error");
			++Itr;
			Assert::IsTrue(Itr == Cir.end(), L"End itr error");
		}
		Cir.pop_back();
		Assert::AreEqual(0, Cir.end() - Cir.begin(), L"iterator diff error");
		Assert::IsTrue(Cir.begin() == Cir.end(), L"empty error");
	}
	};
	unsigned int test_circular::value_t::Cnt = 0;

}
