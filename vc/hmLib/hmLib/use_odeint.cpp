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
struct my_system {
	using state = std::vector<hmLib::varray<double, 2>>;
	void operator()(const state& xseq, state& dxseq, double t) {
		auto itr = xseq.begin();
		auto ditr = dxseq.begin();
		for(; itr!=xseq.end(); ++itr, ++ditr) {
			(*ditr)[0] = 1.0 - 0.2*t;
			(*ditr)[1] = 0.2*t-(*itr)[0];
		}
	}
};

int main() {
	namespace bodeint = boost::numeric::odeint;
	my_system Sys;
	using state = typename my_system::state;
	state x;
	x.push_back(hmLib::varray<double, 2>{ 1.0, 1.0 });
	x.push_back(hmLib::varray<double, 2>{ 0.5, 1.5 });


	auto CmpSys = hmLib::odeint::system_compose(
		Sys, hmLib::odeint::state_for_each<state>(x.size(), hmLib::odeint::state_for_each<hmLib::varray<double,2>>(2, hmLib::odeint::require_greater_equal(0.0)))
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
