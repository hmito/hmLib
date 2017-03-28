#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/lattices.hpp>
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

		Assert::AreEqual(4u, Lattice.axis_size<0>(), L"Axis Size Error");
		Assert::AreEqual(5u, Lattice.axis_size<1>(), L"Axis Size Error");
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

		Assert::AreEqual(4u, Lattice.axis_size<0>(), L"Axis Size Error");
		Assert::AreEqual(5u, Lattice.axis_size<1>(), L"Axis Size Error");
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

		Assert::AreEqual(4u, Sub.axis_size<0>());
		Assert::AreEqual(5u, Sub.axis_size<1>());

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
	/*	TEST_METHOD(locator) {
		using container = std::vector<int>;
		using iterator = container::iterator;
		using namespace hmLib::euclidean;

		container Con(1000, 0);

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 20, 10);

		auto lotr = Lattice.locate(2,4);
		auto lotr2 = Lattice.locate(3, Lattice.axis_size<1>-1);
		auto flotr = Lattice.front_locate();	//locate(0,0);
		auto blotr = Lattice.back_locate();		//locate(axis_size<0>-1, axis_size<1>-1);
		auto dif = blotr - flotr;				//difference is point type
		lotr = blotr + dif / 2;					//operator+ pointer is ok.
		Assert::AreEqual(0,*lotr,L"");

		lotr[0]++;								//change lotr indicating point
		lotr -= make_point(3, 3);

		//iterator -> locator is OK
		lotr = Lattice.begin();

		//locator -> iterator is NG
		//	iterator can be used as the context of the focal lattice.
		//iterator itr = lotr;

		//locator + lattice -> iterator is maybe OK, but maybe not necessary.
		auto Itr = Lattice.to_iterator(lotr);

		//two locator pair can make a lattice
		//	locator belongs to different lattice will throw an exception.
		auto SubLattice = sublattice(lotr - make_point(2, 3), lotr + make_point(3, 2));
		lotr.add(3, 4);//same with lotr+=make_point(3,4);
		lotr.plus(3, 4);//same with lotr+make_point(3,4);

		//locator of lattice and its sublattice can be comparable.
		auto Distance = lotr - SubLattice.locate(2, 3);

		//locator just behave as an indexer?

		//sublatticeは、どこまでも行けるけど、実際にはlotrで区切られた点に過ぎない
		//特に指定しなければ、lattice作成時の最初の値が0,0。そのsublatticeはどれもそいつが基準。
		//locatorが参照しているlatticeごとに変えてやる必要なし。
		lotr.at(-3, -3);

		for (auto Itr : Lattice) {
			auto Lotr = to_locator(Itr);
			
			*Itr == *Lotr && *Lotr == Lotr.at(0, 0);
			for (int x = -3; x <= 3; ++x) {
				for (int y= -3; y <= 3; ++y) {
					Lotr.at(x, y);
				}
			}

			auto SubLattice = Lattice.sublattice(to_locator(Itr,-3,-3), to_locator(Itr, 3, 3));
			for (auto SItr : SubLattice) {
				distance(Itr, SItr);
			}
		}

		auto SubLattice = Lattice, sublattice(make_point(2, 3), make_point(10, 5));
	}
	TEST_METHOD(sublattice_point) {
		using container = std::vector<int>;
		using iterator = container::iterator;
		using namespace hmLib::euclidean;

		container Con(1000);

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 20, 10);

		auto SubLattice = Lattice, sublattice(make_point(2, 3), make_point(10, 5));

		Assert::AreEqual(10, Lattice.axis_size<0>(), L"Axis Size Error");
		Assert::AreEqual(5, Lattice.axis_size<1>(), L"Axis Size Error");

		Assert::AreEqual(0, Lattice.axis_gap<0>(), L"Axis Gap Error");
		Assert::AreEqual(0, Lattice.axis_gap<1>(), L"Axis Gap Error");
	}
	TEST_METHOD(sublattice_itr) {
		using container = std::vector<int>;
		using iterator = container::iterator;
		using namespace hmLib::euclidean;

		container Con(1000);

		lattice_view<iterator, 2>  Lattice(Con.begin(), Con.end(), 20, 10);


		auto Itr = Lattice.begin();
		auto SubLattice = Lattice, sublattice(Itr+4, make_point(10, 5));

		Assert::AreEqual(4, Lattice.axis_size<0>(), L"Axis Size Error");
		Assert::AreEqual(5, Lattice.axis_size<1>(), L"Axis Size Error");

		Assert::AreEqual(0, Lattice.axis_gap<0>(), L"Axis Gap Error");
		Assert::AreEqual(0, Lattice.axis_gap<1>(), L"Axis Gap Error");
	}
	/*
	TEST_METHOD(construct_with_gap){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		lattice_view<iterator, 2>  Lattice(Con.begin(),Con.end(), std::make_pair(4,2),std::make_pair(5,3));

		Assert::AreEqual(4, Lattice.size<0>(), L"Size Error");
		Assert::AreEqual(2, Lattice.gap<0>(), L"Size Error");
		Assert::AreEqual(5, Lattice.size<1>(), L"Size Error");
		Assert::AreEqual(3, Lattice.gap<1>(), L"Size Error");
	}
	TEST_METHOD(raw_position){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), 5, 6);

		Assert::AreEqual(0, Lat.raw_position(0, 0), L"Access Error");
		Assert::AreEqual(1, Lat.raw_position(0, 1), L"Access Error");
		Assert::AreEqual(2, Lat.raw_position(0, 2), L"Access Error");
		Assert::AreEqual(6, Lat.raw_position(1, 0), L"Access Error");
		Assert::AreEqual(8, Lat.raw_position(1, 2), L"Access Error");
		Assert::AreEqual(21, Lat.raw_position(3, 3), L"Access Error");
	}
	TEST_METHOD(at){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), 5, 6);
		
		Assert::AreEqual(0, Lat.at(0, 0), L"Access Error");
		Assert::AreEqual(1, Lat.at(0, 1), L"Access Error");
		Assert::AreEqual(2, Lat.at(0, 2), L"Access Error");
		Assert::AreEqual(6, Lat.at(1, 0), L"Access Error");
		Assert::AreEqual(8, Lat.at(1, 2), L"Access Error");
		Assert::AreEqual(21, Lat.at(3, 3), L"Access Error");
	}
	TEST_METHOD(at_with_gap){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), std::make_pair(5,3), std::make_pair(6,4));

		Assert::AreEqual(0, Lat.at(0, 0), L"Access Error");
		Assert::AreEqual(5, Lat.at(0, 1), L"Access Error");
		Assert::AreEqual(10, Lat.at(0, 2), L"Access Error");
		Assert::AreEqual(33, Lat.at(1, 0), L"Access Error");
		Assert::AreEqual(43, Lat.at(1, 2), L"Access Error");
		Assert::AreEqual(114, Lat.at(3, 3), L"Access Error");
	}
	TEST_METHOD(lattices_point){
		lattices::point<4> Pos{4, 5, 6, 7};

		Assert::AreEqual(4u, Pos.dim(), L"Point Access Error");
		Assert::AreEqual(4, Pos[0],L"Point Access Error");
		Assert::AreEqual(5, Pos[1], L"Point Access Error");
		Assert::AreEqual(6, Pos[2], L"Point Access Error");
		Assert::AreEqual(7, Pos[3], L"Point Access Error");
	}
	TEST_METHOD(use_make_point){
		auto Pos = lattices::make_point(4, 5, 6, 7);

		Assert::AreEqual(4u, Pos.dim(), L"Point Access Error");
		Assert::AreEqual(4, Pos[0], L"Point Access Error");
		Assert::AreEqual(5, Pos[1], L"Point Access Error");
		Assert::AreEqual(6, Pos[2], L"Point Access Error");
		Assert::AreEqual(7, Pos[3], L"Point Access Error");
	}
	TEST_METHOD(use_make_lattice){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		auto Lattice=make_lattice(Con.begin(), Con.end(), std::make_pair(4, 2), std::make_pair(5, 3),6);

		Assert::AreEqual(3u, Lattice.dim(), L"Dim Error");
		Assert::AreEqual(4, Lattice.size<0>(), L"Size Error");
		Assert::AreEqual(2, Lattice.gap<0>(), L"Size Error");
		Assert::AreEqual(5, Lattice.size<1>(), L"Size Error");
		Assert::AreEqual(3, Lattice.gap<1>(), L"Size Error");
		Assert::AreEqual(6, Lattice.size<2>(), L"Size Error");
		Assert::AreEqual(0, Lattice.gap<2>(), L"Size Error");
	}
	TEST_METHOD(at_point){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), std::make_pair(5, 3), std::make_pair(6, 4));

		Assert::AreEqual(0, Lat.at(lattices::make_point(0, 0)), L"Access Error");
		Assert::AreEqual(5, Lat.at(lattices::make_point(0, 1)), L"Access Error");
		Assert::AreEqual(10, Lat.at(lattices::make_point(0, 2)), L"Access Error");
		Assert::AreEqual(33, Lat.at(lattices::make_point(1, 0)), L"Access Error");
		Assert::AreEqual(43, Lat.at(lattices::make_point(1, 2)), L"Access Error");
		Assert::AreEqual(114, Lat.at(lattices::make_point(3, 3)), L"Access Error");
	}
	TEST_METHOD(operator_access){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), std::make_pair(5, 3), std::make_pair(6, 4));

		Assert::AreEqual(0, Lat[lattices::make_point(0, 0)], L"Access Error");
		Assert::AreEqual(0, Lat[{0, 0}], L"Access Error");
		Assert::AreEqual(5, Lat[lattices::make_point(0, 1)], L"Access Error");
		Assert::AreEqual(10, Lat[lattices::make_point(0, 2)], L"Access Error");
		Assert::AreEqual(33, Lat[lattices::make_point(1, 0)], L"Access Error");
		Assert::AreEqual(43, Lat[lattices::make_point(1, 2)], L"Access Error");
		Assert::AreEqual(114, Lat[lattices::make_point(3, 3)], L"Access Error");
	}
	TEST_METHOD(access_2d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), 5, 6);

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				Assert::AreEqual(x * 6 + y, Lat[lattices::make_point(x, y)], L"Access Error");
			}
		}
	}
	TEST_METHOD(iterator_2d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), 5, 6);

		auto Begin = Lat.begin();
		auto Itr = Begin;
		int Cnt = 0;
		auto End = Lat.end();

		Assert::AreEqual(Lat.lattice_size<0>(), End - Begin, L"Distance Error");
		Assert::AreEqual(-Lat.lattice_size<0>(), Begin - End, L"Distance Error");

		Assert::IsFalse(Begin == End, L"Iterator Compare Error");
		Assert::IsFalse(Begin >= End, L"Iterator Compare Error");
		Assert::IsFalse(Begin > End, L"Iterator Compare Error");
		Assert::IsTrue(Begin <= End, L"Iterator Compare Error");
		Assert::IsTrue(Begin < End, L"Iterator Compare Error");

		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				Assert::AreEqual(x*6+y, *Itr, L"Access Error");
				Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
				Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
				Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
				Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
				Assert::IsFalse(Itr == End, L"Iterator Compare Error");
				Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
				Assert::IsFalse(Itr > End, L"Iterator Compare Error");
				Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
				Assert::IsTrue(Itr < End, L"Iterator Compare Error");
				++Itr;
				++Cnt;
			}
		}
		Assert::IsTrue(Itr == End, L"Iterator Compare Error");
	}
	TEST_METHOD(access_3d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), 5, 6, 7);

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					Assert::AreEqual(x * 6 * 7 + y * 7 + z, Lat[lattices::make_point(x,y,z)], L"Access Error");
				}
			}
		}
	}
	TEST_METHOD(iterator_3d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), 5, 6, 7);

		auto Begin = Lat.begin();
		auto Itr = Begin;
		int Cnt = 0;
		auto End = Lat.end();

		Assert::AreEqual(Lat.lattice_size<0>(), End - Begin, L"Distance Error");
		Assert::AreEqual(-Lat.lattice_size<0>(), Begin - End, L"Distance Error");

		Assert::IsFalse(Begin == End, L"Iterator Compare Error");
		Assert::IsFalse(Begin >= End, L"Iterator Compare Error");
		Assert::IsFalse(Begin > End, L"Iterator Compare Error");
		Assert::IsTrue(Begin <= End, L"Iterator Compare Error");
		Assert::IsTrue(Begin < End, L"Iterator Compare Error");

		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					Assert::AreEqual(x * 6 * 7 + y * 7 + z, *Itr, L"Access Error");
					Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
					Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
					Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
					Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
					Assert::IsFalse(Itr == End, L"Iterator Compare Error");
					Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
					Assert::IsFalse(Itr > End, L"Iterator Compare Error");
					Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
					Assert::IsTrue(Itr < End, L"Iterator Compare Error");
					++Itr;
					++Cnt;
				}
			}
		}
		Assert::IsTrue(Itr == End, L"Iterator Compare Error");
	}
	TEST_METHOD(access_gap_3d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5,1), std::make_pair(6, 2), std::make_pair(7, 3));

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					Assert::AreEqual(x * 181 + y * 30 + z*4, Lat[lattices::make_point(x, y, z)], L"Access Error");
				}
			}
		}
	}
	TEST_METHOD(iterator_gap_3d){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5, 1), std::make_pair(6, 2), std::make_pair(7, 3));

		auto Begin = Lat.begin();
		auto Itr = Begin;
		int Cnt = 0;
		auto End = Lat.end();

		Assert::AreEqual(Lat.lattice_size<0>(), End - Begin, L"Distance Error");
		Assert::AreEqual(-Lat.lattice_size<0>(), Begin - End, L"Distance Error");

		Assert::IsFalse(Begin == End, L"Iterator Compare Error");
		Assert::IsFalse(Begin >= End, L"Iterator Compare Error");
		Assert::IsFalse(Begin > End, L"Iterator Compare Error");
		Assert::IsTrue(Begin <= End, L"Iterator Compare Error");
		Assert::IsTrue(Begin < End, L"Iterator Compare Error");

		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					Assert::AreEqual(x * 181 + y * 30 + z * 4, *Itr, L"Access Error");
					Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
					Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
					Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
					Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
					Assert::IsFalse(Itr == End, L"Iterator Compare Error");
					Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
					Assert::IsFalse(Itr > End, L"Iterator Compare Error");
					Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
					Assert::IsTrue(Itr < End, L"Iterator Compare Error");
					++Itr;
					++Cnt;
				}
			}
		}
		Assert::IsTrue(Itr == End, L"Iterator Compare Error");
	}
	TEST_METHOD(iterator_gap_3d_rev){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5, 1), std::make_pair(6, 2), std::make_pair(7, 3));

		auto Begin = Lat.begin();
		auto End = Lat.end();
		auto Itr = Begin+210;
		int Cnt = 210;

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					--Itr;
					--Cnt;
					Assert::AreEqual((4-x) * 181 + (5-y) * 30 + (6-z) * 4, *Itr, L"Access Error");
					Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
					Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
					Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
					Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
					Assert::IsFalse(Itr == End, L"Iterator Compare Error");
					Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
					Assert::IsFalse(Itr > End, L"Iterator Compare Error");
					Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
					Assert::IsTrue(Itr < End, L"Iterator Compare Error");

				}
			}
		}
		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");
	}
	TEST_METHOD(iterator_gap_3d_revend){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(900);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5, 1), std::make_pair(6, 2), std::make_pair(7, 3));

		auto Begin = Lat.begin();
		auto End = Lat.end();
		auto Itr = End;
		int Cnt = 210;

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					--Itr;
					--Cnt;
					Assert::AreEqual((4 - x) * 181 + (5 - y) * 30 + (6 - z) * 4, *Itr, L"Access Error");
					Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
					Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
					Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
					Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
					Assert::IsFalse(Itr == End, L"Iterator Compare Error");
					Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
					Assert::IsFalse(Itr > End, L"Iterator Compare Error");
					Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
					Assert::IsTrue(Itr < End, L"Iterator Compare Error");

				}
			}
		}
		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");
	}
	TEST_METHOD(list_iterator_gap_3d_revend){
		using container = std::list<int>;
		using iterator = container::iterator;

		container Con(900);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5, 1), std::make_pair(6, 2), std::make_pair(7, 3));

		auto Begin = Lat.begin();
		auto End = Lat.end();
		auto Itr = End;
		int Cnt = 210;

		for(int x = 0; x < 5; ++x){
			for(int y = 0; y < 6; ++y){
				for(int z = 0; z < 7; ++z){
					--Itr;
					--Cnt;
					Assert::AreEqual((4 - x) * 181 + (5 - y) * 30 + (6 - z) * 4, *Itr, L"Access Error");
					Assert::AreEqual(Cnt, Itr - Begin, L"Distance Error");
					Assert::AreEqual(Lat.lattice_size<0>() - Cnt, End - Itr, L"Distance Error");
					Assert::AreEqual(-Cnt, Begin - Itr, L"Distance Error");
					Assert::AreEqual(-Lat.lattice_size<0>() + Cnt, Itr - End, L"Distance Error");
					Assert::IsFalse(Itr == End, L"Iterator Compare Error");
					Assert::IsFalse(Itr >= End, L"Iterator Compare Error");
					Assert::IsFalse(Itr > End, L"Iterator Compare Error");
					Assert::IsTrue(Itr <= End, L"Iterator Compare Error");
					Assert::IsTrue(Itr < End, L"Iterator Compare Error");

				}
			}
		}
		Assert::IsTrue(Itr == Begin, L"Iterator Compare Error");
	}
	TEST_METHOD(iterator_gap_3d_random){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 3> Lat(Con.begin(), Con.end(), std::make_pair(5, 1), std::make_pair(6, 2), std::make_pair(7, 3));

		auto Begin = Lat.begin();
		auto Itr = Begin;
		int Cnt = 0;
		auto End = Lat.end();

		Assert::AreEqual(16, Itr[4], L"AccessError");
		Assert::AreEqual(16, *(Itr + 4), L"AccessError");
		Assert::AreEqual(16, *(4 + Itr), L"AccessError");
		Itr += 4;
		Assert::AreEqual(16, *Itr, L"AccessError");

		Assert::AreEqual(0, Itr[-4], L"AccessError");
		Assert::AreEqual(0, *(Itr - 4), L"AccessError");
		Assert::AreEqual(0, *(-4 + Itr), L"AccessError");

		Itr += 45;
		Assert::AreEqual(211, *Itr, L"AccessError");
	}
	TEST_METHOD(point){
		lattices::point<5> Pos1{3,4,5,6,2};
		Pos1.fill(5);

		for(auto val : Pos1){
			Assert::AreEqual(5, val, L"fill Error");
		}

		Pos1 += Pos1;
		for(auto val : Pos1){
			Assert::AreEqual(10, val, L"fill Error");
		}

		Pos1 = Pos1*2;
		for(auto val : Pos1){
			Assert::AreEqual(20, val, L"fill Error");
		}

	}	*/
};
}
