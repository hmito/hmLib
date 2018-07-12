#ifndef HMLIB_ITERATORS_ZIPCONTAINER_INC
#define HMLIB_ITERATORS_ZIPCONTAINER_INC 100
#
#include<limits>
#include<tuple>
#include"../tuple.hpp"
namespace hmLib {
	template<typename index_type_, typename... ranges>
	struct zip_container {
		using pack = std::tuple<ranges&...>;
		using index_type = index_type_;
		using size_type = unsigned int;
		using reference = std::tuple<decltype((std::declval<typename std::decay<ranges>::type>().at(std::declval<index_type>())))...>;
	public:
		template<typename... other_ranges>
		zip_container(other_ranges&... Ranges_):Ranges(Ranges_...) {
			Size = tuple_reduce([](size_type i, auto& v) {return std::min<size_type>(v.size(), i); }, Ranges, std::numeric_limits<size_type>::max());
		}
		reference operator[](index_type_ Index) { return tuple_for_each([Index](auto& v)->decltype((v[Index])) {return v[Index]; },Ranges); }
		reference at(index_type_ Index) { 
			hmLib_assert(0 <= Index && static_cast<size_type>(Index)<Size, hmLib::access_exceptions::out_of_range_access, "out of range access for zip_container.");
			return tuple_for_each([Index](auto& v)->decltype((v.at(Index))) {return v.at(Index); }, Ranges); 
		}
		size_type size() { return Size; }
	private:
		pack Ranges;
		size_type Size;
	};
	template<typename index_type_, typename... ranges>
	auto make_zip_container(ranges&... Ranges_) {
		return zip_container<index_type_, typename std::decay<ranges>::type...>(Ranges_...);
	}
}
#
#endif
