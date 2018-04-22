#ifndef HMLIB_LATTICES_INDEXER_INC
#define HMLIB_LATTICES_INDEXER_INC 100
#
//!@brief Definition of the indexer

#include<numeric>
#include"../algorithm/compare.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib {
	namespace lattices {
		template<unsigned int dim_>
		struct indexer {
			using this_type = indexer<dim_>;
			using point_type = lattices::point_type<dim_>;
			using extent_type = lattices::extent_type<dim_>;
			using index_type = lattices::index_type;
		public:
			static constexpr unsigned int dim() { return dim_; }
		public:
			indexer() : Extent(0) {}
			explicit indexer(const extent_type& Extent_) : Extent(Extent_){}
			indexer(const this_type&) = default;
			indexer(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return std::accumulate(Extent.begin(), Extent.end(), size_type(1), [](size_type v1, size_type v2)->size_type {return v1*v2; }); }
			//!Get point_type Extent
			const extent_type& extent()const {return Extent;}
			//!Set point_type Extent and Gap
			void resize(const extent_type& Extent_) {
				Extent = Extent_;
			}
		public:
			//Get index value from point without checking over range.
			index_type calc_index(const point_type& Point_)const {
				index_type Index = 0;
				index_type Step = 1;
				for(unsigned int i = 0; i < dim_; ++i) {
					Index += Point_[i] * Step;
					Step *= static_cast<index_type>(Extent[i]);
				}

				return Index;
			}
			//Get index value from point with checking over range.
			index_type index(const point_type& Point_)const {
				hmLib_assert((all_less_equal_than(point_type(0),Point_) && all_less_than(Point_, static_cast<point_type>(Extent))), lattices::out_of_range_access, "Requested point is out of lattice.");
				return operator()(Point_);
			}
			//Get index value from point without checking over range.
			index_type operator()(const point_type& Point_)const {
				return calc_index(Point_);
			}
			//Get point from index value without checking over range
			point_type calc_point(index_type Index)const {
				point_type Pos;
				for(unsigned int i = 0; i < dim_; ++i) {
					Pos[i] = (Index%Extent[i]);
					Index /= static_cast<index_type>(Extent[i]);
				}

				return Pos;
			}
			//Get point from index value with checking over range
			point_type point(index_type Index)const {
				hmLib_assert(0 <= Index && static_cast<size_type>(Index) < lattice_size(), lattices::out_of_range_access, "Requested index is out of lattice.");
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
					Ans[i] = positive_mod<index_type>(Ans[i], Extent[i]);
				}
				return Ans;
			}
			void fit_for_torus(point_type& Point) {
				for(unsigned int i = 0; i < dim_; ++i) {
					Point[i] = positive_mod<index_type>(Point[i], Extent[i]);
				}
			}
			//Get index value from torus point
			index_type torus_index(const point_type& Point_)const {
				return calc_index(translate_for_torus(Point_));
			}
		private:
			extent_type Extent;
		};
	}
}
#
#endif
