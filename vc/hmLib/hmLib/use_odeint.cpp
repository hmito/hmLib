#include<array>
#include<iostream>
#include<vector>
#include"../../../odeint/varray_wrapper.hpp"
#include<boost/numeric/odeint.hpp>
#include"../../../odeint.hpp"
#include"../../../odeint/composite.hpp"
#include"../../../varray.hpp"

std::ostream& operator<<(std::ostream& out, const hmLib::varray<double, 2>& v) {
	out<<"("<<v[0]<<", "<<v[1]<<")";
	return out;
}
struct my_system1 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = 1;
		dx[1] = 1;
	}
};
struct my_system2 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = 1;
		dx[1] = -1;
	}
};
struct my_system3 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = -1;
		dx[1] = 0;
	}
};
int main() {
	using state = hmLib::varray<double, 2>;
	namespace bode = boost::numeric::odeint;
	namespace hode = hmLib::odeint;
	auto CmpSys = hode::system_switch(
		hode::state_at<state>(0,hode::case_greater_equal(0){}
		Sys
	);
	using base_stepper_type = boost::numeric::odeint::runge_kutta_dopri5<state>;
	auto Stepper = hmLib::odeint::make_composite_dense_output(1.0e-10, 1.0e-6, 1.0e-3, base_stepper_type());

	hmLib::odeint::stream_observer Observer(std::cout,"\t");

	CmpSys.update(x,0.0);
	Stepper.initialize(x, 0, 0.01);
	bodeint::integrate_const(Stepper, CmpSys, x, 0.0, 10.0, 0.1, Observer);

	system("pause");
	return 0;
}
