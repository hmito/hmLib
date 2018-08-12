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
			Assert::AreEqual<int>(Index.size(), std::distance(Range.begin(), Range.end()));
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
			Assert::AreEqual<int>(Index.size(), std::distance(Range.begin(), Range.end()));
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
			Assert::AreEqual<int>(Index.size(), std::distance(Range.begin(), Range.end()));
		}
		TEST_METHOD(write_index_access_iterator) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_access_range(Val.begin(), Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto& v:Range) {
				v = 0.0;
			}

			for(auto i:Index) {
				Assert::AreEqual(0.0, Val[i], 1e-10);
			}
		}
		TEST_METHOD(write_index_at_access_iterator) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_at_access_range(Val, Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto& v:Range) {
				v = 0.0;
			}

			for(auto i:Index) {
				Assert::AreEqual(0.0, Val[i], 1e-10);
			}
		}
		TEST_METHOD(write_index_op_access_iterator) {
			std::vector<double> Val{ 0.5,1.2,2.3,3.2,4.5,7.3 };
			std::vector<int> Index{ 2,3,5 };

			auto Range = make_index_op_access_range(Val, Index.begin(), Index.end());

			auto itr = Index.begin();
			for(auto& v:Range) {
				v = 0.0;
			}

			for(auto i:Index) {
				Assert::AreEqual(0.0, Val[i], 1e-10);
			}
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
			Assert::IsTrue(std::is_same<typename decltype(Range)::range_category,std::bidirectional_iterator_tag>::value);
				
			auto Itr = Range.begin();
			auto End = Range.end();
			Assert::AreEqual(4, std::distance(std::get<0>(Itr.pack()), std::get<0>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<1>(Itr.pack()), std::get<1>(End.pack())));
			Assert::AreEqual(4, std::distance(std::get<2>(Itr.pack()), std::get<2>(End.pack())));

			decltype(Range)::value_type Val = *(Range.begin());
			std::get<0>(Val) = 100;
			std::get<1>(Val) = 100;
			std::get<2>(Val) = 100;

			decltype(Range)::reference Ref = *(Range.begin());
			Assert::AreEqual(0, std::get<0>(Ref));
			Assert::AreEqual(10, std::get<1>(Ref));
			Assert::AreEqual(20, std::get<2>(Ref));
		}
		TEST_METHOD(test_zip_container) {
			std::string List{ '0','1','2','3','4','5' };
			std::vector<int> Vec{ 10,11,12,13,14,15,16,17 };
			std::array<double, 4> Arr{ 20,21,22,23 };

			auto Zip = make_zip_range(List, Vec, Arr);
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

			auto Zip = make_zip_range(List, Vec, Arr);
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
	TEST_CLASS(test_transform_iterator) {
		TEST_METHOD(test_ref) {
			std::vector<std::pair<double, int>> Vec(20, std::make_pair(2.0,1));
			auto Range = hmLib::make_transform_range(Vec.begin(), Vec.end(), [](std::pair<double, int>& p)->int& {return p.second; });
			static_assert(std::is_same<decltype(Range)::iterator::iterator_category, std::random_access_iterator_tag>::value, "iterator category is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::value_type, int>::value, "value type is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::reference, int&>::value, "reference is incorrect");

			int cnt = 0;
			for(auto& v:Range)v = cnt++;

			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(Vec.at(i).second, i);
			}

			auto itr = Range.begin();
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(*itr, i);
				itr++;
			}
			Assert::IsTrue(itr==Range.end());
		}
		TEST_METHOD(test_cref) {
			std::vector<std::pair<double, int>> Vec(20, std::make_pair(2.0, 1));
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Vec.at(i).second = i;
			}

			auto Range = hmLib::make_transform_range(Vec.cbegin(), Vec.cend(), [](auto& p)->const int&{return p.second; });
			static_assert(std::is_same<decltype(Range)::iterator::iterator_category, std::random_access_iterator_tag>::value, "iterator category is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::value_type, const int>::value, "value type is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::reference, const int&>::value, "reference is incorrect");

			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(Vec.at(i).second, i);
			}

			auto itr = Range.begin();
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(*itr, i);
				itr++;
			}
			Assert::IsTrue(itr==Range.end());
		}
		TEST_METHOD(test_value) {
			std::vector<std::pair<double, int>> Vec(20, std::make_pair(2.0, 1));
			auto Range = hmLib::make_transform_range(Vec.begin(), Vec.end(), [](std::pair<double, int>& p){return p.second; });
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Vec.at(i).second = i;
			}

			static_assert(std::is_same<decltype(Range)::iterator::iterator_category, std::random_access_iterator_tag>::value, "iterator category is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::value_type, const int>::value, "value type is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::reference, const int>::value, "reference is incorrect");

			auto itr = Range.begin();
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(*itr, i);
				itr++;
			}
			Assert::IsTrue(itr==Range.end());
		}
		TEST_METHOD(test_arrow) {
			std::vector<std::pair<double, int>> Vec(20, std::make_pair(2.0, 1));
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Vec.at(i).first = 1.0/(i+1.0);
				Vec.at(i).second = i;
			}
	
			auto Range = hmLib::make_transform_range(Vec.begin(), Vec.end(), [](std::pair<double, int>& p) {return std::make_pair(p.second,p.first); });

			static_assert(std::is_same<decltype(Range)::iterator::iterator_category, std::random_access_iterator_tag>::value, "iterator category is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::value_type, const std::pair<int,double>>::value, "value type is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::reference, const std::pair<int, double>>::value, "reference is incorrect");

			auto itr = Range.begin();
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(itr->first, i);
				Assert::AreEqual(itr->second, 1.0/(i+1.0), 1e-10);
				itr++;
			}
			Assert::IsTrue(itr==Range.end());
		}
		TEST_METHOD(test_list) {
			std::list<std::pair<double, int>> Vec(20, std::make_pair(2.0, 1));
			auto Range = hmLib::make_transform_range(Vec.begin(), Vec.end(), [](std::pair<double, int>& p)->int& {return p.second; });
			static_assert(std::is_same<decltype(Range)::iterator::iterator_category, std::bidirectional_iterator_tag>::value, "iterator category is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::value_type, int>::value, "value type is incorrect");
			static_assert(std::is_same<decltype(Range)::iterator::reference, int&>::value, "reference is incorrect");

			int cnt = 0;
			for(auto& v:Range)v = cnt++;

			auto itr = Range.begin();
			for(int i = 0; i<static_cast<int>(Vec.size()); ++i) {
				Assert::AreEqual(*itr, i);
				itr++;
			}
			Assert::IsTrue(itr==Range.end());
		}

	};
}
