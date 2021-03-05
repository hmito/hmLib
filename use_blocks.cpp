#include<iostream>
#include<utility>
#include<fstream>
#include<chrono>
#include"odeint/recursive.hpp"
#include"odeint/observer/observer_pack.hpp"
#include"blocks.hpp"
#include"block_vector.hpp"
#include"io_utility.hpp"


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
			if (n > 0) {
				nx[n+1] += (*itr)*(1-R);
			} else if (n < 0) {
				nx[n - 1] += (*itr) * (1 - R);
			} else {
				nx[1] += *itr * (1 - R);
				nx[-1] += *itr * (1 - R);
			}
		}
		nx[0] = std::max(0., std::sin(2 * 3.14 * t / (T-D*t))-0.9);

		nx.block_erase_if_all_of([](auto v) {return v < 1e-10; });
		nx.block_erase_if_all_of([](auto v) {return v < 1e-10; });
		std::swap(nx, x);
	}
};

template<typename time_type>
struct progress_observer {
	hmLib::progress_bar<time_type> progress;
	progress_observer(time_type end_t_, time_type strt_t_ = 0) :progress(end_t_, strt_t_) {}
	void reset(time_type end_t_, time_type strt_t_ = 0) {
		progress.reset(end_t_, strt_t_);
	}
	template<typename state_type>
	void operator()(const state_type&, time_type t) {
		progress(std::cout, t);
	}
};
template<typename state>
void trial(state x,std::string name) {
	auto strt = std::chrono::system_clock::now();

	sin_dsystem<state> Sys(1000, 0.05, 0.0001);

	progress_observer<int> Obs(100000);
	hmLib::odeint::recurrence_increment(Sys, x, 0, 100000, Obs);
	std::ofstream fout(name);

	for (auto itr = x.begin(); itr != x.end(); ++itr) {
		auto n = itr.index();
		fout << n << "," << *itr << std::endl;
	}
	auto stop = std::chrono::system_clock::now();

	std::cout << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - strt).count() << std::endl;
}
int main() {
	trial(hmLib::block_vector<double>(16), "block_vector.csv");
	trial(hmLib::blocks<double,int,hmLib::default_block_hasher<6,int>>(), "blocks.csv");

	return 0;
}