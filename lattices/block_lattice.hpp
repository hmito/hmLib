#ifndef HMLIB_LATICES_BLOCKLATTICE_INC
#define HMLIB_LATICES_BLOCKLATTICE_INC 100
#include<vector>
#include"utility.hpp"
#include"exceptions.hpp"
#include"indexer.hpp"
#include"iterator.hpp"
#include"locator.hpp"
#include"lattice.hpp"
namespace hmLib {
	template<typename T, unsigned int dim_>
	struct block_lattice {
	private:
		using this_type = block_lattice<T, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
		using indexer = lattices::indexer<dim_>;
	public:
		using element_type = typename std::decay<T>::type;
		using element_reference = element_type&;
		using element_const_reference = const element_type&;
		using element_pointer = element_type*;
		using element_const_pointer = const element_type*;
		using value_type = std::pair<point_type, element_type>;
		using reference = std::pair<point_type, element_reference>;
		using const_reference = std::pair<point_type, element_const_reference>;
		using pointer = std::pair<point_type, element_pointer>;
		using const_pointer = std::pair<point_type, element_const_pointer>;
	public:
		struct block {
			using container = std::vector<element_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		public:
			block(point_type Pos_, container Data_):Pos(Pos_), Data(Data_) {}
			iterator begin() { return Data.begin(); }
			iterator end() { return Data.end(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return Data.begin(); }
			const_iterator cend()const { return Data.end(); }
			point_type point()const { return Pos; }
		public:
			element_reference at(point_type Pos_) {
				return Data[Indexer.torus_index(Pos)];
			}
			element_const_reference at(point_type Pos_)const {
				return Data[Indexer.torus_index(Pos)];
			}
			template<typename... others>
			element_reference at(index_type Pos_, others... Others_) {return at(lattices::point(Pos_, Others_...));}
			template<typename... others>
			element_const_reference at(index_type Pos_, others... Others_)const { return at(lattices::point(Pos_, Others_...)); }
			element_reference ref(point_type Pos_) {
				return at(Pos);
			}
			element_const_reference ref(point_type Pos_)const {
				return at(Pos);
			}
			template<typename... others>
			element_reference ref(index_type Pos_, others... Others_) { return ref(lattices::point(Pos_, Others_...)); }
			template<typename... others>
			element_const_reference ref(index_type Pos_, others... Others_)const { return ref(lattices::point(Pos_, Others_...)); }
			element_reference operator[](point_type Pos_) {return ref(Pos);}
			element_const_reference operator[](point_type Pos_)const {return ref(Pos);}
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return Indexer.lattice_size(); }
			//!Get point_type Size
			const extent_type& extent()const { return Indexer.extent(); }
			//!Convert from index to point
			point_type index_to_point(index_type Index_)const { return Indexer.point(Index_); }
			//!Convert from point to index
			index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			reference index_at(index_type Index_) { return Data.at(Index_); }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			const_reference index_at(index_type Index_)const { return Data.at(Index_); }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			reference index_ref(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			const_reference index_ref(index_type Index_)const { return Data[Index_]; }
		private:
			point_type Pos;
			indexer Indexer;
			container Data;
		};
	private:
		using block_container = std::vector<block>;
	public:
		using block_iterator = typename block_container::iterator;
		using block_const_iterator = typename block_container::const_iterator;
		using element_iterator = typename block::iterator;
		using element_const_iterator = typename block::const_iterator;
		struct iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::reference;
			using pointer = typename this_type::pointer;
			using iterator_category = std::forward_iterator_tag;
		public:
			reference operator*() { return reference(*Itr; }
			pointer operator->() { return &operator*(); }
			iterator& operator++() {
				++Itr;
				if(Itr==End) {
					++BItr;
					Itr = BItr->second.begin();
					End = BItr->second.end();
				}
			}
			iterator operator++(int) {
				iterator Ans = *this;
				operator++();
				return Ans;
			}
			friend operator==(iterator v1, iterator v2) {
				return v1.Itr == v2.Itr;
			}
			friend operator!=(iterator v1, iterator v2) {
				return v1.Itr != v2.Itr;
			}
		private:
			using block_iterator = typename blocks::iterator;
			using element_iterator = typename blocks::element_iterator;
		private:
			block_iterator BItr;
			element_iterator Itr;
			element_iterator Beg;
			element_iterator End;
		};
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(point_type Point_) {
			auto Itr = block_find(Point_/Indexer.extent());
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(point_type Point_)const {
			auto Itr = block_find(Point_/Indexer.extent());
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
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
		reference ref(point_type Point_) {
			auto Itr = block_get(Point_/Indexer.extent());
			return Itr->at(Point_);
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](point_type Point_) {
			return ref(Point_);
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return Blocks.size()*BlockSize; }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { 
			return Blocks.at(static_cast<index_type>(Index_/BlockSize)).point() + Indexer.point(Index_%BlockSize);
		}
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const {
			auto Itr = block_find(Point_/Indexer.extent());
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return std::distance(block_begin(), Itr)*BlockSize + Indexer.torus_index(Point_);
		}
		//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
		reference index_at(index_type Index_) {
			return Blocks.at(static_cast<index_type>(Index_/BlockSize)).index_ref(Index_%BlockSize);
		}
		//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
		const_reference index_at(index_type Index_)const {
			return Blocks.at(static_cast<index_type>(Index_/BlockSize)).index_ref(Index_%BlockSize);
		}
		//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
		reference index_ref(index_type Index_) {
			return Blocks[static_cast<index_type>(Index_/BlockSize)].index_ref(Index_%BlockSize);
		}
	public://manipulation related to block
		//!Get block size
		extent_type block_extent()const { return Indexer.block_extent(); }
		//!Get block number
		std::size_t block_num()const { return Blocks.size(); }
		//!Get block access iterator at begin
		block_iterator block_begin() { return Blocks.begin(); }
		//!Get block access iterator at begin
		block_const_iterator block_begin()const { return Blocks.begin(); }
		//!Get block access iterator at end
		block_iterator block_end() { return End; }
		//!Get block access iterator at end
		block_const_iterator block_end()const { return End; }
		void block_erase(point_type Pos_) {
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });

			if(Itr!=block_end() && Itr->first == Pos_) {
				erase(Itr);
			}
		}
		void block_erase(block_iterator Itr) {
			if(Itr==block_end())return;
			std::rotate(Itr, Itr+1, End);
			--End;
		}
		//!Remove block
		template<typename block_condition_>
		void block_erase_if(block_condition_ BlockCondition){
			End = std::remove_if(block_begin(), block_end(), BlockCondition);
		}
	private:
		block_iterator block_find(point_type Pos_) {
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();
			return Itr;
		}
		block_const_iterator block_find(point_type Pos_)const {
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();
			return Itr;
		}
		block_iterator block_get(point_type Pos_) {
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });

			//fail to find
			if(Itr==block_end() || Itr->point() != Pos_) {
				//Still remain more than one element
				if(std::next(End)!=Blocks.end()) {
					End->assign(Pos_);
					std::rotate(Itr, End, End+1);
					++End;
				} else {
					Itr = Data.insert(Itr, block(Pos_, Indexer, BlockSize));
					End = Data.end()-1;
				}
			}

			return Itr;
		}
	private:
		block_container Blocks;
		block_iterator End;
		indexer Indexer;
		std::size_t BlockSize;
	};
}
#
#endif
