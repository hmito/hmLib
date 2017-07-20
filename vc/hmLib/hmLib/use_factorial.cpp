#include<chrono>
#include<vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<array>
#include<utility>
#include<string>
#include <cstdint>
#include"../../../hmLib/factorial.hpp"
unsigned int raw_factorial(unsigned int val){
	return val>1 ? val*raw_factorial(val - 1) : 1;
}
int main(){
	using namespace std::chrono;
	using namespace hmLib;

	{
		const auto t0 = high_resolution_clock::now();

		double val = 0;
		for(unsigned int i = 0; i < 1000000; ++i){
			val += hmLib::factorial(15) / hmLib::factorial(11) * hmLib::factorial(7) / hmLib::factorial(5);
		}

		const auto t1 = high_resolution_clock::now();
		const auto time = duration_cast<milliseconds>(t1 - t0);
		std::cout <<val << " = " << time.count() << "msec" << std::endl;
	}

	{
		const auto t0 = high_resolution_clock::now();

		double val = 0;
		for(unsigned int i = 0; i < 1000000; ++i){
			val += raw_factorial(15) / raw_factorial(11)* raw_factorial(7) / raw_factorial(5);
		}

		const auto t1 = high_resolution_clock::now();
		const auto time = duration_cast<milliseconds>(t1 - t0);
		std::cout << val << " = " << time.count() << "msec" << std::endl;
	}

	system("pause");
	return 0;
}