#include<iostream>
#include<utility>
#include<fstream>
#include"recursive.hpp"
#include"blocks.hpp"
#include"block_vector.hpp"

template<typename state_>
struct sin_dsystem {
	using state = state_;
private:
	double T;
	double D;
	double R;
	state nx;
public:
	sin_dsystem(double T_, double D_,double R_) :T(T_),D(D_),R(R_) {}
	void operator()(state& x, unsigned int t) {
		nx.clear();

		for (auto itr = x.begin(); itr != x.end(); ++itr) {
			auto n = itr.index();
			nx[n] += (1 - R)*(*itr) * (1 - D);
			nx[n - 1] += (1 - R) * (*itr) * D / 2;
			nx[n + 1] += (1 - R) * (*itr) * D / 2;
		}
		nx[0] = std::max(0.,std::cos(2 * 3.14 * T));

		nx.block_erase_if_all_of([](auto v) {return v < 1e-5; });
		std::swap(nx, x);
	}
};
template<typename state>
void trial(state x) {
	sin_dsystem<state> Sys(100, 0.05, 0.02);
	hmLib::recursive::recurrence_increment(Sys, x, 0, 2000);
	std::ofstream fout("out.csv");

	for (auto itr = x.begin(); itr != x.end(); ++itr) {
		auto n = itr.index();
		fout << n << "," << *itr << std::endl;
	}
}
int main() {
	trial(hmLib::block_vector<double>(16));

	return 0;
}