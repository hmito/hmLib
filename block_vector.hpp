#ifndef HMLIB_BLOCKVECTOR_INC
#define HMLIB_BLOCKVECTOR_INC
#
#include<vector>
#include<algorithm>
#include<utility>
#include"algorithm/compare.hpp"
#include"exceptions.hpp"
#include"iterators/integer_iterator.hpp"
namespace hmLib{
	template<typename T>
	struct block_vector {
	private:
		using this_type = block_vector<T>;
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using index_type = int;
		using size_type = std::size_t;
	public:
		struct block {
			friend struct block_vector<T>;
			using container = std::vector<value_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		public:
			block() = default;
			block(index_type Pos_, std::size_t Size_):Pos(Pos_), Data(Size_, 0) {}
		public:
			iterator begin() { return Data.begin(); }
			iterator end() { return Data.end(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return Data.begin(); }
			const_iterator cend()const { return Data.end(); }
			reference at(index_type Pos_) {
				hmLib_assert(inside(Pos),hmLib::access_exceptions::out_of_range_access,"out of range access");
				return Data[Pos_-Pos];
			}
			const_reference at(index_type Pos_)const {
				hmLib_assert(inside(Pos),hmLib::access_exceptions::out_of_range_access,"out of range access");
				return Data[Pos_-Pos];
			}
			reference ref(index_type Pos_) {return Data[Pos_-Pos];}
			const_reference ref(index_type Pos_)const {return Data[Pos_-Pos];}
			reference operator[](index_type Pos_) {return ref(Pos_);}
			const_reference operator[](index_type Pos_)const {return ref(Pos_);}
		public:
			index_type index(const_iterator itr)const { return Pos+std::distance(Data.begin(),itr); }
			index_type index_begin()const { return Pos; }
			index_type index_end()const { return Pos+Data.size(); }
			integer_range<index_type> index_range()const{return integer_range<index_type>(Pos,Pos+size()-1);} 
			size_type size()const { return Data.size(); }
			//!Return true if the given point is inclueded incide of this block.
			bool inside(index_type Pos_)const {
				return Pos <= Pos_ && static_cast<std::size_t>(Pos_)< Pos+size();
			}
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			reference local_at(index_type Index_) { return Data.at(Index_); }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			const_reference local_at(index_type Index_)const { return Data.at(Index_); }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			reference local_ref(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			const_reference local_ref(index_type Index_)const { return Data[Index_]; }
		private:
			void assign(index_type Pos_, unsigned int Size_) {
				Pos = Pos_;
				Data.assign(Size_,0);
			}
		private:
			index_type Pos;
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
			using iterator_category = std::random_access_iterator_tag;
		public:
			const_iterator() = default;
			const_iterator(block_const_iterator BItr_, element_const_iterator Itr_):BItr(BItr_), Itr(Itr_) {}
		public:
			reference operator*() {
				return Itr.operator*();
			}
			reference operator[](difference_type n) {
				return (*this + n).operator*();
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
			const_iterator& operator+=(difference_type n) {
				if(n<0)return operator-=(-n);

				if(n < std::distance(Itr, BItr->end())) {
					Itr += n;
				}else{
					auto BlockSize = BItr->size();
					n -= std::distance(Itr, BItr->end());
					++BItr;

					BItr += n/BlockSize;
					Itr = std::next(BItr->begin(), n%BlockSize);
				}
				return *this;
			}
			const_iterator& operator-=(difference_type n) {
				if(n<0)return operator+=(-n);

				if(n < std::distance(BItr->begin(), Itr)) {
					Itr -= n;
				} else {
					n -= std::distance(BItr->begin(), Itr);
					--BItr;
					auto BlockSize = BItr->size();

					BItr -= n/BlockSize;
					Itr = std::next(BItr->end()-1, -(n%BlockSize));
				}
				return *this;
			}
			friend const_iterator operator+(const const_iterator& v, difference_type n) {
				auto v2 = v;
				v2 += n;
				return v2;
			}
			friend const_iterator operator+(difference_type n, const const_iterator& v) {
				return v+n;
			}
			friend const_iterator operator-(const const_iterator& v, difference_type n) {
				auto v2 = v;
				v2 -= n;
				return v2;
			}
			friend difference_type operator-(const const_iterator& v1, const const_iterator&  v2) {
				if(v1.BItr > v2.BItr)return std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*(v2.Bitr->size());
				else if(v1.BItr < v2.BItr)return std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*(v1.Bitr->size());
				else return v1.Itr - v2.Itr;
			}
			friend bool operator==(const const_iterator& v1, const const_iterator&  v2) {
				return v1.BItr == v2.BItr && v1.Itr == v2.Itr;
			}
			friend bool operator!=(const const_iterator&  v1, const const_iterator&  v2) {
				return v1.BItr != v2.BItr || v1.Itr != v2.Itr;
			}
			friend bool operator< (const const_iterator& v1, const const_iterator&  v2) {
				if(v1.BItr < v2.BItr) return true;
				else if(v1.BItr > v2.BItr) return false;
				else return v1.Itr < v2.Itr;
			}
			friend bool operator<=(const const_iterator& v1, const const_iterator&  v2) {
				if(v1.BItr < v2.BItr) return true;
				else if(v1.BItr > v2.BItr) return false;
				else return v1.Itr <= v2.Itr;
			}
			friend bool operator> (const const_iterator& v1, const const_iterator&  v2) {
				return operator<(v2, v1);
			}
			friend bool operator>=(const const_iterator& v1, const const_iterator&  v2) {
				return operator<=(v2, v1);
			}
		public:
			index_type index()const { return BItr->index(Itr); }
			block_const_iterator block_itr() { return BItr; }
			element_const_iterator element_itr() { return Itr; }
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
			using iterator_category = std::random_access_iterator_tag;
		public:
			iterator() = default;
			iterator(block_iterator BItr_, element_iterator Itr_):BItr(BItr_),Itr(Itr_){}
			operator const_iterator() { return const_iterator(BItr, Itr); }
		public:
			reference operator*() {
				return Itr.operator*();
			}
			reference operator[](difference_type n) {
				return (*this + n).operator*();
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
			iterator& operator+=(difference_type n) {
				if(n<0)return operator-=(-n);

				if(n < std::distance(Itr, BItr->end())) {
					Itr += n;
				} else {
					auto BlockSize = BItr->size();
					n -= std::distance(Itr, BItr->end());
					++BItr;

					BItr += n/BlockSize;
					Itr = std::next(BItr->begin(), n%BlockSize);
				}
				return *this;
			}
			iterator& operator-=(difference_type n) {
				if(n<0)return operator+=(-n);

				if(n < std::distance(BItr->begin(), Itr)) {
					Itr -= n;
				} else {
					n -= std::distance(BItr->begin(), Itr);
					--BItr;
					auto BlockSize = BItr->size();

					BItr -= n/BlockSize;
					Itr = std::next(BItr->end()-1, -(n%BlockSize));
				}
				return *this;
			}
			friend iterator operator+(const iterator& v, difference_type n) {
				auto v2 = v;
				v2 += n;
				return v2;
			}
			friend iterator operator+(difference_type n, const iterator& v) {
				return v+n;
			}
			friend iterator operator-(const iterator& v, difference_type n) {
				auto v2 = v;
				v2 -= n;
				return v2;
			}
			friend difference_type operator-(const iterator& v1, const iterator&  v2) {
				if(v1.BItr > v2.BItr)return std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*(v2.Bitr->size());
				else if(v1.BItr < v2.BItr)return std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*(v1.Bitr->size());
				else return v1.Itr - v2.Itr;
			}
			friend bool operator==(const iterator& v1, const iterator& v2) {
				return v1.BItr == v2.BItr && v1.Itr == v2.Itr;
			}
			friend bool operator!=(const iterator& v1, const iterator& v2) {
				return v1.BItr != v2.BItr || v1.Itr != v2.Itr;
			}
			friend bool operator< (const iterator& v1, const iterator&  v2) {
				if(v1.BItr < v2.BItr) return true;
				else if(v1.BItr > v2.BItr) return false;
				else return v1.Itr < v2.Itr;
			}
			friend bool operator<=(const iterator& v1, const iterator&  v2) {
				if(v1.BItr < v2.BItr) return true;
				else if(v1.BItr > v2.BItr) return false;
				else return v1.Itr <= v2.Itr;
			}
			friend bool operator> (const iterator& v1, const iterator&  v2) {
				return operator<(v2, v1);
			}
			friend bool operator>=(const iterator& v1, const iterator&  v2) {
				return operator<=(v2, v1);
			}
		public:
			index_type index()const { return BItr->index(Itr); }
			block_iterator block_itr() { return BItr; }
			element_iterator element_itr() { return Itr; }
		private:
			block_iterator BItr;
			element_iterator Itr;
		};
	public:
		block_vector()noexcept:block_vector(size_type(10)) {}
		explicit block_vector(size_type Size_)noexcept:Blocks(), HintPos(0), BlockNum(0), BlockSize(Size_){}
	public:
		bool empty()const{ return BlockNum==0; }
		void clear(){BlockNum = 0; }
		size_type size()const { return block_num()*block_size(); }
		void resize(size_type Size_){
			if(Blocks.empty()) {
				BlockSize = Size_;
			} else {
				this_type New(BlockSize);
				auto End = end();
				for(auto Itr = begin(); Itr!=End; ++Itr) {
					New[Itr.point()] = *Itr;
				}
				*this = std::move(New);
			}
		}
	public://manipulation related to block
		//!Get block size
		size_type block_size()const { return BlockSize; }
		//!Get block number
		size_type block_num()const { return BlockNum; }
		//!Get block access iterator at begin
		block_iterator block_begin() { return Blocks.begin(); }
		//!Get block access iterator at begin
		block_const_iterator block_begin()const { return Blocks.begin(); }
		//!Get block access iterator at end
		block_iterator block_end() { return Blocks.begin()+BlockNum; }
		//!Get block access iterator at end
		block_const_iterator block_end()const { return Blocks.begin()+BlockNum; }
		//!Remove block
		void block_erase(block_iterator Itr) {
			if(Itr == block_end())return;
			if(std::distance(block_begin(), Itr)<=HintPos) {
				--HintPos;
			}
			std::rotate(Itr, Itr+1, block_end());
			--BlockNum;
		}
		//!Remove block
		template<typename block_condition_>
		void block_erase_if(block_condition_ BlockCondition){
			BlockNum = std::distance(
				block_begin(),
				std::partition(block_begin(), block_end(), [cond = std::move(BlockCondition)](block& b){return !BlockCOndition(b); })
			);
			HintPos = 0;
		}
		template<typename element_condition_>
		void block_erase_if_all_of(element_condition_ ElementConditiion) {
			BlockNum = std::distance(
				block_begin(),
				std::partition(block_begin(), block_end(), [cond = std::move(ElementConditiion)](block& b) {return !std::all_of(b.begin(), b.end(), cond); })
			);
			HintPos = 0;
		}
		std::size_t block_capacity()const{ return std::max(Blocks.size(),1)-1;}
		void block_reserve(std::size_t n)const{
			while(block_capacity() < n){
				Blocks.push_back(block(0, BlockSize));
			}
		}
		void block_shrink_to_fit()const{
			while(block_num() < block_capacity()){
				Blocks.pop_back();
			}
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(index_type n) {
			auto Itr = block_find(n,block_begin()+HintPos);
			hmLib_assert(Itr!=block_end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->at(n);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(index_type n)const {
			auto Itr = block_find(n, block_begin()+HintPos);
			hmLib_assert(Itr!=block_end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->at(n);
		}
		//!Return reference of the elemtn at the given point with range check
		reference at(iterator Hint_, index_type n) {
			auto Itr = block_find(n, Hint_.block_itr());
			hmLib_assert(Itr!=block_end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->at(n);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const_iterator Hint_, index_type n)const {
			auto Itr = block_find(n, Hint_.block_itr());
			hmLib_assert(Itr!=block_end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->at(n);
		}
		//!Return reference of the elemtn at the given point with range check
		iterator find(index_type n) {
			auto Itr = block_find(n, block_begin()+HintPos);
			if(Itr==block_end())return end();
			return iterator(Itr, Itr->begin()+n);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(index_type n)const {
			auto Itr = block_find(n, block_begin()+HintPos);
			if(Itr==block_end())return end();
			return const_iterator(Itr, Itr->begin()+n);
		}
		//!Return reference of the elemtn at the given point with range check
		iterator find(iterator Hint_, index_type n) {
			auto Itr = block_find(n, Hint_.block_itr());
			if(Itr==block_end())return end();
			return iterator(Itr, Itr->begin()+n);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(const_iterator Hint_, index_type n)const {
			auto Itr = block_find(n, Hint_.block_itr());
			if(Itr==block_end())return end();
			return const_iterator(Itr, Itr->begin()+n);
		}
		//!Return reference of the elemtn at the given point
		reference ref(index_type n) {
			auto Itr = block_get(n, block_begin()+HintPos);
			return Itr->ref(n);
		}
		//!Return reference of the elemtn at the given point
		reference ref(iterator Hint_, index_type n) {
			auto Itr = block_get(n, Hint_);
			return Itr->ref(n);
		}
		//!Return reference of the elemtn at the given point
		reference operator[](index_type n) {
			return ref(n);
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
	private:
		block_iterator block_find(index_type n) {
			n = euclidean_div(n, static_cast<int>(BlockSize))*BlockSize;
			auto Itr = std::partition_point(block_begin(), block_end(), [n](const block& Block) {return Block.index_begin()<n; });
			if(Itr != block_end() && Itr->index_begin() != n) return block_end();			
			HintPos = std::distance(block_begin(), Itr);
			return Itr;
		}
		block_const_iterator block_find(index_type n)const {
			n = euclidean_div(n, static_cast<int>(BlockSize))*BlockSize;
			auto Itr = std::partition_point(block_begin(), block_end(), [n](const block& Block) {return Block.index_begin()<n; });
			if(Itr != block_end() && Itr->index_begin() != n) return block_end();
			HintPos = std::distance(block_begin(), Itr);
			return Itr;
		}
		block_iterator block_find(index_type n, block_iterator Hint_) {
			if(Hint_!=block_end() && Hint_->inside(n)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_find(n);
		}
		block_const_iterator block_find(index_type n, block_const_iterator Hint_) {
			if(Hint_!=block_end() && Hint_->inside(n)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_find(n);
		}
		block_iterator block_get(index_type n) {
			n = euclidean_div(n, static_cast<int>(BlockSize))*BlockSize;

			//first time
			if(Blocks.empty()) {
				Blocks.push_back(block(n, BlockSize));
				Blocks.push_back(block());//should keep empty element in the end of container!!!
				BlockNum = 1;
				HintPos = 0;
				return Blocks.begin();
			}

			auto Itr = std::partition_point(block_begin(), block_end(), [n](const block& Block) {return Block.index_begin()<n; });

			//fail to find
			if(Itr==block_end() || Itr->index_begin() != n) {
				if(BlockNum+1 < Blocks.size()) {
					//Still remain more than one element
					Blocks[BlockNum].assign(n, BlockSize);
					std::rotate(Itr, block_end(), block_end()+1);
					++BlockNum;
				} else {
					Itr = Blocks.insert(Itr, block(n, BlockSize));
					++BlockNum;
				}
			}

			HintPos = std::distance(Blocks.begin(), Itr);
			return Itr;
		}
		block_iterator block_get(index_type n, block_iterator Hint_) {
			if(Hint_!=block_end() && Hint_->inside(n)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_get(n);
		}
	private:
		//BLocks should keep unused empty element in back()
		//	In other words, Blocks.end() != End should be satishied anytime
		//	This is because of end iterator's requirement.
		block_container Blocks;
		unsigned int HintPos;
		std::size_t BlockNum;
		std::size_t BlockSize;
	};
}
#
#endif
