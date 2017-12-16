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
		using size_type = unsigned int;
		using size_diff_pair = std::pair<size_type, diff_type>;

		template<unsigned int dim_>
		using point = varray<index_type, dim_>;
		template<typename ...others>
		auto make_point(others... Others)->point<sizeof...(others)>{
			return point<sizeof...(others)>{static_cast<index_type>(Others)...};
		}
	}
}
#
#endif
