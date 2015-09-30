#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<limits>
#include<hmLib/random/random_engine.hpp>
#include<hmLib/algorithm/nonrepeat_random_integrals.hpp>



int main(){
	try{
		auto Ans = hmLib::algorithm::nonrepeat_random_integrals(10, 0, 1000,hmLib::random::Engine);
		for(auto val : Ans)std::cout << val << "\t";

		Ans = hmLib::algorithm::nonrepeat_random_integrals(10, 0, 20, hmLib::random::Engine);
		for(auto val : Ans)std::cout << val << "\t";

		Ans = hmLib::algorithm::nonrepeat_random_integrals(10, 0, 10, hmLib::random::Engine);
		for(auto val : Ans)std::cout << val << "\t";

		Ans = hmLib::algorithm::nonrepeat_random_integrals(10, 0, 5, hmLib::random::Engine);
		for(auto val : Ans)std::cout << val << "\t";
	}
	catch(std::exception& Excp){
		std::cout << Excp.what();
	}
	system("pause");
	
	return 0;
}
