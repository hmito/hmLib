#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/lattices/lattice_indexer.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_lattice_indexer) {
public:
	TEST_METHOD(construct) {
		lattices::lattice_indexer<3> Indexer(2,3,4);

		Assert::AreEqual(2u, Indexer.axis_size<0>(), L"Size Error");
		Assert::AreEqual(3u, Indexer.axis_size<1>(), L"Size Error");
		Assert::AreEqual(4u, Indexer.axis_size<2>(), L"Size Error");
		Assert::AreEqual(0, Indexer.axis_gap<0>(), L"Gap Error");
		Assert::AreEqual(0, Indexer.axis_gap<1>(), L"Gap Error");
		Assert::AreEqual(0, Indexer.axis_gap<2>(), L"Gap Error");

		Assert::AreEqual(24u, Indexer.lattice_size(), L"Lattice size Errpr");
	}
	TEST_METHOD(construct_with_gap) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u,2), std::make_pair(4u, 5));

		Assert::AreEqual(2u, Indexer.axis_size<0>(), L"Size Error");
		Assert::AreEqual(3u, Indexer.axis_size<1>(), L"Size Error");
		Assert::AreEqual(4u, Indexer.axis_size<2>(), L"Size Error");
		Assert::AreEqual(0, Indexer.axis_gap<0>(), L"Gap Error");
		Assert::AreEqual(2, Indexer.axis_gap<1>(), L"Gap Error");
		Assert::AreEqual(5, Indexer.axis_gap<2>(), L"Gap Error");

		Assert::AreEqual(24u, Indexer.lattice_size(), L"Lattice size Errpr");
	}
	TEST_METHOD(index) {
		lattices::lattice_indexer<3> Indexer(2, 3, 4);

		Assert::AreEqual(0, Indexer.index(0, 0, 0), L"Index Error");
		Assert::AreEqual(2, Indexer.index(0, 1, 0), L"Index Error");
		Assert::AreEqual(8, Indexer.index(0, 1, 1), L"Index Error");
		Assert::AreEqual(9, Indexer.index(1, 1, 1), L"Index Error");
	}
	TEST_METHOD(index_with_gap) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));

		Assert::AreEqual(0, Indexer.index(0, 0, 0), L"Index Error");
		Assert::AreEqual(4, Indexer.index(0, 1, 0), L"Index Error");
		Assert::AreEqual(4 + (4 * 3 + 5), Indexer.index(0, 1, 1), L"Index Error");
		Assert::AreEqual(1 + 4 + (4 * 3 + 5), Indexer.index(1, 1, 1), L"Index Error");
	}
	TEST_METHOD(point_index_with_gap) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));

		Assert::AreEqual(0, Indexer.index(lattices::make_point(0, 0, 0)), L"Index Error");
		Assert::AreEqual(4, Indexer.index(lattices::make_point(0, 1, 0)), L"Index Error");
		Assert::AreEqual(4 + (4 * 3 + 5), Indexer.index(lattices::make_point(0, 1, 1)), L"Index Error");
		Assert::AreEqual(1 + 4 + (4 * 3 + 5), Indexer.index(lattices::make_point(1, 1, 1)), L"Index Error");
	}
	TEST_METHOD(operator_index_with_gap) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));

		Assert::AreEqual(0, Indexer(0, 0, 0), L"Index Error");
		Assert::AreEqual(4, Indexer(0, 1, 0), L"Index Error");
		Assert::AreEqual(4 + (4 * 3 + 5), Indexer(0, 1, 1), L"Index Error");
		Assert::AreEqual(1 + 4 + (4 * 3 + 5), Indexer(1, 1, 1), L"Index Error");
	}
	TEST_METHOD(point_operator_index_with_gap) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));

		Assert::AreEqual(0, Indexer(lattices::make_point(0, 0, 0)), L"Index Error");
		Assert::AreEqual(4, Indexer(lattices::make_point(0, 1, 0)), L"Index Error");
		Assert::AreEqual(4 + (4 * 3 + 5), Indexer(lattices::make_point(0, 1, 1)), L"Index Error");
		Assert::AreEqual(1 + 4 + (4 * 3 + 5), Indexer(lattices::make_point(1, 1, 1)), L"Index Error");
	}
	TEST_METHOD(index_range) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));

		auto Ans = Indexer.index_range();
		Assert::AreEqual(0, Ans.first, L"Index range Error");
		Assert::AreEqual(1 + 4*2 + (4*3 +5)*3, Ans.second, L"Index range Error");
	}
	TEST_METHOD(resize_rebase) {
		lattices::lattice_indexer<3> Indexer(5, 1, 3);

		Indexer.axis_resize<0>(2u);
		Indexer.axis_resize<1>(3u, 2);
		Indexer.axis_resize<2>(4u);
		Indexer.axis_set_gap<2>(5);

		Assert::AreEqual(2u, Indexer.axis_size<0>(), L"Size Error");
		Assert::AreEqual(3u, Indexer.axis_size<1>(), L"Size Error");
		Assert::AreEqual(4u, Indexer.axis_size<2>(), L"Size Error");
		Assert::AreEqual(0, Indexer.axis_gap<0>(), L"Gap Error");
		Assert::AreEqual(2, Indexer.axis_gap<1>(), L"Gap Error");
		Assert::AreEqual(5, Indexer.axis_gap<2>(), L"Gap Error");

		Assert::AreEqual(24u, Indexer.lattice_size(), L"Lattice size Errpr");

		auto Ans2 = Indexer.index_range();
		Assert::AreEqual(0, Ans2.first, L"Index range Error");
		Assert::AreEqual(1 + 4 * 2 + (4 * 3 + 5) * 3, Ans2.second, L"Index range Error");

		Assert::AreEqual(0, Indexer.base(), L"Lattice size Errpr");
		Indexer.set_base(-20);
		Assert::AreEqual(-20, Indexer.base(), L"Lattice size Errpr");

		auto Ans = Indexer.index_range();
		Assert::AreEqual(0 - 20, Ans.first, L"Index range Error");
		Assert::AreEqual(1 + 4 * 2 + (4 * 3 + 5) * 3 - 20, Ans.second, L"Index range Error");

		Assert::AreEqual(24u, Indexer.lattice_size(), L"Lattice size Errpr");
	}
	TEST_METHOD(point_operator_index_with_gap_rebase) {
		lattices::lattice_indexer<3> Indexer(2, std::make_pair(3u, 2), std::make_pair(4u, 5));
		Indexer.set_base(30);
		Assert::AreEqual(0+30, Indexer(lattices::make_point(0, 0, 0)), L"Index Error");
		Assert::AreEqual(4 + 30, Indexer(lattices::make_point(0, 1, 0)), L"Index Error");
		Assert::AreEqual(4 + (4 * 3 + 5) + 30, Indexer(lattices::make_point(0, 1, 1)), L"Index Error");
		Assert::AreEqual(1 + 4 + (4 * 3 + 5) + 30, Indexer(lattices::make_point(1, 1, 1)), L"Index Error");
	}
	};
}
