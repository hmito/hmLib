#include <hmLib/config_vc.h>
#include <array>
#include <cmath>
#include <fstream>
#include <boost/numeric/odeint.hpp>
#include <hmLib/odeint.hpp>
#include <hmLib/odeint/breakable_integrate.hpp>
#include <hmLib/odeint/container_observer.hpp>
#include <hmLib/odeint/stream_observer.hpp>
#include <hmLib/odeint/iterator_observer.hpp>
#include <hmLib/odeint/break_observer.hpp>
#include <hmLib/odeint/eqstate_break_observer.hpp>
#include <hmLib/odeint/breakable_integrate.hpp>
#define TEST_METHOD(Name) struct Name{};
namespace hmLib{
	struct test_system{
		using state = std::array<double, 2>;
		void operator()(const state& x, state& dxdt, double t){
			double base = std::sqrt(x[0] * x[0] + x[1] * x[1]);
			dxdt[0] = -x[1];
			dxdt[1] = x[0];
		}
	};
}
int main(){
	using namespace hmLib;
	namespace bodeint = boost::numeric::odeint;
	TEST_METHOD(create){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::container_observer<test_system::state> Observer;

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));

		std::ofstream fout("result1.csv");
		for(auto val : Observer){
			fout << val.first << "," << val.second[0] << "," << val.second[1] << std::endl;
		}
	}
	TEST_METHOD(stream_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::stream_observer Observer(std::cout);

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(s_iterator_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(st_iterator_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		std::vector<double> TLog;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log), std::back_inserter(TLog));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(pair_iterator_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<std::pair<double, test_system::state> > Log;
		auto Observer = odeint::make_pair_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(break_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		auto Observer = odeint::make_break_observer([](const test_system::state& State)->bool{return State[0] > 1.0; });

		odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(break_observer_with_container_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		auto Observer = odeint::make_break_observer([](const test_system::state& State)->bool{return State[0] > 1.0; }, odeint::container_observer<test_system::state>());

		odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, std::ref(Observer));
	}
	TEST_METHOD(eqstate_break_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::eqstate_break_observer<test_system::state> Observer(0.001,1);

		//odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));
		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));

		std::ofstream fout("result2.csv");
		for(auto val : Observer){
			fout << val.first << "," << val.second[0] << "," << val.second[1] << std::endl;
		}
	}
	system("pause");

	return 0;
}