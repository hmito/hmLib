#include "stdafx.h"
#include "CppUnitTest.h"
#include <array>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include "../../../iterators.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_overflow_iterator) {
	public:
		TEST_METHOD(use_overflow_iterator) {
			std::array<int, 3> Arr;

			auto Itr = hmLib::make_overflow_iterator(Arr.begin(), Arr.end());
			*(Itr++) = 0;
			Assert::IsFalse(Itr.overflow());
			Assert::IsFalse(Itr.empty());
			*(Itr++) = 1;
			Assert::IsFalse(Itr.overflow());
			Assert::IsFalse(Itr.empty());
			*(Itr++) = 2;
			Assert::IsFalse(Itr.overflow());
			Assert::IsTrue(Itr.empty());
			*(Itr++) = 3;
			Assert::IsTrue(Itr.overflow());
			Assert::IsTrue(Itr.empty());

			for(int i = 0; i < 3; ++i) {
				Assert::AreEqual(i, Arr[i]);
			}
		}
	};
	TEST_CLASS(test_index_access_iterator) {
	public:
		TEST_METHOD(use_index_access_iterator_list) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::list<int> Index{ 2,3,5 };

			auto Range = make_index_access_range(Val.begin(), Index.begin(),Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
		}
		TEST_METHOD(use_index_access_iterator_vector) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_access_range(Val.begin(), Index.begin(),Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
			Assert::AreEqual(Index.size(), Range.size());
		}
		TEST_METHOD(use_index_at_access_iterator_list) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::list<int> Index{ 2,3,5 };

			auto Range = make_index_at_access_range(Val, Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
		}
		TEST_METHOD(use_index_at_access_iterator_vector) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_at_access_range(Val, Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
			Assert::AreEqual(Index.size(), Range.size());
		}
		TEST_METHOD(use_index_op_access_iterator_vector) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_op_access_range(Val, Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto v:Range) {
				Assert::AreEqual(v, Val[*itr++]);
			}
			Assert::IsTrue(itr==Index.end());
			Assert::AreEqual(Index.size(), Range.size());
		}
	};
	TEST_CLASS(test_zip_iterator) {
		TEST_METHOD(test_make_zip) {
			std::list<int> List{ 0,1,2,3,4,5 };
			std::vector<int> Vec{ 10,11,12,13,14,15,16,17 };
			std::array<int, 4> Arr{ 20,21,22,23 };

			auto Itr = make_zip_iterator(List.begin(), Vec.begin(), Arr.begin());

			Assert::AreEqual(0, std::get<0>(*Itr));
			Assert::AreEqual(10, std::get<1>(*Itr));
			Assert::AreEqual(20, std::get<2>(*Itr));

			auto End = make_zip_iterator(List.end(), Vec.end(), Arr.end());
			Assert::AreEqual(6, std::distance(std::get<0>(Itr.pack()), std::get<0>(End.pack())));
			Assert::AreEqual(8, std::distance(std::get<1>(Itr.pack()), std::get<1>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<2>(Itr.pack()), std::get<2>(End.pack())));

			End = zip_iterator_shorten(Itr, End);
			Assert::AreEqual(4, std::distance(std::get<0>(Itr.pack()), std::get<0>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<1>(Itr.pack()), std::get<1>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<2>(Itr.pack()), std::get<2>(End.pack())));
		}
		TEST_METHOD(test_zip_range) {
			std::list<int> List{ 0,1,2,3,4,5 };
			std::vector<int> Vec{ 10,11,12,13,14,15,16,17 };
			std::array<int, 4> Arr{ 20,21,22,23 };

			auto Range = make_zip_range(List, Vec, Arr);
			auto Itr = Range.begin();
			auto End = Range.end();
			Assert::AreEqual(4, std::distance(std::get<0>(Itr.pack()), std::get<0>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<1>(Itr.pack()), std::get<1>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<2>(Itr.pack()), std::get<2>(End.pack())));
		}
		TEST_METHOD(test_zip_container) {
			std::string List{ '0','1','2','3','4','5' };
			std::vector<int> Vec{ 10,11,12,13,14,15,16,17 };
			std::array<double, 4> Arr{ 20,21,22,23 };

			auto Zip = make_zip_container<int>(List, Vec, Arr);
			Assert::AreEqual('0', std::get<0>(Zip.at(0)));
			Assert::AreEqual(10, std::get<1>(Zip.at(0)));
			Assert::AreEqual(20.0, std::get<2>(Zip.at(0)),1e-10);
			Assert::AreEqual('3', std::get<0>(Zip.at(3)));
			Assert::AreEqual(13, std::get<1>(Zip.at(3)));
			Assert::AreEqual(23.0, std::get<2>(Zip.at(3)), 1e-10);
		}
		TEST_METHOD(test_zip_container_w) {
			std::string List{ '0','1','2','3','4','5' };
			std::vector<int> Vec{ 10,11,12,13,14,15,16,17 };
			std::array<double, 4> Arr{ 20,21,22,23 };

			auto Zip = make_zip_container<int>(List, Vec, Arr);
			std::get<0>(Zip.at(0)) = 'A';
			std::get<1>(Zip.at(0)) = 100;
			std::get<2>(Zip.at(0)) = 5.5;

			Assert::AreEqual('A', std::get<0>(Zip.at(0)));
			Assert::AreEqual(100, std::get<1>(Zip.at(0)));
			Assert::AreEqual(5.5, std::get<2>(Zip.at(0)), 1e-10);
			Assert::AreEqual('3', std::get<0>(Zip.at(3)));
			Assert::AreEqual(13, std::get<1>(Zip.at(3)));
			Assert::AreEqual(23.0, std::get<2>(Zip.at(3)), 1e-10);
		}
	};
}
