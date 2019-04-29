#include<array>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<fstream>
#include<boost/numeric/odeint.hpp>
#include"../../../odeint/varray_wrapper.hpp"
#include"../../../odeint.hpp"
#include"../../../varray.hpp"

struct circle_dysys {
	using state = hmLib::varray<double, 2>;
	using result = hmLib::odeint::validate_result;
	void operator()(const state& x, state& dx, double t)const {
		double r2 = (x[0] - 1.0) * (x[0] - 1.0) + (x[1] - 0.8) * (x[1] - 0.8);
		dx[0] = x[1] - 0.8;
		dx[1] = -(x[0] - 1.0);
	}
	bool is_invalid_step(const state& x, double t)const {
		if (x[0] < 0 || x[1] < 0)return true;
		return false;
	}
	result validate(const state& x1, const state& x2, double t, state& nx)const {
		nx = x2;
		nx[0] = std::max(0.0, nx[0]);
		nx[1] = std::max(0.0, nx[1]);

		return result::assigned;
	}
	result validate(const state& x, double t, state& nx)const {
		result res = result::none;
		if (x[0] < 0 || x[1] < 0) {
			nx = x;
			nx[0] = std::max(0.0, nx[0]);
			nx[1] = std::max(0.0, nx[1]);
			res = result::assigned;
		}
		return res;
	}
};
int main(void) {
	using dysys = circle_dysys;
	using state = typename dysys::state;

	dysys Dysys;
	state State{ 1.0,1.7 };
	auto Stepper = hmLib::odeint::make_step_validate(0.001, 100, boost::numeric::odeint::make_dense_output(0.01, 0.01, boost::numeric::odeint::runge_kutta_dopri5<state>()));
	std::ofstream fout("test.csv");
	hmLib::odeint::stream_observer obs(fout);

	obs(State, -1.0);
	obs(state{0.5,0.5}, -0.5);
	obs(State, 0.0);
	hmLib::odeint::integrate_adaptive(Stepper, Dysys, State, 0.0, 10.0, 0.01, obs);

	return 0;
}