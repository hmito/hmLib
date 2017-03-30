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
		lattices::lattice_indexer<3> Indexer(lattices::make_point(2,3,4));

		Assert::AreEqual(2, Indexer.size()[0], L"Size Error");
		Assert::AreEqual(3, Indexer.size()[1], L"Size Error");
		Assert::AreEqual(4, Indexer.size()[2], L"Size Error");

		Assert::AreEqual(24u, Indexer.lattice_size(), L"Lattice size Errpr");
	}
	TEST_METHOD(index) {
		lattices::lattice_indexer<3> Indexer(lattices::make_point(2, 3, 4));

		Assert::AreEqual(0, Indexer.index(lattices::make_point(0, 0, 0)), L"Index Error");
		Assert::AreEqual(2, Indexer.index(lattices::make_point(0, 1, 0)), L"Index Error");
		Assert::AreEqual(8, Indexer.index(lattices::make_point(0, 1, 1)), L"Index Error");
		Assert::AreEqual(9, Indexer.index(lattices::make_point(1, 1, 1)), L"Index Error");
	}
	};
}
