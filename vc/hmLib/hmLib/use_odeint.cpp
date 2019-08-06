#include<array>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<bitset>
#include<fstream>
#include<boost/numeric/odeint.hpp>
#include"../../../odeint/observer/stream_observer.hpp"
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

#include"../../../tuple.hpp"


struct circle_dysys {
	using state = hmLib::varray<double, 2>;
	using result = hmLib::odeint::validate_result;
	using mborders = hmLib::odeint::borders< hmLib::odeint::lower_border<double>, hmLib::odeint::lower_border<double>>;
	using appendix = typename mborders::appendix;
public:
	circle_dysys() {
		Borders.get<0>().set(0, 0.0);
		Borders.get<1>().set(1, 0.0);
	}
	void initialize(const appendix& ap_) { sysap = ap_; }
	void deriv(const state& x, state& dx, double t)const {
		dx[0] = x[1] - 0.8;
		dx[1] = -(x[0] - 1.0);
	}
	void operator()(const state& x, state& dx, double t)const {
		deriv(x, dx, t);
		Borders(x, dx, t, sysap);
	}
	bool is_invalid_step(const state& x, double t)const {
		state dx;
		deriv(x, dx, t);

		return Borders.is_invalid_step(x, dx, t, sysap);
	}
	result validate(const state& x1, const state& x2, double t, state& nx, appendix& nap)const {
		nx = x2;

		state dx;
		deriv(nx, dx, t);

		Borders.validate(nx, dx, t, nap);

		return result::assigned;
	}
	result validate(const state& x, double t, state& nx, appendix& nap)const {
		if (Borders.will_validate(x, t, nap)) {
			nx = x;
			state dx;
			deriv(nx, dx, t);
			if (Borders.validate(nx, dx, t, nap)) {
				return result::assigned;
			}
		}

		return result::none;
	}
private:
	appendix sysap;
	mborders Borders;
};
int main(void) {
	using dysys = circle_dysys;
	using state = typename dysys::state;
	using appendix = typename dysys::appendix;
	auto State = std::make_pair(state{ 1.0,1.7 }, appendix());
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

	hmLib::odeint::integrate_const(Stepper, Dysys, State, 0.0, 10.0, 0.01, obs);

	return 0;
}
