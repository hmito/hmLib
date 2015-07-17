#include<fstream>
#include<iostream>
#include<sstream>
#include<hmLib/csv_iterator.hpp>

int main(){
	std::ifstream fin("test1.csv");

	hmLib::icsv_iterator itr(fin);
	hmLib::ocsv_iterator oitr(std::cout, '\t');


	hmLib::advance_line(itr);

	while(!itr.eof()){

		std::string Name;
		unsigned int Val;
		unsigned char Sex;
		*itr++ >> Name;
		*itr++ >> Val;
		*itr++ >> Sex;

		*oitr++ << Name;
		*oitr++ << Val;
		*oitr++ << Sex;
		oitr.endl();
	}

	system("pause");
	return 0;
}