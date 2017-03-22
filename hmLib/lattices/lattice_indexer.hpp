#ifndef HMLIB_LATTICES_LATTICEINDEXER_INC
#define HMLIB_LATTICES_LATTICEINDEXER_INC 100
#
//!@brief Definition of the lattice_indexer

#include "../euclidean.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib{
	namespace lattices{
		/*!@brief Indexer of the given point on the lattice space without gaps

		This class calculate the index (position) values from the point data on the lattice space.
		
		@attention This class require that there is no gaps on the terget containor. In the case for the lattice with gaps, please use gap_lattice_indexer.
		@sa gap_lattice_indexer
		*/
		template<unsigned int dim_>
		struct lattice_indexer {
			friend struct lattice_indexer<dim_ + 1>;
			using this_type = lattice_indexer<dim_>;
			using lower_type = lattice_indexer<dim_ - 1>;
			using point_type = point<dim_>;
		public:
			static constexpr unsigned int dim() { return dim_; }
		public:
			lattice_indexer() : Size(0), Lower() {}
			template<typename... others>
			lattice_indexer(size_type  Size_, others... Others) : Size(Size_), Lower(Others...) {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
			}
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return Size*Lower.lattice_size(); }
			//!Get the size of req_dim_ dimmension
			template<unsigned int  req_dim_>
			size_type size()const {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return dim_solver<req_dim_>().size(*this);
			}
			//!Set the size of req_dim_ dimmension
			template<unsigned int  req_dim_>
			void resize(size_type Size_) {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				dim_solver<req_dim_>().resize(*this, Size_);
			}
		public:
			//Get index value from number list with checking over range.
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				return index_calc(0, 1 , Pos_, Others...);
			}
			//Get index value from point with checking over range.
			index_type index(point_type Point_)const {
				return point_index_calc(0, 1 , 0, Point_);
			}
			//Get index value from number list without checking over range.
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				return operator_index_calc(0, 1 , Pos_, Others...);
			}
			//Get index value from point without checking over range.
			index_type operator()(point_type Point_)const {
				return point_operator_index_calc(0, 1 , 0, Point_);
			}
			//Get minimum and maximum index value of the lattice.
			std::pair<index_type, index_type> index_range()const { return index_range_calc(0, 0, 1); }
		private:
			size_type Size;
			lower_type Lower;
		private:
			std::pair<index_type, index_type> index_range_calc(index_type Min_, index_type Max_, diff_type Step_)const {
				if (Step_ < 0) {
					Min_ += Step_ * (Size - 1);
				} else {
					Max_ += Step_ * (Size - 1);
				}

				return Lower.index_range_calc(Min_, Max_, Step_ * Size);
			}
			template<typename... others>
			index_type index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const {
				hmLib_assert(0 <= Pos_ && static_cast<size_type>(Pos_) < Size, lattices::out_of_range_access, "Pos is larger than Size.");
				return Lower.index_calc(Sum_ + Pos_ * Step_, Step_*Size, Others...);
			}
			template<typename point_t>
			index_type point_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				hmLib_assert(0 <= Point_[No_] && static_cast<size_type>(Point_[No_]) < Size, lattices::out_of_range_access, "Pos is larger than Size.");
				return Lower.point_index_calc(Sum_ + Point_[No_] * Step_, Step_*Size, No_ + 1, Point_);
			}
			template<typename... others>
			index_type operator_index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const {
				return Lower.operator_index_calc(Sum_ + Pos_ * Step_, Step_*Size, Others...);
			}
			template<typename point_t>
			index_type point_operator_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				return Lower.point_operator_index_calc(Sum_ + Point_[No_] * Step_, Step_*Size, No_ + 1, Point_);
			}
			template<unsigned int req_dim_, typename T = void>
			struct dim_solver {
				size_type size(const this_type& This) { return This.Lower.size<req_dim_ - 1>(); }
				void resize(this_type& This, size_type Size_) { This.Lower.resize<req_dim_ - 1>(Size_); }
			};
			template<typename T>
			struct dim_solver<0, T> {
				size_type size(const this_type& This) { return This.Size; }
				void resize(this_type& This, size_type Size_) { This.Size = Size_; }
			};
		};
		template<>
		struct lattice_indexer<0> {
			friend struct lattice_indexer<1>;
			using this_type = lattice_indexer<0>;
		public:
			lattice_indexer() = default;
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const {
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::index is not enough");
				return operator()(Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const { return 0; }
			size_type lattice_size()const { return 1; }
			template<unsigned int req_dim_>
			size_type size()const {
				return 1;
			}
		private:
			std::pair<index_type, index_type> index_range_calc(index_type Min_, index_type Max_, diff_type Step_)const {
				return std::pair<index_type, size_type>(Min_, Max_);
			}
			index_type index_calc(index_type Sum_, diff_type Step_)const {
				return Sum_;
			}
			index_type operator_index_calc(index_type Sum_, diff_type Step_)const {
				return Sum_;
			}
			template<typename point_t>
			index_type point_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				return Sum_;
			}
			template<typename point_t>
			index_type point_operator_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				return Sum_;
			}
		};
	}
}
#
#endif
