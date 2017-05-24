#ifndef HMLIB_LATTICES_LATTICEINDEXER_INC
#define HMLIB_LATTICES_LATTICEINDEXER_INC 100
#
//!@brief Definition of the lattice_indexer

#include<numeric>
#include "../euclidean.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib {
	namespace lattices {
		/*!@brief Indexer of the given point on the lattice space with gaps

		This class calculate the index (position) values from the point data on the lattice space including gaps.
		Gap on a dimmension is the distance between the step for lower
		Size on a dimmension is the maximum step num on this axis.

		Tha calculated index is
		(0,0,0) = 0
		(1,0,0) = 1+gap<0>
		(2,0,0) = 2*(1+gap<0>)
		(0,1,0) = size<0>*(1+gap<0>) + gap<1>
		(0,2,0) = 2*(size<0>*(1+gap<0>) + gap<1>)

		@attention Because of the consideration of gaps, The performance of lattice_indexer is less than that of lattice_indexer.
		@sa lattice_indexer
		*/
		template<unsigned int dim_>
		struct lattice_indexer {
			using this_type = lattice_indexer<dim_>;
			using point_type = point<dim_>;
		public:
			static constexpr unsigned int dim() { return dim_; }
		public:
			lattice_indexer() : Size() {}
			lattice_indexer(const point_type& Size_) : Size(Size_){}
			lattice_indexer(const this_type&) = default;
			lattice_indexer(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return std::accumulate(Size.begin(), Size.end(), 1, [](int v1, int v2)->int {return v1*v2; }); }
			//!Get point_type Size
			point_type size()const {return Size;}
			//!Set point_type Size and Gap
			void resize(const point_type& Size_) {
				Size = Size_;
			}
		public:
			//Get index value from point without checking over range.
			index_type operator()(const point_type& Point_)const {
				index_type Index = 0;
				index_type Step = 1;
				for(unsigned int i = 0; i < dim_; ++i){
					Index += Point_[i] * Step;
					Step *= Size[i];
				}

				return Index;
			}
			//Get index value from point without checking over range.
			index_type index(const point_type& Point_)const{
				hmLib_assert((lattices::make_filled_point<dim_>(0) <<= Point_) && (Point_ << Size), lattices::out_of_range_access, "Requested point is out of lattice.");
				return operator()(Point_);
			}
		private:
			point_type Size;
		};
	}
}
#
#endif
