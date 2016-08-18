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
#include<hmLib/odeint/range_stepper.hpp>
#define TEST_METHOD(Name) struct Name{};
namespace hmLib{
	struct test_system{
		static constexpr double region_error(){ return 0.00001; }
		using state = std::array<double, 2>;
		void operator()(const state& x, state& dxdt, double t){
			operator()(x, dxdt, t, region(x,t));
		}
		void operator()(const state& x, state& dxdt, double t, int r){
			double base = std::sqrt(x[0] * x[0] + x[1] * x[1]);
			dxdt[0] = -x[1];
			dxdt[1] = x[0];

			if(r == 2 && dxdt[0] > 0)dxdt[0] = 0.0;
			else if(r == 3 && dxdt[1] < 0)dxdt[1] = 0.0;
			else if(r == 4){
				if(dxdt[0] > 0)dxdt[0] = 0.0;
				if(dxdt[1] < 0)dxdt[1] = 0.0;
			}
		}
		int region(const state& x, double t){
			if(x[0] >= 0.5 - region_error()){
				if(x[1] <= -0.75 - region_error())return 4;
				return 2;
			}
			if(x[1] <= -0.75 - region_error())return 3;

			return 1;
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

		auto Observer = odeint::make_break_observer([](const test_system::state& State,double t)->bool{return State[0] > 1.0; });

		odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(break_observer_with_container_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		auto Observer = odeint::make_break_observer([](const test_system::state& State, double t)->bool{return State[0] > 1.0; }, odeint::container_observer<test_system::state>());

		odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, std::ref(Observer));
	}
	TEST_METHOD(eqstate_break_observer){
		bodeint::runge_kutta_dopri5<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::eqstate_break_observer<test_system::state> Observer(odeint::detail::eqstate_breaker<test_system::state,double>(1));

		//odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));
		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));

		std::ofstream fout("result2.csv");
		for(auto val : Observer){
			fout << val.first << "," << val.second[0] << "," << val.second[1] << std::endl;
		}
	}
	TEST_METHOD(range_stepper){
		auto BaseStepper = bodeint::make_dense_output(1.0e-10, 1.0e-6, bodeint::runge_kutta_dopri5<test_system::state>());
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::region_abridged_stepper<decltype(BaseStepper)> Stepper(System.region_error(), std::move(BaseStepper));

//		odeint::container_observer<test_system::state> Observer;

		//odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));

		std::ofstream fout("result3b.csv");
		Stepper.initialize(State, 0.0, 0.001);
		for(unsigned int i = 0; i < 100; ++i){
			auto Ans=Stepper.do_step(System);
			fout << Ans.first << "," << Ans.second << "," << Stepper.current_time() << "," << Stepper.current_time_step() << "," << Stepper.current_state()[0] << "," << Stepper.current_state()[1] << "," << Stepper.current_region() << std::endl;
		}

	}
	TEST_METHOD(range_stepper_integrate){
		auto BaseStepper = bodeint::make_dense_output(1.0e-10, 1.0e-6, bodeint::runge_kutta_dopri5<test_system::state>());
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::region_abridged_stepper<decltype(BaseStepper)> Stepper(System.region_error(), std::move(BaseStepper));
		Stepper.initialize(State, 0.0, 0.001);

		//odeint::eqstate_break_observer<test_system::state> Observer(0.001, 1);
		odeint::container_observer<test_system::state> Observer;

		//odeint::breakable_integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));
		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 100.0, 0.1, std::ref(Observer));

		std::ofstream fout("result3.csv");
		for(auto val : Observer){
			fout << val.first << "," << val.second[0] << "," << val.second[1] << std::endl;
		}
	}
	system("pause");

	return 0;
}
