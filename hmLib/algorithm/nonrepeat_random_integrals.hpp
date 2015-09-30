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
#include <hmLib/exceptions.hpp>

#ifndef hmLib_assert
#	define hmLib_assert(condition,except,str)
#endif

namespace hmLib{
	namespace algorithm{
		template<typename type,typename generator>
		std::vector<type> nonrepeat_random_integrals_by_unique(const std::size_t size, type rand_min, type rand_max, generator& Engine){
			using unsigned_type = std::make_unsigned<type>::type;

			if(rand_min > rand_max) std::swap(rand_min, rand_max);
			const unsigned_type max_min_diff = rand_max - rand_min + 1;
			hmLib_assert(size<=max_min_diff,std::runtime_error,"rand_max - rand_min is smaller than requested size.");

			std::vector<type> tmp;
			std::uniform_int_distribution<type> distribution(rand_min, rand_max);

			//ひとまず、size/5だけ多めに作ってから、重複を消す。
			//この数字に根拠はないので、より最適な値がある可能性あり
			const std::size_t make_size = (std::numeric_limits<std::size_t>::max() - size / 5) < size ? size : size + size / 5;

			tmp.reserve(make_size);
			while(tmp.size() < size){
				while(tmp.size() < make_size) tmp.push_back(distribution(Engine));
				std::sort(tmp.begin(), tmp.end());
				auto unique_end = std::unique(tmp.begin(), tmp.end());

				if(size < static_cast<std::size_t>(std::distance(tmp.begin(), unique_end))){
					unique_end = std::next(tmp.begin(), size);
				}
				tmp.erase(unique_end, tmp.end());
			}

			std::shuffle(tmp.begin(), tmp.end(), Engine);
			return tmp;
		}
		template<typename type, typename generator>
		std::vector<type> nonrepeat_random_integrals_by_select(const std::size_t size, type rand_min, type rand_max, generator& Engine){
			using unsigned_type = std::make_unsigned<type>::type;

			if(rand_min > rand_max) std::swap(rand_min, rand_max);
			const unsigned_type max_min_diff = rand_max - rand_min + 1;
			hmLib_assert(size <= max_min_diff, std::runtime_error, "rand_max - rand_min is smaller than requested size.");

			std::vector<type> tmp;
			tmp.assign(max_min_diff,0);
			auto val = rand_min;
			std::generate(tmp.begin(), tmp.end(), [&val](){return val++; });

			std::uniform_int_distribution<type> distribution(rand_min, rand_max);

			for(std::size_t cnt = 0; cnt < size; ++cnt){
				std::size_t pos = std::uniform_int_distribution<std::size_t>(cnt, tmp.size() - 1)(Engine);

				if(cnt != pos) std::swap(tmp[cnt], tmp[pos]);
			}
			tmp.erase(std::next(tmp.begin(), size), tmp.end());

			return tmp;
		}
		template<typename type, typename generator>
		std::vector<type> nonrepeat_random_integrals(const std::size_t size, type rand_min, type rand_max, generator& Engine){
			using unsigned_type = std::make_unsigned<type>::type;

			if(rand_min > rand_max) std::swap(rand_min, rand_max);
			const unsigned_type max_min_diff = rand_max - rand_min + 1;

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
