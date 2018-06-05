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
		using value_type = typename std::decay<T>::type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	public:
		struct block {
			friend struct block_lattice<T, dim_>;
			using container = std::vector<value_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		public:
			block(point_type Pos_, indexer Indexer_, unsigned int Size_):Pos(Pos_), Indexer(Indexer_), Data(Size_, 0) {}
			iterator begin() { return Data.begin(); }
			iterator end() { return Data.end(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return Data.begin(); }
			const_iterator cend()const { return Data.end(); }
			point_type point()const { return Pos; }
			point_type point(const_iterator Itr)const { return Pos + Indexer.calc_point(std::distance(begin(),Itr)); }
		public:
			reference at(point_type Pos_) {
				return Data[Indexer.torus_index(Pos_)];
			}
			const_reference at(point_type Pos_)const {
				return Data[Indexer.torus_index(Pos_)];
			}
			template<typename... others>
			reference at(index_type Pos_, others... Others_) {return at(lattices::point(Pos_, Others_...));}
			template<typename... others>
			const_reference at(index_type Pos_, others... Others_)const { return at(lattices::point(Pos_, Others_...)); }
			reference ref(point_type Pos_) {
				return at(Pos_);
			}
			const_reference ref(point_type Pos_)const {
				return at(Pos_);
			}
			template<typename... others>
			reference ref(index_type Pos_, others... Others_) { return ref(lattices::point(Pos_, Others_...)); }
			template<typename... others>
			const_reference ref(index_type Pos_, others... Others_)const { return ref(lattices::point(Pos_, Others_...)); }
			reference operator[](point_type Pos_) {return ref(Pos_);}
			const_reference operator[](point_type Pos_)const {return ref(Pos_);}
		public:
			//!Get number of elements included in the lattice
			size_type lattice_size()const { return Indexer.lattice_size(); }
			//!Get point_type Size
			const extent_type& extent()const { return Indexer.extent(); }
			//!Return true if the given point is inclueded incide of this block.
			bool inside(point_type Point_)const {
				for(unsigned int i = 0; i<dim_; ++i) {
					if(Point_[i]<Pos[i] || Pos[i]+Indexer.extent()[i]<=Point_[i])return false;
				}
				return true;
			}
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
			void assign(point_type Pos_, indexer Indexer_, unsigned int Size_) {
				Pos = Pos_;
				Indexer = Indexer_;
				Data.assign(Size_,0);
			}
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
		struct const_iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::const_reference;
			using pointer = typename this_type::const_pointer;
			using iterator_category = std::bidirectional_iterator_tag;
		public:
			const_iterator() = default;
			const_iterator(block_const_iterator BItr_, element_const_iterator Itr_):BItr(BItr_), Itr(Itr_) {}
		public:
			reference operator*() {
				return Itr.operator*();
			}
			pointer operator->() {
				return Itr.operator->();
			}
			const_iterator& operator++() {
				++Itr;
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator Ans = *this;
				operator++();
				return Ans;
			}
			const_iterator& operator--() {
				if(Itr == BItr->begin()) {
					--BItr;
					Itr = BItr->end();
				}
				--Itr;
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator Ans = *this;
				operator--();
				return Ans;
			}
			friend bool operator==(const const_iterator& v1, const const_iterator&  v2) {
				return v1.BItr == v2.BItr && v1.Itr == v2.Itr;
			}
			friend bool operator!=(const const_iterator&  v1, const const_iterator&  v2) {
				return v1.BItr != v2.BItr || v1.Itr != v2.Itr;
			}
		public:
			point_type point()const { return BItr->point(Itr); }
		public:
			block_iterator block_itr() { return BItr; }
			element_iterator element_itr() { return Itr; }
		private:
			block_const_iterator BItr;
			element_const_iterator Itr;
		};
		struct iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::reference;
			using pointer = typename this_type::pointer;
			using iterator_category = std::bidirectional_iterator_tag;
		public:
			iterator() = default;
			iterator(block_iterator BItr_, element_iterator Itr_):BItr(BItr_),Itr(Itr_){}
		public:
			reference operator*() {
				return Itr.operator*();
			}
			pointer operator->() {
				return Itr.operator->();
			}
			iterator& operator++() {
				++Itr;
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				return *this;
			}
			iterator operator++(int) {
				iterator Ans = *this;
				operator++();
				return Ans;
			}
			iterator& operator--() {
				if(Itr == BItr->begin()) {
					--BItr;
					Itr = BItr->end();
				}
				--Itr;
				return *this;
			}
			iterator operator--(int) {
				iterator Ans = *this;
				operator--();
				return Ans;
			}
			operator const_iterator() { return const_iterator(BItr, Itr); }
			friend bool operator==(const iterator& v1, const iterator& v2) {
				return v1.BItr == v2.BItr && v1.Itr == v2.Itr;
			}
			friend bool operator!=(const iterator& v1, const iterator& v2) {
				return v1.BItr != v2.BItr || v1.Itr != v2.Itr;
			}
		public:
			point_type point()const { return BItr->point(Itr); }
		public:
			block_iterator block_itr() { return BItr; }
			element_iterator element_itr() { return Itr; }
		private:
			block_iterator BItr;
			element_iterator Itr;
		};
	public:
		block_lattice() = delete;
		explicit block_lattice(extent_type BlockExtent) {
			assign(BlockExtent);
		}
		template<typename... others>
		explicit block_lattice(std::size_t Size_, others... Others_) {
			assign(extent_type{ Size_,static_cast<std::size_t>(Others_)... });
		}
		void assign(extent_type BlockExtent) {
			Indexer.resize(BlockExtent);
			BlockSize = Indexer.lattice_size();
			Blocks.assign(1, block(point_type(0), Indexer, BlockSize));
			End = Blocks.begin();
		}
		template<typename... others>
		void assign(std::size_t Size_, others... Others_) {
			assign(extent_type{ Size_,static_cast<std::size_t>(Others_)... });
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(point_type Point_) {
			auto Itr = block_find(Point_);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(point_type Point_)const {
			auto Itr = block_find(Point_);
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
		//!Return reference of the elemtn at the given point with range check
		reference at(iterator Hint_, point_type Point_) {
			auto Itr = block_find(Point_, Hint_);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const_iterator Hint_, point_type Point_)const {
			auto Itr = block_find(Point_, Hint_);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(iterator Hint_, index_type Pos_, others... Others_) {
			return at(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_reference at(const_iterator Hint_, index_type Pos_, others... Others_)const {
			return at(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(point_type Point_) {
			auto Itr = block_get(Point_);
			return Itr->at(Point_);
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(iterator Hint_, point_type Point_) {
			auto Itr = block_get(Point_, Hint_);
			return Itr->at(Point_);
		}
		template<typename... others>
		reference ref(iterator Hint_, index_type Pos_, others... Others_) {
			return ref(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](point_type Point_) {
			return ref(Point_);
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return block_num()*BlockSize; }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { 
			return Blocks.at(static_cast<index_type>(Index_/BlockSize)).point() + Indexer.point(Index_%BlockSize);
		}
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const {
			auto Itr = block_find(Point_);
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
	public:
		//!Return begin iterator fot the lattice
		iterator begin() { return iterator(block_begin(), block_begin()->begin()); }
		//!Return end iterator fot the lattice
		iterator end() { return iterator(block_end(), block_end()->begin()); }
		//!Return begin const_iterator fot the lattice
		const_iterator begin()const { return cbegin(); }
		//!Return end const_iterator fot the lattice
		const_iterator end()const { return cend(); }
		//!Return begin const_iterator fot the lattice
		const_iterator cbegin()const { return const_iterator(block_begin(), block_begin()->begin());}
		//!Return end const_iterator fot the lattice
		const_iterator cend()const { return const_iterator(block_end(), block_end()->begin());}
	public://manipulation related to block
		//!Get block size
		extent_type block_extent()const { return Indexer.extent(); }
		//!Get block number
		size_type block_num()const { return std::distance(block_begin(),block_end()); }
		//!Get block access iterator at begin
		block_iterator block_begin() { return Blocks.begin(); }
		//!Get block access iterator at begin
		block_const_iterator block_begin()const { return Blocks.begin(); }
		//!Get block access iterator at end
		block_iterator block_end() { return End; }
		//!Get block access iterator at end
		block_const_iterator block_end()const { return End; }
		//!Remove block
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
			Pos_ = Pos_ - (Pos_%Indexer.extent());
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();
			return Itr;
		}
		block_const_iterator block_find(point_type Pos_)const {
			Pos_ = Pos_ - (Pos_%Indexer.extent());
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();
			return Itr;
		}
		block_iterator block_find(point_type Pos_, iterator Hint_) {
			auto Itr = Hint_->block_itr();
			if(Itr->inside(Pos_))return Itr;
			return block_find(Pos_);
		}
		block_const_iterator block_find(point_type Pos_, const_iterator Hint_) {
			auto Itr = Hint_->block_itr();
			if(Itr->inside(Pos_))return Itr;
			return block_find(Pos_);
		}
		block_iterator block_get(point_type Pos_) {
			Pos_ = Pos_ - (Pos_%Indexer.extent());
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });

			//fail to find
			if(Itr==block_end() || Itr->point() != Pos_) {
				//Still remain more than one element
				if(std::next(End)!=Blocks.end()) {
					End->assign(Pos_, Indexer, BlockSize);
					std::rotate(Itr, End, End+1);
					++End;
				} else {
					Itr = Blocks.insert(Itr, block(Pos_, Indexer, BlockSize));
					End = Blocks.end()-1;
				}
			}

			return Itr;
		}
		block_iterator block_get(point_type Pos_, iterator Hint_) {
			auto Itr = Hint_->block_itr();
			if(Itr->inside(Pos_))return Itr;
			return block_get(Pos_);
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
