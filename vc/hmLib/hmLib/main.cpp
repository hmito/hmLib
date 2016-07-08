#include <array>
#include <cmath>
#include <hmLib/config_vc.h>
#include <hmLib/odeint.hpp>
#include <hmLib/odeint/breakable_integrate.hpp>
#include <hmLib/odeint/container_observer.hpp>
#include <hmLib/odeint/stream_observer.hpp>
#include <hmLib/odeint/iterator_observer.hpp>
#define TEST_METHOD(Name) struct Name{};
namespace hmLib{
	struct test_system{
		using state = std::array<double, 2>;
		void operator()(const state& x, state& dxdt, double t){
			dxdt[0] = std::cos(x[0]);
			dxdt[1] = -std::sin(x[1]);
		}
	};
}
int main(){
	using namespace hmLib;
	namespace bodeint = boost::numeric::odeint;
	TEST_METHOD(create){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::container_observer<test_system::state> Observer;

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, std::ref(Observer));
	}
	TEST_METHOD(stream_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		odeint::stream_observer Observer(std::cout);

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(s_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(st_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<test_system::state> Log;
		std::vector<double> TLog;
		auto Observer = odeint::make_iterator_observer(std::back_inserter(Log), std::back_inserter(TLog));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	TEST_METHOD(pair_iterator_observer){
		bodeint::euler<test_system::state> Stepper;
		test_system System;
		test_system::state State{0.0,1.0};

		std::vector<std::pair<double, test_system::state> > Log;
		auto Observer = odeint::make_pair_iterator_observer(std::back_inserter(Log));

		bodeint::integrate_adaptive(Stepper, System, State, 0.0, 10.0, 0.1, Observer);
	}
	//"/

	system("pause");

	return 0;
}