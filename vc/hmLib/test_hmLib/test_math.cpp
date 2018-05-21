#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../iterators.hpp"
#include "../../../math/root.hpp"
#include "../../../math/axis.hpp"
#include "../../../math/multiaxis.hpp"
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

			auto Axis2 = make_axis(0.0, 10.0, 11, math::make_axis_option::none);
			Assert::AreEqual(11u, Axis2.size());
			Assert::AreEqual(0.0, Axis2.lower());
			Assert::AreEqual(10.0, Axis2.upper());

			auto Axis3 = make_axis(0.0, 10.0, 9, math::make_axis_option::exclude_boundary);
			Assert::AreEqual(9u, Axis3.size());
			Assert::AreEqual(1.0, Axis3.lower());
			Assert::AreEqual(9.0, Axis3.upper());

			auto Axis4 = make_axis(0.0, 10.0, 10, math::make_axis_option::exclude_lower_boundary);
			Assert::AreEqual(10u, Axis4.size());
			Assert::AreEqual(1.0, Axis4.lower());
			Assert::AreEqual(10.0, Axis4.upper());

			auto Axis5 = make_axis(0.0, 10.0, 10, math::make_axis_option::exclude_upper_boundary);
			Assert::AreEqual(10u, Axis5.size());
			Assert::AreEqual(0.0, Axis5.lower());
			Assert::AreEqual(9.0, Axis5.upper());
		}
		TEST_METHOD(axis_grid_index) {
			{
				auto Axis = make_axis(0.0, 1.0, 11);
				Assert::AreEqual(0.1, Axis.interval());
				for(unsigned int i = 0; i<Axis.size(); ++i) {
					Assert::AreEqual<int>(i, Axis.index(0.1*i-0.04));
					Assert::AreEqual<int>(i, Axis.index(0.1*i+0.04));
				}
			}
			{
				auto Axis = make_axis(0.0, 1.0, 11, math::grid_adjuster<math::grid_policy::floor_grid_tag,-8>());
				Assert::AreEqual(0.1, Axis.interval());
				for(unsigned int i = 0; i<Axis.size(); ++i) {
					Assert::AreEqual<int>(i, Axis.index(0.1*i+0.04));
					Assert::AreEqual<int>(i, Axis.index(0.1*i+0.08));
				}
			}
			{
				auto Axis = make_axis(0.0, 1.0, 11, math::grid_policy::ceil_grid);
				Assert::AreEqual(0.1, Axis.interval());
				for(unsigned int i = 0; i<Axis.size(); ++i) {
					Assert::AreEqual<int>(i, Axis.index(0.1*i-0.04));
					Assert::AreEqual<int>(i, Axis.index(0.1*i-0.08));
				}
			}
		}
		TEST_METHOD(axis_flort_index) {
			{
				auto Axis = make_axis(0.0, 1.0, 11);

				Assert::AreEqual(0.0, Axis.float_index(0.00), 1e-5);
				Assert::AreEqual(0.2, Axis.float_index(0.02), 1e-5);
				Assert::AreEqual(4.6, Axis.float_index(0.46), 1e-5);
				Assert::AreEqual(8.3, Axis.float_index(0.83), 1e-5);
			}
		}
		TEST_METHOD(axis_grid_round_range) {
			auto Axis = make_axis(0.0, 1.0, 11);

			Assert::AreEqual(0.1, Axis.interval());

			{
				auto Ans = Axis.weighted_index(0.30-0.04, 0.30+0.04);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(0.8, Ans.weight(),1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.30-0.04, 0.30+0.06);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(1.0, Ans.weight(),1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9 , Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.30-0.04, 0.30+0.26);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3.0, Ans.weight(), 1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(1.0, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5, Ans.at(2).first);
				Assert::AreEqual(1.0, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6, Ans.at(3).first);
				Assert::AreEqual(0.1, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_grid_floor_range) {
			auto Axis = make_axis(0.0, 1.0, 11,math::grid_policy::floor_grid);

			Assert::AreEqual(0.1, Axis.interval());

			{
				auto Ans = Axis.weighted_index(0.30+0.04, 0.30+0.08);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.30+0.01, 0.30+0.11);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(1.0, Ans.weight(), 1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.30+0.01, 0.30+0.31);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3.0, Ans.weight(), 1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(1.0, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5, Ans.at(2).first);
				Assert::AreEqual(1.0, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6, Ans.at(3).first);
				Assert::AreEqual(0.1, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_grid_ceil_range) {
			auto Axis = make_axis(0.0, 1.0, 11, math::grid_policy::ceil_grid);

			Assert::AreEqual(0.1, Axis.interval());

			{
				auto Ans = Axis.weighted_index(0.20+0.04, 0.20+0.08);
				Assert::AreEqual(1u, Ans.size());
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(1.0, Ans.at(0).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.20+0.01, 0.20+0.11);
				Assert::AreEqual(2u, Ans.size());
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(0.1, Ans.at(1).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}

			{
				auto Ans = Axis.weighted_index(0.20+0.01, 0.20+0.31);
				Assert::AreEqual(4u, Ans.size());
				Assert::AreEqual(3.0, Ans.weight(), 1e-5);
				Assert::AreEqual(3, Ans.at(0).first);
				Assert::AreEqual(0.9, Ans.at(0).second, 1e-5);
				Assert::AreEqual(4, Ans.at(1).first);
				Assert::AreEqual(1.0, Ans.at(1).second, 1e-5);
				Assert::AreEqual(5, Ans.at(2).first);
				Assert::AreEqual(1.0, Ans.at(2).second, 1e-5);
				Assert::AreEqual(6, Ans.at(3).first);
				Assert::AreEqual(0.1, Ans.at(3).second, 1e-5);

				auto Itr = Ans.begin();
				auto End = Ans.end();
				Assert::IsFalse(Itr==End);
				Assert::AreEqual(3, (*Itr).first);
				Assert::AreEqual(0.9, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(4, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(5, (*Itr).first);
				Assert::AreEqual(1.0, (*Itr).second, 1e-5);
				++Itr;
				Assert::AreEqual(6, (*Itr).first);
				Assert::AreEqual(0.1, (*Itr).second, 1e-5);
				++Itr;
				Assert::IsTrue(Itr==End);
			}
		}
		TEST_METHOD(axis_mapping_floor_floor) {
			auto Axis1 = make_axis(0.2, 0.89, 4, math::grid_policy::floor_grid);	//[0.2, 0.43, 0.66, 0.89]
			auto Axis2 = make_axis(0.02, 0.74, 7, math::grid_policy::floor_grid);	//[0.02, 0.14, 0.26, 0.38, 0.50, 0.62, 0.74]


			auto Mapper = hmLib::map_axis(Axis1, Axis2);
			//lower & upper
			Assert::AreEqual(0, Mapper.lower());
			Assert::AreEqual(1, Mapper.upper());

			//inside
			Assert::IsTrue(Mapper.inside(0));
			Assert::IsTrue(Mapper.inside(1));
			Assert::IsFalse(Mapper.inside(2));
			Assert::IsFalse(Mapper.inside(3));

			auto WI = Mapper.weighted_index(0);
			Assert::AreEqual(3u, WI.size());
			Assert::AreEqual(1, WI.at(0).first);
			Assert::AreEqual((0.26-0.20)/0.12, WI.at(0).second, 1e-5);
			Assert::AreEqual(2, WI.at(1).first);
			Assert::AreEqual(1.0, WI.at(1).second, 1e-5);
			Assert::AreEqual(3, WI.at(2).first);
			Assert::AreEqual((0.43-0.38)/0.12, WI.at(2).second, 1e-5);
		}
		TEST_METHOD(axis_mapping_round_round) {
			auto Axis1 = make_axis(0.2+0.23/2, 0.89+0.23/2, 4, math::grid_policy::round_grid);	//[0.2, 0.43, 0.66, 0.89]
			auto Axis2 = make_axis(0.02+0.12/2, 0.74+0.12/2, 7, math::grid_policy::round_grid);	//[0.02, 0.14, 0.26, 0.38, 0.50, 0.62, 0.74]


			auto Mapper = hmLib::map_axis(Axis1, Axis2);
			//lower & upper
			Assert::AreEqual(0, Mapper.lower());
			Assert::AreEqual(1, Mapper.upper());

			//inside
			Assert::IsTrue(Mapper.inside(0));
			Assert::IsTrue(Mapper.inside(1));
			Assert::IsFalse(Mapper.inside(2));
			Assert::IsFalse(Mapper.inside(3));

			auto WI = Mapper.weighted_index(0);
			Assert::AreEqual(3u, WI.size());
			Assert::AreEqual(1, WI.at(0).first);
			Assert::AreEqual((0.26-0.20)/0.12, WI.at(0).second, 1e-5);
			Assert::AreEqual(2, WI.at(1).first);
			Assert::AreEqual(1.0, WI.at(1).second, 1e-5);
			Assert::AreEqual(3, WI.at(2).first);
			Assert::AreEqual((0.43-0.38)/0.12, WI.at(2).second, 1e-5);
		}
		TEST_METHOD(axis_mapping_ceil_round) {
			auto Axis1 = make_axis(0.2+0.23, 0.89+0.23, 4, math::grid_policy::ceil_grid);	//[0.2, 0.43, 0.66, 0.89]
			auto Axis2 = make_axis(0.02+0.12/2, 0.74+0.12/2, 7, math::grid_policy::round_grid);	//[0.02, 0.14, 0.26, 0.38, 0.50, 0.62, 0.74]


			auto Mapper = hmLib::map_axis(Axis1, Axis2);
			//lower & upper
			Assert::AreEqual(0, Mapper.lower());
			Assert::AreEqual(1, Mapper.upper());

			//inside
			Assert::IsTrue(Mapper.inside(0));
			Assert::IsTrue(Mapper.inside(1));
			Assert::IsFalse(Mapper.inside(2));
			Assert::IsFalse(Mapper.inside(3));

			auto WI = Mapper.weighted_index(0);
			Assert::AreEqual(3u, WI.size());
			Assert::AreEqual(1, WI.at(0).first);
			Assert::AreEqual((0.26-0.20)/0.12, WI.at(0).second, 1e-5);
			Assert::AreEqual(2, WI.at(1).first);
			Assert::AreEqual(1.0, WI.at(1).second, 1e-5);
			Assert::AreEqual(3, WI.at(2).first);
			Assert::AreEqual((0.43-0.38)/0.12, WI.at(2).second, 1e-5);
		}
		TEST_METHOD(axis_mapping_round_ceil) {
			auto Axis1 = make_axis(0.2+0.23/2, 0.89+0.23/2, 4, math::grid_policy::round_grid);	//[0.2, 0.43, 0.66, 0.89]
			auto Axis2 = make_axis(0.02+0.12, 0.74+0.12, 7, math::grid_policy::ceil_grid);	//[0.02, 0.14, 0.26, 0.38, 0.50, 0.62, 0.74]


			auto Mapper = hmLib::map_axis(Axis1, Axis2);
			//lower & upper
			Assert::AreEqual(0, Mapper.lower());
			Assert::AreEqual(1, Mapper.upper());

			//inside
			Assert::IsTrue(Mapper.inside(0));
			Assert::IsTrue(Mapper.inside(1));
			Assert::IsFalse(Mapper.inside(2));
			Assert::IsFalse(Mapper.inside(3));

			auto WI = Mapper.weighted_index(0);
			Assert::AreEqual(3u, WI.size());
			Assert::AreEqual(1, WI.at(0).first);
			Assert::AreEqual((0.26-0.20)/0.12, WI.at(0).second, 1e-5);
			Assert::AreEqual(2, WI.at(1).first);
			Assert::AreEqual(1.0, WI.at(1).second, 1e-5);
			Assert::AreEqual(3, WI.at(2).first);
			Assert::AreEqual((0.43-0.38)/0.12, WI.at(2).second, 1e-5);
		}
	};
	TEST_CLASS(test_math_multiaxis) {
		TEST_METHOD(construct) {
			multiaxis<double, 3> Axes{
				make_axis<double>(0,1.0,10),
				make_axis<double>(0,1.5,15),
				make_axis<double>(0,2.0,20) 
			};
			auto Lower = Axes.lower();
			auto Upper = Axes.upper();
			auto Extent = Axes.extent();

			Assert::AreEqual(0.0, Lower[0], 1e-10);
			Assert::AreEqual(0.0, Lower[1], 1e-10);
			Assert::AreEqual(0.0, Lower[2], 1e-10);

			Assert::AreEqual(1.0, Upper[0], 1e-10);
			Assert::AreEqual(1.5, Upper[1], 1e-10);
			Assert::AreEqual(2.0, Upper[2], 1e-10);

			Assert::AreEqual(10u, Extent[0]);
			Assert::AreEqual(15u, Extent[1]);
			Assert::AreEqual(20u, Extent[2]);
		}
		TEST_METHOD(make) {
			multiaxis<double, 3> Axes = make_multiaxis(
				varray<double, 3>{0.0,0.0,0.0},
				varray<double, 3>{1.0,1.5,2.0},
				varray<std::size_t, 3>{10,15,20}
			);

			auto Lower = Axes.lower();
			auto Upper = Axes.upper();
			auto Extent = Axes.extent();

			Assert::AreEqual(0.0, Lower[0], 1e-10);
			Assert::AreEqual(0.0, Lower[1], 1e-10);
			Assert::AreEqual(0.0, Lower[2], 1e-10);

			Assert::AreEqual(1.0, Upper[0], 1e-10);
			Assert::AreEqual(1.5, Upper[1], 1e-10);
			Assert::AreEqual(2.0, Upper[2], 1e-10);

			Assert::AreEqual(10u, Extent[0]);
			Assert::AreEqual(15u, Extent[1]);
			Assert::AreEqual(20u, Extent[2]);
		}
		TEST_METHOD(bind) {
			multiaxis<double, 3> Axes = bind_axis(
				make_axis<double>(0, 1.0, 10),
				make_axis<double>(0, 1.5, 15),
				make_axis<double>(0, 2.0, 20)
			);

			auto Lower = Axes.lower();
			auto Upper = Axes.upper();
			auto Extent = Axes.extent();

			Assert::AreEqual(0.0, Lower[0], 1e-10);
			Assert::AreEqual(0.0, Lower[1], 1e-10);
			Assert::AreEqual(0.0, Lower[2], 1e-10);

			Assert::AreEqual(1.0, Upper[0], 1e-10);
			Assert::AreEqual(1.5, Upper[1], 1e-10);
			Assert::AreEqual(2.0, Upper[2], 1e-10);

			Assert::AreEqual(10u, Extent[0]);
			Assert::AreEqual(15u, Extent[1]);
			Assert::AreEqual(20u, Extent[2]);
		}
		TEST_METHOD(make_assign_axis_each) {
			multiaxis<double, 3> Axes;

			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(0, make_axis<double>(0, 1, 10));
			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(1, make_axis<double>(0, 1.5, 15));
			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(2, make_axis<double>(0, 2, 20));
			Assert::IsFalse(Axes.empty());

			auto Lower = Axes.lower();
			auto Upper = Axes.upper();
			auto Extent = Axes.extent();

			Assert::AreEqual(0.0, Lower[0], 1e-10);
			Assert::AreEqual(0.0, Lower[1], 1e-10);
			Assert::AreEqual(0.0, Lower[2], 1e-10);

			Assert::AreEqual(1.0, Upper[0], 1e-10);
			Assert::AreEqual(1.5, Upper[1], 1e-10);
			Assert::AreEqual(2.0, Upper[2], 1e-10);

			Assert::AreEqual(10u, Extent[0]);
			Assert::AreEqual(15u, Extent[1]);
			Assert::AreEqual(20u, Extent[2]);
		}
		TEST_METHOD(make_assign_axis_emplace) {
			multiaxis<double, 3> Axes;

			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(0, 0, 1, 10);
			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(1, 0, 1.5, 15);
			Assert::IsTrue(Axes.empty());
			Axes.assign_axis(2, 0, 2, 20);
			Assert::IsFalse(Axes.empty());

			auto Lower = Axes.lower();
			auto Upper = Axes.upper();
			auto Extent = Axes.extent();

			Assert::AreEqual(0.0, Lower[0], 1e-10);
			Assert::AreEqual(0.0, Lower[1], 1e-10);
			Assert::AreEqual(0.0, Lower[2], 1e-10);

			Assert::AreEqual(1.0, Upper[0], 1e-10);
			Assert::AreEqual(1.5, Upper[1], 1e-10);
			Assert::AreEqual(2.0, Upper[2], 1e-10);

			Assert::AreEqual(10u, Extent[0]);
			Assert::AreEqual(15u, Extent[1]);
			Assert::AreEqual(20u, Extent[2]);
		}
		TEST_METHOD(multiaxis_at) {
			multiaxis<double, 3> Axes{
				make_axis<double>(0,1.0,11),
				make_axis<double>(0,1.5,16),
				make_axis<double>(0,2.0,21)
			};
			using value_point_type = typename decltype(Axes)::value_point_type;

			for(unsigned int x = 0; x<11; ++x) {
				for(unsigned int y = 0; y<16; ++y) {
					for(unsigned int z = 0; z<21; ++z) {
						auto p = Axes.at(x, y, z);
						Assert::AreEqual(p[0], x*0.1, 1e-10);
						Assert::AreEqual(p[1], y*0.1, 1e-10);
						Assert::AreEqual(p[2], z*0.1, 1e-10);
					}
				}
			}
		}
		TEST_METHOD(multiaxis_ref) {
			multiaxis<double, 3> Axes{
				make_axis<double>(0,1.0,11),
				make_axis<double>(0,1.5,16),
				make_axis<double>(0,2.0,21)
			};
			using value_point_type = typename decltype(Axes)::value_point_type;

			for(unsigned int x = 0; x<11; ++x) {
				for(unsigned int y = 0; y<16; ++y) {
					for(unsigned int z = 0; z<21; ++z) {
						auto p = Axes.ref(x, y, z);
						Assert::AreEqual(p[0], x*0.1, 1e-10);
						Assert::AreEqual(p[1], y*0.1, 1e-10);
						Assert::AreEqual(p[2], z*0.1, 1e-10);
					}
				}
			}
		}
		TEST_METHOD(iterator_access) {
			multiaxis<double, 3> Axes{
				make_axis<double>(0,1.0,11),
				make_axis<double>(0,1.5,16),
				make_axis<double>(0,2.0,21)
			};

			auto Itr = Axes.begin();
			auto End = Axes.end();

			for(unsigned int z = 0; z<21; ++z) {
				for(unsigned int y = 0; y<16; ++y) {
					for(unsigned int x = 0; x<11; ++x) {
						Assert::IsFalse(Itr==End);

						auto p = *Itr;
						Assert::AreEqual(p[0], x*0.1, 1e-10);
						Assert::AreEqual(p[1], y*0.1, 1e-10);
						Assert::AreEqual(p[2], z*0.1, 1e-10);

						++Itr;
					}
				}
			}
			Assert::IsTrue(Itr==End);
		}
		TEST_METHOD(locator_access) {
			multiaxis<double, 3> Axes{
				make_axis<double>(0,0.5,6),
				make_axis<double>(0,0.7,8),
				make_axis<double>(0,0.9,10)
			};

			for(unsigned int x = 1; x<5; ++x) {
				for(unsigned int y = 1; y<7; ++y) {
					for(unsigned int z = 1; z<9; ++z) {
						auto ltr = Axes.locate(x, y, z);

						auto q = *ltr;
						Assert::AreEqual(q[0], x*0.1, 1e-10);
						Assert::AreEqual(q[1], y*0.1, 1e-10);
						Assert::AreEqual(q[2], z*0.1, 1e-10);

						for(int dx = -1; dx<=1; ++dx) {
							for(int dy = -1; dy<=1; ++dy) {
								for(int dz = -1; dz<=1; ++dz) {
									auto p = ltr.ref(dx,dy,dz);
									Assert::AreEqual(p[0], (x+dx)*0.1, 1e-10);
									Assert::AreEqual(p[1], (y+dy)*0.1, 1e-10);
									Assert::AreEqual(p[2], (z+dz)*0.1, 1e-10);
								}
							}
						}
					}
				}
			}
		}
		TEST_METHOD(weighted_iterator) {
			multiaxis<double, 2> Axes{
				make_axis<double>(0,1.0,11),
				make_axis<double>(0,2.0,21)
			};

			auto Weighted = Axes.weighted_point(
				varray<double, 2>{0.06, 1.07}, varray<double, 2>{0.58, 1.33}
			);
			//[1:6,11:13]

			Assert::IsFalse(Weighted.empty());
			Assert::AreEqual(6u*3u, Weighted.size());
			Assert::AreEqual(5.2*2.6, Weighted.weight(), 1e-10);

			auto Itr = Weighted.begin();
			auto End = Weighted.end();

			for(int x = 1; x<=6; ++x) {
				Assert::IsFalse(Itr==End);
				auto wp = *Itr;
				Assert::AreEqual(x, wp.first[0]);
				Assert::AreEqual(11, wp.first[1]);

				double wx = 1.0;
				if(x==1) {
					wx = 0.9;
				} else if(x==6) {
					wx = 0.3;
				}
				Assert::AreEqual(wx*0.8, wp.second, 1e-10);
				++Itr;
			}

			for(int x = 1; x<=6; ++x) {
				Assert::IsFalse(Itr==End);
				auto wp = *Itr;
				Assert::AreEqual(x, wp.first[0]);
				Assert::AreEqual(12, wp.first[1]);

				double wx = 1.0;
				if(x==1) {
					wx = 0.9;
				} else if(x==6) {
					wx = 0.3;
				}

				Assert::AreEqual(wx*1.0, wp.second, 1e-10);
				++Itr;
			}

			for(int x = 1; x<=6; ++x) {
				Assert::IsFalse(Itr==End);
				auto wp = *Itr;
				Assert::AreEqual(x, wp.first[0]);
				Assert::AreEqual(13, wp.first[1]);

				double wx = 1.0;
				if(x==1) {
					wx = 0.9;
				} else if(x==6) {
					wx = 0.3;
				}
				Assert::AreEqual(wx*0.8, wp.second, 1e-10);
				++Itr;
			}
		}
	};
}
