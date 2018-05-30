#ifndef HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC 100
#
#include<iterator>
#include"../utility.hpp"
#include"../type_traits.hpp"
namespace hmLib {
	template<typename range_type_, typename index_iterator_, typename index_iterator_category_ = typename std::iterator_traits<index_iterator_>::iterator_category>
	struct index_at_access_iterator {};
	template<typename range_type_, typename index_iterator_>
	struct index_at_access_iterator<range_type_, index_iterator_, std::input_iterator_tag> {
		static_assert(has_begin_and_end<range_type_>::value, "range_type should have both begin and end functions.");
	private:
		using this_type = index_at_access_iterator<range_type_, index_iterator_, std::input_iterator_tag>;
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<range_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<range_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::input_iterator_tag;
	private:
		range_type* pContainer;
		index_iterator IItr;
	public://constructer
		index_at_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		index_at_access_iterator(range_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
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
	template<typename range_type_, typename index_iterator_>
	struct index_at_access_iterator<range_type_, index_iterator_, std::forward_iterator_tag> {
		static_assert(has_begin_and_end<range_type_>::value, "range_type should have both begin and end functions.");
	private:
		using this_type = index_at_access_iterator<range_type_, index_iterator_, std::forward_iterator_tag>;
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<range_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<range_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::forward_iterator_tag;
	private:
		range_type* pContainer;
		index_iterator IItr;
	public://constructer
		index_at_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		index_at_access_iterator(range_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
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
	template<typename range_type_, typename index_iterator_>
	struct index_at_access_iterator<range_type_, index_iterator_, std::bidirectional_iterator_tag> {
		static_assert(has_begin_and_end<range_type_>::value, "range_type should have both begin and end functions.");
	private:
		using this_type = index_at_access_iterator<range_type_, index_iterator_, std::bidirectional_iterator_tag>;
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<range_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<range_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
		range_type* pContainer;
		index_iterator IItr;
	public://constructer
		index_at_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		index_at_access_iterator(range_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
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
	template<typename range_type_, typename index_iterator_>
	struct index_at_access_iterator<range_type_, index_iterator_, std::random_access_iterator_tag> {
		static_assert(has_begin_and_end<range_type_>::value, "range_type should have both begin and end functions.");
	private:
		using this_type = index_at_access_iterator<range_type_, index_iterator_, std::random_access_iterator_tag>;
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using value_type = decltype(std::declval<range_type>().at(*std::declval<index_iterator>()));
		using difference_type = int;
		using reference = decltype((std::declval<range_type>().at(*std::declval<index_iterator>())));
		using pointer = void;
		using iterator_category = std::random_access_iterator_tag;
	private:
		range_type* pContainer;
		index_iterator IItr;
	public://constructer
		index_at_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		index_at_access_iterator(range_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
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
	template<typename range_type, typename index_iterator>
	auto make_index_at_access_iterator(range_type& Container, index_iterator iitr) {
		return index_at_access_iterator<range_type, index_iterator>(Container, iitr);
	}

	template<typename range_type_, typename index_iterator_>
	struct index_at_access_range {
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using iterator = index_at_access_iterator<range_type, index_iterator>;
		using reference = typename std::iterator_traits<iterator>::reference;
	private:
		range_type* pContainer;
		index_iterator IBeg;
		index_iterator IEnd;
	public:
		index_at_access_range() = default;
		index_at_access_range(range_type& Container_, index_iterator IBeg_, index_iterator IEnd_):pContainer(&Container_), IBeg(IBeg_), IEnd(IEnd_) {}
		iterator begin()const { return iterator(*pContainer, IBeg); }
		iterator end()const { return iterator(*pContainer, IEnd); }
		reference at(std::size_t Index)const { return pContainer->at(*std::next(IBeg, Index)); }
		std::size_t size()const { return std::distance(IBeg, IEnd); }
	};
	template<typename range_type, typename index_iterator>
	auto make_index_at_access_range(range_type& Container, index_iterator IndexBegin, index_iterator IndexEnd) {
		return index_at_access_range<range_type, index_iterator>(Container, IndexBegin, IndexEnd);
	}
}
#
#endif
