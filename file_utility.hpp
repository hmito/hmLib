#ifndef HMLIB_FILEUTILITY_INC
#define HMLIB_FILEUTILITY_INC 100
#
#include<fstream>
#include<string>
#include<utility>
#include<algorithm>

#ifdef _MSC_VER
# 	define HMLIB_FILEPATH_SEP '\\'
#else
# 	define HMLIB_FILEPATH_SEP '/'
#endif				

namespace hmLib {
	inline bool is_exist_file(const std::string& Name_) {
		std::ifstream fin(Name_);
		return fin.is_open();
	}
	inline std::pair<std::string, std::string> split_file_path(std::string FullPath){
		auto FileLength = std::distance(
			FullPath.rbegin(), 
			std::find(FullPath.rbegin(), FullPath.rend(), HMLIB_FILEPATH_SEP)
		);
		auto SlashPos = FullPath.size() - FileLength - 1;
		return std::pair<std::string, std::string>(
			FullPath.substr(0, SlashPos),
			FullPath.substr(SlashPos+1, FileLength)
		);
	}
	inline std::pair<std::string, std::string> split_file_extension(std::string FullPath){
		auto ExtLength = std::distance(
			FullPath.rbegin(), 
			std::find(FullPath.rbegin(), FullPath.rend(), '.')
		);
		auto DotPos = FullPath.size() - ExtLength - 1;
		return std::pair<std::string, std::string>(
			FullPath.substr(0, DotPos),
			FullPath.substr(DotPos+1, ExtLength)
		);
	}
}
#
#endif
