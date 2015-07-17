#include<fstream>
#include<iostream>
#include<sstream>
#include<hmLib/csv_iterator.hpp>

int main(){
	std::ifstream fin("test1.csv");

	hmLib::icsv_iterator itr(fin);

	auto End = itr.get_file_end();

	std::cout << End.pos() << std::endl;
//	for(unsigned int i=0;i<10;++i){
	while(!itr.eof()){
		std::cout <<"["<<itr.pos()<<"]"<< *itr << "\t";
		if(itr.eol())std::cout << std::endl;
		++itr;
	}

	hmLib::ocsv_iterator oitr(std::cout);
	*oitr = "na";
	*oitr++ = "me";
	*oitr++ = "value";

	oitr.endl();

	*oitr << 2.3;
	++oitr;
	*oitr << 23;
	++oitr;
	oitr.endl();



	system("pause");
	return 0;
}