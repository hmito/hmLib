#ifndef HMLIB_LATICES_BLOCKLATTICE_INC
#define HMLIB_LATICES_BLOCKLATTICE_INC 100
#include<vector>
#include<functional>
#include<algorithm>
#include<boost/pool/pool.hpp>
#include"../math/static_math.hpp"
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
		static constexpr unsigned int block_size(){return hmLib::static_pow<dim()>(block_interval_);}
	private://block
		struct block_pool{
		private:
			std::vector<std::unique_ptr<T[]>> BlockPool;
		public:
			std::unique_ptr<T[]> get(){
				std::unique_ptr<T[]> Ans;
				if(BlockPool.empty()){
					Ans = std::make_unique<T[]>(block_size());
				}else{
					Ans = std::move(BlockPool.back());
					BlockPool.pop_back();
				}
				std::fill(Ans.get(),Ans.get()+block_size(), 0);
				return std::move(Ans);
			}
			void release(std::unique_ptr<T[]>&& Block)noexcept{
				BlockPool.push_back(std::move(Block));
			}
		};
		static block_pool BlockPool;
	public://block
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
			iterator end() { return Data.get()+block_size(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return Data.get(); }
			const_iterator cend()const { return Data.get()+block_size(); }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			reference index_at(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
			const_reference index_at(index_type Index_)const { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			reference index_ref(index_type Index_) { return Data[Index_]; }
			//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
			const_reference index_ref(index_type Index_)const { return Data[Index_]; }
			reference at(point_type Pos_) {return index_at(indexer().torus_index(Pos_));}
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
			point_type point(const_iterator Itr)const { return Pos + indexer_type(extent_type(block_interval())).calc_point(std::distance(begin(),Itr)); }
			//!Get point_type BlockNum
			extent_type extent()const { return extent_type(block_interval()); }
			indexer_type indexer()const{ return indexer_type(extent());}
			bool inside(point_type Point_)const {
				for(unsigned int i = 0; i<dim_; ++i) {
					if( !(Pos[i]<=Point_[i] && Point_[i]<Pos[i]+static_cast<index_type>(block_interval())) )return false;
				}
				return true;
			}
			point_type index_to_point(index_type Index_)const { return Indexer.calc_point(Index_); }
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
		struct blockset {
			using container = std::vector<block>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			container BlockSet;
			std::size_t HintPos;
		public:
			blockset()noexcept:BlockSet(), HintPos(0) { BlockSet.push_back(block()); }
		public:
			void clear()const { BlockSet.clear(); BlockSet.push_back(block()); }
			bool empty()const { return size()==0; }
			std::size_t size()const { return BlockSet.size()-1; }
			block& at(index_type n) { return BlockSet.at(n); }
			const block& at(index_type n)const { return BlockSet.at(n); }
			block& operator[](index_type n) { return BlockSet[n]; }
			const block& operator[](index_type n)const { return BlockSet[n]; }
			iterator begin() { return BlockSet.begin(); }
			iterator end() { return BlockSet.end()-1; }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
			const_iterator cbegin()const { return BlockSet.begin(); }
			const_iterator cend()const { return BlockSet.end()-1; }
			void erase(iterator Itr) {
				if(Itr == end())return;
				auto Pos = std::distance(begin(), Itr);
				BlockSet.erase(Itr);

				if(Pos<HintPos) {
					--HintPos;
				} else if(Pos==HintPos) {
					HintPos = size();
				}
			}
			template<typename block_condition_>
			void erase_if(block_condition_ BlockCondition) {
				auto Itr = std::remove_if(begin(), end(), std::forward<block_condition_>(BlockCondition));
				BlockSet.erase(Itr, end());
				HintPos = size();
			}
			template<typename element_condition_>
			void erase_if_all_of(element_condition_ ElementConditiion) {
				erase_if([&cond = ElementConditiion](const block& b) {return std::all_of(b.begin(), b.end(), cond); });
			}
			iterator find(point_type Pos_) {
				return find(begin()+HintPos, Pos_);
			}
			iterator find(iterator Hint_, point_type Pos_) {
				if(Hint_!= end() && Hint_->inside(Pos_)) {
					HintPos = std::distance(begin(), Hint_);
					return Hint_;
				}
				for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
					Pos_[i] = euclidean_div<index_type>(Pos_[i], block_interval())*block_interval();
				}
				return find_impl(Pos_);
			}
			const_iterator find(point_type Pos_)const {
				return find(begin()+HintPos, Pos_);
			}
			const_iterator find(const_iterator Hint_, point_type Pos_)const {
				if(Hint_!= end() && Hint_->inside(Pos_)) {
					HintPos = std::distance(begin(), Hint_);
					return Hint_;
				}
				for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
					Pos_[i] = euclidean_div<index_type>(Pos_[i], block_interval())*block_interval();
				}
				return find_impl(Pos_);
			}
			iterator get(point_type Pos_) {
				return get(begin()+HintPos,Pos_);
			}
			iterator get(iterator Hint_, point_type Pos_) {
				if(Hint_!= end() && Hint_->inside(Pos_)) {
					HintPos = std::distance(begin(), Hint_);
					return Hint_;
				}
				return insert(Pos_);
			}
			iterator insert(point_type Pos_) {
				for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
					Pos_[i] = euclidean_div<index_type>(Pos_[i], block_interval())*block_interval();
				}
				return get_impl(Pos_);
			}
			iterator insert(iterator Hint_, point_type Pos_) {
				for(unsigned int i = 0; i<Pos_.static_size(); ++i) {
					Pos_[i] = euclidean_div<index_type>(Pos_[i], block_interval())*block_interval();
				}

				if((Hint_==begin() || std::prev(Hint_)->point() < Pos_) && (Hint_==end() || Pos_ < Hint_->point())) {
					auto Itr = BlockSet.insert(Hint_, block(Pos_));
					HintPos = std::distance(BlockSet.begin(), Itr);
					return Itr;
				}
				return get_impl(Pos_);
			}
		private:
			iterator find_impl(point_type Pos_) {
				auto Itr = std::partition_point(begin(), end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
				if(Itr == end() || Itr->point() != Pos_) return end();
				HintPos = std::distance(begin(), Itr);
				return Itr;
			}
			const_iterator find_impl(point_type Pos_)const {
				auto Itr = std::partition_point(begin(), end(), [Pos_](const block& Block) {return Block.point()<Pos_; });
				if(Itr == end() || Itr->point() != Pos_) return end();
				HintPos = std::distance(begin(), Itr);
				return Itr;
			}
			iterator get_impl(point_type Pos_) {
				auto Itr = std::partition_point(begin(), end(), [Pos_](const block& Block) {return Block.point()<Pos_; });

				//fail to find
				if(Itr == end() || Itr->point() != Pos_) {
					Itr = BlockSet.insert(Itr, block(Pos_));
				}
				HintPos = std::distance(BlockSet.begin(), Itr);

				return Itr;
			}
		};
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
			const_iterator(typename blockset::const_iterator BItr_, typename block::const_iterator Itr_):BItr(BItr_),Itr(Itr_){}
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
			typename blockset::const_iterator block_itr() { return BItr; }
			typename block::const_iterator value_itr() { return Itr; }
		private:
			typename blockset::const_iterator  BItr;
			typename block::const_iterator  Itr;
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
			iterator(typename blockset::iterator BItr_, typename block::iterator  Itr_):BItr(BItr_),Itr(Itr_){}
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
				return std::distance(v1.BItr->begin(), v1.Itr) - std::distance(v2.BItr->begin(), v2.Itr) + (v1.BItr - v2.BItr)*block_size();
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
			typename blockset::iterator block_itr() { return BItr; }
			typename block::iterator  value_itr() { return Itr; }
		private:
			typename blockset::iterator  BItr;
			typename block::iterator  Itr;
		};
	public://container access functions
		indexer_type indexer()const { return indexer_type(extent_type(block_interval())); }
		blockset& blocks() { return Blocks; }
		const blockset& blocks()const { return Blocks; }
		std::size_t size()const { return Blocks.size()*block_size(); }
		bool empty()const{ return Blocks.empty(); }
		void clear() { Blocks.clear();}
		//!Return reference of the elemtn at the given point with range check
		reference at(point_type Point_) {
			auto Itr = Blocks.find(Point_);
			hmLib_assert(Itr!=Blocks.end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(point_type Point_)const {
			auto Itr = Blocks.find(Point_);
			hmLib_assert(Itr!=Blocks.end(), lattices::out_of_range_access, "out of range.");
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
			auto Itr = Blocks.find(Hint_.block_itr(),Point_);
			hmLib_assert(Itr!=Blocks.end(), lattices::out_of_range_access, "out of range.");
			return Itr->at(Point_);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const_iterator Hint_, point_type Point_)const {
			auto Itr = Blocks.find(Hint_.block_itr(),Point_);
			hmLib_assert(Itr!=Blocks.end(), lattices::out_of_range_access, "out of range.");
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
			auto Itr = Blocks.find(Point_);
			if(Itr==Blocks.end())return end();
			return iterator(Itr, Itr->begin()+indexer().torus_index(Point_));
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(point_type Point_)const {
			auto Itr = Blocks.find(Point_);
			if(Itr==Blocks.end())return end();
			return const_iterator(Itr, Itr->begin()+indexer().torus_index(Point_));
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
			auto Itr = Blocks.find(Hint_.block_itr(), Point_);
			if(Itr==Blocks.end())return end();
			return iterator(Itr, Itr->begin()+indexer().torus_index(Point_));
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_iterator find(const_iterator Hint_, point_type Point_)const {
			auto Itr = Blocks.find(Hint_.block_itr(), Point_);
			if(Itr==Blocks.end())return end();
			return const_iterator(Itr, Itr->begin()+indexer().torus_index(Point_));
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
			auto Itr = Blocks.get(Point_);
			return Itr->ref(Point_);
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(iterator Hint_, point_type Point_) {
			auto Itr = Blocks.get(Himt_.block_itr(), Point_);
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
		iterator begin() { return iterator(Blocks.begin(), Blocks.begin()->begin()); }
		//!Return end iterator fot the lattice
		iterator end() { return iterator(Blocks.end(), Blocks.end()->begin()); }
		//!Return begin const_iterator fot the lattice
		const_iterator begin()const { return cbegin(); }
		//!Return end const_iterator fot the lattice
		const_iterator end()const { return cend(); }
		//!Return begin const_iterator fot the lattice
		const_iterator cbegin()const { return const_iterator(block_begin(), block_begin()->begin());}
		//!Return end const_iterator fot the lattice
		const_iterator cend()const { return const_iterator(block_end(), block_end()->begin());}
	public://lattice functions
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { 
			return Blocks.at(static_cast<index_type>(Index_/block_size())).point() + Indexer.point(Index_%block_size());
		}
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const {
			auto Itr = block_find(Point_);
			hmLib_assert(Itr!=block_end(), lattices::out_of_range_access, "out of range.");
			return std::distance(block_begin(), Itr)*block_size() + indexer().torus_index(Point_);
		}
	private://variables
		blockset Blocks;
	};

	template<typename T, unsigned int dim_, unsigned int block_interval_>
	typename block_lattice<T,dim_,block_interval_>::block_pool block_lattice<T,dim_,block_interval_>::BlockPool;
}
#
#endif
