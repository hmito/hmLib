#ifndef HMLIB_LATICES_BLOCKLATTICE_INC
#define HMLIB_LATICES_BLOCKLATTICE_INC 100
#include<vector>
#include<functional>
#include<algorithm>
#include<boost/pool/pool.hpp>
#include"../algorithm/algorithm.hpp"
#include"utility.hpp"
#include"exceptions.hpp"
#include"indexer.hpp"
#include"iterator.hpp"
#include"locator.hpp"
#include"lattice.hpp"
namespace hmLib {
	template<typename T, unsigned int dim_, unsigned int block_interval_>
	struct block_lattice {
	private://type definition
		using this_type = block_lattice<T, dim_, block_interval_>;
		template<typename U>
		using other_type = block_lattice<U, dim_, block_interval_>;
	public://type definition
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
		using indexer_type = lattices::indexer<dim_>;
		using value_type = typename std::decay<T>::type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		static constexpr unsigned int dim() { return dim_; }
		static constexpr unsigned int block_interval(){return block_interval_;}
		static constexpr unsigned int block_size(){return hmLib::static_pow(block_interval_,dim);}
	public://block
		struct block_pool{
		private:
			std::vector<std::unique_ptr<T[]>> BlockPool;
		public:
			std::unique_ptr<T[]> get(){
				std::unique_ptr<T[]> Ans;
				if(BlockPool.empty()){
					Ans = std::make_unique<T[]>(lattice_size());
				}else{
					auto Block = std::move(BlockPool.back());
					BlockPool.pop_back();
				}
				std::fill(Ans.get(),Ans.get()+lattice_size(),0);
				return std::move(Block);
			}
			void release(std::unique_ptr<T[]>&& Block)noexcept{
				BlockPool.push_back(std::move(Block));
			}
		};
		static static_block_pool BlockPool;
		struct block {
			friend struct block_lattice<T, dim_, block_interval_>;
			using container = std::unique_ptr<T[]>;
			using iterator = T*;
			using const_iterator = const T*;
		public:
			block():Pos(),Data(BlockPool.get()){}
			block(const block&)=delete;
			block& operator=(const block&)=delete;
			block(block&&)=default;
			block& operator=(block&&)=default;
			explicit block(point_type Pos_):Pos(Pos_),Data(BlockPool.get()){}
			~block()noexcept{if(Data)BlockPool.release(std::move(Data));}
		public:
			iterator begin() { return Data.get(); }
			iterator end() { return Data.get()+lattice_size(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return Data.get(); }
			const_iterator cend()const { return Data.get()+lattice_size(); }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			reference index_at(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			const_reference index_at(index_type Index_)const { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			reference index_ref(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			const_reference index_ref(index_type Index_)const { return Data[Index_]; }
			reference at(point_type Pos_) {return index_at(indexer(extent()).torus_index(Pos_));}
			const_reference at(point_type Pos_)const {return index_at(indexer().torus_index(Pos_));}
			template<typename... others>
			reference at(index_type Pos_, others... Others_) {return at(lattices::point(Pos_, Others_...));}
			template<typename... others>
			const_reference at(index_type Pos_, others... Others_)const { return at(lattices::point(Pos_, Others_...)); }
			reference ref(point_type Pos_) {return at(Pos_);}
			const_reference ref(point_type Pos_)const {return at(Pos_);}
			template<typename... others>
			reference ref(index_type Pos_, others... Others_) { return ref(lattices::point(Pos_, Others_...)); }
			template<typename... others>
			const_reference ref(index_type Pos_, others... Others_)const { return ref(lattices::point(Pos_, Others_...)); }
			reference operator[](point_type Pos_) {return ref(Pos_);}
			const_reference operator[](point_type Pos_)const {return ref(Pos_);}
		public:
			point_type point()const { return Pos; }
			point_type point(const_iterator Itr)const { return Pos + Indexer.calc_point(std::distance(begin(),Itr)); }
			//!Get point_type BlockNum
			extent_type extent()const { return extent_type(block_interval()); }
			indexer_type indexer()const{return indexer(extent());}
			size_type lattice_size()const { return lattice_size(); }
			//!Return true if the given point is inclueded incide of this block.
			bool inside(point_type Point_)const {
				for(unsigned int i = 0; i<dim_; ++i) {
					if( !(Pos[i]<=Point_[i] && Point_[i]<Pos[i]+static_cast<index_type>(block_interval())) )return false;
				}
				return true;
			}
			//!Convert from index to point
			point_type index_to_point(index_type Index_)const { return Indexer.calc_point(Index_); }
			//!Convert from point to index
			index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
		private:
			void assign(point_type Pos_) {
				Pos = Pos_;
				std::fill(begin(),end(),0);
			}
		private:
			point_type Pos;
			container Data;
		};
	private://block definition
		using block_container = std::vector<block>;
	public://block definition
		using block_iterator = typename block_container::iterator;
		using block_const_iterator = typename block_container::const_iterator;
		using element_iterator = typename block::iterator;
		using element_const_iterator = typename block::const_iterator;
	public://iterator
		struct const_iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::reference;
			using pointer = typename this_type::pointer;
			using iterator_category = std::random_access_iterator_tag;
		public:
			const_iterator() = default;
			const_iterator(block_const_iterator BItr_, element_const_iterator Itr_):BItr(BItr_),Itr(Itr_){}
		ublic:
			reference operator*() {
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				return *Itr;
			}
			reference operator[](difference_type n) {
				return (*this + n).operator*();
			}
			pointer operator->() {
				return Itr.operator->();
			}
			const_iterator& operator++() {
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				++Itr;
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

				n += std::distance(BItr->begin(), Itr);
				BItr += (n-1)/block_size();
				Itr = std::next(BItr->begin(), (n-1)%block_size()+1);

				return *this;
			}
			const_iterator& operator-=(difference_type n) {
				if(n<0)return operator+=(-n);

				n += std::distance(Itr, BItr->end());
				BItr -= (n-1)/block_size();
				Itr = std::prev(BItr->end(), (n-1)%block_size()+1);

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
				std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*block_size();
			}
			friend bool operator==(const const_iterator& v1, const const_iterator& v2) {
				return v1-v2 == 0;
			}
			friend bool operator!=(const const_iterator& v1, const const_iterator& v2) {
				return v1-v2 != 0;
			}
			friend bool operator< (const const_iterator& v1, const const_iterator&  v2) {
				return v1-v2<0;
			}
			friend bool operator<=(const const_iterator& v1, const const_iterator&  v2) {
				return v1-v2<=0;
			}
			friend bool operator> (const const_iterator& v1, const const_iterator&  v2) {
				return v1-v2>0;
			}
			friend bool operator>=(const const_iterator& v1, const const_iterator&  v2) {
				return v1-v2>=0;
			}
		public:
			point_type point()const { return BItr->point(Itr); }
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
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				return *Itr;
			}
			reference operator[](difference_type n) {
				return (*this + n).operator*();
			}
			pointer operator->() {
				return Itr.operator->();
			}
			iterator& operator++() {
				if(Itr == BItr->end()) {
					++BItr;
					Itr = BItr->begin();
				}
				++Itr;
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

				n += std::distance(BItr->begin(), Itr);
				BItr += (n-1)/block_size();
				Itr = std::next(BItr->begin(), (n-1)%block_size()+1);

				return *this;
			}
			iterator& operator-=(difference_type n) {
				if(n<0)return operator+=(-n);

				n += std::distance(Itr, BItr->end());
				BItr -= (n-1)/block_size();
				Itr = std::prev(BItr->end(), (n-1)%block_size()+1);

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
				std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*block_size();
			}
			friend bool operator==(const iterator& v1, const iterator& v2) {
				return v1-v2 == 0;
			}
			friend bool operator!=(const iterator& v1, const iterator& v2) {
				return v1-v2 != 0;
			}
			friend bool operator< (const iterator& v1, const iterator&  v2) {
				return v1-v2<0;
			}
			friend bool operator<=(const iterator& v1, const iterator&  v2) {
				return v1-v2<=0;
			}
			friend bool operator> (const iterator& v1, const iterator&  v2) {
				return v1-v2>0;
			}
			friend bool operator>=(const iterator& v1, const iterator&  v2) {
				return v1-v2>=0;
			}
		public:
			point_type point()const { return BItr->point(Itr); }
			block_iterator block_itr() { return BItr; }
			element_iterator element_itr() { return Itr; }
		private:
			block_iterator BItr;
			element_iterator Itr;
		};
	public://constructor
		block_lattice()noexcept: Blocks(), HintPos(0){}
	public://container access functions
		std::size_t size()const { return Blocks.size(); }
		bool empty()const{ return size()==0; }
		void clear() { Blocks.clear(); HintPos = 0; }
		//!Return reference of the elemtn at the given point with range check
		reference at(point_type Point_) {
			auto Itr = block_find(Point_,block_begin()+HintPos);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(point_type Point_)const {
			auto Itr = block_find(Point_, block_begin()+HintPos);
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
			auto Itr = block_find(Point_, Hint_.block_itr());
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const_iterator Hint_, point_type Point_)const {
			auto Itr = block_find(Point_, Hint_.block_itr());
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
		//!Return reference of the elemtn at the given point with range check
		iterator find(point_type Point_) {
			auto Itr = block_find(Point_, block_begin()+HintPos);
			if(Itr==block_end())return end();
			return iterator(Itr, Itr->begin()+Indexer.torus_index(Point_));
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(point_type Point_)const {
			auto Itr = block_find(Point_, block_begin()+HintPos);
			if(Itr==block_end())return end();
			return const_iterator(Itr, Itr->begin()+Indexer.torus_index(Point_));
		}
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		iterator find(index_type Pos_, others... Others_) {
			return find(lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_iterator find(index_type Pos_, others... Others_)const {
			return find(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point with range check
		iterator find(iterator Hint_, point_type Point_) {
			auto Itr = block_find(Point_, Hint_.block_itr());
			if(Itr==block_end())return end();
			return iterator(Itr, Itr->begin()+Indexer.torus_index(Point_));
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(const_iterator Hint_, point_type Point_)const {
			auto Itr = block_find(Point_, Hint_.block_itr());
			if(Itr==block_end())return end();
			return const_iterator(Itr, Itr->begin()+Indexer.torus_index(Point_));
		}
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		iterator find(iterator Hint_, index_type Pos_, others... Others_) {
			return find(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_iterator find(const_iterator Hint_, index_type Pos_, others... Others_)const {
			return find(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(point_type Point_) {
			auto Itr = block_get(Point_, block_begin()+HintPos);
			return Itr->ref(Point_);
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(iterator Hint_, point_type Point_) {
			auto Itr = block_get(Point_, Hint_);
			return Itr->ref(Point_);
		}
		template<typename... others>
		reference ref(iterator Hint_, index_type Pos_, others... Others_) {
			return ref(Hint_, lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](point_type Point_) {
			return ref(Point_);
		}
	public://iterator functions
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
	public://lattice functions
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return block_num()*block_size(); }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { 
			return Blocks.at(static_cast<index_type>(Index_/block_size())).point() + Indexer.point(Index_%block_size());
		}
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const {
			auto Itr = block_find(Point_);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return std::distance(block_begin(), Itr)*block_size() + Indexer.torus_index(Point_);
		}
	public://block functions
		//!Get block extent
		extent_type block_extent()const { return Indexer.extent(); }
		//!Get block number
		size_type block_num()const { return BlockNum; }
		//!Get block number without malloc
		std::size_t block_capacity()const { return Blocks.size()-1; }
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
				hmLib::swap_remove_if(block_begin(), block_end(), std::forward<block_condition_>(BlockCondition))
			);
			HintPos = BlockNum;
		}
		template<typename element_condition_>
		void block_erase_if_all_of(element_condition_ ElementConditiion) {
			block_erase_if([&cond = ElementConditiion](const block& b) {return std::all_of(b.begin(), b.end(), cond); });
		}
		void block_reserve(std::size_t n){
			while(block_capacity() < n){
				Blocks.push_back(block(point_type(0)));
			}
		}
		void block_shrink_to_fit(){
			while(block_num() < block_capacity()) {
				Blocks.pop_back();
			}
		}
		block_iterator block_find(point_type Pos_) {
			for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
				Pos_[i] = euclidean_div<index_type>(Pos_[i], Indexer.extent()[i])*Indexer.extent()[i];
			}
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();			
			HintPos = std::distance(block_begin(), Itr);
			return Itr;
		}
		block_const_iterator block_find(point_type Pos_)const {
			for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
				Pos_[i] = euclidean_div<index_type>(Pos_[i], Indexer.extent()[i])*Indexer.extent()[i];
			}
			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
			if(Itr != block_end() && Itr->point() != Pos_) return block_end();
			HintPos = std::distance(block_begin(), Itr);
			return Itr;
		}
		block_iterator block_find(point_type Pos_, block_iterator Hint_) {
			if(Hint_<block_end() && Hint_->inside(Pos_)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_find(Pos_);
		}
		block_const_iterator block_find(point_type Pos_, block_const_iterator Hint_)const {
			if(Hint_<block_end() && Hint_->inside(Pos_)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_find(Pos_);
		}
		block_iterator block_get(point_type Pos_) {
			for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
				Pos_[i] = euclidean_div<index_type>(Pos_[i], Indexer.extent()[i])*Indexer.extent()[i];
			}

			auto Itr = std::partition_point(block_begin(), block_end(), [Pos_](const block& Block) {return Block.point()<Pos_; });

			//fail to find
			if(Itr==block_end() || Itr->point() != Pos_) {			
				//Add new block by using last unused block.
				block_end()->assign(Pos_);
				if(Itr!=block_end())std::rotate(Itr, block_end(), block_end()+1);
				HintPos = std::distance(Blocks.begin(), Itr);
				//Is there no unused block?
				if(block_num() == block_capacity()) {
					//All iterators are broken
					Blocks.push_back(block(point_type(0)));
					Itr = std::next(Blocks.begin(), HintPos);
				}
				//Increment block_end
				++BlockNum;
			} else {
				HintPos = std::distance(Blocks.begin(), Itr);
			}

			return Itr;
		}
		block_iterator block_get(point_type Pos_, block_iterator Hint_) {
			if(Hint_!=block_end() && Hint_->inside(Pos_)) {
				HintPos = std::distance(block_begin(), Hint_);
				return Hint_;
			}
			return block_get(Pos_);
		}
	private://variables
		//BLocks should keep unused empty element in back()
		//	In other words, Blocks.end() != End should be satishied anytime
		//	This is because of end iterator's requirement.
		block_container Blocks;
		indexer Indexer;
		unsigned int BlockNum;
		unsigned int HintPos;
	};

	template<typename T, unsigned int dim_, unsigned int block_interval_>
	block_lattice<T,dim_,block_interval_>::block_pool block_lattice<T,dim_,block_interval_>::BlockPool;
}
#
#endif
