#ifndef HMLIB_ALGORITHM_KEEPIF_INC
#define HMLIB_ALGORITHM_KEEPIF_INC 100
#
#include<iterator>
#include<utility>
#include<vector>
#include"../utility.hpp"
#include"../type_traits.hpp"
#include"../iterators/index_access_iterator.hpp"
namespace hmLib {
	namespace algorithm {
		template<typename forward_iterator_, bool is_const = is_const_iterator<forward_iterator_>::value>
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
			using const_reference = typename std::add_const<reference>::type;
			using const_pointer = typename std::add_const<pointer>::type;
		public:
			struct const_iterator;
			struct iterator {
				friend const_iterator;
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				element_iterator EItr;
			public:
				iterator() = default;
				explicit iterator(element_iterator EItr_)
					: EItr(EItr_){
				}
				reference operator*() {
					return (*EItr).operator*();
				}
				pointer operator->() {
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
				base_iterator base() { return *EItr; }
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return *EItr; }
				friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.EItr ==itr2.EItr; }
				friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.EItr !=itr2.EItr; }
			};
			struct const_iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::const_reference;
				using pointer = typename this_type::const_pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				element_iterator EItr;
			public:
				const_iterator() = default;
				const_iterator(iterator itr):EItr(itr.EItr){}
				explicit const_iterator(element_iterator EItr_)
					: EItr(EItr_) {
				}
				reference operator*()const {
					return (*EItr).operator*();
				}
				pointer operator->()const {
					return (*EItr).operator->();
				}
				const_iterator& operator++() {
					++EItr;
					return *this;
				}
				const_iterator operator++(int) {
					const_iterator Prev = *this;
					++(*this);
					return Prev;
				}
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return *EItr; }
				friend bool operator==(const const_iterator& itr1, const const_iterator& itr2) { return itr1.EItr ==itr2.EItr; }
				friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2) { return itr1.EItr !=itr2.EItr; }
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
			void push_back(base_iterator Itr_) {
				KeptBlock.push_back(Itr_);
			}
			std::size_t size()const { return KeptBlock.size(); }
			iterator begin() { return iterator(KeptBlock.begin()); }
			iterator end() { return iterator(KeptBlock.end()); }
			const_iterator cbegin()const { return iterator(KeptBlock.begin()); }
			const_iterator cend()const { return iterator(KeptBlock.end()); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
		private:
			element_container KeptBlock;
		};
		template<typename forward_iterator_>
		struct element_keeper<forward_iterator_, true> {
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
		public:
			struct const_iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				element_iterator EItr;
			public:
				const_iterator() = default;
				const_iterator(element_iterator EItr_)
					: EItr(EItr_) {
				}
				reference operator*()const {
					return (*EItr).operator*();
				}
				pointer operator->()const {
					return (*EItr).operator->();
				}
				const_iterator& operator++() {
					++EItr;
					return *this;
				}
				const_iterator operator++(int) {
					const_iterator Prev = *this;
					++(*this);
					return Prev;
				}
				base_iterator base() const { return *EItr; }
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return *EItr; }
				friend bool operator==(const const_iterator& itr1, const const_iterator& itr2) { return itr1.EItr ==itr2.EItr; }
				friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2) { return itr1.EItr !=itr2.EItr; }
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
			void push_back(base_iterator Itr_) {
				KeptBlock.push_back(Itr_);
			}
			std::size_t size()const { return KeptBlock.size(); }
			const_iterator cbegin()const { return const_iterator(KeptBlock.begin()); }
			const_iterator cend()const { return const_iterator(KeptBlock.end()); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
		private:
			element_container KeptBlock;
		};
		template<typename forward_iterator_, bool is_const = is_const_iterator<forward_iterator_>::value>
		struct block_keeper {
		private:
			using this_type = block_keeper <forward_iterator_>;
			using base_iterator = forward_iterator_;
			using block_container = std::vector<std::pair<base_iterator, base_iterator>>;
			using block_iterator = typename block_container::const_iterator;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using const_reference = typename std::add_const<reference>::type;
			using const_pointer = typename std::add_const<pointer>::type;
		public:
			struct const_iterator;
			struct iterator{
				friend const_iterator;
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
				reference operator*() {
					return Itr.operator*();
				}
				pointer operator->() {
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
				base_iterator base()const { return Itr; }
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return Itr; }
				friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.Itr ==itr2.Itr; }
				friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.Itr !=itr2.Itr; }
			};
			struct const_iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::const_reference;
				using pointer = typename this_type::const_pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				block_iterator BItr;
				base_iterator Itr;
			public:
				const_iterator() = default;
				const_iterator(iterator itr):BItr(itr.BItr),Itr(itr.Itr){}
				const_iterator(block_iterator BlockBeg_, base_iterator Itr_)
					: BItr(BlockBeg_)
					, Itr(Itr_) {
				}
				reference operator*()const {
					return Itr.operator*();
				}
				pointer operator->()const {
					return Itr.operator->();
				}
				const_iterator& operator++() {
					++Itr;
					if(Itr == BItr->first) {
						Itr = BItr->second;
						++BItr;
					}
					return *this;
				}
				const_iterator operator++(int) {
					const_iterator Prev = *this;
					++(*this);
					return Prev;
				}
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return Itr; }
				friend bool operator==(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Itr ==itr2.Itr; }
				friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Itr !=itr2.Itr; }
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
						break;
					}
				}

				if(Beg_==End_) { return; }

				base_iterator From = End_;
				bool InKeepBlock = true;

				++Beg_;
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
			bool empty()const { return Size==0; }
			iterator begin() { return iterator(JumpBlock.begin(), Itr); }
			iterator end() { 
				if(empty())return iterator(JumpBlock.end(), Itr);
				return iterator(JumpBlock.begin(), JumpBlock.back().second);
			}
			const_iterator begin()const{ return cbegin(); }
			const_iterator end() const { return cend(); }
			const_iterator cbegin()const { return const_iterator(JumpBlock.begin(), Itr); }
			const_iterator cend() const {
				if(JumpBlock.empty())return const_iterator(JumpBlock.end(), Itr);
				return iterator(JumpBlock.begin(), JumpBlock.back().second);
			}
		private:
			base_iterator Itr;
			std::size_t Size;
		private:
			block_container JumpBlock;
		};
		template<typename forward_iterator_>
		struct block_keeper<forward_iterator_,true>{
		private:
			using this_type = block_keeper <forward_iterator_>;
			using base_iterator = forward_iterator_;
			using block_container = std::vector<std::pair<base_iterator, base_iterator>>;
			using block_iterator = typename block_container::const_iterator;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
		public:
			struct const_iterator {
				using value_type = typename this_type::value_type;
				using difference_type = typename this_type::difference_type;
				using reference = typename this_type::reference;
				using pointer = typename this_type::pointer;
				using iterator_category = std::forward_iterator_tag;
			private:
				block_iterator BItr;
				base_iterator Itr;
			public:
				const_iterator() = default;
				const_iterator(block_iterator BlockBeg_, base_iterator Itr_)
					: BItr(BlockBeg_)
					, Itr(Itr_) {
				}
				reference operator*()const {
					return Itr.operator*();
				}
				pointer operator->()const {
					return Itr.operator->();
				}
				const_iterator& operator++() {
					++Itr;
					if(Itr == BItr->first) {
						Itr = BItr->second;
						++BItr;
					}
					return *this;
				}
				const_iterator operator++(int) {
					const_iterator Prev = *this;
					++(*this);
					return Prev;
				}
				base_iterator base()const { return Itr; }
				template<typename const_base_iterator, typename std::enable_if<std::is_convertible<base_iterator, const_base_iterator>::value && hmLib::is_const_iterator<const_base_iterator>::value>::type*& = hmLib::utility::enabler>
				const_base_iterator const_base()const { return Itr; }
				friend bool operator==(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Itr ==itr2.Itr; }
				friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2) { return itr1.Itr !=itr2.Itr; }
			};
		public:
			block_keeper() = default;
			template<typename condition>
			block_keeper(base_iterator Beg_, base_iterator End_, condition ShouldKeep) {
				reset(Beg_, End_, std::forward<condition>(ShouldKeep));
			}
			template<typename condition>
			void reset(base_iterator Beg_, base_iterator End_, condition ShouldKeep) {
				reset();

				for(; Beg_!=End_; ++Beg_) {
					if(ShouldKeep(*Beg_)) {
						Itr = Beg_;
						++Size;
						break;
					}
				}

				if(Beg_==End_)return;

				base_iterator From = End_;
				bool InKeepBlock = true;

				++Beg_;
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
				} else {
					JumpBlock.emplace_back(From, End_);
				}
			}
			void reset() {
				JumpBlock.clear();
				Size = 0;
			}
			bool empty()const { return Size==0; }
			std::size_t size()const { return Size; }
			const_iterator cbegin()const { return const_iterator(JumpBlock.begin(), Itr); }
			const_iterator cend()const {
				if(JumpBlock.empty())return const_iterator(JumpBlock.end(), Itr);
				return const_iterator(JumpBlock.end(), JumpBlock.back().second);
			}
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
		private:
			base_iterator Itr;
			std::size_t Size;
		private:
			block_container JumpBlock;
		};

		template<typename index_type_ = int>
		struct element_index_keeper {
		private:
			using this_type = element_index_keeper <index_type_>;
			using index_type = index_type_;
			using element_container = std::vector<index_type>;
			using const_iterator = typename element_container::const_iterator;
		public:
			using value_type = index_type;
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using reference = value_type;
			using pointer = void;
			using const_reference = void;
			using const_pointer = void;
		public:
			element_index_keeper() = default;
			template<typename forward_iterator, typename condition>
			element_index_keeper(forward_iterator Beg_, forward_iterator End_, condition ShouldKeep) {
				reset(Beg_, End_, std::forward<condition>(ShouldKeep));
			}
			template<typename forward_iterator, typename condition>
			void reset(forward_iterator Beg_, forward_iterator End_, condition ShouldKeep) {
				reset();

				index_type Index = 0;
				for(; Beg_!=End_; ++Beg_, ++Index) {
					if(ShouldKeep(*Beg_)) {
						KeptBlock.push_back(Index);
					}
				}
			}
			void reset() { KeptBlock.clear(); }
			void push_back(index_type Index_) {
				KeptBlock.push_back(Index_);
			}
			std::size_t size()const { return KeptBlock.size(); }
			const_iterator cbegin()const { return KeptBlock.cbegin(); }
			const_iterator cend()const { return KeptBlock.cend(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
			template<typename random_access_iterator, typename std::enable_if<std::is_constructible<typename std::iterator_traits<random_access_iterator>::iterator_category, typename std::random_access_iterator_tag>::value,std::nullptr_t>::type = nullptr>
			auto range(random_access_iterator Beg)const{
				return make_index_access_range(Beg, begin(),end());
			}
		private:
			element_container KeptBlock;
		};
		template<typename index_type_ = int>
		struct block_index_keeper {
		private:
			using this_type = block_index_keeper <index_type_>;
			using index_type = index_type_;
			using block_container = std::vector<std::pair<index_type, index_type>>;
			using block_iterator = typename block_container::const_iterator;
		public:
			using value_type = index_type;
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
				index_type Index;
			public:
				const_iterator() = default;
				const_iterator(block_iterator BlockBeg_, index_type Index_)
					: BItr(BlockBeg_)
					, Index(Index_) {
				}
				reference operator*()const {
					return Index;
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
			block_index_keeper()noexcept : JumpBlock(), Size(0), Index(0) {}
			template<typename forward_iterator, typename condition>
			block_index_keeper(forward_iterator Beg_, forward_iterator End_, condition ShouldKeep) {
				reset(Beg_, End_, std::forward<condition>(ShouldKeep));
			}
			template<typename forward_iterator, typename condition>
			void reset(forward_iterator Beg_, forward_iterator End_, condition ShouldKeep) {
				reset();

				index_type Cnt=0;
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
			const_iterator cbegin()const { return const_iterator(JumpBlock.begin(), Index); }
			const_iterator cend()const {
				if(JumpBlock.empty())return const_iterator(JumpBlock.end(), Index);
				return const_iterator(JumpBlock.end(), JumpBlock.back().second);
			}
			const_iterator begin()const { return cbegin(); }
			const_iterator end() const { return cend(); }
			template<typename random_access_iterator, typename std::enable_if<std::is_constructible<typename std::iterator_traits<random_access_iterator>::iterator_category, typename std::random_access_iterator_tag>::value, std::nullptr_t>::type = nullptr>
			auto range(random_access_iterator Beg)const {
				return make_index_access_range(Beg, begin(), end());
			}
		private:
			index_type Index;
			std::size_t Size;
			block_container JumpBlock;
		};

		static constexpr struct element_keep_tag {}element_keep;
		static constexpr struct block_keep_tag {}block_keep;
	}
	template<typename iterator, typename condition>
	auto keep_if(algorithm::element_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::element_keeper<iterator>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_if(algorithm::block_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::block_keeper<iterator>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_if(iterator Beg, iterator End, condition ShouldKeep) {
		return keep_if(algorithm::block_keep,Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_index_if(algorithm::element_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::element_index_keeper<iterator>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_index_if(algorithm::block_keep_tag, iterator Beg, iterator End, condition ShouldKeep) {
		return algorithm::block_index_keeper<typename std::iterator_traits<iterator>::difference_type>(Beg, End, std::forward<condition>(ShouldKeep));
	}
	template<typename iterator, typename condition>
	auto keep_index_if(iterator Beg, iterator End, condition ShouldKeep) {
		return keep_index_if(algorithm::block_keep, Beg, End, std::forward<condition>(ShouldKeep));
	}
}
#
#endif
