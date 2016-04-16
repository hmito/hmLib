#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include<algorithm>
#include <hmLib/lattice_iterator.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_lattice_iterator){
	public:
		TEST_METHOD(lattice_itr1){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			auto val = std::begin({0, 2});
			lattice_iterator<iterator, 1> Itr(Con.begin(), 0, 3, 0);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(1, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
		TEST_METHOD(lattice_itr2){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			lattice_iterator<iterator, 1> Itr(Con.begin(), 0, 3, 1);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
		TEST_METHOD(lattice_itr1_somestep){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			lattice_iterator<iterator, 1> Itr(Con.begin(), 0, 30, 0);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			Itr+=11;
			Assert::AreEqual(11, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			Itr+=11;
			Assert::AreEqual(22, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
		TEST_METHOD(lattice_itr12){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);
			lattice_iterator<iterator, 2> Itr(Con.begin(), {0,0}, {4,3}, {1,0});

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(1, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(5, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(6, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(8, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
		TEST_METHOD(lattice_itr24){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			lattice_iterator<iterator, 2> Itr(Con.begin(), {0,0}, {4,3}, {1,1});
			

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(7, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(9, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(11, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			++Itr;
			Assert::AreEqual(14, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
		TEST_METHOD(lattice_itr24_somestep){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			lattice_iterator<iterator, 2> Itr(Con.begin(), {0,0}, {4,3}, {1,1});

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			Itr+=4;
			Assert::AreEqual(9, std::distance(Con.begin(), Itr.ref()), L"Position Error");
			Itr+=2;
			Assert::AreEqual(14, std::distance(Con.begin(), Itr.ref()), L"Position Error");
		}
	}; 
}
