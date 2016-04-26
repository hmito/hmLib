#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<limits>
#include<hmLib/random/random_engine.hpp>
#include<hmLib/random/random_bits.hpp>
#include<hmLib/algorithm/nonrepeat_random_integrals.hpp>

int main(){
	try{
		std::cout << hmLib::random::defaul_bits_engine::interval()<<","<<hmLib::random::defaul_bits_engine::max_interval_bits() << std::endl;
		std::cout << hmLib::random::uniform_bool() << std::endl;
		std::cout << hmLib::random::uniform_bits<5>() << std::endl;
		std::cout << hmLib::random::uniform_bits<37>() << std::endl;
	}
	catch(std::exception& Excp){
		std::cout << Excp.what();
	}
	system("pause");
	
	return 0;
}
