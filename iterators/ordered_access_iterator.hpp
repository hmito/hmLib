#ifndef HMLIB_ITERATORS_ORDEREDACCESSITERATOR_INC
#define HMLIB_ITERATORS_ORDEREDACCESSITERATOR_INC 100
#
#include<iterator>
#include"transform_iterator.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename iterator>
			struct itr_index_accessor {
				using difference_type = typename std::iterator_traits<iterator>::difference_type;
				using reference = typename std::iterator_traits<iterator>::reference;
			private:
				iterator beg;
			public:
				itr_index_accessor(iterator beg_):beg(beg_) {}
				reference operator()(difference_type n) { return *std::next(beg, n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct op_index_accessor {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				op_index_accessor(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->operator[](n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct at_index_accessor {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				at_index_accessor(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->at(n); }
			};
		}
	}
	template<typename iterator_, typename index_iterator_>
	using ordered_itr_access_iterator = transform_iterator<index_iterator_, iterators::detail::itr_index_accessor<iterator_>>;
	template<typename iterator_, typename index_iterator_>
	using ordered_itr_access_range = transform_range<index_iterator_, iterators::detail::itr_index_accessor<iterator_>>;

	template<typename iterator_, typename index_iterator_>
	auto make_ordered_itr_access_iterator(iterator_ TergetItr, index_iterator_ IndexItr) {
		return ordered_itr_access_iterator<iterator_, index_iterator_>(
			IndexItr,
			iterators::detail::itr_index_accessor<iterator_>(TergetItr)
			);
	}
	template<typename iterator_, typename index_iterator_>
	auto make_ordered_itr_access_range(iterator_ TergetItr, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return ordered_itr_access_range<iterator_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::itr_index_accessor<iterator_>(TergetItr)
		);
	}

	template<typename container_, typename index_iterator_>
	using ordered_at_access_iterator = transform_iterator<index_iterator_, iterators::detail::at_index_accessor<container_>>;
	template<typename container_, typename index_iterator_>
	using ordered_at_access_range = transform_range<index_iterator_, iterators::detail::at_index_accessor<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_ordered_at_access_iterator(container_& Container, index_iterator_ IndexItr) {
		return ordered_itr_access_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::at_index_accessor<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_ordered_at_access_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return ordered_at_access_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::at_index_accessor<container_>(Container)
		);
	}

	template<typename container_, typename index_iterator_>
	using ordered_op_access_iterator = transform_iterator<index_iterator_, iterators::detail::op_index_accessor<container_>>;
	template<typename container_, typename index_iterator_>
	using ordered_op_access_range = transform_range<index_iterator_, iterators::detail::op_index_accessor<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_ordered_op_access_iterator(container_& Container, index_iterator_ IndexItr) {
		return ordered_itr_access_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::op_index_accessor<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_ordered_op_access_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return ordered_op_access_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::op_index_accessor<container_>(Container)
		);
	}
}
#
#endif
