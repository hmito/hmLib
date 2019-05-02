#include<array>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<bitset>
#include<fstream>
#include<boost/numeric/odeint.hpp>
#include"../../../odeint/varray_wrapper.hpp"
#include"../../../odeint.hpp"
#include"../../../varray.hpp"
#include<boost/numeric/odeint/algebra/range_algebra.hpp>
#include<boost/numeric/odeint/algebra/default_operations.hpp>

struct stream_observer {
private:
	std::ostream& out;
	std::string delm;
	std::string end;
public:
	stream_observer(std::ostream& out_, std::string delm_ = ", ", std::string end_ = "\n") :out(out_), delm(delm_), end(end_) {}
	template<typename state, typename time>
	void operator()(const state& x, time t) {
		out << t << delm << x.second;
		for (const auto& val : x.first) {
			out << delm << val;
		}
		out << end;
	}
};
struct circle_dysys {
	using state = hmLib::varray<double, 2>;
	using appendix = std::bitset<2>;
	using result = hmLib::odeint::validate_result;
public:
	void initialize(const appendix& ap_) { sysap = ap_; }
	void operator()(const state& x, state& dx, double t)const {

		double r2 = (x[0] - 1.0) * (x[0] - 1.0) + (x[1] - 0.8) * (x[1] - 0.8);

		dx[0] = x[1] - 0.8;
		dx[1] = -(x[0] - 1.0);

		if (sysap[0])dx[0] = 0.0;
		if (sysap[1])dx[1] = 0.0;
	}
	bool is_invalid_step(const state& x, double t)const {

		if (!sysap[0] && !sysap[1]) {
			if (x[0] < 0 || x[1] < 0)return true;
		}
		if (sysap[0]) {
			if (x[1] - 0.8 > 0)return true;
		}
		if (sysap[1]) {
			if (-(x[0] - 1.0) > 0)return true;
		}
		return false;
	}
	result validate(const state& x1, const state& x2, double t, state& nx, appendix& nap)const {
		nx = x2;
		nap = sysap;
		if (!sysap[0] && !sysap[1]) {
			if (nx[0] <= 0) {
				nx[0] = 0;
				nap[0] = true;
			}
			if (nx[1] <= 0) {
				nx[1] = 0;
				nap[1] = true;
			}
		}
		if (sysap[0]) {
			if (x2[1] - 0.8 > 0) {
				nap[0] = false;
			}
		}
		if (sysap[1]) {
			if (-(x2[0] - 1.0) > 0) {
				nap[1] = false;
			}
		}

		return result::assigned;
	}
	result validate(const state& x, double t, state& nx, appendix& nap)const {
		result res = result::none;
		if (x[0] < 0 || x[1] < 0) {
			nx = x;
			nx[0] = std::max(0.0, nx[0]);
			nx[1] = std::max(0.0, nx[1]);
			res = result::assigned;
		}
		return res;
	}
private:
	appendix sysap;
};
struct circle_dysys2 {
	using state = hmLib::varray<double, 2>;
	//using appendix = std::bitset<2>;
	using result = hmLib::odeint::validate_result;
public:
	void operator()(const state& x, state& dx, double t)const {

		double r2 = (x[0] - 1.0) * (x[0] - 1.0) + (x[1] - 0.8) * (x[1] - 0.8);

		dx[0] = x[1] - 0.8;
		dx[1] = -(x[0] - 1.0);
	}
	bool is_invalid_step(const state & x, double t)const {
		return false;
	}
	result validate(const state & x1, const state & x2, double t, state & nx)const {
		nx = x2;


		return result::assigned;
	}
	result validate(const state & x, double t, state & nx)const {
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
	using appendix = typename dysys::appendix;
	auto State = std::make_pair(state{ 1.0,1.7 }, appendix(0));
	//auto State = state{ 1.0,1.7 };

	dysys Dysys;
//	auto Stepper = hmLib::odeint::make_state_validate<appendix>(boost::numeric::odeint::runge_kutta4<state>());
//	auto Stepper = hmLib::odeint::make_state_validate<appendix>(boost::numeric::odeint::make_controlled(1e-5, 1e-5, boost::numeric::odeint::runge_kutta_dopri5<state>()));
//	auto Stepper = hmLib::odeint::make_state_validate<appendix>(boost::numeric::odeint::make_dense_output(1e-5, 1e-5, boost::numeric::odeint::runge_kutta_dopri5<state>()));
//	auto Stepper = hmLib::odeint::make_step_validate<appendix>(0.1, 1e-3, 1e-3, 100, boost::numeric::odeint::runge_kutta4<state>());
//	auto Stepper = hmLib::odeint::make_step_validate<appendix>(0.1, 1e-3, 1e-3, 100, boost::numeric::odeint::make_controlled(1e-5, 1e-5, boost::numeric::odeint::runge_kutta_dopri5<state>()));
	auto Stepper = hmLib::odeint::make_step_validate<appendix>(0.1, 1e-3, 1e-3, 100, boost::numeric::odeint::make_dense_output(1e-5, 1e-5, boost::numeric::odeint::runge_kutta_dopri5<state>()));
//	auto Stepper = boost::numeric::odeint::make_dense_output(0.01, 0.01, boost::numeric::odeint::runge_kutta_dopri5<state>());
	std::ofstream fout("test3.csv");
	auto obs = hmLib::odeint::make_non_appendix_observer(hmLib::odeint::stream_observer(fout));

	hmLib::odeint::integrate_adaptive(Stepper, Dysys, State, 0.0, 10.0, 0.001, obs);

	return 0;
}
