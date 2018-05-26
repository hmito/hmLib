#ifndef HMLIB_LATICES_BLOCKLATTICE_INC
#define HMLIB_LATICES_BLOCKLATTICE_INC 100
#include<vector>
#include"utility.hpp"
#include"lattice.hpp"
namespace hmLib{
	template<typename T, unsigned int dim_>
	struct block_lattice{
	private:
		using this_type = block_lattice<T, dim_>;
	public:
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename std::decay<T>::type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	public:
		using indexer = lattices::indexer<dim_>;
		using locator = lattices::basic_locator<iterator_base, indexer>;
		using const_locator = lattices::basic_locator<const_iterator_base, indexer>;
		using iterator = lattices::basic_iterator<iterator_base,indexer>;
		using const_iterator = lattices::basic_iterator<const_iterator_base, indexer>;
		using view = sublattice_view<iterator_base, dim_>;
		using const_view = sublattice_view<const_iterator_base, dim_>;
	public:
		static constexpr unsigned int dim(){return dim_;}
		struct block{
			using data_type = std::vector<value_type>;
			using data_iterator = typename data_type::iterator;
			using data_const_iterator = typename data_type::const_iterator;
		public:
			block(point_type Pos_, std::size_t BufferSize_, value_type IniVal_):Pos(Pos_),Data(BufferSize_,IniVal_){}
			data_iterator begin();
			data_iterator end();
			data_const_iterator begin()const;
			data_const_iterator end()const;
			data_const_iterator cbegin()const;
			data_const_iterator cend()const;
			point_type point()const{return Pos;}
			void assign(point_type Pos_, value_type IniVal_){ Pos=Pos_; std::fill(Data.begin(),Data.end(), IniVal_); }
		private:
			point_type Pos;
			data_type Data;
		};
	private:
		struct block_container{
		public:
			using container = std::vector<block>;
			using iterator = typename container::const_iterator;
		private:
			struct block_less{
				point_type Pos;
				block_less(point_type Pos_):Pos(Pos_){}
				bool operator()(const block& Blck){return Blck.point()<Pos;}
			};
		public:
			iterator find(point_type Pos_){
				auto Itr = std::partition_point(begin(),end(), block_less(Pos_));
				if(Itr != end() && Itr->point() != Pos_) return end();
				return Itr;
			}
			iterator get(point_type Pos_){
				auto Itr = std::partition_point(begin(),end(),block_less(Pos_));

				//fail to find
				if(Itr==end() || Itr->point() != Pos_){
					if(End!=Data.end()){
						End->assign(Pos_);
						std::rotate(Itr, End, End+1);
						++End;
					}else{
						Itr = Data.insert(Itr,block(Pos_,std::vector<T>(BlockSize,0.0)));
						End = Data.end();
					}
				}

				return Itr;
			}
			void erase(point_type Pos_){
				auto Itr = std::partition_point(begin(),end(),block_less(Pos_));

				if(Itr!=end() && Itr->point() == Pos_){
					erase(Itr);
				}
			}
			void erase(iterator Itr){
				if(Itr==end())return;
				std::rotate(Itr,Itr+1,End);
				--End;
			}
			iterator begin()const{return Blocks.begin();}
			iterator end()const{return Blocks.end();}
		private:
			container Blocks;
			iterator End;
			std::size_t BlockSize;
			extent_type BlockExtent;
		};
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_) {
			return Blocks.get(Point_/block_extent())[Indexer.torus_index(Point_)];
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const point_type& Point_)const  {
			return Blocks.get(Point_/block_extent())[Indexer.torus_index(Point_)];
		}
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_) {
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_reference at(index_type Pos_, others... Others_)const {
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_) {
			return Blocks.get(Point_/block_extent())[[Indexer.torus_index(Point_)];
		}
		//!Return const_reference of the elemtn at the given point
		const_reference operator[](const point_type& Point_)const{
			return Blocks.get(Point_/block_extent())[Indexer.torus_index(Point_)];
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return operator[](lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point
		template<typename... others>
		const_reference ref(index_type Pos_, others... Others_)const {
			return operator[](lattices::point(Pos_, Others_...));
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return Indexer.lattice_size(); }
		//!Get point_type Size
		const extent_type& extent()const;
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return Indexer.point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
		//!Get block size
		extent_type block_extent()const{Indexer.extent();}

	};
}
#
#endif
