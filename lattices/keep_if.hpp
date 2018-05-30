#ifndef HMLIB_LATTICES_KEEPIF_INC
#define HMLIB_LATTICES_KEEPIF_INC 100
#
#include<vector>
#include<utility>
#include<limits>
#include<tuple>
#include"../iterators/index_at_access_iterator.hpp"
#include"../tuple.hpp"
#include"utility.hpp"
#include"indexer.hpp"
namespace hmLib {
	namespace lattices {
		template<typename... lattices_>
		struct zip_lattice {
			using pack = std::tuple<lattices_&...>;
			using reference = std::tuple<decltype(std::declval<typename std::decay<lattices_>::type>().at(std::declval<point_type>()))...>;
		public:
			template<typename... other_lattices>
			zip_lattice(other_lattices&... Lattices_):Ranges(Lattices_...) {
			}
			reference operator[](point_type Pos) { return tuple_for_each([Pos](auto& v)->decltype((v[Pos])) {return v[Pos]; }, Ranges); }
			reference at(point_type Pos) {
				return tuple_for_each([Pos](auto& v)->decltype((v.at(Pos))) {return v.at(Pos); }, Ranges);
			}
			size_type size() { return Size; }
		private:
			pack Ranges;
		};
		template<typename... lattices_>
		auto make_zip_lattice(lattices_&... Lattices_) {
			return zip_lattice<typename std::decay<lattices_>::type...>(Lattices_...);
		}

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
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using reference = value_type;
			using pointer = void;
			using const_reference = void;
			using const_pointer = void;
		public:
			element_point_keeper() = default;
			template<typename lattice_type, typename condition>
			element_point_keeper(lattice_type& Lattice, condition ShouldKeep) {
				reset(Lattice, std::forward<condition>(ShouldKeep));
			}
			template<typename lattice_type, typename condition>
			void reset(lattice_type& Lattice, condition ShouldKeep) {
				indexer<dim_> Indexer(Lattice.extent());

				reset();

				index_type Index = 0;
				for(auto Itr = Lattice.begin(), End = Lattice.end(); Itr!=End; ++Itr, ++Index) {
					if(ShouldKeep(*Itr)) {
						KeptBlock.push_back(Indexer.point(Index));
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
				return make_index_at_access_range(Lat, begin(), end());
			}
			template<typename lattice_type1, typename lattice_type2, typename... others>
			auto range(lattice_type1& Lat1, lattice_type2& Lat2, others&... Others)const {
				//index_at_access capture the reference of argument
				//	make_zip_lattice is captured although it is temporal variable.
				//return make_index_at_access_range(make_zip_lattice(Lat1,Lat2,Others...), begin(), end());
			}
		private:
			element_container KeptBlock;
		};
		template<unsigned int dim_, typename keep_point_type_ = lattices::point_type<dim_>>
		struct block_point_keeper {
		public:
			constexpr static unsigned int dim() { return dim_; }
		private:
			using this_type = block_point_keeper <dim_, keep_point_type_>;
			using keep_point_type = keep_point_type_;
			using index_type = int;
			using block_container = std::vector<std::pair<index_type, index_type>>;
			using block_iterator = typename block_container::const_iterator;
			using indexer = lattices::indexer<dim_>;
		public:
			using value_type = keep_point_type;
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using reference = value_type;
			using pointer = void;
			using const_reference = void;
			using const_pointer = void;
		public:
			struct const_iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				block_iterator BItr;
				indexer Indexer;
				index_type Index;
			public:
				const_iterator() = default;
				const_iterator(block_iterator BlockBeg_, indexer Indexer_, index_type Index_)
					: BItr(BlockBeg_)
					, Indexer(Indexer_)
					, Index(Index_) {
				}
				reference operator*()const {
					return Indexer.calc_point(Index);
				}
				const_iterator& operator++() {
					++Index;
					if(Index == BItr->first) {
						Index = BItr->second;
						++BItr;
					}
					return *this;
				}
				const_iterator operator++(int) {
					const_iterator Prev = *this;
					++(*this);
					return Prev;
				}
				friend bool operator==(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Index ==itr2.Index; }
				friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Index !=itr2.Index; }
			};
		public:
			block_point_keeper()noexcept : JumpBlock(), Size(0), Index(0) {}
			template<typename lattice_type, typename condition>
			block_point_keeper(lattice_type& Lat, condition ShouldKeep) {
				reset(Lat, std::forward<condition>(ShouldKeep));
			}
			template<typename lattice_type, typename condition>
			void reset(lattice_type& Lat, condition ShouldKeep) {
				reset();

				Indexer.resize(Lat.extent());
				auto Beg_ = Lat.begin();
				auto End_ = Lat.end();
				index_type Cnt = 0;
				for(; Beg_!=End_; ++Beg_, ++Cnt) {
					if(ShouldKeep(*Beg_)) {
						Index = Cnt;
						++Size;
						break;
					}
				}

				if(Beg_==End_) {
					return;
				}

				index_type From = 0;
				bool InKeepBlock = true;

				++Cnt;
				++Beg_;
				for(; Beg_!=End_; ++Beg_, ++Cnt) {
					if(ShouldKeep(*Beg_)) {
						if(!InKeepBlock) {
							JumpBlock.emplace_back(From, Cnt);
							InKeepBlock = true;
						}
						++Size;
					} else {
						if(InKeepBlock) {
							From = Cnt;
							InKeepBlock = false;
						}
					}
				}
				if(InKeepBlock) {
					JumpBlock.emplace_back(Cnt, Cnt);
				} else {
					JumpBlock.emplace_back(From, Cnt);
				}
			}
			void reset() {
				JumpBlock.clear();
				Size = 0;
				Index = 0;
			}
			void push_back(value_type Index_) {
				if(Size==0) {
					Index = Index_;
					JumpBlock.emplace_back(Index_+1, Index_+1);
				} else {
					hmLib_assert(Index_>=JumpBlock.back().second, hmLib::numeric_exceptions::incorrect_arithmetic_request, "block_index_keeper::push_back require the increasing-order");
					//next of the current end
					if(JumpBlock.back().first == Index_) {
						JumpBlock.back().first = Index_+1;
						JumpBlock.back().second = Index_+1;
					} else {
						JumpBlock.back().second = Index_;
						JumpBlock.emplace_back(Index_+1, Index_+1);
					}
				}
				++Size;
			}
			bool empty()const { return Size==0; }
			std::size_t size()const { return Size; }
			const_iterator cbegin()const { return const_iterator(JumpBlock.begin(), Indexer, Index); }
			const_iterator cend()const {
				if(JumpBlock.empty())return const_iterator(JumpBlock.end(), Indexer, Index);
				return const_iterator(JumpBlock.end(), Indexer, JumpBlock.back().second);
			}
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
			template<typename lattice_type>
			auto range(lattice_type& Lat)const {
				return make_index_at_access_range(Lat, begin(), end());
			}
		private:
			index_type Index;
			std::size_t Size;
			block_container JumpBlock;
			indexer Indexer;
		};
	}
}
#
#endif
