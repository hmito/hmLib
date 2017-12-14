#include<array>
#include<iostream>
#include<vector>
#include<string>
#include<boost/numeric/odeint.hpp>
#include"../../../odeint/varray_wrapper.hpp"
#include"../../../odeint.hpp"
#include"../../../varray.hpp"
#include"../../../tuple.hpp"
/*
std::ostream& operator<<(std::ostream& out, const hmLib::varray<double, 2>& v) {
	out<<"("<<v[0]<<", "<<v[1]<<")";
	return out;
}
struct my_system1 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = 1;
		dx[1] = 1;
		//std::cout<<"1"<<std::endl;
	}
};
struct my_system2 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = 1;
		dx[1] = -1;
		//std::cout<<"2"<<std::endl;
	}
};
struct my_system3 {
	using state = hmLib::varray<double, 2>;
	void operator()(const state& x, state& dx, double t) {
		dx[0] = -1;
		dx[1] = 0;
		//std::cout<<"3"<<std::endl;
	}
};
struct interferer {
	using state = hmLib::varray<double, 2>;
	hmLib::odeint::interfere_type operator()(const state& x, state& ix, double t) {
		if(x.at(1)>1)return hmLib::odeint::interfere_type::terminate;
		return hmLib::odeint::interfere_type::ignore;
	}
};
int main() {
	using state = hmLib::varray<double, 2>;
	namespace bode = boost::numeric::odeint;
	namespace hode = hmLib::odeint;
	auto CmpSys = hode::system_switch(
		hode::case_logical_and(
			hode::state_at<state>(1, hode::case_lower_boundary(0.0)),
			hode::state_at<state>(0, hode::case_greater_equal(-1.0))
		), my_system3(),
		hode::state_at<state>(0,hode::case_greater_equal(0.0)), my_system2(),
		my_system1()
	);

	state x{ -1,0.1 };

	CmpSys.update(x, 0);

	using base_stepper_type = boost::numeric::odeint::runge_kutta_dopri5<state>;
	auto Stepper = hmLib::odeint::make_composite_dense_output(1.0e-10, 1.0e-6, 1.0e-3, base_stepper_type());

	hmLib::odeint::stream_observer Observer(std::cout,"\t");
	interferer Interferer;

	Stepper.initialize(x, 0, 0.01);
	hode::interfere_integrate_const(Stepper, CmpSys, x, 0.0, 10.0, 0.1, Interferer, Observer);

	system("pause");
	return 0;
}
*/
double f2(double a, double b) {
	return a+b;
}
std::string f2(double a, int b) {
	return "double_int";
}
int f2(int a, int b) {
	return a+b;
}
std::string f2(const std::string& a, int b) {
	std::string n;
	for(int i = 0; i < b; ++i)n += a;
	return n;
}
std::string f2(const std::string& a, const std::string& b) {
	return a + b;
}
int main(void) {
	auto Tuple1 = std::make_tuple(0.4, 0.7, "test", "std", 5);
	auto Tuple2 = std::make_tuple(0.4, 5, "game", 4, 5);

	auto A = hmLib::tuple_for_each([](auto a, auto b) {return f2(a,b); }, Tuple1, Tuple2);

	return 0;
}