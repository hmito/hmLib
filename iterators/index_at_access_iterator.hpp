#ifndef HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC 100
#
#include<iterator>
#include"../utility.hpp"
#include"../type_traits.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename this_type, typename range_type_, typename index_iterator_, typename index_iterator_category>
			struct index_at_access_iterator_mixin{
			protected:
				template<typename this_type_ref>
				static auto& container(this_type_ref& This) { return static_cast<this_type&>(This).pContainer; }
				template<typename this_type_ref>
				static const auto& container(const this_type_ref& This) { return static_cast<const this_type&>(This).pContainer; }
				template<typename this_type_ref>
				static auto& iitr(this_type_ref& This) { return static_cast<this_type&>(This).IItr; }
				template<typename this_type_ref>
				static const auto& iitr(const this_type_ref& This) { return static_cast<const this_type&>(This).IItr; }
			public:
				using value_type = decltype(std::declval<range_type_>().at(*std::declval<index_iterator_>()));
				using difference_type = int;
				using reference = decltype((std::declval<range_type_>().at(*std::declval<index_iterator_>())));
				using pointer = void;
			};
			template<typename this_type, typename range_type_, typename index_iterator_>
			struct index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::input_iterator_tag>
				:public index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, void> {
			public:
				reference operator*()const { return container(*this)->at(*iitr(*this)); }
				this_type& operator++() {
					this_type& This = static_cast<this_type&>(*this);
					++iitr(This);
					return This;
				}
				this_type operator++(int) {
					this_type Prev = static_cast<this_type&>(*this);
					operator++();
					return Prev;
				}
				friend bool operator==(const this_type& v1, const this_type& v2) {
					return iitr(v1) == iitr(v2);
				}
				friend bool operator!=(const this_type& v1, const this_type& v2) {
					return iitr(v1) != iitr(v2);
				}
			};
			template<typename this_type, typename range_type_, typename index_iterator_>
			struct index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::forward_iterator_tag>
				:public index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::input_iterator_tag> {
			};
			template<typename this_type, typename range_type_, typename index_iterator_>
			struct index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::bidirectional_iterator_tag>
				:public index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::forward_iterator_tag> {
			public:
				this_type& operator++() {
					this_type& This = static_cast<this_type&>(*this);
					--iitr(This);
					return This;
				}
				this_type operator++(int) {
					this_type Prev = static_cast<this_type&>(*this);
					operator--();
					return Prev;
				}
			};
			template<typename this_type, typename range_type_, typename index_iterator_>
			struct index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::random_access_iterator_tag>
				:public index_at_access_iterator_mixin<this_type, range_type_, index_iterator_, std::bidirectional_iterator_tag> {
			public:
				reference operator[](difference_type d) { return container(*this)->at(IItr[d]); }
				this_type& operator+=(difference_type d) {
					iitr(*this) = +d;
					return *this;
				}
				this_type& operator-=(difference_type d) {
					iitr(*this) = -d;
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
					return iitr(itr1) - iitr(itr2);
				}
				friend bool operator>(const this_type& v1, const this_type& v2) {
					return iitr(v1) > iitr(v2);
				}
				friend bool operator<(const this_type& v1, const this_type& v2) {
					return iitr(v1) < iitr(v2);
				}
				friend bool operator>=(const this_type& v1, const this_type& v2) {
					return iitr(v1) >= iitr(v2);
				}
				friend bool operator<=(const this_type& v1, const this_type& v2) {
					return iitr(v1) <= iitr(v2);
				}
			};
		}
	}
	template<typename range_type_, typename index_iterator_, typename index_iterator_category_ = typename std::iterator_traits<index_iterator_>::iterator_category>
	struct index_at_access_iterator
		: public iterators::detail::index_at_access_iterator_mixin<
			index_at_access_iterator<range_type_, index_iterator_, index_iterator_category_>, 
			range_type_, index_iterator_, index_iterator_category_		
		>{
		friend struct iterators::detail::index_at_access_iterator_mixin<
			index_at_access_iterator<range_type_, index_iterator_, index_iterator_category_>,
			range_type_, index_iterator_, void
		>;
	private:
		using this_type = index_at_access_iterator<range_type_, index_iterator_, index_iterator_category_>;
		using range_type = range_type_;
		using index_iterator = index_iterator_;
	public:
		using iterator_category = index_iterator_category_;
	private:
		range_type* pContainer;
		index_iterator IItr;
	public://constructer
		index_at_access_iterator()noexcept : pContainer(nullptr), IItr() {}
		index_at_access_iterator(range_type& Container_, index_iterator_ IItr_):pContainer(&Container_), IItr(IItr_) {}
	public:
		auto index()const { return *(static_cast<this_type&>(*this).IItr); }
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
