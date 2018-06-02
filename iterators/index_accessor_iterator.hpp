#ifndef HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXATACCESSITERATOR_INC 100
#
#include<iterator>
#include"../utility.hpp"
#include"../type_traits.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename container_type_, typename index_type_>
			struct container_at_accessor {
			public:
				using container_type = container_type_;
				using index_type = index_type_;
			private:
				container_type* pContainer;
			public:
				container_at_accessor()noexcept : pContainer(nullptr) {}
				container_at_accessor(container_type* pContainer_)noexcept : pContainer(pContainer_) {}
				auto& operator()(index_type Index){ return pContainer->at(Index); }
			};
			template<typename container_type_, typename index_type_>
			struct container_op_accessor {
			public:
				using container_type = container_type_;
				using index_type = index_type_;
			private:
				container_type* pContainer;
			public:
				container_op_accessor()noexcept : pContainer(nullptr) {}
				container_op_accessor(container_type* pContainer_)noexcept : pContainer(pContainer_) {}
				auto& operator()(index_type Index){ return pContainer->operator[](Index); }
			};

			template<typename this_type_, typename accessor_, typename index_iterator_, typename index_iterator_category = typename std::iterator_traits<index_iterator_>::iterator_category>
			struct index_accessor_iterator_mixin{
				using this_type = this_type_;
				using accessor = accessor_;
				using index_iterator = index_iterator_;
				using index_type = decltype(*std::declval<index_iterator>());
			public:
				using value_type = decltype(std::declval<accessor>()(std::declval<index_type>()));
				using difference_type = int;
				using reference = decltype((std::declval<accessor>()(std::declval<index_type>())));
				using pointer = void;
			public:
				index_accessor_iterator_mixin(accessor Acs_, index_iterator IItr_):Acs(std::move(Acs_)), IItr(std::move(IItr_)) {}
				auto index()const { return *IItr; }
			protected:
				accessor Acs;
				index_iterator IItr;
			};
			template<typename this_type_, typename accessor_, typename index_iterator_>
			struct index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::input_iterator_tag>
				: public index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, void> {
			private:
				using base_type = index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, void>;
			public:
				index_accessor_iterator_mixin(accessor Acs_, index_iterator IItr_):base_type(std::move(Acs_), std::move(IItr_)) {}
			public:
				reference operator*() { return Acs(*IItr); }
				this_type& operator++() {
					++IItr;
					return static_cast<this_type&>(*this);
				}
				this_type operator++(int) {
					this_type Prev = static_cast<this_type&>(*this);
					operator++();
					return Prev;
				}
				friend bool operator==(const this_type& v1, const this_type& v2) {
					return v1.IItr == v2.IItr;
				}
				friend bool operator!=(const this_type& v1, const this_type& v2) {
					return v1.IItr != v2.IItr;
				}
			};
			template<typename this_type_, typename accessor_, typename index_iterator_>
			struct index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::forward_iterator_tag>
				: public index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::input_iterator_tag> {
			private:
				using base_type = index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::input_iterator_tag>;
			public:
				index_accessor_iterator_mixin(accessor Acs_, index_iterator IItr_):base_type(std::move(Acs_), std::move(IItr_)) {}
			};
			template<typename this_type_, typename accessor_, typename index_iterator_>
			struct index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::bidirectional_iterator_tag>
				: public index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::forward_iterator_tag> {
			private:
				using base_type = index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::forward_iterator_tag>;
			public:
				index_accessor_iterator_mixin(accessor Acs_, index_iterator IItr_):base_type(std::move(Acs_), std::move(IItr_)) {}
			public:
				this_type& operator--() {
					--IItr;
					return static_cast<this_type&>(*this);
				}
				this_type operator--(int) {
					this_type Prev = static_cast<this_type&>(*this);
					operator--();
					return Prev;
				}
			};
			template<typename this_type_, typename accessor_, typename index_iterator_>
			struct index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::random_access_iterator_tag>
				: public index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::bidirectional_iterator_tag> {
			private:
				using base_type = index_accessor_iterator_mixin<this_type_, accessor_, index_iterator_, std::bidirectional_iterator_tag>;
			public:
				index_accessor_iterator_mixin(accessor Acs_, index_iterator IItr_):base_type(std::move(Acs_), std::move(IItr_)) {}
			public:
				reference operator[](difference_type d) { return Acs(IItr[d]); }
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
				friend difference_type operator-(const this_type& v1, const this_type& v2) {
					return v1.IItr - v2.IItr;
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
		}

		template<template<typename, typename> typename accessor_, typename container_type_, typename index_iterator_>
		struct index_container_access_iterator
			: public detail::index_accessor_iterator_mixin<
			index_container_access_iterator<accessor_, container_type_, index_iterator_>,
			accessor_<container_type_, decltype(*std::declval<index_iterator_>())>,
			index_iterator_
			> {
		private:
			using this_type = index_container_access_iterator<accessor_, container_type_, index_iterator_>;
			using accessor = accessor_<container_type_, decltype(*std::declval<index_iterator_>())>;
			using index_iterator = index_iterator_;
			using base_type = detail::index_accessor_iterator_mixin<this_type, accessor, index_iterator>;
		public:
			using iterator_category = typename std::iterator_traits<index_iterator>::iterator_category;
		public://constructer
			index_container_access_iterator()noexcept : base_type() {}
			index_container_access_iterator(container_type_& Container_, index_iterator_ IItr_):base_type(accessor(&Container_), IItr_) {}
		};
		template<template<typename, typename> typename accessor_, typename container_type_, typename index_iterator_>
		struct index_container_access_range {
			using container_type = container_type_;
			using index_iterator = index_iterator_;
		public:
			using iterator = index_container_access_iterator<accessor_, container_type, index_iterator>;
			using reference = typename std::iterator_traits<iterator>::reference;
		private:
			container_type* pContainer;
			index_iterator IBeg;
			index_iterator IEnd;
		public:
			index_container_access_range() = default;
			index_container_access_range(container_type& Container_, index_iterator IBeg_, index_iterator IEnd_):pContainer(&Container_), IBeg(IBeg_), IEnd(IEnd_) {}
			iterator begin()const { return iterator(*pContainer, IBeg); }
			iterator end()const { return iterator(*pContainer, IEnd); }
			reference at(std::size_t Index)const { return pContainer->at(*std::next(IBeg, Index)); }
			std::size_t size()const { return std::distance(IBeg, IEnd); }
		};
	}

	template<typename container_type, typename index_iterator>
	using index_at_access_iterator = iterators::index_container_access_iterator<iterators::detail::container_at_accessor, container_type, index_iterator>;
	template<typename container_type, typename index_iterator>
	using index_at_access_range = iterators::index_container_access_range<iterators::detail::container_at_accessor, container_type, index_iterator>;
	template<typename range_type, typename index_iterator>
	auto make_index_at_access_iterator(range_type& Container, index_iterator iitr) {return index_at_access_iterator<range_type, index_iterator>(Container, iitr);}
	template<typename range_type, typename index_iterator>
	auto make_index_at_access_range(range_type& Container, index_iterator IndexBegin, index_iterator IndexEnd) {return index_at_access_range<range_type, index_iterator>(Container, IndexBegin, IndexEnd);}

	template<typename container_type, typename index_iterator>
	using index_op_access_iterator = iterators::index_container_access_iterator<iterators::detail::container_op_accessor, container_type, index_iterator>;
	template<typename container_type, typename index_iterator>
	using index_op_access_range = iterators::index_container_access_range<iterators::detail::container_op_accessor, container_type, index_iterator>;
	template<typename range_type, typename index_iterator>
	auto make_index_op_access_iterator(range_type& Container, index_iterator iitr) { return index_op_access_iterator<range_type, index_iterator>(Container, iitr); }
	template<typename range_type, typename index_iterator>
	auto make_index_op_access_range(range_type& Container, index_iterator IndexBegin, index_iterator IndexEnd) { return index_op_access_range<range_type, index_iterator>(Container, IndexBegin, IndexEnd); }
}
#
#endif
