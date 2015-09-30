#ifndef HMLIB_ALGORITHM_NONREPEATRANDOMINTEGRALS_INC
#define HMLIB_ALGORITHM_NONREPEATRANDOMINTEGRALS_INC 100
#
#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <functional>
#include <limits>
#include <type_traits>

namespace hmLib{
	namespace algorithm{
		template<typename type,typename generator>
		std::vector<type> nonrepeat_random_integrals_by_unique(const unsigned int size, type rand_min, type rand_max, generator& Engine){
			if(rand_min > rand_max) std::swap(rand_min, rand_max);
			const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
			if(max_min_diff < size) throw std::runtime_error("Invalid argument");

			std::vector<type> tmp;
			std::uniform_int_distribution<type> distribution(rand_min, rand_max);

			//ひとまず、size/5だけ多めに作ってから、重複を消す。
			//この数字に根拠はないので、より最適な値がある可能性あり
			const unsigned int make_size = (std::numeric_limits<unsigned int>::max() - size / 5) < size) ? size : size + size / 5;

			tmp.reserve(make_size);
			while(tmp.size() < size){
				while(tmp.size() < make_size) tmp.push_back(distribution(Engine));
				std::sort(tmp.begin(), tmp.end());
				auto unique_end = std::unique(tmp.begin(), tmp.end());

				if(size < static_cast<unsigned int>(std::distance(tmp.begin(), unique_end))){
					unique_end = std::next(tmp.begin(), size);
				}
				tmp.erase(unique_end, tmp.end());
			}

			std::shuffle(tmp.begin(), tmp.end(), Engine);
			return tmp;
		}
		template<typename type, typename generator>
		std::vector<type> nonrepeat_random_integrals_by_select(const unsigned int size, type rand_min, type rand_max, generator& Engine){
			if(rand_min > rand_max) std::swap(rand_min, rand_max);
			const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
			if(max_min_diff < size) throw std::runtime_error("Invalid argument");

			std::vector<type> tmp;
			tmp.reserve(max_min_diff);

			for(auto i = rand_min; i <= rand_max; ++i)tmp.push_back(i);

			auto engine = create_rand_engine();
			std::uniform_int_distribution<type> distribution(rand_min, rand_max);

			for(unsigned int cnt = 0; cnt < size; ++cnt){
				unsigned int pos = std::uniform_int_distribution<unsigned int>(cnt, tmp.size() - 1)(Engine);

				if(cnt != pos) std::swap(tmp[cnt], tmp[pos]);
			}
			tmp.erase(std::next(tmp.begin(), size), tmp.end());

			return tmp;
		}
		template<typename type, typename generator>
		std::vector<type> nonrepeat_random_integrals(const unsigned int size, type rand_min, type rand_max, generator& Engine){
			if(size < max_min_diff / 33){
				return nonrepeat_random_integrals_by_unique(size, rand_min, rand_max, Engine);
			} else{
				return nonrepeat_random_integrals_by_select(size, rand_min, rand_max, Engine);
			}
		}
	}
}
#
#endif
