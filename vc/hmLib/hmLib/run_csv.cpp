#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
#include<hmLib/csv_iterator.hpp>
#include<hmLib/iocsv.hpp>


int main(){
	std::ifstream fin("test1.csv");

	auto Table = hmLib::read_csv("test1.csv",',','\n',true);

	Table.push_back_column();
	Table.insert_column(Table.column_begin() + 2);

	Table.push_back_row();
	Table.insert_row(Table.row_begin() + 2);

	Table.swap_column(Table.column_begin(), Table.column_begin() + 1);

	Table.swap_row(Table.row_begin(), Table.row_begin() + 1);

	std::cout <<Table.row_size()<<","<< Table.column_size() << std::endl;

	for(unsigned int x = 0; x < Table.row_size(); ++x){
		for(unsigned int y = 0; y < Table.column_size(); ++y){
			std::cout << "[" << x << "," << y << "]: " << Table.at(x, y).ref() << std::endl;
		}
	}

	system("pause");
	
	return 0;
/*
	hmLib::icsv_iterator itr(fin, ',');
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



	data_type Data;

	column_proxy cp(Data);

	auto bitr = cp.begin();
	auto eitr = cp.end();
*/

	return 0;
}