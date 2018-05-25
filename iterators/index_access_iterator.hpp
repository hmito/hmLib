#ifndef HMLIB_ITERATORS_INDEXACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXACCESSITERATOR_INC 100
#
#include<iterator>
#include"../utility.hpp"
#include"../type_traits.hpp"
namespace hmLib {
	template<typename random_access_iterator_, typename index_iterator_, typename index_iterator_category_ = typename std::iterator_traits<index_iterator_>::iterator_category>
	struct index_access_iterator {};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::input_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::input_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::input_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::forward_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::forward_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::forward_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		base_iterator base() { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::bidirectional_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::bidirectional_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::random_access_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::random_access_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::random_access_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		reference operator[](difference_type d) { return Beg[IItr[d]]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		this_type& operator+=(difference_type d) {
			IItr = +d;
			return *this;
		}
		this_type& operator-=(difference_type d) {
			IItr = -d;
			return *this;
		}
		friend this_type operator+(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans += d;
			return ans;
		}
		friend this_type operator+(difference_type d, const this_type& itr) {
			return operator+(itr, d);
		}
		friend this_type operator-(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans -= d;
			return ans;
		}
		friend difference_type operator-(const this_type& itr1, const this_type& itr2) {
			return itr1.IItr - itr2.IItr;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
		friend bool operator>(const this_type& v1, const this_type& v2) {
			return v1.IItr > v2.IItr;
		}
		friend bool operator<(const this_type& v1, const this_type& v2) {
			return v1.IItr < v2.IItr;
		}
		friend bool operator>=(const this_type& v1, const this_type& v2) {
			return v1.IItr >= v2.IItr;
		}
		friend bool operator<=(const this_type& v1, const this_type& v2) {
			return v1.IItr <= v2.IItr;
		}
	};
	template<typename random_access_iterator, typename index_iterator>
	auto make_index_access_iterator(random_access_iterator itr, index_iterator iitr) { return index_access_iterator<random_access_iterator, index_iterator>(itr, iitr); }

	template<typename container_type_, typename index_iterator_, typename index_iterator_category_ = typename std::iterator_traits<index_iterator_>::iterator_category>
	struct container_index_access_iterator {};
	template<typename container_type_, typename index_iterator_>
	struct container_index_access_iterator<container_type_, index_iterator_, std::input_iterator_tag> {
		static_assert(has_begin_and_end<container_type_>::value, "container_type should have both begin and end functions.");
	private:
		using this_type = container_index_access_iterator<container_type_, index_iterator_, std::input_iterator_tag>;
		using container_type = container_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<container_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<container_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::input_iterator_tag;
	private:
		container_type* pContainer;
		index_iterator IItr;
	public://constructer
		container_index_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		container_index_access_iterator(container_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
	public:
		reference operator*()const { return pContainer->at(*IItr); }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename container_type_, typename index_iterator_>
	struct container_index_access_iterator<container_type_, index_iterator_, std::forward_iterator_tag> {
		static_assert(has_begin_and_end<container_type_>::value, "container_type should have both begin and end functions.");
	private:
		using this_type = container_index_access_iterator<container_type_, index_iterator_, std::forward_iterator_tag>;
		using container_type = container_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<container_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<container_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::forward_iterator_tag;
	private:
		container_type* pContainer;
		index_iterator IItr;
	public://constructer
		container_index_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		container_index_access_iterator(container_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
	public:
		reference operator*()const { return pContainer->at(*IItr); }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename container_type_, typename index_iterator_>
	struct container_index_access_iterator<container_type_, index_iterator_, std::bidirectional_iterator_tag> {
		static_assert(has_begin_and_end<container_type_>::value, "container_type should have both begin and end functions.");
	private:
		using this_type = container_index_access_iterator<container_type_, index_iterator_, std::bidirectional_iterator_tag>;
		using container_type = container_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<container_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<container_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
		container_type* pContainer;
		index_iterator IItr;
	public://constructer
		container_index_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		container_index_access_iterator(container_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
	public:
		reference operator*()const { return pContainer->at(*IItr); }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename container_type_, typename index_iterator_>
	struct container_index_access_iterator<container_type_, index_iterator_, std::random_access_iterator_tag> {
		static_assert(has_begin_and_end<container_type_>::value, "container_type should have both begin and end functions.");
	private:
		using this_type = container_index_access_iterator<container_type_, index_iterator_, std::random_access_iterator_tag>;
		using container_type = container_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<container_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<container_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::random_access_iterator_tag;
	private:
		container_type* pContainer;
		index_iterator IItr;
	public://constructer
		container_index_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		container_index_access_iterator(container_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
	public:
		reference operator*()const { return pContainer->at(*IItr); }
		reference operator[](difference_type d) { return pContainer->at(IItr[d]); }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		this_type& operator+=(difference_type d) {
			IItr = +d;
			return *this;
		}
		this_type& operator-=(difference_type d) {
			IItr = -d;
			return *this;
		}
		friend this_type operator+(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans += d;
			return ans;
		}
		friend this_type operator+(difference_type d, const this_type& itr) {
			return operator+(itr, d);
		}
		friend this_type operator-(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans -= d;
			return ans;
		}
		friend difference_type operator-(const this_type& itr1, const this_type& itr2) {
			return itr1.IItr - itr2.IItr;
		}
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
		friend bool operator>(const this_type& v1, const this_type& v2) {
			return v1.IItr > v2.IItr;
		}
		friend bool operator<(const this_type& v1, const this_type& v2) {
			return v1.IItr < v2.IItr;
		}
		friend bool operator>=(const this_type& v1, const this_type& v2) {
			return v1.IItr >= v2.IItr;
		}
		friend bool operator<=(const this_type& v1, const this_type& v2) {
			return v1.IItr <= v2.IItr;
		}
	};
	template<typename container_type, typename index_iterator>
	auto make_container_index_access_iterator(container_type& Container, index_iterator iitr) {
		return container_index_access_iterator<container_type, index_iterator>(Container, iitr);
	}

	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_range {
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using iterator = index_access_iterator<random_access_iterator_, index_iterator_, typename std::iterator_traits<index_iterator_>::iterator_category>;
		using reference = typename std::iterator_traits<iterator>::reference;
	private:
		base_iterator Base;
		index_iterator IBeg;
		index_iterator IEnd;
	public:
		index_access_range() = default;
		index_access_range(base_iterator Base_, index_iterator IBeg_, index_iterator IEnd_):Base(Base_), IBeg(IBeg_), IEnd(IEnd_) {}
		iterator begin()const { return iterator(Base, IBeg); }
		iterator end()const { return iterator(Base, IEnd); }
		reference at(std::size_t Index)const { return *std::next(Base, *std::next(IBeg,Index)); }
		std::size_t size()const { return std::distance(IBeg, IEnd); }
	};
	template<typename iterator, typename index_iterator, typename std::enable_if<std::is_same<typename std::iterator_traits<iterator>::iterator_category, std::random_access_iterator_tag>::value, std::nullptr_t>::type = nullptr>
	auto make_index_access_range(iterator Itr, index_iterator IndexBegin, index_iterator IndexEnd) {
		return index_access_range<iterator, index_iterator>(Itr, IndexBegin, IndexEnd);
	}

	template<typename container_type_, typename index_iterator_>
	struct container_index_access_range {
		using container_type = container_type_;
		using index_iterator = index_iterator_;
	public:
		using iterator = container_index_access_iterator<container_type, index_iterator>;
		using reference = typename std::iterator_traits<iterator>::reference;
	private:
		container_type* pContainer;
		index_iterator IBeg;
		index_iterator IEnd;
	public:
		container_index_access_range() = default;
		container_index_access_range(container_type& Container_, index_iterator IBeg_, index_iterator IEnd_):pContainer(&Container_), IBeg(IBeg_), IEnd(IEnd_) {}
		iterator begin()const { return iterator(*pContainer, IBeg); }
		iterator end()const { return iterator(*pContainer, IEnd); }
		reference at(std::size_t Index)const { return pContainer->at(*std::next(IBeg, Index)); }
		std::size_t size()const { return std::distance(IBeg, IEnd); }
	};
	template<typename container_type, typename index_iterator>
	auto make_container_index_access_range(container_type& Container, index_iterator IndexBegin, index_iterator IndexEnd) {
		return container_index_access_range<container_type, index_iterator>(Container, IndexBegin, IndexEnd);
	}
}
#
#endif
