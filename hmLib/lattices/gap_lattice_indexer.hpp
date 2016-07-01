#ifndef HMLIB_LATTICES_GAPLATTICEINDEXER_INC
#define HMLIB_LATTICES_GAPLATTICEINDEXER_INC 100
#
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib{
	namespace lattices{
		template<unsigned int dim_>
		struct gap_lattice_indexer{
			friend struct gap_lattice_indexer<dim_ + 1>;
			using this_type = gap_lattice_indexer<dim_>;
			using lower_type = gap_lattice_indexer<dim_ - 1>;
		public:
			static constexpr unsigned int dim(){ return dim_; }
		private:
			size_type Size;
			diff_type Gap;
			lower_type Lower;
		public:
			gap_lattice_indexer() : Size(0), Lower(){}
			template<typename... others>
			gap_lattice_indexer(size_type  Size_, others... Others) : Size(Size_), Lower(Others...){}
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const{
				return index_step(0, 1 + Gap, Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const{
				return operator_index_calc(0, 1 + Gap, Pos_, Others...);
			}
			std::pair<index_type, index_type> lattice_range()const{ return lattice_pos_size_calc(0, 0, 1 + Gap); }
			index_type lattice_size()const{ auto Pair = lattice_pos_size_calc(0, 0, 1 + Gap); return Pair.second - Pair.first; }
			size_type size()const{ return Size*Lower.size(); }
			template<typename req_dim_>
			size_type dim_size()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return dim_size_getter<req_dim_>(*this);
			}
			template<typename req_dim_>
			diff_type dim_gap()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return dim_gap_getter<req_dim_>(*this);
			}
		private:
			std::pair<index_type, index_type> lattice_range_calc(index_type Min_, index_type Max_, diff_type Step_){
				if(Step_ < 0)Min_ = std::min(Min_, Step_ * Size);
				else Max_ = std::max(Max_, Step_ * Size);

				return Lower.lattice_range_calc(Min_, Max_, Step_ * Size + Lower.dim_gap<0>());
			}
			template<typename... others>
			index_type index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const{
				hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "Pos is larger than Size.");
				return Lower.index_calc(Sum_ + Pos_ * Step_, Step_*Size + Lower.gap<0>(), Others...);
			}
			template<typename... others>
			index_type operator_index_calc(index_type Sum_, diff_type Step_, index_type Pos_, others... Others)const{
				return Lower.operator_index_calc(Sum_ + Pos_ * Step_, Step_*Size + Lower.gap<0>(), Others...);
			}
			template<unsigned int req_dim_, typename T = void>
			struct dim_size_getter{
				size_type operator()(const this_type& This){ return This.Lower.dim_size<req_dim_ - 1>(); }
			};
			template<typename T>
			struct dim_size_getter<0, T>{
				size_type operator()(const this_type& This){ return This.Size; }
			};
			template<unsigned int req_dim_, typename T = void>
			struct dim_gap_getter{
				diff_type operator()(const this_type& This){ return This.Lower.dim_gap<req_dim_ - 1>(); }
			};
			template<typename T>
			struct dim_gap_getter<0, T>{
				diff_type operator()(const this_type& This){ return This.Gap; }
			};
		};
		template<>
		struct gap_lattice_indexer<0>{
			friend struct gap_lattice_indexer<1>;
			using this_type = gap_lattice_indexer<0>;
		public:
			gap_lattice_indexer() = default;
			template<typename... others>
			gap_lattice_indexer(others... Others){
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::lattice_indexer is not enough");
			}
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const{
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::index is not enough");
				return operator()(Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const{return 0;}
			size_type size()const{ return 1; }
			template<typename req_dim_>
			size_type dim_size()const{
				return 1;
			}
			template<typename req_dim_>
			diff_type dim_gap()const{
				return 0;
			}
		private:
			std::pair<index_type, index_type> lattice_range_calc(index_type Min_, index_type Max_, diff_type Step_){
				return std::pair<index_type, size_type>(Min_, Max_);
			}
			index_type index_calc(index_type Sum_, diff_type Step_, index_type Pos_)const{
				return Sum_;
			}
			index_type operator_index_calc(index_type Sum_, diff_type Step_, index_type Pos_)const{
				return Sum_;
			}
		};
	}
}
#
#endif
