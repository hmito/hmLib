#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/lattices.hpp>
#include <hmLib/lattices/torus_lattice_view.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{

TEST_CLASS(test_lattice_view){
public:
	TEST_METHOD(construct){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]() {return ++Val; });

		lattice_view<iterator, 2>  Lattice(Con.begin(),Con.end(), 4, 5);

		auto Size = Lattice.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");
	}
	TEST_METHOD(construct_with_point) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		auto UsedSize = lattices::make_point(4, 5);
		Assert::AreEqual(4, UsedSize[0], L"Axis Size Error");
		Assert::AreEqual(5, UsedSize[1], L"Axis Size Error");

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), UsedSize);

		auto Size = Lattice.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");
	}
	TEST_METHOD(iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int{return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for (int i = 0; i < 90; ++i) {
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.get_index());
			Assert::IsTrue(Begin != End);
			Assert::AreEqual(i, *Begin);
			Assert::AreEqual<int>(90-i, std::distance(Begin,End));
			++Begin;
		}
		Assert::IsTrue(Begin == End);
	}
	TEST_METHOD(const_iterator) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });


		lattice_view<iterator, 2>  Lattice2(Con.begin(), Con.end(), 9, 10);
		const lattice_view<iterator, 2>& Lattice = Lattice2;

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for (int i = 0; i < 90; ++i) {
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.get_index());
			Assert::IsTrue(Begin != End);
			Assert::AreEqual(i, *Begin);
			Assert::AreEqual<int>(90 - i, std::distance(Begin, End));
			++Begin;
		}
		Assert::IsTrue(Begin == End);
	}
	TEST_METHOD(locator) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);
		auto lotr = Lattice.front_locate();

		for (int i = 0; i < 9; ++i) {
			Assert::AreEqual(i + (i+1) * 9, *(lotr.plus(i, i + 1)));
		}

		lotr = Lattice.back_locate();
		for (int i = 0; i < 9; ++i) {
			Assert::AreEqual(8-i + (8-i) * 9, *(lotr.plus(-i, -i - 1)));
		}

		lotr = Lattice.locate(3, 4);
		Assert::AreEqual(3+4*9, *lotr);
		Assert::AreEqual(3 + 4 * 9, lotr.at(0, 0));
		Assert::AreEqual(4 + 6 * 9, lotr.at(1, 2));
		Assert::AreEqual(3 + 4 * 9, lotr.at(0, 0));
	}
	TEST_METHOD(dif_locator) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);
		auto lotr = Lattice.front_locate();
		auto blotr = Lattice.locate(7, 5);

		auto Dif = lotr - blotr;
		Assert::AreEqual(-7, Dif[0]);
		Assert::AreEqual(-5, Dif[1]);

		lotr -= Dif;
		Assert::IsTrue(lotr == blotr);
	}
	TEST_METHOD(sublattice) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Size = Sub.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");

		Assert::AreEqual(2 + 3 * 9, Sub.at(0, 0));
		Assert::AreEqual(5 + 4 * 9, Sub.at(3, 1));
	}
	TEST_METHOD(sublattice_iterator) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Itr = Sub.begin();
		for (int j = 0; j < 5; ++j) {
			for (int i = 0; i < 4; ++i) {
				Assert::AreEqual(2 + i + (3 + j) * 9, *Itr);
				Assert::IsFalse(Itr == Sub.end());
				++Itr;
			}
		}
		Assert::IsTrue(Itr == Sub.end());
	}
	TEST_METHOD(sublattice_locator) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Lotr1 = Lattice.locate(2, 3);
		auto Lotr2 = Sub.locate(0, 0);
		Assert::IsTrue(Lotr1 == Lotr2);

		Lotr1.add(2, 3);
		Lotr2.add(2, 3);
		Assert::IsTrue(Lotr1 == Lotr2);
		Assert::IsTrue(Lotr1.plus(1, 1) == Sub.back_locate());

		Assert::AreEqual(4, Sub.size()[0]);
		Assert::AreEqual(5, Sub.size()[1]);
	}
	TEST_METHOD(sublattice_locator_ex){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Lotr = Sub.locate(2, 3);

		Assert::AreEqual(4 + 4 + 9 * (6 + 0), Lotr.at(4, 0));
		Assert::ExpectException<lattices::out_of_range_access>([&](){Lotr.at(5, 0); });
		Assert::AreEqual(4 + 0 + 9 * (6 + 3), Lotr.at(0, 3));
		Assert::ExpectException<lattices::out_of_range_access>([&](){Lotr.at(0, 4); });
		Assert::AreEqual(4 + -4 + 9 * (6 + 0), Lotr.at(-4, 0));
		Assert::ExpectException<lattices::out_of_range_access>([&](){Lotr.at(-5, 0); });
		Assert::AreEqual(4 + 0 + 9 * (6 + -6), Lotr.at(0, -6));
		Assert::ExpectException<lattices::out_of_range_access>([&](){Lotr.at(0, -7); });
	}
};

TEST_CLASS(test_torus_lattice_view){
public:
	TEST_METHOD(construct){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]() {return ++Val; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 4, 5);

		auto Size = Lattice.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");
	}
	TEST_METHOD(construct_with_point){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		auto UsedSize = lattices::make_point(4, 5);
		Assert::AreEqual(4, UsedSize[0], L"Axis Size Error");
		Assert::AreEqual(5, UsedSize[1], L"Axis Size Error");

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), UsedSize);

		auto Size = Lattice.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");
	}
	TEST_METHOD(iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int{return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for(int i = 0; i < 90; ++i){
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.get_index());
			Assert::IsTrue(Begin != End);
			Assert::AreEqual(i, *Begin);
			Assert::AreEqual<int>(90 - i, std::distance(Begin, End));
			++Begin;
		}
		Assert::IsTrue(Begin == End);
	}
	TEST_METHOD(const_iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });


		torus_lattice_view<iterator, 2>  Lattice2(Con.begin(), Con.end(), 9, 10);
		const torus_lattice_view<iterator, 2>& Lattice = Lattice2;

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for(int i = 0; i < 90; ++i){
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.get_index());
			Assert::IsTrue(Begin != End);
			Assert::AreEqual(i, *Begin);
			Assert::AreEqual<int>(90 - i, std::distance(Begin, End));
			++Begin;
		}
		Assert::IsTrue(Begin == End);
	}
	TEST_METHOD(at){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });


		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 10, 9);

		Assert::AreEqual(0, Lattice.at(0, 0));
		Assert::AreEqual(1, Lattice.at(1, 0));
		Assert::AreEqual(9, Lattice.at(-1, 0));
		Assert::AreEqual(8+80, Lattice.at(-2, -1));
	}
	TEST_METHOD(locator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);
		auto lotr = Lattice.front_locate();

		for(int i = 0; i < 9; ++i){
			Assert::AreEqual(i + (i + 1) * 9, *(lotr.plus(i, i + 1)));
		}

		lotr = Lattice.back_locate();
		for(int i = 0; i < 9; ++i){
			Assert::AreEqual(8 - i + (8 - i) * 9, *(lotr.plus(-i, -i - 1)));
		}

		lotr = Lattice.locate(3, 4);
		Assert::AreEqual(3 + 4 * 9, *lotr);
		Assert::AreEqual(3 + 4 * 9, lotr.at(0, 0));
		Assert::AreEqual(4 + 6 * 9, lotr.at(1, 2));
		Assert::AreEqual(3 + 4 * 9, lotr.at(0, 0));
		Assert::AreEqual(88, lotr.at(-5, -5));
	}
	TEST_METHOD(dif_locator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);
		auto lotr = Lattice.front_locate();
		auto blotr = Lattice.locate(7, 5);

		auto Dif = lotr - blotr;
		Assert::AreEqual(-7, Dif[0]);
		Assert::AreEqual(-5, Dif[1]);

		lotr -= Dif;
		Assert::IsTrue(lotr == blotr);
	}
	TEST_METHOD(sublattice){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Size = Sub.size();
		Assert::AreEqual(4, Size[0], L"Size Error");
		Assert::AreEqual(5, Size[1], L"Size Error");

		Assert::AreEqual(2 + 3 * 9, Sub.at(0, 0));
		Assert::AreEqual(5 + 4 * 9, Sub.at(3, 1));
	}
	TEST_METHOD(sublattice_iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Itr = Sub.begin();
		for(int j = 0; j < 5; ++j){
			for(int i = 0; i < 4; ++i){
				Assert::AreEqual(2 + i + (3 + j) * 9, *Itr);
				Assert::IsFalse(Itr == Sub.end());
				++Itr;
			}
		}
		Assert::IsTrue(Itr == Sub.end());
	}
	TEST_METHOD(sublattice_locator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Lotr1 = Lattice.locate(2, 3);
		auto Lotr2 = Sub.locate(0, 0);
		Assert::IsTrue(Lotr1 == Lotr2);

		Lotr1.add(2, 3);
		Lotr2.add(2, 3);
		Assert::IsTrue(Lotr1 == Lotr2);
		Assert::IsTrue(Lotr1.plus(1, 1) == Sub.back_locate());

		Assert::AreEqual(4, Sub.size()[0]);
		Assert::AreEqual(5, Sub.size()[1]);
	}
	TEST_METHOD(sublattice_locator_ex){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(2, 3), lattices::make_point(4, 5));

		auto Lotr = Sub.locate(2, 3);

		Assert::AreEqual(4 + 4 + 9 * (6 + 0), Lotr.at(4, 0));
		Assert::AreEqual(4 + 5 -9 + 9 * (6 + 0), Lotr.at(5, 0));
		Assert::AreEqual(4 + 0 + 9 * (6 + 3), Lotr.at(0, 3));
		Assert::AreEqual(4 + 0 + 9 * (6 + 4 - 10), Lotr.at(0, 4));
		Assert::AreEqual(4 + -4 + 9 * (6 + 0), Lotr.at(-4, 0));
		Assert::AreEqual(4 + -5 + 9 + 9 * (6 + 0), Lotr.at(-5, 0));
		Assert::AreEqual(4 + 0 + 9 * (6 + -6), Lotr.at(0, -6));
		Assert::AreEqual(4 + 0 + 9 * (6 + -7 + 10), Lotr.at(0, -7));
	}
	TEST_METHOD(bsublattice){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(-2, -3), lattices::make_point(4, 5));

		Assert::AreEqual(-2 + 0 + 9 + 9 * (-3 + 0 + 10), Sub.at(0, 0));
		Assert::AreEqual(-2 + 1 + 9 + 9 * (-3 + 0 + 10), Sub.at(1, 0));
		Assert::AreEqual(-2 + 2 + 9 * (-3 + 0 + 10), Sub.at(2, 0));
		Assert::AreEqual(-2 + 3 + 9 * (-3 + 0 + 10), Sub.at(3, 0));
		Assert::AreEqual(-2 + 0 + 9 + 9 * (-3 + 1 + 10), Sub.at(0, 1));
		Assert::AreEqual(-2 + 0 + 9 + 9 * (-3 + 2 + 10), Sub.at(0, 2));
		Assert::AreEqual(-2 + 0 + 9 + 9 * (-3 + 3), Sub.at(0, 3));
		Assert::AreEqual(-2 + 3 + 9 * (-3 + 4), Sub.at(3, 4));
	}
	TEST_METHOD(bsublattice_iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 9, 10);

		auto Sub = Lattice.sublattice(lattices::make_point(-2, -3), lattices::make_point(4, 5));

		auto Itr = Sub.cbegin();

		for(int y = 0; y < 5; ++y){
			for(int x = 0; x < 4; ++x){
				Assert::IsFalse(Itr == Sub.end());
				Assert::AreEqual(hmLib::algorithm::positive_mod(-2 + x,9) + 9*hmLib::algorithm::positive_mod(-3 + y, 10),*Itr);
				++Itr;
			}
		}
	}
};

}
