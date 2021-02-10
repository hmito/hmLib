#include "stdafx.h"
#include "CppUnitTest.h"
#include <array>
#include <cmath>
#include <boost/numeric/odeint.hpp>
#include "../../../varray.hpp"
#include "../../../odeint.hpp"
#include "../../../odeint/observer/null_observer.hpp"
#include "../../../odeint/observer/stream_observer.hpp"
#include "../../../odeint/observer/iterator_observer.hpp"
#include "../../../odeint/observer/histgoram_observer.hpp"
#include "../../../odeint/observer/observer_pack.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	namespace boost_odeint = boost::numeric::odeint;
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
		TEST_METHOD(stream_observer){
			boost_odeint::euler<test_system::state> Stepper;
			test_system System;
			test_system::state State{0.0,1.0};

			odeint::stream_observer Observer(std::cout);

			boost_odeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
		}
		TEST_METHOD(s_iterator_observer){
			boost_odeint::euler<test_system::state> Stepper;
			test_system System;
			test_system::state State{0.0,1.0};

			std::vector<test_system::state> Log;
			auto Observer = odeint::make_iterator_observer(std::back_inserter(Log));

			boost_odeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
		}
		TEST_METHOD(st_iterator_observer){
			boost_odeint::euler<test_system::state> Stepper;
			test_system System;
			test_system::state State{0.0,1.0};

			std::vector<test_system::state> Log;
			std::vector<double> TLog;
			auto Observer = odeint::make_iterator_observer(std::back_inserter(Log), std::back_inserter(TLog));

			boost_odeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
		}
		TEST_METHOD(pair_iterator_observer){
			boost_odeint::euler<test_system::state> Stepper;
			test_system System;
			test_system::state State{0.0,1.0};

			std::vector<std::pair<double, test_system::state> > Log;
			auto Observer = odeint::make_pair_iterator_observer(std::back_inserter(Log));

			boost_odeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1);
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
	TEST_CLASS(test_odeint_breakable) {
	private:
		using state = varray<double, 2>;
		struct dynamics {
			void operator()(const state& x, state& dx, double t)const {
				dx[0] = -0.5 * x[0] + 0.9 * x[1]+0.2;
				dx[1] = -0.6 * x[1] + 0.7 * x[0]+0.2;
			}
		};
		struct breaker {
			bool operator()(const state& x, double t)const {
				return x[0] < 0 || x[1] > 0.1;
			}
		};
		struct observer {
			void operator()(const state& x, double t)const {}
		};
	public:
		TEST_METHOD(test_integrate_st) {
			dynamics Dyno;
			observer Observer;
			boost_odeint::runge_kutta4<state> Stepper;
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
		}
		TEST_METHOD(test_breakable_integrate_st) {
			state State{ 0.0,0.0 };
			dynamics Dyno;
			breaker Breaker;
			observer Observer;
			boost_odeint::runge_kutta4<state> Stepper;

			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
		}
		TEST_METHOD(test_integrate_ctrst) {
			dynamics Dyno;
			observer Observer;
			auto Stepper = boost_odeint::make_controlled(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
		}
		TEST_METHOD(test_breakable_integrate_ctrst) {
			dynamics Dyno;
			breaker Breaker;
			observer Observer;
			auto Stepper = boost_odeint::make_controlled(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
		}
		TEST_METHOD(test_integrate_adpst) {
			state State{ 0.0,0.0 };
			dynamics Dyno;
			observer Observer;
			auto Stepper = hmLib::odeint::make_adaptive(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());

			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
		}
		TEST_METHOD(test_breakable_integrate_adpst) {
			state State{ 0.0,0.0 };
			dynamics Dyno;
			breaker Breaker;
			observer Observer;
			auto Stepper = hmLib::odeint::make_adaptive(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());

			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
		}
		TEST_METHOD(test_integrate_dnsst) {
			state State{ 0.0,0.0 };
			dynamics Dyno;
			observer Observer;
			auto Stepper = boost_odeint::make_dense_output(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());

			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.01, Observer);
				Assert::AreEqual(0.238, State[0], 0.001);
				Assert::AreEqual(0.216, State[1], 0.001);
			}
		}
		TEST_METHOD(test_breakable_integrate_dnsst) {
			state State{ 0.0,0.0 };
			dynamics Dyno;
			breaker Breaker;
			observer Observer;
			auto Stepper = boost_odeint::make_dense_output(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state>());

			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_const(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
			{
				state State{ 0.0,0.0 };
				hmLib::odeint::breakable_integrate_adaptive(Stepper, Dyno, State, 0.0, 1.0, 0.001, Breaker, Observer);
				Assert::AreEqual(0.110, State[0], 0.02);
				Assert::AreEqual(0.105, State[1], 0.01);
			}
		}
	};
	/*
	TEST_CLASS(test_odeint_adaptive) {

	}
	TEST_CLASS(test_odeint_validate) {
		using state_t = varray<double, 2>;
		struct system_t {
			void operator()(const state_t& x, state_t& dx, double t) {
				dx[0] = -0.5*x[0]+0.8*x[1];
				dx[1] = -0.4*x[1]+0.9*x[0];
			}
			hmLib::odeint::validate_result validate(const state_t& x, double t, state_t& newx) {
				return hmLib::odeint::validate_result::none;
			}
		};
	public:
		TEST_METHOD(try_interfere_integrate_adaptive_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::runge_kutta_dopri5<state_t>();
			odeint::interfere_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
		TEST_METHOD(try_interfere_integrate_adaptive_controlled_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::make_controlled(1e-3,1e-3, boost_odeint::runge_kutta_dopri5<state_t>());
			odeint::interfere_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
		TEST_METHOD(try_interfere_integrate_adaptive_dense_output_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::make_dense_output(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state_t>());
			odeint::interfere_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
		TEST_METHOD(try_interfere_integrate_const_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::runge_kutta_dopri5<state_t>();
			odeint::interfere_integrate_const(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
		TEST_METHOD(try_interfere_integrate_const_controlled_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::make_controlled(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state_t>());
			odeint::interfere_integrate_const(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
		TEST_METHOD(try_interfere_integrate_const_dense_output_stepper) {
			auto State = state_t{ 0.5, 0.5 };
			auto System = system_t();
			auto Stepper = boost_odeint::make_dense_output(1e-3, 1e-3, boost_odeint::runge_kutta_dopri5<state_t>());
			odeint::interfere_integrate_const(Stepper, System, State, 0.0, 10.0, 0.1, 1e-4);
		}
	};
	*/
}
