#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include<algorithm>
#include <hmLib/lattice_view.hpp>
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

		Assert::AreEqual(4u, Pos.size(), L"Point Access Error");
		Assert::AreEqual(4, Pos[0],L"Point Access Error");
		Assert::AreEqual(5, Pos[1], L"Point Access Error");
		Assert::AreEqual(6, Pos[2], L"Point Access Error");
		Assert::AreEqual(7, Pos[3], L"Point Access Error");
	}
	TEST_METHOD(use_make_point){
		auto Pos = lattices::make_point(4, 5, 6, 7);

		Assert::AreEqual(4u, Pos.size(), L"Point Access Error");
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
	/*
	TEST_METHOD(point){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		lattice_view<iterator, 2> Lat(Con.begin(), Con.end(), 5, 6);
		
		auto Pos = Lat.point(4, 5);
		Assert::AreEqual(2, Pos.dim(), L"Point Error");
		Assert::AreEqual(4, Pos.get<0>, L"Point Error");
		Assert::AreEqual(5, Pos.get<1>, L"Point Error");
	}*/
};
}
