#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>
#include <hmLib/csv_iterator.hpp>
#include <hmLib/table.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_csv_iterator){
	public:
		TEST_METHOD(TestMethod1){
			std::ifstream fin("test1.csv");

			hmLib::icsv_iterator itr(fin);

			while(!itr.eof()){
				std::cout << *itr<<"\t";
				if(itr.eol())std::cout << std::endl;
			}
		}
	};
	TEST_CLASS(test_table){
	public:
		TEST_METHOD(TestMethod1){
			table Table;
			Table.size();
			Table.assign(10, 20);
			for(auto itr = Table.column_begin(); itr != Table.column_end(); ++itr){
				itr->rename("test"+boost::lexical_cast<std::string>(itr-Table.column_begin()));
			}
		}

	};
}