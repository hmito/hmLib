#ifndef HMLIB_LATTICES_POINT_INC
#define HMLIB_LATTICES_POINT_INC 100
#
#include<array>
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
		auto make_point(others... Others)->point<sizeof...(others)>{
			std::array<int,sizeof...(others)> Array{Others...};
			return point<sizeof...(others)>(Array.begin(),Array.end());
		}

		template<unsigned int dim_>
		auto make_filled_point(index_type Val)->point<dim_> { point<dim_> Pos; Pos.fill(Val); return Pos; }

		template<unsigned int dim_>
		point<dim_> index_to_point(index_type Index, const point<dim_>& Size) {
			point<dim_> Pos;
			for (unsigned int no = 0; no < dim_;++no) {
				Pos[no] = (Index%Size[no]);
				Index /= Size[no];
			}

			return Pos;
		}
		template<unsigned int dim_>
		index_type point_to_index(point<dim_> Point, const point<dim_>& Size) {
			diff_type Index = Point[0];
			for (unsigned int no = 1; no < dim_; ++no) {
				Index += Point[no]*Size[no-1];
			}

			return Index;
		}
		template<unsigned int dim_>
		point<dim_> make_torus_point(const point<dim_>& Point, const point<dim_>& Size){
			point<dim_> Ans = Point;
			for(unsigned int i = 0; i < dim_; ++i){
				Ans[i] = algorithm::positive_mod(Ans[i], Size[i]);
			}
			return Ans;
		}
		template<unsigned int dim_>
		void torus_point(point<dim_>& Point, const point<dim_>& Size){
			for(unsigned int i = 0; i < dim_; ++i){
				Point[i] = algorithm::positive_mod(Ans[i], Size[i]);
			}
		}
	}
}
#
#endif
