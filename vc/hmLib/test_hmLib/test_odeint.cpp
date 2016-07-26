#include "stdafx.h"
#include "CppUnitTest.h"
#include <array>
#include <cmath>
#include <hmLib/odeint.hpp>
#include <hmLib/odeint/breakable_integrate.hpp>
#include <hmLib/odeint/container_observer.hpp>
#include <hmLib/odeint/stream_observer.hpp>
#include <hmLib/odeint/iterator_observer.hpp>
#include <hmLib/odeint/break_observer.hpp>
#include <hmLib/odeint/eqstate_break_observer.hpp>
#include <hmLib/odeint/range_stepper.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	namespace bodeint = boost::numeric::odeint;
	TEST_CLASS(test_odeint_observer){
		struct test_system{
			using state = std::array<double, 2>;
			void operator()(const state& x, state& dxdt, double t){
				operator()(x, dxdt, t, region(x, t));
			}
			void operator()(const state& x, state& dxdt, double t, unsigned int r){
				dxdt[0] = std::sin(x[0]);
				dxdt[1] = std::cos(x[1]);
			}
			unsigned int region(const state&, double){ return 1; }
		};
public:
	TEST_METHOD(container_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::container_observer<test_system::state> Observer;

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
	}
	TEST_METHOD(stream_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::stream_observer Observer(std::cout);

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
	}
	TEST_METHOD(s_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
	}
	TEST_METHOD(st_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		std::vector<double> TLog;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log), std::back_inserter(TLog));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
	}
	TEST_METHOD(pair_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<std::pair<double, test_system::state> > Log;
		auto Observer = odeint::make_pair_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
	}
	TEST_METHOD(break_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<std::pair<double, test_system::state> > Log;
		auto Observer = odeint::make_break_observer([](const test_system::state& State, double)->bool{return State[0] > 1.0; });

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(range_stepper){
		auto Stepper = bodeint::make_dense_output(1.0e-10, 1.0e-6, bodeint::runge_kutta_dopri5<test_system::state>());
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::region_abridged_stepper<decltype(Stepper)> RStepper(0.001, std::move(Stepper));

		RStepper.do_step(System);
	}
	};
	TEST_CLASS(test_segment_cross){
	private:
		bool is_cross_segment(std::array<double, 2> a1, std::array<double, 2> a2, std::array<double, 2> b1, std::array<double, 2> b2){
			//x座標によるチェック
			if(a1[0] >= a2[0]){
				if((a1[0] < b1[0] && a1[0] < b2[0]) || (a2[0] > b1[0] && a2[0] > b2[0])){
					return false;
				}
			}else{
				if((a2[0] < b1[0] && a2[0] < b2[0]) || (a1[0] > b1[0] && a1[0] > b2[0])){
					return false;
				}
			}
			//y座標によるチェック
			if(a1[1] >= a2[1]){
				if((a1[1] < b1[1] && a1[1] < b2[1]) || (a2[1] > b1[1] && a2[1] > b2[1])){
					return false;
				}
			} else{
				if((a2[1] < b1[1] && a2[1] < b2[1]) || (a1[1] > b1[1] && a1[1] > b2[1])){
					return false;
				}
			}

			if(((a1[0] - a2[0]) * (b1[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b1[0])) *
				((a1[0] - a2[0]) * (b2[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b2[0])) > 0){
				return false;
			}
			if(((b1[0] - b2[0]) * (a1[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a1[0])) *
				((b1[0] - b2[0]) * (a2[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a2[0])) > 0){
				return false;
			}
			return true;
		}
	public:
		TEST_METHOD(same_line_x_cross){
			std::array<double, 2> a1{0.0, 0.0};
			std::array<double, 2> a2{1.0, 0.0};

			std::array<double, 2> b1{0.0, 0.0};
			std::array<double, 2> b2{1.0, 0.0};

			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");

			b1[0] = 1.0;
			b2[0] = 0.0;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b1[0] = 2.0;
			b2[0] = 0.9;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b1[0] = 2.0;
			b2[0] = 1.1;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");
		}
		TEST_METHOD(same_line_y_cross){
			std::array<double, 2> a1{0.0, 0.0};
			std::array<double, 2> a2{0.0, 1.0};

			std::array<double, 2> b1{0.0, 0.0};
			std::array<double, 2> b2{0.0, 1.0};

			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");

			b1[1] = 1.0;
			b2[1] = 0.0;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b1[1] = 2.0;
			b2[1] = 0.9;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b1[1] = 2.0;
			b2[1] = 1.1;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");
		}
		TEST_METHOD(same_line_cross){
			std::array<double, 2> a1{0.0, 0.0};
			std::array<double, 2> a2{1.0, 1.0};

			std::array<double, 2> b1{0.5, 0.5};
			std::array<double, 2> b2{2.0, 2.0};

			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");

			b1[0] = 0.9;
			b1[1] = 0.9;

			b1[0] = 1.1;
			b1[1] = 1.1;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");
		}
		TEST_METHOD(cross_check1){
			std::array<double, 2> a1{0.0, 0.0};
			std::array<double, 2> a2{1.0, 1.0};

			std::array<double, 2> b1{2.0, 0.0};
			std::array<double, 2> b2{1.5, 0.5};
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");

			b2[0] = 1.0;
			b2[1] = 0.8;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b2[0] = 0.7;
			b2[1] = 0.65;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b2[0] = 0.6;
			b2[1] = 0.65;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");
		}
		TEST_METHOD(cross_check2){
			std::array<double, 2> a1{0.0, 0.0};
			std::array<double, 2> a2{1.0, 1.0};

			std::array<double, 2> b1{2.0, 2.1};
			std::array<double, 2> b2{1.0, 1.1};
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");

			b2[0] = 0.8;
			b2[1] = 0.82;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b2[0] = -0.7;
			b2[1] = -0.695;
			Assert::IsFalse(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsFalse(is_cross_segment(b1, b2, a1, a2), L"Same line");

			b2[0] = 0.6;
			b2[1] = 0.59;
			Assert::IsTrue(is_cross_segment(a1, a2, b1, b2), L"Same line");
			Assert::IsTrue(is_cross_segment(b1, b2, a1, a2), L"Same line");
		}
	};
}
