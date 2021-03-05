#ifndef HMLIB_BLOCKS_INC
#define HMLIB_BLOCKS_INC 100
#include<functional>
#include<unordered_map>
#include<optional>
#include"exceptions.hpp"
namespace hmLib {
	template<std::size_t block_bits, typename index_type, std::enable_if_t<std::is_integral_v<index_type>, nullptr_t> = nullptr>
	struct default_block_hasher {
		static constexpr std::size_t block_size() { return std::size_t(1) << block_bits; }
		std::pair<std::size_t, std::size_t> operator()(index_type index)const {
			return std::make_pair(
				index >> block_bits,
				static_cast<std::size_t>(index & (~((~index_type(0)) << block_bits)))
			);
		}
		index_type operator()(std::size_t first, std::size_t second)const {
			return (first << block_bits) + second;
		}
	};
	template<typename T, typename index_type_ = int, typename block_hasher_ = default_block_hasher<6, index_type_>>
	struct blocks{
	private:
		using this_type = blocks<T, index_type_, block_hasher_>;
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using index_type = index_type_;
		using size_type = std::size_t;
		using block_hasher = block_hasher_;
	private:
		using block_type = std::array<T,block_hasher::block_size()>;
		using block_container = std::unordered_map<std::size_t, block_type>;
		using element_iterator = typename block_type::iterator;
		using element_const_iterator = typename block_type::const_iterator;
	public:
		using block_iterator = typename block_container::iterator;
		using block_const_iterator = typename block_container::const_iterator;
		struct const_iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::const_reference;
			using pointer = typename this_type::const_pointer;
			using iterator_category = std::forward_iterator_tag;
		public:
			const_iterator() = default;
			explicit const_iterator(block_const_iterator bitr_) :bitr(bitr_), eitropt() {}
			const_iterator(block_const_iterator bitr_,element_const_iterator eitr_) :bitr(bitr_), eitropt(eitr_) {}
		public:
			reference operator*() {
				validate();
				return eitropt->operator*();
			}
			pointer operator->() {
				validate();
				return eitropt->operator->();
			}
			const_iterator& operator++() {
				validate();
				++(*eitropt);
				if ((*eitropt) == bitr->second.end()) {
					++bitr;
					eitropt.reset();
				}
				return *this;
			}
			const_iterator operator++(int) {
				const_iterator Ans = *this;
				operator++();
				return Ans;
			}
			friend bool operator==(const const_iterator& v1, const const_iterator& v2) {
				if (v1.bitr != v2.bitr)return false;
				if(v1.eitropt == v2.eitropt)return true;
				if (v1.eitropt.has_value() == v2.eitropt.has_value())return false;
				v1.validate();
				v2.validate();
				return v1.eitropt == v2.eitropt;
			}
			friend bool operator!=(const const_iterator& v1, const const_iterator& v2) {
				return !(v1==v2);
			}
			index_type index()const { 
				block_hasher bh;  
				if (!eitropt) {
					eitropt = bitr->second.begin();
				}
				return bh(bitr->first, std::distance(bitr->second.begin(),eitr)); 
			}
			block_const_iterator block_itr()const { return bitr; }
			element_const_iterator element_itr()const { return *eitropt; }
		private:
			void validate()const {
				if (!eitropt) {
					eitropt = bitr->second.begin();
				}
			}
		private:
			block_const_iterator bitr;
			mutable std::optional<element_const_iterator> eitropt;
		};
		struct iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using reference = typename this_type::reference;
			using pointer = typename this_type::pointer;
			using iterator_category = std::forward_iterator_tag;
		public:
			iterator() = default;
			explicit iterator(block_iterator bitr_) :bitr(bitr_), eitropt() {}
			iterator(block_iterator bitr_, element_iterator eitr_) :bitr(bitr_), eitropt(eitr_) {}
			operator const_iterator()const {
				if (eitropt) {
					return const_iterator(bitr, *eitropt);
				} else {
					return const_iterator(bitr);
				}
			}
		public:
			reference operator*() {
				validate();
				return eitropt->operator*();
			}
			pointer operator->() {
				validate();
				return eitropt->operator->();
			}
			iterator& operator++() {
				validate();
				++(*eitropt);
				if ((*eitropt) == bitr->second.end()) {
					++bitr;
					eitropt.reset();
				}
				return *this;
			}
			iterator operator++(int) {
				iterator Ans = *this;
				operator++();
				return Ans;
			}
			friend bool operator==(const iterator& v1, const iterator& v2) {
				if (v1.bitr != v2.bitr)return false;
				if (v1.eitropt == v2.eitropt)return true;
				if (v1.eitropt.has_value() == v2.eitropt.has_value())return false;
				v1.validate();
				v2.validate();
				return v1.eitropt == v2.eitropt;
			}
			friend bool operator!=(const iterator& v1, const iterator& v2) {
				return !(v1 == v2);
			}
			index_type index()const {
				block_hasher bh;
				if (!eitropt) {
					eitropt = bitr->second.begin();
				}
				return bh(bitr->first, std::distance(bitr->second.begin(), *eitropt));
			}
			block_iterator block_itr() const{ return bitr; }
			element_iterator element_itr()const { return *eitropt; }
		private:
			void validate()const {
				if (!eitropt) {
					eitropt = bitr->second.begin();
				}
			}
		private:
			block_iterator bitr;
			mutable std::optional<element_iterator> eitropt;
		};
	public:
		//!Check whether empty
		bool empty()const { return Blocks.empty(); }
		//!Clear all elements
		void clear() { Blocks.clear(); }
		//!Get element number
		size_type size()const { return Blocks.size() * block_hasher::block_size(); }
		//!Get block size
		constexpr size_type block_size()const { return block_hasher::block_size(); }
		//!Get block number
		size_type block_num()const { return Blocks.size(); }
		//!Get block access iterator at begin
		block_iterator block_begin() { Blocks.begin(); }
		//!Get block access iterator at begin in const
		block_const_iterator block_begin()const { Blocks.begin(); }
		//!Get block access iterator at end
		block_iterator block_end() { Blocks.end(); }
		//!Get block access iterator at end in const
		block_const_iterator block_end()const { Blocks.end(); }
		//!Remove block
		block_iterator block_erase(block_const_iterator Itr) {
			return Blocks.erase(Itr);
		}
		//!Remove block if the block satisfies the given condition 
		template<typename block_condition_>
		void block_erase_if(block_condition_ BlockCondition) {
			auto itr = Blocks.begin();
			while(itr != Blocks.end()) {
				if (BlockCondition(*itr)) {
					itr = Blocks.erase(itr);
				} else {
					++itr;
				}
			}
		}
		//!Remove block if all elements of the block satisfies the given condition 
		template<typename element_condition_>
		void block_erase_if_all_of(element_condition_ ElementConditiion) {
			block_erase_if([&cond = ElementConditiion](const block_type& b) {return std::all_of(b.begin(), b.end(), cond); });
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(index_type n) {
			block_hasher Hasher;
			auto block_pos = Hasher(n);
			auto Itr = Blocks.find(block_pos.first);
			hmLib_assert(Itr != Blocks.end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->second.at(block_pos.second);
		}
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(index_type n)const {
			block_hasher Hasher;
			auto block_pos = Hasher(n);
			auto Itr = Blocks.find(block_pos.first);
			hmLib_assert(Itr != Blocks.end(), hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Itr->second.at(block_pos.second);
		}
		//!Return reference of the elemtn at the given point without range check
		reference ref(index_type n) {
			block_hasher Hasher;
			auto block_pos = Hasher(n);
			auto Itr = Blocks.find(block_pos.first);
			if (Itr == Blocks.end()) {
				Itr = Blocks.insert(std::make_pair(block_pos.first, block_type())).first;
				Itr->second.fill(0);
			}
			return Itr->second.at(block_pos.second);
		}
		//!Return reference of the elemtn at the given point without range check
		reference operator[](index_type n) {
			return ref(n);
		}
		//!Return iterator with the index
		iterator find(index_type n) {
			block_hasher Hasher;
			auto block_pos = Hasher(n);
			auto bitr = Blocks.find(block_pos.first);
			if (bitr == Blocks.end())return iterator(bitr);
			return iterator(bitr, std::next(bitr->second.begin(),block_pos.second));
		}
		//!Return iterator with the index in const
		const_iterator find(index_type n)const{
			block_hasher Hasher;
			auto block_pos = Hasher(n);
			auto bitr = Blocks.find(block_pos.first);
			if (bitr == Blocks.end())return const_iterator(bitr);
			return const_iterator(bitr, std::next(bitr->second.begin(), block_pos.second));
		}
	public:
		//!Return begin iterator
		iterator begin() { return iterator(Blocks.begin()); }
		//!Return end iterator
		iterator end() { return iterator(Blocks.end()); }
		//!Return begin const_iterator
		const_iterator begin()const { return cbegin(); }
		//!Return end const_iterator
		const_iterator end()const { return cend(); }
		//!Return begin const_iterator
		const_iterator cbegin()const { return const_iterator(Blocks.begin());}
		//!Return end const_iterator
		const_iterator cend()const { return const_iterator(Blocks.end());}
	private:
		block_container Blocks;
	};
}
#
#endif
