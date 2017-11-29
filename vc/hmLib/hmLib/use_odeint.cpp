#include<array>
#include<boost/numeric/odeint.hpp>
#include"../../../odeint.hpp"
#include"../../../varray.hpp"
struct my_system {
	using state = hmLib::varray<double, 2>;

	void operator()(const state& x, state& dx, double t) {
		if(x[0]<=0.0) {
			dx[0] = std::max(0.0, dx[1] - 5);
			dx[1] = 1;
		} else {
			dx[0] = -dx[1];
			dx[1] = dx[0];
		}
	}
	bool is_exceed(const state& px, double pt, const state& nx, double nt) {
		return (px[0]>0 && nx[0]<=0 || px[1]>0 && nx[1]<=0);
	}
	void exceed(state& px, double& pt, const state& nx1, double nt1, const state& nx2, double nt2) {
		px[0] = (nx1[0]+nx2[0])/2.0;
		px[1] = (nx1[1]+nx2[1])/2.0;
		pt = (nt1+nt2)/2.0;

		if(px[0]<=0.0) {
			px[0] = 0.0;
		}
		if(px[1]<=0.0) {
			px[1] = 0.0;
		}
	}
};

int main() {
	my_system Sys;
	using state = typename my_system::state;
	state State{ 2.0,1.0 };

	using base_stepper_type = boost::numeric::odeint::runge_kutta_dopri5<state>;
	auto Stepper = hmLib::odeint::make_composite_dense_output(1.0e-10, 1.0e-6, 1.0e-3, base_stepper_type());

	Stepper.initialize(State, 0.0, 0.1);

	auto Ans = Stepper.do_step(Sys);

	return 0;
}
