#ifndef HMLIB_ITERATORS_INDEXACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXACCESSITERATOR_INC 100
#
#include<iterator>
#include"transform_iterator.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename iterator>
			struct index_iterator_access {
				using difference_type = typename std::iterator_traits<iterator>::difference_type;
				using reference = typename std::iterator_traits<iterator>::reference;
			private:
				iterator beg;
			public:
				index_iterator_access(iterator beg_):beg(beg_) {}
				reference operator()(difference_type n) { return *std::next(beg, n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct index_op_access {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				index_op_access(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->operator[](n); }
			};
			template<typename container, typename index_type = unsigned int>
			struct index_at_access {
				using reference = decltype((std::declval<container>()[std::declval<index_type>()]));
			private:
				container* pContainer;
			public:
				index_at_access(container& Container_): pContainer(&Container_) {}
				reference operator()(index_type n) { return pContainer->at(n); }
			};
		}
	}
	template<typename iterator_, typename index_iterator_>
	using index_access_iterator = transform_iterator<index_iterator_, iterators::detail::index_iterator_access<iterator_>>;
	template<typename iterator_, typename index_iterator_>
	using index_access_range = transform_range<index_iterator_, iterators::detail::index_iterator_access<iterator_>>;

	template<typename iterator_, typename index_iterator_>
	auto make_index_access_iterator(iterator_ TergetItr, index_iterator_ IndexItr) {
		return index_access_iterator<iterator_, index_iterator_>(
			IndexItr,
			iterators::detail::index_iterator_access<iterator_>(TergetItr)
			);
	}
	template<typename iterator_, typename index_iterator_>
	auto make_index_access_range(iterator_ TergetItr, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return index_access_range<iterator_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::index_iterator_access<iterator_>(TergetItr)
		);
	}

	template<typename container_, typename index_iterator_>
	using index_at_access_iterator = transform_iterator<index_iterator_, iterators::detail::index_at_access<container_>>;
	template<typename container_, typename index_iterator_>
	using index_at_access_range = transform_range<index_iterator_, iterators::detail::index_at_access<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_index_at_access_iterator(container_& Container, index_iterator_ IndexItr) {
		return index_access_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::index_at_access<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_index_at_access_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return index_at_access_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::index_at_access<container_>(Container)
		);
	}

	template<typename container_, typename index_iterator_>
	using index_op_access_iterator = transform_iterator<index_iterator_, iterators::detail::index_op_access<container_>>;
	template<typename container_, typename index_iterator_>
	using index_op_access_range = transform_range<index_iterator_, iterators::detail::index_op_access<container_>>;

	template<typename container_, typename index_iterator_>
	auto make_index_op_access_iterator(container_& Container, index_iterator_ IndexItr) {
		return index_access_iterator<container_, index_iterator_>(
			IndexItr,
			iterators::detail::index_op_access<container_>(Container)
		);
	}
	template<typename container_, typename index_iterator_>
	auto make_index_op_access_range(container_& Container, index_iterator_ IndexBegin, index_iterator_ IndexEnd) {
		return index_op_access_range<container_, index_iterator_>(
			IndexBegin, IndexEnd,
			iterators::detail::index_op_access<container_>(Container)
		);
	}
}
#
#endif
