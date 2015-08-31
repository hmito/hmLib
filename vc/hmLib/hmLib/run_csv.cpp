#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
#include<hmLib/csv_iterator.hpp>
#include<hmLib/iocsv.hpp>


void output_table(hmLib::table& Table){
	for(unsigned int x = 0; x < Table.column_size(); ++x){
		std::cout << "[" << Table.column(x).name() << "] ";
		for(unsigned int y = 0; y < Table.row_size(); ++y){
			if(y != 0)std::cout << ",";
			std::cout<< Table.column(x).at(y);
		}
		std::cout << std::endl;
	}
}
void output_table_citr(hmLib::table& Table){
	for(auto itr = Table.column_begin(); itr != Table.column_end(); ++itr){
		std::cout << "[" << itr->name() << "] ";
		for(auto iitr = itr->begin(); iitr != itr->end(); ++iitr){
			if(iitr != itr->begin())std::cout << ",";
			std::cout << *iitr;
		}
		std::cout << std::endl;
	}
}
void output_table_ritr(hmLib::table& Table){
	for(auto itr = Table.column_name_begin(); itr != Table.column_name_end(); ++itr){
		if(itr != Table.column_name_begin())std::cout << ",";
		std::cout << *itr;
	}
	std::cout << std::endl;

	for(auto itr = Table.row_begin(); itr != Table.row_end(); ++itr){
		for(auto iitr = itr->begin(); iitr != itr->end(); ++iitr){
			if(iitr != itr->begin())std::cout << ",";
			std::cout << *iitr;
		}
		std::cout << std::endl;
	}
}
int main(){
	std::ifstream fin("test1.csv");

	hmLib::table Table;
	std::cout << std::endl << "=== true-false ===" << std::endl;
	Table = hmLib::read_csv("test1.csv", true, false);
	output_table(Table);

	std::cout << "===citr===" << std::endl;
	output_table_citr(Table);

	std::cout << "===ritr===" << std::endl;
	output_table_ritr(Table);

	hmLib::write_csv(Table, "test1_o.csv", true, false);

	system("pause");
	
	return 0;
}