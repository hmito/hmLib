#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>
#include <hmLib/csv_iterator.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_csv_iterator){
	public:
		TEST_METHOD(TestMethod1){
			std::ifstream fin("test1.csv");

			hmLib::icsv_iterator itr(fin);

			auto End = itr.get_file_end();
			while(itr != End){
				std::cout << *itr<<"\t";
				if(itr.eol())std::cout << std::endl;
			}
		}

	};
}