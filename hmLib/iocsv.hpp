#ifndef HMLIB_IOCSV_INC
#define HMLIB_IOCSV_INC 100
#
#include<string>
#include<hmLib/table.hpp>
#include<hmLib/csv_iterator.hpp>
namespace hmLib{
	table<std::string> read_csv();
}
#
#endif
