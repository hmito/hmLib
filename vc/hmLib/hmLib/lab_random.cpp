#include"../../../hmLib/random.hpp"
#include<array>
#include<iostream>
void test_func_random(){
	std::random_device RD;
	hmLib::xorshift128 Engine(RD());
	Engine.discard(6);

	std::uniform_int_distribution<int> Dist(0, 5);
	std::array<unsigned int, 6> Cnt;
	Cnt.fill(0);

	for(unsigned int i = 0; i < 100000; ++i){
		++Cnt[Dist(Engine)];
	}
	for(auto val : Cnt){
		std::cout << val << std::endl;
	}
}

hmLib::pair_normal_distribution<> PairDist(3.0, 3.0, 1.0, 1.0, 0.5);

int main(){
	test_func_random();

	system("pause");
}