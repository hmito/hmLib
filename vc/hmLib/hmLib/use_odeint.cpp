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
#include<boost/numeric/odeint/algebra/range_algebra.hpp>
#include<boost/numeric/odeint/algebra/default_operations.hpp>
#include"../../../odeint/state_with_appendix.hpp"

struct circle_dysys {
	using base_state = hmLib::varray<double, 2>;
	using state = hmLib::odeint::state_with_appendix<base_state, int>;
	using result = hmLib::odeint::validate_result;
	void operator()(const state& st, state& stdx, double t)const {
		const base_state& x = st.x;
		base_state& dx = stdx.x;
		double r2 = (x[0] - 1.0) * (x[0] - 1.0) + (x[1] - 0.8) * (x[1] - 0.8);

		dx[0] = x[1] - 0.8;
		dx[1] = -(x[0] - 1.0);

		if (x[0] <= 1e-20)dx[0] = 0.0;
		if (x[1] <= 1e-20)dx[1] = 0.0;
	}
	bool is_invalid_step(const state& st, double t)const {
		const base_state& x = st.x;

		if (x[0] < 0 || x[1] < 0)return true;
		if (x[0] <= 1e-20 && x[1] - 0.8 > 0)return true;
		if (x[1] <= 1e-20 && -(x[0] - 1.0) > 0)return true;
		return false;
	}
	result validate(const state& st1, const state& st2, double t, state& nst)const {
		const base_state& x1 = st1.x;
		const base_state& x2 = st2.x;
		base_state& nx = nst.x;

		nx = x2;
		nx[0] = std::max(0.0, nx[0]);
		nx[1] = std::max(0.0, nx[1]);

		if (x2[0] <= 1e-20 && x2[1] - 0.8 > 0)nx[0] = 1.1e-20;
		if (x2[1] <= 1e-20 && -(x2[0] - 1.0) > 0)nx[1] = 1.1e-20;

		return result::assigned;
	}
	result validate(const state& st, double t, state& nst)const {
		const base_state& x = st.x;
		base_state& nx = nst.x;
		
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
	using base_state = typename dysys::base_state;
	using state = typename dysys::state;

	dysys Dysys;
	state State({ 1.0,1.7 },0);
//	auto Stepper = hmLib::odeint::make_step_validate(0.001, 100, boost::numeric::odeint::runge_kutta_dopri5<state>());
//	auto Stepper = hmLib::odeint::make_step_validate(0.001, 100, boost::numeric::odeint::make_controlled(0.01, 0.01, boost::numeric::odeint::runge_kutta_dopri5<state>()));
	auto Stepper = hmLib::odeint::make_step_validate(0.001, 100, boost::numeric::odeint::make_dense_output(0.01, 0.01, boost::numeric::odeint::runge_kutta_dopri5<state>()));
//	auto Stepper = boost::numeric::odeint::make_dense_output(0.01, 0.01, boost::numeric::odeint::runge_kutta_dopri5<state>());
	std::ofstream fout("test.csv");
	hmLib::odeint::stream_observer obs(fout);

	hmLib::odeint::integrate_const(Stepper, Dysys, State, 0.0, 10.0, 0.01, obs);

	return 0;
}
