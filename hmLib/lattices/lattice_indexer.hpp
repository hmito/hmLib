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
			lattice_indexer() : Pos(), Size(), OSize() {}
			lattice_indexer(point_type Size_) : Pos(), Size(Size_), OSize(Size_) {}
			lattice_indexer(point_type Pos_, point_type Size_, point_type OSize_) : Pos(Pos_), Size(Size_), OSize(OSize_) {}
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return std::accumulate(Size.begin(), Size.end(), 1, [](int v1, int v2)->int {return v1*v2; }); }
			//!Get point_type Size
			point_type size()const {return Size;}
			//!Set point_type Size and Gap
			void resize(point_type Size_) {
				Pos.fill(0);
				Size = Size_;
				OSize = Size_;
			}
			//!Set point_type Size and Gap
			void resize(point_type Pos_, point_type Size_, point_type OSize_) {
				Pos = Pos_;
				Size = Size_;
				OSize = OSize_;
			}
		public:
			//Get index value from point with checking over range.
			index_type index(point_type Point_)const {
				Pos + Point_
			}
			//Get index value from point without checking over range.
			index_type operator()(point_type Point_)const {
				return point_operator_index_calc(0, 1 + Gap, 0, Point_);
			}
		private:
			point_type Pos;
			point_type Size;
			point_type OSize;
		};
		template<>
		struct lattice_indexer<0> {
			friend struct lattice_indexer<1>;
			using this_type = lattice_indexer<0>;
		public:
			lattice_indexer() :Base(0) {}
			template<typename... others>
			index_type index(diff_type Pos_, others... Others)const {
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::index is not enough");
				return operator()(Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(diff_type Pos_, others... Others)const { return 0; }
			size_type lattice_size()const { return 1; }
			template<unsigned int req_dim_>
			size_type axis_size()const {
				return 1;
			}
			template<unsigned int req_dim_>
			diff_type axis_gap()const {
				return 0;
			}
			diff_type base()const { return Base; }
			void set_base(diff_type Base_) { Base = Base_; }
		private:
			diff_type Base;
		private:
			template<typename iterator>
			void size_element_get(iterator Beg, iterator End)const {}
			template<typename iterator>
			void gap_element_get(iterator Beg, iterator End)const {}
			template<typename iterator>
			void size_element_set(iterator Beg, iterator End) {}
			template<typename iterator>
			void gap_element_set(iterator Beg, iterator End) {}
			std::pair<index_type, index_type> index_range_calc(index_type Min_, index_type Max_, diff_type Step_)const {
				return std::pair<index_type, size_type>(Min_ + Base, Max_ + Base);
			}
			index_type index_calc(index_type Sum_, diff_type Step_)const {
				return Sum_ + Base;
			}
			index_type operator_index_calc(index_type Sum_, diff_type Step_)const {
				return Sum_ + Base;
			}
			template<typename point_t>
			index_type point_index_calc(index_type Sum_, diff_type Step_, diff_type No_, const point_t& Point_)const {
				return Sum_ + Base;
			}
			template<typename point_t>
			index_type point_operator_index_calc(index_type Sum_, diff_type Step_, diff_type No_, const point_t& Point_)const {
				return Sum_ + Base;
			}
		};
	}
}
#
#endif
