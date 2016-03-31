#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include<algorithm>
#include <hmLib/lattice_iterator.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_basic_lattice_iterator){
	public:
		TEST_METHOD(lattice_itr1){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr(Con.begin(),0,3,1);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr),L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(1, std::distance(Con.begin(), Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr2){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr(Con.begin(), 0, 3, 2);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr1_somestep){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr(Con.begin(), 0, 30, 1);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr), L"Position Error");
			Itr.advance(11);
			Assert::AreEqual(11, std::distance(Con.begin(), Itr.Itr), L"Position Error");
			Itr.advance(11);
			Assert::AreEqual(22, std::distance(Con.begin(), Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr12){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr1(Con.begin(), 0, 3, 1);
			basic_lattice_iterator<iterator, 2> Itr(Itr1, 0, 4, 4);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(1, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(5, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(6, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr24){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr1(Con.begin(), 0, 3, 2);
			basic_lattice_iterator<iterator, 2> Itr(Itr1, 0, 4, 7);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(2, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(4, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(7, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(9, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(11, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(1);
			Assert::AreEqual(14, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr24_somestep){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			basic_lattice_iterator<iterator, 1> Itr1(Con.begin(), 0, 3, 2);
			basic_lattice_iterator<iterator, 2> Itr(Itr1, 0, 4, 7);

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(4);
			Assert::AreEqual(9, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(2);
			Assert::AreEqual(14, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
		}
		TEST_METHOD(lattice_itr24_make){
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con(1000);

			auto itr = basic_lattice_iterator_maker<2>({0,0}, {3,4}, {2,2})(Con.begin());

			Assert::AreEqual(0, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(4);
			Assert::AreEqual(9, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
			Itr.advance(2);
			Assert::AreEqual(14, std::distance(Con.begin(), Itr.Itr.Itr), L"Position Error");
		}
	};
}
