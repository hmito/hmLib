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
			lattice_indexer(size_type  Size_, others... Others) : Size(Size_), Lower(Others...){
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
			}
			template<typename... others>
			index_type index(index_type Pos_, others... Others)const{
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "Pos is larger than Size.");
				return Pos_ + Lower.index(Others...)*Lower.size<0>();
			}
			template<typename... others>
			index_type operator()(index_type Pos_, others... Others)const{
				static_assert(sizeof...(others)+1 == dim_, "The argument number is different from the dim number");
				return Pos_ + Lower(Others...)*Lower.size<0>();
			}
			std::pair<index_type, index_type> lattice_range()const{ return std::pair<index_type,index_type>(0, size()); }
			index_type lattice_size()const{ return size(); }
			size_type size()const{ return Size*Lower.size();}
			template<typename req_dim_>
			size_type dim_size()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return dim_size_getter<req_dim_>(*this);
			}
		private:
			template<unsigned int req_dim_, typename T = void>
			struct dim_size_getter{
				size_type operator()(const this_type& This){ return This.Lower.dim_size<req_dim_ - 1>(); }
			};
			template<typename T>
			struct dim_size_getter<0, T>{
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
			index_type index()const{
				return 0;
			}
			index_type operator()(void)const{
				return 0;
			}
			size_type lattice_size()const{ return 1; }
			template<typename req_dim_>
			size_type size()const{ return 1; }
		};
	}
}
#
#endif
