#ifndef HMLIB_LATTICES_KEEPIF_INC
#define HMLIB_LATTICES_KEEPIF_INC 100
#
#include<vector>
#include<utility>
#include<limits>
#include<tuple>
#include"../iterators/index_accessor_iterator.hpp"
#include"utility.hpp"
#include"indexer.hpp"
namespace hmLib {
	namespace lattices {
		template<unsigned int dim_, typename keep_point_type_ = lattices::point_type<dim_>>
		struct element_point_keeper {
		public:
			constexpr static unsigned int dim() { return dim_; }
		private:
			using this_type = element_point_keeper <dim_, keep_point_type_>;
			using keep_point_type = keep_point_type_;
			using element_container = std::vector<keep_point_type>;
			using const_iterator = typename element_container::const_iterator;
		public:
			using value_type = keep_point_type;
			using difference_type = int;
			using reference = value_type;
			using pointer = void;
			using const_reference = void;
			using const_pointer = void;
		public:
			element_point_keeper() = default;
			template<typename lattice_iterator, typename condition>
			element_point_keeper(lattice_iterator Beg, lattice_iterator End, condition ShouldKeep) {
				reset(Beg, End, std::forward<condition>(ShouldKeep));
			}
			template<typename lattice_iterator, typename condition>
			void reset(lattice_iterator Beg, lattice_iterator End, condition ShouldKeep) {
				reset();

				for(; Beg!=End; ++Beg) {
					if(ShouldKeep(*Beg)) {
						KeptBlock.push_back(Beg.point());
					}
				}
			}
			void reset() { KeptBlock.clear(); }
			void push_back(keep_point_type Point_) {
				KeptBlock.push_back(Point_);
			}
			std::size_t size()const { return KeptBlock.size(); }
			const_iterator cbegin()const { return KeptBlock.cbegin(); }
			const_iterator cend()const { return KeptBlock.cend(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
			template<typename lattice_type>
			auto range(lattice_type& Lat)const {
				return make_index_op_access_range(Lat, begin(), end());
			}
		private:
			element_container KeptBlock;
		};
		template<unsigned int dim_, typename point_type_ = lattices::point_type<dim_>>
		struct block_point_keeper {
		public:
			constexpr static unsigned int dim() { return dim_; }
		private:
			using this_type = block_point_keeper <dim_, point_type_>;
			using point_type = point_type_;
			using point_element = int;
			using block_element = std::pair<point_type, point_element>;
			using block_container = std::vector<block_element>;
			using block_iterator = typename block_container::const_iterator;
		public:
			struct iterator {
				using value_type = point_type;
				using difference_type = int;
				using reference = value_type;
				using pointer = void;
				using iterator_category = std::bidirectional_iterator_tag;
			private:
				block_iterator BItr;
				point_element Point0;
			public:
				iterator() = default;
				iterator(block_iterator BItr_, point_element Point0_)
					: BItr(BItr_)
					, Point0(Point0_) {
				}
				reference operator*()const {
					auto Point = BItr->first;
					Point[0] = Point0;
					return Point;
				}
				iterator& operator++() {
					++Point0;
					if(Point0 == BItr->second) {
						++BItr;
						Point0 = (BItr->first)[0];
					}
					return *this;
				}
				iterator operator++(int) {
					iterator Prev = *this;
					++(*this);
					return Prev;
				}
				iterator& operator--() {
					if(Point0 == (BItr->first)[0]) {
						--BItr;
						Point0 = BItr->second;
					}
					--Point0;
					return *this;
				}
				iterator operator--(int) {
					iterator Prev = *this;
					--(*this);
					return Prev;
				}
				friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.BItr ==itr2.BItr && itr1.Point0 ==itr2.Point0; }
				friend bool operator!=(const iterator& itr1, const iterator& itr2) { return !(itr1==itr2); }
			};
		public:
			block_point_keeper() { reset(); }
			template<typename lattice_iterator, typename condition>
			block_point_keeper(lattice_iterator Beg, lattice_iterator End, condition ShouldKeep) {
				reset(Beg, End, std::forward<condition>(ShouldKeep));
			}
			template<typename lattice_iterator, typename condition>
			void reset(lattice_iterator Beg, lattice_iterator End, condition ShouldKeep) {
				KeepBlock.clear();
				Size = 0;

				for(; Beg!=End; ++Beg) {
					if(ShouldKeep(*Beg)) {
						break;
					}
				}
				if(Beg==End)return;

				point_type Pos = Beg.point();
				point_element Cnt = Pos[0]+1;
				bool InKeepBlock = true;
				++Size;
				++Beg;

				for(; Beg!=End; ++Beg) {
					if(ShouldKeep(*Beg)) {
						if(!InKeepBlock) {
							Pos = Beg.point();
							InKeepBlock = true;
							Cnt = Pos[0]+1;
						} else {
							if(is_same(Beg.point(), Cnt, Pos)) {
								++Cnt;
							} else {
								KeepBlock.emplace_back(Pos, Cnt);
								Pos = Beg.point();
								Cnt = Pos[0]+1;
							}
						}
						++Size;
					} else {
						if(InKeepBlock) {
							KeepBlock.emplace_back(Pos, Cnt);
							InKeepBlock = false;
						}
					}
				}
				if(InKeepBlock) {
					KeepBlock.emplace_back(Pos, Cnt);
				}

				//sebinel element
				KeepBlock.emplace_back(point_type{ 0 }, 0);
			}
			void reset() {
				Size = 0;
				KeepBlock.clear();

				//sebinel element
				KeepBlock.emplace_back(point_type{ 0 }, 0);
			}
			bool empty()const { return Size==0; }
			std::size_t size()const { return Size; }
			iterator cbegin()const {return iterator(KeepBlock.begin(), (KeepBlock.front().first)[0]); }
			iterator cend()const { return iterator(KeepBlock.end()-1, KeepBlock.back().second); }
			iterator begin()const { return cbegin(); }
			iterator end() const { return cend(); }
			template<typename lattice_type>
			auto range(lattice_type& Lat)const {
				return make_index_op_access_range(Lat, begin(), end());
			}
		private:
			static bool is_same(point_type p1, unsigned int p2_0, point_type p2) {
				if(p1[0] != p2_0)return false;
				for(unsigned int i=1; i<dim(); ++i) {
					if(p1[i]!=p2[i])return false;
				}
				return true;
			}
		private:
			std::size_t Size;
			block_container KeepBlock;
		};
	}
}
#
#endif
