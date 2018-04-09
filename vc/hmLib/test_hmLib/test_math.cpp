#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../iterators.hpp"
#include "../../../math/root.hpp"
#include "../../../math/axis.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib {
	TEST_CLASS(test_math_root) {
	public:
		TEST_METHOD(root_finding_toms748) {
		auto f = [](double x) {return (x - 0.3)*(x - 0.4549)*(x - 0.8991); };

		std::vector<double> Ans;
		hmLib::math::stable_root_toms748(f, 0.0, 1.0, 0.001, 1e-5, hmLib::back_inserter(Ans));

		Assert::AreEqual(1u, Ans.size(), L"Ans Num Error");
//		Assert::AreEqual(0.3, Ans.at(0), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.4549, Ans.at(0), 1e-5, L"Ans Num Error");
//		Assert::AreEqual(0.8991, Ans.at(2), 1e-5, L"Ans Num Error");
		}
		TEST_METHOD(root_finding_bisect) {
		auto f = [](double x) {return (x - 0.3)*(x - 0.4549)*(x - 0.8991); };

		std::vector<double> Ans;
		hmLib::math::bisect_root_stepper<double> Stepper(0.001, 1e-5);
		hmLib::math::stable_root(Stepper, f, 0.0, 1.0, hmLib::back_inserter(Ans));

		Assert::AreEqual(1u, Ans.size(), L"Ans Num Error");
//		Assert::AreEqual(0.3, Ans.at(0), 1e-5, L"Ans Num Error");
		Assert::AreEqual(0.4549, Ans.at(0), 1e-5, L"Ans Num Error");
//		Assert::AreEqual(0.8991, Ans.at(2), 1e-5, L"Ans Num Error");
		}
	};
	TEST_CLASS(test_math_axis) {
		TEST_METHOD(make_axis_with_borders) {
			auto Axis1 = make_axis(0.0, 10.0, 11);
			Assert::AreEqual(11u, Axis1.size());
			Assert::AreEqual( 0.0, Axis1.lower());
			Assert::AreEqual(10.0, Axis1.upper());

			auto Axis2 = make_axis(0.0, 10.0, 11, math::include_boundary_tag());
			Assert::AreEqual(11u, Axis2.size());
			Assert::AreEqual(0.0, Axis2.lower());
			Assert::AreEqual(10.0, Axis2.upper());

			auto Axis3 = make_axis(0.0, 10.0, 9, math::exclude_boundary_tag());
			Assert::AreEqual(9u, Axis3.size());
			Assert::AreEqual(1.0, Axis3.lower());
			Assert::AreEqual(9.0, Axis3.upper());

			auto Axis4 = make_axis(0.0, 10.0, 10, math::exclude_lower_boundary_tag());
			Assert::AreEqual(10u, Axis4.size());
			Assert::AreEqual(1.0, Axis4.lower());
			Assert::AreEqual(10.0, Axis4.upper());

			auto Axis5 = make_axis(0.0, 10.0, 10, math::exclude_upper_boundary_tag());
			Assert::AreEqual(10u, Axis5.size());
			Assert::AreEqual(0.0, Axis5.lower());
			Assert::AreEqual(9.0, Axis5.upper());
		}
		TEST_METHOD(axis_grid_index) {
			auto Axis = make_axis(0.0, 1.0, 11);

			Assert::AreEqual(0.1, Axis.grid_width());

			for(unsigned int i = 0; i<Axis.size(); ++i) {
				Assert::AreEqual(i, Axis.grid_round_index(0.1*i-0.04));
				Assert::AreEqual(i, Axis.grid_round_index(0.1*i+0.04));

				Assert::AreEqual(i, Axis.grid_floor_index(0.1*i+0.04));
				Assert::AreEqual(i, Axis.grid_floor_index(0.1*i+0.08));

				Assert::AreEqual(i, Axis.grid_ceil_index(0.1*i-0.04));
				Assert::AreEqual(i, Axis.grid_ceil_index(0.1*i-0.08));

				Assert::AreEqual(0.1*i, Axis.grid_round(0.1*i-0.04), 1e-5);
				Assert::AreEqual(0.1*i, Axis.grid_round(0.1*i+0.04), 1e-5);

				Assert::AreEqual(0.1*i, Axis.grid_floor(0.1*i+0.04), 1e-5);
				Assert::AreEqual(0.1*i, Axis.grid_floor(0.1*i+0.08), 1e-5);

				Assert::AreEqual(0.1*i, Axis.grid_ceil(0.1*i-0.04), 1e-5);
				Assert::AreEqual(0.1*i, Axis.grid_ceil(0.1*i-0.08), 1e-5);
			}
		}
		TEST_METHOD(axis_grid_round_range) {
			auto Axis = make_axis(0.0, 1.0, 11);

			Assert::AreEqual(0.1, Axis.grid_width());

			{
				auto Ans = Axis.grid_round_range(0.30-0.04, 0.30+0.04);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_round_range(0.30-0.04, 0.30+0.06);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.9 , Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_round_range(0.30-0.04, 0.30+0.26);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.09/0.30, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.10/0.30, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5u, Ans.at(2).first);
				Assert::AreEqual(0.10/0.30, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6u, Ans.at(3).first);
				Assert::AreEqual(0.01/0.30, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.09/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6u, (*Itr).first);
				Assert::AreEqual(0.01/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_grid_floor_range) {
			auto Axis = make_axis(0.0, 1.0, 11);

			Assert::AreEqual(0.1, Axis.grid_width());

			{
				auto Ans = Axis.grid_floor_range(0.30+0.04, 0.30+0.08);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_floor_range(0.30+0.01, 0.30+0.11);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_floor_range(0.30+0.01, 0.30+0.31);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.09/0.30, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.10/0.30, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5u, Ans.at(2).first);
				Assert::AreEqual(0.10/0.30, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6u, Ans.at(3).first);
				Assert::AreEqual(0.01/0.30, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.09/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6u, (*Itr).first);
				Assert::AreEqual(0.01/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_grid_ceil_range) {
			auto Axis = make_axis(0.0, 1.0, 11);

			Assert::AreEqual(0.1, Axis.grid_width());

			{
				auto Ans = Axis.grid_ceil_range(0.20+0.04, 0.20+0.08);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_ceil_range(0.20+0.01, 0.20+0.11);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.grid_ceil_range(0.20+0.01, 0.20+0.31);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3u, Ans.at(0).first);
				Assert::AreEqual(0.09/0.30, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4u, Ans.at(1).first);
				Assert::AreEqual(0.10/0.30, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5u, Ans.at(2).first);
				Assert::AreEqual(0.10/0.30, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6u, Ans.at(3).first);
				Assert::AreEqual(0.01/0.30, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3u, (*Itr).first);
				Assert::AreEqual(0.09/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5u, (*Itr).first);
				Assert::AreEqual(0.1/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6u, (*Itr).first);
				Assert::AreEqual(0.01/0.30, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_translate) {
			auto Axis1 = make_axis(0.2, 0.89, 4);	//[0.2, 0.43, 0.66, 0.89]
			auto Axis2 = make_axis(0.02, 0.76, 7);	//[0.02, 0.14, 0.26, 0.38, 0.50, 0.62, 0.74]


			auto Trans = Axis1.grid_round_translate(Axis2);

			{
				auto Ans = Trans.grid_round_at(0);
				
			}
		}
	};
}
