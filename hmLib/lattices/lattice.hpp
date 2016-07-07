#ifndef HMLIB_LATTICES_LATTICE_INC
#define HMLIB_LATTICES_LATTICE_INC 100
#
#include <vector>
#include "lattice_indexer.hpp"
namespace hmLib{
	template<typename T, unsigned int dim_>
	struct lattice{
		using indexer = lattices::lattice_indexer<dim_>;
		using size_type = typename indexer::size_type;
		using index_type = typename indexer::index_type;
		using point_type = lattices::point<dim_>;
	private:
		indexer Idx;
		std::vector<T> Vec;
	public:
		lattice() = default;
		template<typename... others>
		lattice(size_type Size, others... Others) : Idx(Size, Others...){
			Vec.assign(Idx.lattice_size());
		}
	public:
		template<typename... others>
		T& at(index_type Pos, others... Others){
			return Vec[idx(Pos, Others...)];
		}
		template<typename... others>
		const T& at(index_type Pos, others... Others)const{
			return Vec[idx(Pos, Others...)];
		}
		T& at(point_type Point){
			point_type()
		}
	};
}
#
#endif
