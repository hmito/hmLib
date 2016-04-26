#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include<algorithm>
#include<list>
#include <hmLib/lattices.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
TEST_CLASS(test_lattice_view){
public:
	TEST_METHOD(construct){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);

		lattice_view<iterator, 2>  Lattice(Con.begin(),Con.end(), 4, 5);

		Assert::AreEqual(4, Lattice.size<0>(), L"Size Error");
		Assert::AreEqual(5, Lattice.size<1>(), L"Size Error");
	}
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
};
}
