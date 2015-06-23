#ifndef HMLIB_STREAMUTILITY_INC
#define HMLIB_STREAMUTILITY_INC 100
#
#include<fstream>
namespace hmLib {
	bool is_exist_file(const std::string& Name_) {
		std::ifstream fin(Name_);
		return fin.is_open();
	}
}
#
#endif
