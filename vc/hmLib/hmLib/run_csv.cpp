#include<fstream>
#include<iostream>
#include<hmLib/csv_iterator.hpp>

int main(){
	std::ifstream fin("test1.csv");

	hmLib::icsv_iterator itr(fin);

	auto End = itr.get_file_end();
	while(itr != End){
		std::cout << *itr << "\t";
		if(itr.eol())std::cout << std::endl;
		++itr;
	}

	system("pause");
	return 0;
}