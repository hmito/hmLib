#ifndef HMLIB_ITERATORS_INDEXACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXACCESSITERATOR_INC 100
#
#include "transform_iterator.hpp"
namespace hmLib{
	namespace iterators{
		namespace detail{
			template<int n>
			struct static_index_access{
				template<typename value_type>
				decltype(auto) operator()(value_type&& val){
					return val[n];
				}
			};
			template<typename index_type>
			struct index_access{
			private:
				index_type n;
			public:
				index_access(index_type n_):n(n_) {}
				template<typename value_type>
				decltype(auto) operator()(value_type&& val) {
					return val[n];
				}
			};
		}
	}
	template<int n, typename iterator>
	decltype(auto) make_static_index_access_iterator(iterator Itr) {
		return hmLib::make_transform_iterator(Itr, iterators::detail::static_index_access<n>());
	}
	template<int n, typename iterator>
	decltype(auto) make_static_index_access_range(iterator Beg, iterator End) {
		return hmLib::make_transform_range(Beg, End, iterators::detail::static_index_access<n>());
	}
	template<typename iterator, typename index_type>
	decltype(auto) make_index_access_iterator(iterator Itr, index_type n) {
		return hmLib::make_transform_iterator(Itr, iterators::detail::index_access<index_type>(n));
	}
	template<typename iterator, typename index_type>
	decltype(auto) make_index_access_range(iterator Beg, iterator End, index_type n) {
		return hmLib::make_transform_range(Beg, End, iterators::detail::index_access<index_type>(n));
	}
}
#
#endif
