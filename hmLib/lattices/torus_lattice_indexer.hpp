#ifndef HMLIB_LATTICES_TORUSLATTICEINDEXER_INC
#define HMLIB_LATTICES_TORUSLATTICEINDEXER_INC 100
#
//!@brief Definition of the torus_lattice_indexer

#include "../euclidean.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib {
	namespace lattices {
		/*!@brief Indexer of the given point on the torus lattice space with gaps

		This class calculate the index (position) values from the point data on the lattice space including gaps.
		Gap on a dimmension is the distance between the step for lower
		Size on a dimmension is the maximum step num on this axis.

		Tha calculated index is
		(0,0,0) = 0
		(1,0,0) = 1+gap<0>
		(2,0,0) = 2*(1+gap<0>)
		(0,1,0) = size<0>*(1+gap<0>) + gap<1>
		(0,2,0) = 2*(size<0>*(1+gap<0>) + gap<1>)

		@attention Because of the consideration of gaps, The performance of torus_lattice_indexer is less than that of lattice_indexer.
		@sa lattice_indexer
		*/
		template<unsigned int dim_>
		struct torus_lattice_indexer {
			friend struct torus_lattice_indexer<dim_ + 1>;
			using this_type = torus_lattice_indexer<dim_>;
			using lower_type = torus_lattice_indexer<dim_ - 1>;
			using point_type = point<dim_>;
		public:
			static constexpr unsigned int dim() { return dim_; }
		public:
			torus_lattice_indexer() : Size(0), Gap(0), Lower() {}
			template<typename... others>
			torus_lattice_indexer(size_type  Size_, others... Others) : Size(Size_), Gap(0), Lower(Others...) {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
			}
			template<typename... others>
			torus_lattice_indexer(std::pair<size_type, index_type>  SizeGap_, others... Others) : Size(SizeGap_.first), Gap(SizeGap_.second), Lower(Others...) {
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
			//!Get the gap of req_dim_ dimmension
			template<unsigned int  req_dim_>
			diff_type gap()const {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return dim_solver<req_dim_>().gap(*this);
			}
			//!Set the size of req_dim_ dimmension
			template<unsigned int  req_dim_>
			void resize(size_type Size_) {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				dim_solver<req_dim_>().resize(*this, Size_);
			}
			//!Set the size of req_dim_ dimmension
			template<unsigned int  req_dim_>
			void resize(size_type Size_, diff_type Gap_) {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				dim_solver<req_dim_>().resize(*this, Size_, Gap_);
			}
			//!Set the gap of req_dim_ dimmension
			template<unsigned int  req_dim_>
			void set_gap(diff_type Gap_) {
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				dim_solver<req_dim_>().set_gap(*this, Gap_);
			}
			//!Get the base index value
			diff_type base()const { return Lower.base(); }
			//!Set the base index value
			void set_base(diff_type Base_) { Lower.set_base(Base_); }
		public:
			//Get index value from number list with checking over range.
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				return index_calc(0, 1 + Gap, Pos_, Others...);
			}
			//Get index value from point with checking over range.
			index_type index(point_type Point_)const {
				return point_index_calc(0, 1 + Gap, 0, Point_);
			}
			//Get index value from number list without checking over range.
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const {
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				return operator_index_calc(0, 1 + Gap, Pos_, Others...);
			}
			//Get index value from point without checking over range.
			index_type operator()(point_type Point_)const {
				return point_operator_index_calc(0, 1 + Gap, 0, Point_);
			}
			//Get minimum and maximum index value of the lattice.
			std::pair<index_type, index_type> index_range()const { return index_range_calc(0, 0, 1 + Gap); }
		private:
			size_type Size;
			diff_type Gap;
			lower_type Lower;
		private:
			std::pair<index_type, index_type> index_range_calc(index_type Min_, index_type Max_, diff_type Step_)const {
				if (Step_ < 0) {
					Min_ += Step_ * (Size - 1);
				} else {
					Max_ += Step_ * (Size - 1);
				}

				return Lower.index_range_calc(Min_, Max_, Step_ * Size + Lower.gap<0>());
			}
			template<typename... others>
			index_type index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const {
				Pos_ = hmLib::algorithm::positive_mod(Pos_, Size);
				return Lower.index_calc(Sum_ + Pos_ * Step_, Step_*Size + Lower.gap<0>(), Others...);
			}
			template<typename point_t>
			index_type point_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				index_type Pos = hmLib::algorithm::positive_mod(Point_[No_], Size);
				return Lower.point_index_calc(Sum_ + Pos * Step_, Step_*Size + Lower.gap<0>(), No_ + 1, Point_);
			}
			template<typename... others>
			index_type operator_index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const {
				Pos_ = hmLib::algorithm::positive_mod(Pos_, Size);
				return Lower.operator_index_calc(Sum_ + Pos_ * Step_, Step_*Size + Lower.gap<0>(), Others...);
			}
			template<typename point_t>
			index_type point_operator_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				index_type Pos = hmLib::algorithm::positive_mod(Point_[No_], Size);
				return Lower.point_index_calc(Sum_ + Pos * Step_, Step_*Size + Lower.gap<0>(), No_ + 1, Point_);
			}
			template<unsigned int req_dim_, typename T = void>
			struct dim_solver {
				size_type size(const this_type& This) { return This.Lower.size<req_dim_ - 1>(); }
				diff_type gap(const this_type& This) { return This.Lower.gap<req_dim_ - 1>(); }
				void resize(this_type& This, size_type Size_) { This.Lower.resize<req_dim_ - 1>(Size_); }
				void resize(this_type& This, size_type Size_, diff_type Gap_) { This.Lower.resize<req_dim_ - 1>(Size_, Gap_); }
				void set_gap(this_type& This, diff_type Gap_) { This.Lower.set_gap<req_dim_ - 1>(Gap_); }
			};
			template<typename T>
			struct dim_solver<0, T> {
				size_type size(const this_type& This) { return This.Size; }
				diff_type gap(const this_type& This) { return This.Gap; }
				void resize(this_type& This, size_type Size_) { This.Size = Size_; }
				void resize(this_type& This, size_type Size_, diff_type Gap_) { This.Size = Size_; This.Gap = Gap_; }
				void set_gap(this_type& This, diff_type Gap_) { This.Gap = Gap_; }
			};
		};
		template<>
		struct torus_lattice_indexer<0> {
			friend struct torus_lattice_indexer<1>;
			using this_type = torus_lattice_indexer<0>;
		public:
			torus_lattice_indexer() :Base(0) {}
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
			template<unsigned int req_dim_>
			diff_type gap()const {
				return 0;
			}
			diff_type base()const { return Base; }
			void set_base(diff_type Base_) { Base = Base_; }
		private:
			diff_type Base;
		private:
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
			index_type point_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				return Sum_ + Base;
			}
			template<typename point_t>
			index_type point_operator_index_calc(index_type Sum_, diff_type Step_, index_type No_, const point_t& Point_)const {
				return Sum_ + Base;
			}
		};
	}
}
#
#endif
