#ifndef HMLIB_ALGORITHM_KEEPIF_INC
#define HMLIB_ALGORITHM_KEEPFI_INC 100
#
#include<iterator>
#include<utility>
#include<vector>
namespace hmLib {
	namespace algorithm {
		template<typename forward_iterator_>
		struct element_keeper {
		private:
			using this_type = element_keeper <forward_iterator_>;
			using base_iterator = forward_iterator_;
			using element_container = std::vector<base_iterator>;
			using element_iterator = typename element_container::const_iterator;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using iterator_category = typename std::iterator_traits<base_iterator>::iterator_category;
		public:
			struct iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				element_iterator EItr;
			public:
				iterator() = default;
				iterator(element_iterator EItr_)
					: EItr(EItr_){
				}
				reference operator*()const {
					return (*EItr).operator*();
				}
				pointer operator->()const {
					return (*EItr).operator->();
				}
				iterator& operator++() {
					++EItr;
					return *this;
				}
				iterator operator++(int) {
					iterator Prev = *this;
					++(*this);
					return Prev;
				}
				friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.EItr ==itr2.EItr; }
				friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.EItr !=itr2.EItr; }
			};
		public:
			element_keeper() = default;
			template<typename condition>
			element_keeper(base_iterator Beg_, base_iterator End_, condition ShouldKeep) {
				reset(Beg_, End_, std::forward<condition>(ShouldKeep));
			}
			template<typename condition>
			void reset(base_iterator Beg_, base_iterator End_, condition ShouldKeep) {
				reset();

				for(; Beg_!=End_; ++Beg_) {
					if(ShouldKeep(*Beg_)) {
						KeptBlock.push_back(Beg_);
					}
				}
			}
			void reset() { KeptBlock.clear(); }
			std::size_t size()const { return KeptBlock.size(); }
			iterator begin()const { return iterator(KeptBlock.begin()); }
			iterator end()const { return iterator(KeptBlock.end()); }
		private:
			element_container KeptBlock;
		};
		template<typename forward_iterator_>
		struct block_keeper {
		private:
			using this_type = block_keeper <forward_iterator_>;
			using base_iterator = forward_iterator_;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using block_container = std::vector<std::pair<base_iterator, base_iterator>>;
			using block_iterator = typename block_container::const_iterator;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using iterator_category = typename std::iterator_traits<base_iterator>::iterator_category;
		public:
			struct iterator{
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				block_iterator BItr;
				base_iterator Itr;
			public:
				iterator() = default;
				iterator(block_iterator BlockBeg_, base_iterator Itr_)
					: BItr(BlockBeg_)
					, Itr(Itr_){
					}
				reference operator*()const {
					return Itr.operator*();
				}
				pointer operator->()const {
					return Itr.operator->();
				}
				iterator& operator++() {
					++Itr;
					if(Itr == BItr->first) {
						Itr = BItr->second;
						++BItr;
					}
					return *this;
				}
				iterator operator++(int) {
					iterator Prev = *this;
					++(*this);
					return Prev;
				}
				friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.Itr ==itr2.Itr; }
				friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.Itr !=itr2.Itr; }
			};
		public:
			block_keeper() = default;
			template<typename condition>
			block_keeper(base_iterator Beg_, base_iterator End_, condition ShouldKeep){
				reset(Beg_, End_, std::forward<condition>(ShouldKeep));
			}
			template<typename condition>
			void reset(base_iterator Beg_, base_iterator End_, condition ShouldKeep){
				reset();

				for(; Beg_!=End_; ++Beg_) {
					if(ShouldKeep(*Beg_)) {
						Itr = Beg_;
						++Size;
						++Beg_;
						break;
					}
				}

				base_iterator From = End_;
				bool InKeepBlock = true;

				for(; Beg_!=End_; ++Beg_) {
					if(ShouldKeep(*Beg_)) {
						if(!InKeepBlock) {
							JumpBlock.emplace_back(From, Beg_);
							InKeepBlock = true;
						}
						++Size;
					} else {
						if(InKeepBlock) {
							From = Beg_;
							InKeepBlock = false;
						}
					}
				}
				if(InKeepBlock) {
					JumpBlock.emplace_back(End_, End_);
				}else{
					JumpBlock.emplace_back(From, End_);
				}
			}
			void reset() {
				JumpBlock.clear();
				Size = 0;
			}
			std::size_t size()const { return Size; }
			iterator begin()const { return iterator(JumpBlock.begin(), Itr); }
			iterator end()const { 
				if(JumpBlock.empty())return iterator(JumpBlock.end(), Itr);
				return iterator(JumpBlock.begin(), JumpBlock.back().second);
			}
		private:
			base_iterator Itr;
			std::size_t Size;
		private:
			block_container JumpBlock;
		};

		struct element_base_keep_tag {};
		struct block_base_keep_tag {};
	}
	template<typename iterator, typename condition>
	auto keep_if(algorithm::element_base_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::element_keeper<iterator>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_if(algorithm::block_base_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::block_keeper<iterator>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_if(iterator Beg, iterator End, condition ShouldKeep) {
		return keep_if(algorithm::block_base_keep_tag(),Beg, End, std::forward<condition>(ShouldKeep));
	}
}
#
#endif
