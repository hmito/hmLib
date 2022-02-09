#include<iostream>
#include<array>
#include "minima.hpp"

int main() {
	auto fn = [](double x) {return (x + 2.6) * (x - 6.6); };
	auto fn2 = [](std::array<double,1> x) {return (x[0] + 2.6) * (x[0] - 6.6); };
	auto fn3 = [](std::array<double,2> x) {return ((x[0] + 2.6) * (x[0] - 6.6)+25)* ((x[1] - 1.2) * (x[1] - 19.2)+90); };
	{
		auto ans = hmLib::minima::golden_section_minima(fn, 0.0, 10.0, 10000, 1e-8, 1e-6);
		std::cout << ans.success << ": count=" << ans.count << " value=" << ans.value << " fvalue=" << ans.fvalue << std::endl;
	}
	{
		auto ans = hmLib::minima::brent_minima(fn, 0.0, 10.0, 10000, 1e-8, 1e-6);
		std::cout << ans.success << ": count=" << ans.count << " value=" << ans.value << " fvalue=" << ans.fvalue << std::endl;
	}
	{
		auto ans = hmLib::minima::nelder_mead_minima(fn2, std::array<double, 1>{5.0}, 1000, 1e-8, 1e-6);
		std::cout << ans.success << ": count=" << ans.count << " value=" << ans.value[0] << " fvalue=" << ans.fvalue << std::endl;
	}
	{
		auto ans = hmLib::minima::nelder_mead_minima(fn3, std::array<double, 2>{5.0,5.0}, 1000, 1e-8, 1e-6);
		std::cout << ans.success << ": count=" << ans.count << " value=(" << ans.value[0]<<"," << ans.value[1] << ") fvalue=" << ans.fvalue << std::endl;
	}
	return 0;
}