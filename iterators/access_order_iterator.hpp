#ifndef HMLIB_ITERATORS_ACCESSORDERITERATOR_INC
#define HMLIB_ITERATORS_ACCESSORDERITERATOR_INC 100
#
#include<iterator>
#include"transform_iterator.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename iterator>
			struct itr_access_order {
				using difference_type = typename std::iterator_traits<iterator>::difference_type;
				using reference = typename std::iterator_traits<iterator>::reference;
			private:
				iterator beg;
			public:
				itr_access_order(iterator beg_):beg(beg_) {}
				reference operator()(difference_type n) { return *std::next(beg, n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct op_access_ordered {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				op_access_ordered(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->operator[](n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct at_access_ordered {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				at_access_ordered(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->at(n); }
			};
		}
	}
	template<typename iterator_, typename index_iterator_>
	using itr_access_order_iterator = transform_iterator<index_iterator_, iterators::detail::itr_access_order<iterator_>>;
	template<typename iterator_, typename index_iterator_>
	using itr_access_order_range = transform_range<index_iterator_, iterators::detail::itr_access_order<iterator_>>;

	template<typename iterator_, typename index_iterator_>
	auto make_itr_access_order_iterator(iterator_ TergetItr, index_iterator_ IndexItr) {
		return itr_access_order_iterator<iterator_, index_iterator_>(
			IndexItr,
			iterators::detail::itr_access_order<iterator_>(TergetItr)
			);
	}
	template<typename iterator_, typename index_iterator_>
	auto make_itr_access_order_range(iterator_ TergetItr, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return itr_access_order_range<iterator_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::itr_access_order<iterator_>(TergetItr)
		);
	}

	template<typename container_, typename index_iterator_>
	using at_access_ordered_iterator = transform_iterator<index_iterator_, iterators::detail::at_access_ordered<container_>>;
	template<typename container_, typename index_iterator_>
	using at_access_ordered_range = transform_range<index_iterator_, iterators::detail::at_access_ordered<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_at_access_ordered_iterator(container_& Container, index_iterator_ IndexItr) {
		return itr_access_order_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::at_access_ordered<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_at_access_ordered_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return at_access_ordered_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::at_access_ordered<container_>(Container)
		);
	}

	template<typename container_, typename index_iterator_>
	using op_access_ordered_iterator = transform_iterator<index_iterator_, iterators::detail::op_access_ordered<container_>>;
	template<typename container_, typename index_iterator_>
	using op_access_ordered_range = transform_range<index_iterator_, iterators::detail::op_access_ordered<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_op_access_ordered_iterator(container_& Container, index_iterator_ IndexItr) {
		return itr_access_order_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::op_access_ordered<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_op_access_ordered_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return op_access_ordered_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::op_access_ordered<container_>(Container)
		);
	}
}
#
#endif
