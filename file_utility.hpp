#ifndef HMLIB_FILEUTILITY_INC
#define HMLIB_FILEUTILITY_INC 100
#
#include<fstream>
#include<string>
#include<utility>
#include<algorithm>
namespace hmLib {
	inline bool is_exist_file(const std::string& Name_) {
		std::ifstream fin(Name_);
		return fin.is_open();
	}
	inline std::pair<std::string, std::string> splite_filepath(std::string FullPath){
		auto FileLength = std::distance(
			FullPath.rbegin(), 
			std::find(FullPath.rbegin(), FullPath.rend(), '\\')
		);
		auto SlashPos = FullPath.size() - FileLength - 1;
		return std::pair<std::string, std::string>(
			FullPath.substr(0, SlashPos),
			FullPath.substr(SlashPos+1, FileLength)
		);
	}
}
#
#endif
