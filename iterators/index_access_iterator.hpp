#ifndef HMLIB_ITERATORS_INDEXACCESSITERATOR_INC
#define HMLIB_ITERATORS_INDEXACCESSITERATOR_INC 100
#
namespace hmLib{
	namespace iterators{
		namespace detail{
			template<int n>
			struct static_index_access{
				template<typename iterator>
				decltype(auto) operator()(iterator&&){
					return (*itr)[n];
				}
			};
			template<typename index_type>
			struct index_access{
			private:
				index_type n;
			public:
				index_access(index_type n_):n(n_) {}
				template<typename iterator>
				decltype(auto) operator()(iterator&&){
					return (*itr)[n];
				}
			};
		}
	}
	template<int n, typename iterator>
	decltype(auto) make_static_index_access_iterator(iterator Itr) {
		return hmLib::make_transform_iterator(Itr, iterators::detail::static_index_access<n>());
	}
	template<int n, typename iterator>
	decltype(auto) make_static_index_access_iterator(iterator Beg, iterator End) {
		return hmLib::make_transform_range(Beg, End, iterators::detail::static_index_access<n>());
	}
	template<typename iterator, typename index_type>
	decltype(auto) make_index_access_iterator(iterator Itr, index_type n) {
		return hmLib::make_transform_iterator(Itr, iterators::detail::index_access(n));
	}
	template<int n, typename iterator>
	decltype(auto) make_index_access_iterator(iterator Beg, iterator End) {
		return hmLib::make_transform_range(Beg, End, iterators::detail::index_access(n));
	}
}
#
#endif
