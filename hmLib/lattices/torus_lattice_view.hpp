#ifndef HMLIB_TORUSLATTICEVIEW_INC
#define HMLIB_TORUSLATTICEVIEW_INC 100
#
#include"point.hpp"
namespace hmLib{
	namespace lattices{
		template<unsigned int dim_>
		struct lattice_index{
			using index_type = lattices::index_type;
			using this_point = lattices::point<dim_>;
		public:
			index_type operator()(this_point Point){

			}
		};
		template<unsigned int dim_>
		struct torus_lattice_index{
			using index_type = lattices::index_type;
			using this_point = lattices::point<dim_>;
		public:
			index_type operator()(this_point Point);
		};
	}
}
#
#endif
