#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include<boost/iterator/iterator_facade.hpp>
#include"../../../random.hpp"
#include "../../../lattices.hpp"
#include"../../../math/multiaxis.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
TEST_CLASS(test_lattice_utility) {
public:
	TEST_METHOD(point) {
		auto Point = lattices::point(2, 3, 4);
		Assert::AreEqual(3u, Point.size(), L"point size error");
		Assert::AreEqual(2, Point[0], L"value error");
		Assert::AreEqual(3, Point[1], L"value error");
		Assert::AreEqual(4, Point[2], L"value error");
	}
	TEST_METHOD(extent) {
		auto Point = lattices::extent(2, 3, 4);
		Assert::AreEqual(3u, Point.size(), L"point size error");
		Assert::AreEqual(2u, Point[0], L"value error");
		Assert::AreEqual(3u, Point[1], L"value error");
		Assert::AreEqual(4u, Point[2], L"value error");
	}
};

TEST_CLASS(test_lattice_view){
public:
	TEST_METHOD(construct){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]() {return ++Val; });

		lattice_view<iterator, 2>  Lattice(Con.begin(),lattices::extent(4, 5));

		auto Size = Lattice.extent();
		Assert::AreEqual<int>(4, Size[0], L"Size Error");
		Assert::AreEqual<int>(5, Size[1], L"Size Error");
	}
	TEST_METHOD(construct_with_point) {
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		auto UsedSize = lattices::point(4, 5);
		static_assert(decltype(UsedSize)::static_size()==2,"");
		Assert::AreEqual(4, UsedSize[0], L"Axis Size Error");
		Assert::AreEqual(5, UsedSize[1], L"Axis Size Error");

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), 4,5);

		auto Size = Lattice.extent();
		Assert::AreEqual<int>(4, Size[0], L"Size Error");
		Assert::AreEqual<int>(5, Size[1], L"Size Error");
	}
	TEST_METHOD(iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int{return Val++; });

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for (int i = 0; i < 90; ++i) {
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual<int>(i, std::distance(Lattice.begin(),Begin));
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


		auto  Lattice2 = make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
		const lattice_view<iterator, 2>& Lattice = Lattice2;

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for (int i = 0; i < 90; ++i) {
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual<int>(i, std::distance(Lattice.begin(), Begin));
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

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
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

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
		auto lotr = Lattice.front_locate();
		auto blotr = Lattice.locate(7, 5);

		auto Dif = lotr - blotr;
		Assert::AreEqual(-7, Dif[0]);
		Assert::AreEqual(-5, Dif[1]);

		lotr -= Dif;
		Assert::IsTrue(lotr == blotr);
	}
	TEST_METHOD(subview){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

		auto Size = Sub.extent();
		Assert::AreEqual(4u, Size[0], L"Size Error");
		Assert::AreEqual(5u, Size[1], L"Size Error");

		Assert::AreEqual(2 + 3 * 9, Sub.at(0, 0));
		Assert::AreEqual(5 + 4 * 9, Sub.at(3, 1));
	}
	TEST_METHOD(sublattice_iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

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

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

		auto Lotr1 = Lattice.locate(2, 3);
		auto Lotr2 = Sub.locate(0, 0);
		Assert::IsTrue(Lotr1 == Lotr2);

		Lotr1.add(2, 3);
		Lotr2.add(2, 3);
		Assert::IsTrue(Lotr1 == Lotr2);
		Assert::IsTrue(Lotr1.plus(1, 1) == Sub.back_locate());

		Assert::AreEqual(4u, Sub.extent()[0]);
		Assert::AreEqual(5u, Sub.extent()[1]);
	}
	TEST_METHOD(sublattice_locator_ex){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		lattice_view<iterator, 2>  Lattice=make_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

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
	TEST_METHOD(lattice_example1){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(210);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		auto Lat = make_lattice_view(Con.begin(), Con.end(), 10, 7, 3);

		for(int x = 0; x < 10; ++x){
			for(int z = 0; z < 3; ++z){
				for(int y = 0; y < 7; ++y){
					Assert::AreEqual(x + 10 * (y + z * 7), Lat.at(x, y, z));
					Assert::AreEqual(x + 10 * (y + z * 7), *(Lat.locate(x,y,z)));
				}
			}
		}

		auto lotr = Lat.front_locate();
		int Val1 = 0;
		for(auto val : {1,1,2,3,2,2,1,1,3,-1,-3,2,-1,-2}){
			if(val == 1){
				lotr.add(1, 0, 0);
				Val1 += 1;
			} else if(val == -1){
				lotr.add(-1, 0, 0);
				Val1 -= 1;
			} else if(val == 2){
				lotr.add(0, 1, 0);
				Val1 += 10;
			} else if(val == -2){
				lotr.add(0, -1, 0);
				Val1 -= 10;
			} else if(val == 3){
				lotr.add(0, 0, 1);
				Val1 += 70;
			} else if(val == -3){
				lotr.add(0, 0, -1);
				Val1 -= 70;
			}

			Assert::AreEqual(Val1, *lotr);
		}

		lotr = Lat.back_locate();
		Val1 = 9+10*(6+7*2);
		for(auto val : {-1,-1,-2,-3,-2,-2,-1,-1,3,1,-3,-2,1,2}){
			if(val == 1){
				lotr.add(1, 0, 0);
				Val1 += 1;
			} else if(val == -1){
				lotr.add(-1, 0, 0);
				Val1 -= 1;
			} else if(val == 2){
				lotr.add(0, 1, 0);
				Val1 += 10;
			} else if(val == -2){
				lotr.add(0, -1, 0);
				Val1 -= 10;
			} else if(val == 3){
				lotr.add(0, 0, 1);
				Val1 += 70;
			} else if(val == -3){
				lotr.add(0, 0, -1);
				Val1 -= 70;
			}

			Assert::AreEqual(Val1, *lotr);
		}
	}
	TEST_METHOD(lattice_example2){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(280);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		auto Lat = make_lattice_view(Con.begin(), Con.end(), 10, 7, 4);

		auto SubLot = hmLib::subview(Lat.locate(3, 2, 1), Lat.locate(3, 2, 1).plus(4, 0, 2));
		typename decltype(SubLot)::indexer Va22l = typename decltype(SubLot)::indexer(Lat.locate(3, 2, 1).plus(4, 0, 2).base_extent());

		typename decltype(SubLot)::indexer Indxer1(lattices::point(3, 4, 4));
		typename decltype(SubLot)::indexer Indxer2 = Indxer1;
		typename decltype(SubLot)::indexer Indxer3 = std::move(Indxer1);

		auto Size = SubLot.extent();
		Assert::AreEqual(5u, Size[0]);
		Assert::AreEqual(1u, Size[1]);
		Assert::AreEqual(3u, Size[2]);
		Assert::AreEqual(15u, SubLot.lattice_size());

		Assert::AreEqual(3, SubLot.front_locate().base_point()[0]);
		Assert::AreEqual(2, SubLot.front_locate().base_point()[1]);
		Assert::AreEqual(1, SubLot.front_locate().base_point()[2]);

		auto Itr = SubLot.begin();
		for(int z = 0; z < 3; ++z){
			for(int x = 0; x < 5; ++x){
				Assert::IsFalse(Itr == SubLot.end());
				Assert::AreEqual(x + 5 * z, std::distance(SubLot.begin(),Itr));
				auto Pos = SubLot.index_to_point(std::distance(SubLot.begin(), Itr));
				Assert::AreEqual(x, Pos[0]);
				Assert::AreEqual(0, Pos[1]);
				Assert::AreEqual(z, Pos[2]);
				Assert::AreEqual(x + 3 + 2*10 + (z+1) * 70, *Itr);
				++Itr;
			}
		}
		Assert::IsTrue(Itr == SubLot.end());
	}
	TEST_METHOD(clattice_example1){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(210);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		auto Lat2 = make_lattice_view(Con.begin(), Con.end(), 10, 7, 3);
		const lattice_view<iterator, 3>& Lat(Lat2);

		for(int x = 0; x < 10; ++x){
			for(int z = 0; z < 3; ++z){
				for(int y = 0; y < 7; ++y){
					Assert::AreEqual(x + 10 * (y + z * 7), Lat.at(x, y, z));
					Assert::AreEqual(x + 10 * (y + z * 7), *(Lat.locate(x, y, z)));
				}
			}
		}

		auto lotr = Lat.front_locate();
		int Val1 = 0;
		for(auto val : {1,1,2,3,2,2,1,1,3,-1,-3,2,-1,-2}){
			if(val == 1){
				lotr.add(1, 0, 0);
				Val1 += 1;
			} else if(val == -1){
				lotr.add(-1, 0, 0);
				Val1 -= 1;
			} else if(val == 2){
				lotr.add(0, 1, 0);
				Val1 += 10;
			} else if(val == -2){
				lotr.add(0, -1, 0);
				Val1 -= 10;
			} else if(val == 3){
				lotr.add(0, 0, 1);
				Val1 += 70;
			} else if(val == -3){
				lotr.add(0, 0, -1);
				Val1 -= 70;
			}

			Assert::AreEqual(Val1, *lotr);
		}

		lotr = Lat.back_locate();
		Val1 = 9 + 10 * (6 + 7 * 2);
		for(auto val : {-1,-1,-2,-3,-2,-2,-1,-1,3,1,-3,-2,1,2}){
			if(val == 1){
				lotr.add(1, 0, 0);
				Val1 += 1;
			} else if(val == -1){
				lotr.add(-1, 0, 0);
				Val1 -= 1;
			} else if(val == 2){
				lotr.add(0, 1, 0);
				Val1 += 10;
			} else if(val == -2){
				lotr.add(0, -1, 0);
				Val1 -= 10;
			} else if(val == 3){
				lotr.add(0, 0, 1);
				Val1 += 70;
			} else if(val == -3){
				lotr.add(0, 0, -1);
				Val1 -= 70;
			}

			Assert::AreEqual(Val1, *lotr);
		}
	}
	TEST_METHOD(clattice_example2){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(280);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		auto Lat2 = make_lattice_view(Con.begin(), Con.end(), 10, 7, 4);
		const lattice_view<iterator, 3>& Lat(Lat2);

		auto SubLot = hmLib::subview(Lat.locate(3, 2, 1), Lat.locate(3, 2, 1).plus(4, 0, 2));
		typename decltype(SubLot)::indexer Va22l = typename decltype(SubLot)::indexer(Lat.locate(3, 2, 1).plus(4, 0, 2).base_extent());

		typename decltype(SubLot)::indexer Indxer1(lattices::extent(3, 4, 4));
		typename decltype(SubLot)::indexer Indxer2 = Indxer1;
		typename decltype(SubLot)::indexer Indxer3 = std::move(Indxer1);

		auto Size = SubLot.extent();
		Assert::AreEqual(5u, Size[0]);
		Assert::AreEqual(1u, Size[1]);
		Assert::AreEqual(3u, Size[2]);
		Assert::AreEqual(15u, SubLot.lattice_size());

		Assert::AreEqual(3, SubLot.front_locate().base_point()[0]);
		Assert::AreEqual(2, SubLot.front_locate().base_point()[1]);
		Assert::AreEqual(1, SubLot.front_locate().base_point()[2]);

		auto Itr = SubLot.begin();
		for(int z = 0; z < 3; ++z){
			for(int x = 0; x < 5; ++x){
				Assert::IsFalse(Itr == SubLot.end());
				Assert::AreEqual(x + 5 * z, std::distance(SubLot.begin(),Itr));
				auto Pos = SubLot.index_to_point(std::distance(SubLot.begin(), Itr));
				Assert::AreEqual(x, Pos[0]);
				Assert::AreEqual(0, Pos[1]);
				Assert::AreEqual(z, Pos[2]);
				Assert::AreEqual(x + 3 + 2 * 10 + (z + 1) * 70, *Itr);
				++Itr;
			}
		}
		Assert::IsTrue(Itr == SubLot.end());
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

		static_assert(decltype(lattices::extent(4, 5))::static_size()==2,"");
		torus_lattice_view<iterator, 2>  Lattice=make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(4, 5));

		auto Size = Lattice.extent();
		Assert::AreEqual(4u, Size[0], L"Size Error");
		Assert::AreEqual(5u, Size[1], L"Size Error");
	}
	TEST_METHOD(construct_with_point){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		auto UsedSize = lattices::point(4, 5);
		Assert::AreEqual(4, UsedSize[0], L"Axis Size Error");
		Assert::AreEqual(5, UsedSize[1], L"Axis Size Error");

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), UsedSize);

		auto Size = Lattice.extent();
		Assert::AreEqual(4u, Size[0], L"Size Error");
		Assert::AreEqual(5u, Size[1], L"Size Error");
	}
	TEST_METHOD(iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int{return Val++; });

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for(int i = 0; i < 90; ++i){
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.base_index());
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


		torus_lattice_view<iterator, 2>  Lattice2=make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
		const torus_lattice_view<iterator, 2>& Lattice = Lattice2;

		auto Begin = Lattice.begin();
		auto End = Lattice.end();
		for(int i = 0; i < 90; ++i){
			Assert::AreEqual(i, Con[i]);
			Assert::AreEqual(i, Begin.base_index());
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


		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(10, 9));

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

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
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

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
		auto lotr = Lattice.front_locate();
		auto blotr = Lattice.locate(7, 5);

		auto Dif = lotr - blotr;
		Assert::AreEqual(-7, Dif[0]);
		Assert::AreEqual(-5, Dif[1]);

		lotr -= Dif;
		Assert::IsTrue(lotr == blotr);
	}
	TEST_METHOD(subview){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::extent(4, 5));

		auto Size = Sub.extent();
		Assert::AreEqual(4u, Size[0], L"Size Error");
		Assert::AreEqual(5u, Size[1], L"Size Error");

		Assert::AreEqual(2 + 3 * 9, Sub.at(0, 0));
		Assert::AreEqual(5 + 4 * 9, Sub.at(3, 1));
	}
	TEST_METHOD(sublattice_iterator){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::extent(4, 5));

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

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

		auto Lotr1 = Lattice.locate(2, 3);
		auto Lotr2 = Sub.locate(0, 0);
		Assert::IsTrue(Lotr1 == Lotr2);

		Lotr1.add(2, 3);
		Lotr2.add(2, 3);
		Assert::IsTrue(Lotr1 == Lotr2);
		Assert::IsTrue(Lotr1.plus(1, 1) == Sub.back_locate());

		Assert::AreEqual(4u, Sub.extent()[0]);
		Assert::AreEqual(5u, Sub.extent()[1]);
	}
	TEST_METHOD(sublattice_locator_ex){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(2, 3), lattices::point(4, 5));

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

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(-2, -3), lattices::point(4, 5));

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

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Sub = Lattice.subview(lattices::point(-2, -3), lattices::point(4, 5));

		auto Itr = Sub.begin();

		for(int y = 0; y < 5; ++y){
			for(int x = 0; x < 4; ++x){
				Assert::IsFalse(Itr == Sub.end());
				Assert::AreEqual(hmLib::positive_mod(-2 + x,9) + 9*hmLib::positive_mod(-3 + y, 10),*Itr);
				++Itr;
			}
		}
	}
	TEST_METHOD(torus_lattice_example1){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice= make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));

		auto Size = hmLib::lattices::point(3,3);

		auto Itr = Lattice.begin();
		for(int y = 0; y < 10; ++y){
			for(int x = 0; x < 9; ++x){
				Assert::IsFalse(Itr == Lattice.end());

				auto Sub = hmLib::subview(Itr.locate() - Size, Itr.locate() + Size);
				auto SItr = Sub.begin();
				for(int sy = -3; sy <= 3; ++sy){
					for(int sx = -3; sx <= 3; ++sx){
						Assert::IsFalse(SItr == Sub.end());
						Assert::AreEqual(positive_mod(x + sx, 9) + positive_mod(y + sy, 10) * 9, *SItr);

						auto Dis = SItr.locate() - Itr.locate();
						Assert::AreEqual(sx, Dis[0]);
						Assert::AreEqual(sy, Dis[1]);

						++SItr;
					}
				}
				Assert::IsTrue(SItr == Sub.end());
				++Itr;
			}
		}
		Assert::IsTrue(Itr == Lattice.end());

	}
	TEST_METHOD(ctorus_lattice_example1){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(90);
		int Val = 0;
		std::generate(Con.begin(), Con.end(), [&]()->int {return Val++; });

		torus_lattice_view<iterator, 2>  Lattice2=make_torus_lattice_view(Con.begin(), Con.end(), lattices::extent(9, 10));
		const torus_lattice_view<iterator, 2>& Lattice(Lattice2);

		auto Size = hmLib::lattices::point(3, 3);

		auto Itr = Lattice.begin();
		for(int y = 0; y < 10; ++y){
			for(int x = 0; x < 9; ++x){
				Assert::IsFalse(Itr == Lattice.end());

				auto Sub = hmLib::subview(Itr.locate() - Size, Itr.locate() + Size);
				auto SItr = Sub.begin();
				for(int sy = -3; sy <= 3; ++sy){
					for(int sx = -3; sx <= 3; ++sx){
						Assert::IsFalse(SItr == Sub.end());
						Assert::AreEqual(positive_mod(x + sx, 9) + positive_mod(y + sy, 10) * 9, *SItr);

						auto Dis = SItr.locate() - Itr.locate();
						Assert::AreEqual(sx, Dis[0]);
						Assert::AreEqual(sy, Dis[1]);

						++SItr;
					}
				}
				Assert::IsTrue(SItr == Sub.end());
				++Itr;
			}
		}
		Assert::IsTrue(Itr == Lattice.end());

	}
};

TEST_CLASS(test_lattice) {
public:
	TEST_METHOD(default_construct) {
		lattice<int, 2> Lat;
		Assert::IsTrue(Lat.empty(), L"Empty error.");
		Assert::AreEqual(0u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(0u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(0u, Lat.lattice_size(), L"Initial size error.");
	}
	TEST_METHOD(extent_construct){
		lattice<int, 2> Lat(lattices::extent(3, 4),1);
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(1, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(1, 2), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(extent_construct_3d) {
		lattice<int, 3> Lat(lattices::extent(3, 4, 5), 1);
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(5u, Lat.extent()[2], L"Initial size error.");
		Assert::AreEqual(60u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(1, Lat.at(0, 0, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 0, 1), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(1, 2, 2), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(0, 3, 4), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 3, 4), L"Ini Value error.");
	}
	TEST_METHOD(itr_construct) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		lattice<int, 2> Lat(lattices::extent(3, 4), Vec.begin(), Vec.end());
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(0, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(9, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(11, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(datamove_construct) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		lattice<int, 2> Lat(lattices::extent(3, 4), std::move(Vec));
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(0, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(9, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(11, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(locate) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		lattice<int, 2> Lat(lattices::extent(3, 4), std::move(Vec));
		auto Ltr1 = Lat.front_locate();
		auto Ltr2 = Lat.back_locate();
		Assert::AreEqual(0, *Ltr1);
		Assert::AreEqual(11, *Ltr2);
		Assert::AreEqual((Ltr2 - Ltr1)[0], 2);
		Assert::AreEqual((Ltr2 - Ltr1)[1], 3);
	}
	TEST_METHOD(out_of_range) {
		lattice<int, 2> Lat(lattices::extent(3, 4), 0);
		Assert::ExpectException<lattices::out_of_range_access>([&]() {Lat.at(0, -1); });
		Assert::ExpectException<lattices::out_of_range_access>([&]() {Lat.at(-1, 0); });
		Assert::ExpectException<lattices::out_of_range_access>([&]() {Lat.at(3, 0); });
		Assert::ExpectException<lattices::out_of_range_access>([&]() {Lat.at(0, 4); });
		Assert::ExpectException<lattices::out_of_range_access>([&]() {Lat.at(3, 4); });
	}
};

TEST_CLASS(test_torus_lattice) {
public:
	TEST_METHOD(default_construct) {
		torus_lattice<int, 2> Lat;
		Assert::IsTrue(Lat.empty(), L"Empty error.");
		Assert::AreEqual(0u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(0u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(0u, Lat.lattice_size(), L"Initial size error.");
	}
	TEST_METHOD(extent_construct) {
		torus_lattice<int, 2> Lat(lattices::extent(3, 4), 1);
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(1, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(1, 2), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(extent_construct_3d) {
		torus_lattice<int, 3> Lat(lattices::extent(3, 4, 5), 1);
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(5u, Lat.extent()[2], L"Initial size error.");
		Assert::AreEqual(60u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(1, Lat.at(0, 0, 0), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 0, 1), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(1, 2, 2), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(0, 3, 4), L"Ini Value error.");
		Assert::AreEqual(1, Lat.at(2, 3, 4), L"Ini Value error.");
	}
	TEST_METHOD(itr_construct) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		torus_lattice<int, 2> Lat(lattices::extent(3, 4), Vec.begin(), Vec.end());
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(0, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(9, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(11, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(datamove_construct) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		torus_lattice<int, 2> Lat(lattices::extent(3, 4), std::move(Vec));
		Assert::IsFalse(Lat.empty(), L"Empty error.");
		Assert::AreEqual(3u, Lat.extent()[0], L"Initial size error.");
		Assert::AreEqual(4u, Lat.extent()[1], L"Initial size error.");
		Assert::AreEqual(12u, Lat.lattice_size(), L"Initial size error.");
		Assert::AreEqual(0, Lat.at(0, 0), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(2, 0), L"Ini Value error.");
		Assert::AreEqual(9, Lat.at(0, 3), L"Ini Value error.");
		Assert::AreEqual(11, Lat.at(2, 3), L"Ini Value error.");
	}
	TEST_METHOD(locate) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		torus_lattice<int, 2> Lat(lattices::extent(3, 4), std::move(Vec));
		auto Ltr1 = Lat.front_locate();
		auto Ltr2 = Lat.back_locate();
		Assert::AreEqual(0, *Ltr1);
		Assert::AreEqual(11, *Ltr2);
		Assert::AreEqual((Ltr2 - Ltr1)[0], 2);
		Assert::AreEqual((Ltr2 - Ltr1)[1], 3);
	}
	TEST_METHOD(out_of_range) {
		std::vector<int> Vec(12, 0);
		int n = 0;
		std::generate_n(Vec.begin(), 12, [&]() {return n++; });
		torus_lattice<int, 2> Lat(lattices::extent(3, 4), std::move(Vec));
		Assert::AreEqual(11, Lat.at(-1, -1), L"Ini Value error.");
		Assert::AreEqual(11, Lat.at(-4, -5), L"Ini Value error.");
		Assert::AreEqual(0, Lat.at(-3, -4), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(-1, 0), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(-1, -8), L"Ini Value error.");
		Assert::AreEqual(2, Lat.at(-4, 8), L"Ini Value error.");
	}
};

TEST_CLASS(test_lattice_keep_if) {
	TEST_METHOD(block_point_keep) {
		lattice<int, 3> Lat(lattices::extent(3u,4u,5u),0);
		{
			auto Itr = Lat.begin();
			for(int i = 0; i<60; ++i) {
				*(Itr++) = i;
			}
		}

		lattices::block_point_keeper<3> Keeper(Lat.begin(), Lat.end(), [](int v) {return (v>=6 && v<10) || (17<=v && v<25)|| (34<=v && v<41); });
		//4,8,7
		auto Range = Keeper.range(Lat);
		Assert::AreEqual(19u, Range.size());

		auto Itr = Range.begin();
		auto End = Range.end();

		for(int i = 6; i<10; ++i) {
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++Itr;
		}
		for(int i = 17; i<25; ++i) {
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++Itr;
		}
		for(int i = 34; i<41; ++i) {
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++Itr;
		}
		Assert::IsTrue(Itr==End);
	}
	TEST_METHOD(element_point_keep) {
		lattice<int, 3> Lat(lattices::extent(3u, 4u, 5u), 0);
		{
			auto Itr = Lat.begin();
			for(int i = 0; i<60; ++i) {
				*(Itr++) = i;
			}
		}

		lattices::element_point_keeper<3> Keeper(Lat.begin(), Lat.end(), [](int v) {return (v>=6 && v<10) || (17<=v && v<25)|| (34<=v && v<41); });
		//4,8,7
		auto Range = Keeper.range(Lat);
		Assert::AreEqual(19u, Range.size());

		auto KItr = Keeper.begin();
		auto KEnd = Keeper.end();
		auto Itr = Range.begin();
		auto End = Range.end();

		for(int i = 6; i<10; ++i) {
			Assert::IsFalse(KItr==KEnd);
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++KItr;
			++Itr;
		}
		for(int i = 17; i<25; ++i) {
			Assert::IsFalse(KItr==KEnd);
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++KItr;
			++Itr;
		}
		for(int i = 34; i<41; ++i) {
			Assert::IsFalse(KItr==KEnd);
			Assert::IsFalse(Itr==End);
			Assert::AreEqual(i, *Itr);
			++KItr;
			++Itr;
		}
		Assert::IsTrue(Itr==End);
		Assert::IsTrue(KItr==KEnd);
	}
	TEST_METHOD(block_point_keep_with_blocklat) {
		block_lattice<int, 3> Lat(lattices::extent(3u, 3u, 3u));
		{
			int cnt = 0;
			for(int x = 2; x<10; ++x) {
				for(int y = 3; y<9; ++y) {
					for(int z = 0; z<4; ++z) {
						Lat.ref(x, y, z) = cnt++;
					}
				}
			}
		}

		lattices::block_point_keeper<3> Keeper;
		Assert::IsTrue(Keeper.empty());
		Assert::AreEqual(0u, Keeper.size());

		Keeper.reset(Lat.begin(), Lat.end(), [](int v) {return (v>=40 && v<60) || (80<=v && v<95); });

		Assert::IsFalse(Keeper.empty());
		Assert::AreEqual(35u, Keeper.size());


		auto Range = Keeper.range(Lat);

		auto Itr = Range.begin();
		auto End = Range.end();

		std::vector<int> Ans;
		for(int i = 40; i<60; ++i) {
			Assert::IsFalse(Itr==End);
			Ans.push_back(*Itr);
			++Itr;
		}
		for(int i = 80; i<95; ++i) {
			Assert::IsFalse(Itr==End);
			Ans.push_back(*Itr);
			++Itr;
		}
		Assert::IsTrue(Itr==End);

		std::sort(Ans.begin(), Ans.end());
		Assert::AreEqual(35u, Ans.size());
		auto AnsItr = Ans.begin();
		for(int i = 40; i<60; ++i) {
			Assert::AreEqual(i, *AnsItr);
			++AnsItr;
		}
		for(int i = 80; i<95; ++i) {
			Assert::AreEqual(i, *AnsItr);
			++AnsItr;
		}
	}
};

TEST_CLASS(test_block_lattice) {
	TEST_METHOD(block_construct) {
		block_lattice<int, 2> Lat(4,4);

		Assert::AreEqual(0u, Lat.block_num());
		Assert::AreEqual(0u, Lat.lattice_size());

		Lat.assign(6, 7);
		Assert::AreEqual(0u, Lat.block_num());
		Assert::AreEqual(0u, Lat.lattice_size());

		Lat = block_lattice<int, 2>(2, 2);
	}
	TEST_METHOD(block_ref) {
		block_lattice<int, 2> Lat(4, 4);

		Lat.ref(0, 0) = 1;
		Assert::AreEqual(1u, Lat.block_num());
		Assert::AreEqual(16u, Lat.lattice_size());

		Lat.ref(3, 1) = 2;
		Assert::AreEqual(1u, Lat.block_num());
		Assert::AreEqual(16u, Lat.lattice_size());

		Lat.ref(7, 9) = 3;
		Assert::AreEqual(2u, Lat.block_num());
		Assert::AreEqual(32u, Lat.lattice_size());

		Assert::AreEqual(1, Lat.ref(0, 0));
		Assert::AreEqual(2, Lat.ref(3, 1));
		Assert::AreEqual(3, Lat.ref(7, 9));
	}
	TEST_METHOD(block_ref2) {
		block_lattice<int, 2> Lat(5, 5);

		Lat.ref(22, 18) = 1;	//[4,3] [2,3]
		Lat.ref(19, 0) = 2; 	//[3,0] [4,0]
		Lat.ref(5, 10) = 3; 	//[1,2] [0,0]
		Lat.ref(45, 1) = 4;		//[9,0] [0,1]
		Lat.ref(23, 16) = 5;	//[4,3] [3,1]
		Lat.ref(49, 4) = 6; 	//[9,0] [4,4]

		auto BItr = Lat.block_begin();
		auto BEnd = Lat.block_end();
		Assert::AreEqual(4, std::distance(BItr, BEnd));

		Assert::IsFalse(BItr==BEnd);
		Assert::IsTrue(BItr->point()==lattices::point(15,0));
		Assert::IsTrue(BItr->at(4, 0)== 2);
		++BItr;
		Assert::IsFalse(BItr==BEnd);
		Assert::IsTrue(BItr->point()==lattices::point(45, 0));
		Assert::IsTrue(BItr->at(0, 1)== 4);
		Assert::IsTrue(BItr->at(4, 4)== 6);
		++BItr;
		Assert::IsFalse(BItr==BEnd);
		Assert::IsTrue(BItr->point()==lattices::point(5, 10));
		Assert::IsTrue(BItr->at(0, 0)== 3);
		++BItr;
		Assert::IsFalse(BItr==BEnd);
		Assert::IsTrue(BItr->point()==lattices::point(20, 15));
		Assert::IsTrue(BItr->at(2, 3)== 1);
		Assert::IsTrue(BItr->at(3, 1)== 5);
		++BItr;
		Assert::IsTrue(BItr==BEnd);

		auto Itr = Lat.begin();
		auto End = Lat.end();
		Assert::AreEqual(4*5*5, std::distance(Itr, End));
	}
	TEST_METHOD(block_itr) {
		block_lattice<int, 2> Lat(4, 4);

		Lat.ref(0, 0) = 10;
		Lat.ref(3, 1) = 20;
		Lat.ref(7, 9) = 30;

		auto Itr = Lat.begin();
		auto End = Lat.end();

		Assert::AreEqual<int>(10, *Itr);
		Assert::IsFalse(Itr==End);
		++Itr;
		for(unsigned int i = 1; i<7; ++i) {
			Assert::AreEqual<int>(0, *Itr);
			Assert::IsFalse(Itr==End);
			++Itr;
		}
		Assert::AreEqual<int>(20, *Itr);
		Assert::IsFalse(Itr==End);
		++Itr;
		for(unsigned int i = 8; i<16; ++i) {
			Assert::AreEqual<int>(0, *Itr);
			Assert::IsFalse(Itr==End);
			++Itr;
		}

		for(unsigned int i = 0; i<7; ++i) {
			Assert::AreEqual<int>(0, *Itr);
			Assert::IsFalse(Itr==End);
			++Itr;
		}
		Assert::AreEqual<int>(30, *Itr);
		Assert::IsFalse(Itr==End);
		++Itr;
		for(unsigned int i = 8; i<16; ++i) {
			Assert::AreEqual<int>(0, *Itr);
			Assert::IsFalse(Itr==End);
			++Itr;
		}
		Assert::IsTrue(Itr==End);
	}
};

TEST_CLASS(test_zip_lattice) {
	TEST_METHOD(zip_access) {
		multiaxis<double, 2> Axes({hmLib::make_axis(0.0,1.0,5),hmLib::make_axis(0.0,1.0,6) });
		lattice<double, 2> Lat(lattices::extent(4,5),0.0);

		auto Zip = lattices::make_zip_lattice(Axes, Lat);

		auto Elm = Zip.at(lattices::point(2,3));
		Assert::AreEqual(0.50, std::get<0>(Elm).at(0), 1e-5);
		Assert::AreEqual(0.60, std::get<0>(Elm).at(1), 1e-5);
		Assert::AreEqual(0.00, std::get<1>(Elm), 1e-5);

		std::get<1>(Elm) = 5.0;
		Assert::AreEqual(5.00, std::get<1>(Elm), 1e-5);
	}
	TEST_METHOD(zip_access_block) {
		multiaxis<double, 2> Axes({ hmLib::make_axis(0.0,10.0,41),hmLib::make_axis(0.0,10.0,51) });
		block_lattice<double, 2> Lat(10,10);

		auto Zip = lattices::make_zip_lattice(Axes, Lat);

		auto Elm = Zip.ref(lattices::point(2, 3));
		Assert::AreEqual(0.50, std::get<0>(Elm).at(0), 1e-5);
		Assert::AreEqual(0.60, std::get<0>(Elm).at(1), 1e-5);
		Assert::AreEqual(0.00, std::get<1>(Elm), 1e-5);

		std::get<1>(Elm) = 5.0;
		Assert::AreEqual(5.00, std::get<1>(Elm), 1e-5);


		Elm = Zip.ref(lattices::point(20, 30));
		Assert::AreEqual(5.0, std::get<0>(Elm).at(0), 1e-5);
		Assert::AreEqual(6.0, std::get<0>(Elm).at(1), 1e-5);
		Assert::AreEqual(0.0, std::get<1>(Elm), 1e-5);
	}
	TEST_METHOD(freq_use_case) {
		multiaxis<double, 2> Axes({ hmLib::make_axis(0.0,100.0,10000),hmLib::make_axis(0.0,100.0,10000) });
		block_lattice<double, 2> Lat(10, 10);

		auto ZipLat = lattices::make_zip_lattice(Axes, Lat);

		lattices::block_point_keeper<2> Keeper;
		//Keeper.reset(Lat.begin(),Lat.end(),)
	}
};
}
