#ifndef HMLIB_LATTICES_POINT_INC
#define HMLIB_LATTICES_POINT_INC 100
#
#include<utility>
#include"../euclidean.hpp"
namespace hmLib{
	namespace lattices{
		using index_type = int;
		using diff_type = int;
		using size_type = unsigned int;
		using size_diff_pair = std::pair<size_type, diff_type>;

		template<unsigned int dim_>
		using point = euclidean::point<index_type, dim_>;

		template<typename ...others>
		auto make_point(others... Others)->point<sizeof...(others)>{return point<sizeof...(others)>{Others...};}

		template<unsigned int dim_>
		point<dim_> index_to_point(diff_type Index, const point<dim_>& Size) {
			point<dim_> Pos;
			for (unsigned int no = 0; no < dim_;++no) {
				Pos[no] = (Index%Size[no]);
				Index /= Size[no];
			}

			return Pos;
		}
	}
}
#
#endif
