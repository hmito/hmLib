#ifndef HMLIB_LATTICES_POINT_INC
#define HMLIB_LATTICES_POINT_INC 100
#
#include<array>
#include<utility>
#include"../algorithm/compare.hpp"
#include"../varray.hpp"
namespace hmLib{
	namespace lattices{
		using index_type = int;
		using diff_type = int;
		using size_type = std::size_t;
		using size_diff_pair = std::pair<size_type, diff_type>;

		template<unsigned int dim_>
		using point_type = varray<index_type, dim_>;
		template<typename ...others>
		auto point(others... Others) {
			return point_type<sizeof...(others)>{static_cast<index_type>(Others)...};
		}

		template<unsigned int dim_>
		using extent_type = varray<size_type, dim_>;
		template<typename ...others>
		auto extent(others... Others) {
			return point_type<sizeof...(others)>{static_cast<size_type>(Others)...};
		}
	}
}
#
#endif
