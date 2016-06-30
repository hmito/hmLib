#ifndef HMLIB_LATTICES_LATTICEINDEXER_INC
#define HMLIB_LATTICES_LATTICEINDEXER_INC 100
#
#include "exceptions.hpp"
#include "utility.hpp"
namespace hmLib{
	namespace lattices{
		template<unsigned int dim_>
		struct lattice_indexer{
			using this_type = lattice_indexer<dim_>;
			using lower_type = lattice_indexer<dim_ - 1>;
		public:
			static constexpr unsigned int dim(){ return dim_; }
		private:
			lower_type Lower;
			size_type Size;
		public:
			lattice_indexer() : Size(0), Lower(){}
			template<typename... others>
			lattice_indexer(size_type  Size_, others... Others) : Size(Size_), Lower(Others...){}
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const{
				hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "Pos is larger than Size.");
				return operator()(Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const{
				return Pos_ + Lower.at(Others...)*Lower.size<0>();
			}
			size_type lattice_size()const{ return Size*Lower.lattice_size();}
			template<typename req_dim_>
			diff_type step()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return step_getter<req_dim_>(*this);
			}
			template<typename req_dim_>
			size_type size()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return size_getter<req_dim_>(*this);
			}
		private:
			template<unsigned int req_dim_, typename T = void>
			struct step_getter{
				diff_type operator()(const this_type& This){ return This.Lower.step<req_dim_ - 1>(); }
			};
			template<typename T>
			struct step_getter<0, T>{
				diff_type operator()(const this_type& This){ return This.Lower.size()*This.Lower.step(); }
			};
			template<unsigned int req_dim_, typename T = void>
			struct size_getter{
				size_type operator()(const this_type& This){ return This.Lower.size<req_dim_ - 1>(); }
			};
			template<typename T>
			struct size_getter<0, T>{
				size_type operator()(const this_type& This){ return This.Size; }
			};
		};
		template<>
		struct lattice_indexer<0>{
			using this_type = lattice_indexer<0>;
		public:
			lattice_indexer() = default;
			template<typename... others>
			lattice_indexer(others... Others){
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::lattice_indexer is not enough");
			}
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const{
				static_assert(sizeof...(Others) == 0, "Argument for lattice_indexer::index is not enough");
				return operator()(Pos_, Others...);
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const{
				return 0;
			}
			size_type lattice_size()const{ return 1; }
			template<typename req_dim_>
			diff_type step()const{ return 1;}
			template<typename req_dim_>
			size_type size()const{ return 1; }
		};
	}
}
#
#endif
