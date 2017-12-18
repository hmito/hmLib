#ifndef HMLIB_LATTICES_LATTICEINDEXER_INC
#define HMLIB_LATTICES_LATTICEINDEXER_INC 100
#
//!@brief Definition of the lattice_indexer

#include<numeric>
#include"../algorithm/compare.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib {
	namespace lattices {
		template<unsigned int dim_>
		struct lattice_indexer {
			using this_type = lattice_indexer<dim_>;
			using point_type = lattices::point_type<dim_>;
			using extnt_type = lattices::extent_type<dim_>;
			using index_type = lattices::index_type;
		public:
			static constexpr unsigned int dim() { return dim_; }
		public:
			lattice_indexer() : Extent(0) {}
			lattice_indexer(const extnt_type& Extent_) : Extent(Extent_){}
			lattice_indexer(const this_type&) = default;
			lattice_indexer(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return std::accumulate(Extent.begin(), Extent.end(), 1, [](int v1, int v2)->int {return v1*v2; }); }
			//!Get point_type Extent
			const extnt_type& extent()const {return Extent;}
			//!Set point_type Extent and Gap
			void resize(const extnt_type& Extent_) {
				Extent = Extent_;
			}
		public:
			//Get index value from point without checking over range.
			index_type calc_index(const point_type& Point_)const {
				index_type Index = 0;
				index_type Step = 1;
				for(unsigned int i = 0; i < dim_; ++i) {
					Index += Point_[i] * Step;
					Step *= Extent[i];
				}

				return Index;
			}
			//Get index value from point with checking over range.
			index_type index(const point_type& Point_)const {
				hmLib_assert((lattices::point<dim_>(0) <<= Point_) && (Point_ << Extent), lattices::out_of_range_access, "Requested point is out of lattice.");
				return operator()(Point_);
			}
			//Get index value from point without checking over range.
			index_type operator()(const point_type& Point_)const {
				return calc_index();
			}
			//Get point from index value without checking over range
			point_type calc_point(index_type Index)const {
				point_type Pos;
				for(unsigned int i = 0; i < dim_; ++i) {
					Pos[i] = (Index%Extent[i]);
					Index /= Extent[i];
				}

				return Pos;
			}
			//Get point from index value with checking over range
			point_type point(index_type Index)const {
				hmLib_assert(0 <= Index && Index <size(), lattices::out_of_range_access, "Requested index is out of lattice.");
				return calc_point(Index);
			}
			//Get point from index value without checking over range
			point_type operator()(index_type Index)const{
				return calc_point(Index);
			}
		public:
			point_type translate_for_torus(const point_type& Point)const {
				point_type Ans = Point;
				for(unsigned int i = 0; i < dim_; ++i) {
					Ans[i] = positive_mod(Ans[i], Extent[i]);
				}
				return Ans;
			}
			void fit_for_torus(point_type& Point) {
				for(unsigned int i = 0; i < dim_; ++i) {
					Point[i] = positive_mod(Point[i], Extent[i]);
				}
			}
			//Get index value from torus point
			index_type torus_index(const point_type& Point_)const {
				return calc_index(translate_for_torus(Point_));
			}
		private:
			extnt_type Extent;
		};
	}
}
#
#endif
