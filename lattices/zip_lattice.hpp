#ifndef HMLIB_LATTICES_ZIPLATTICE_INC
#define HMLIB_LATTICES_ZIPLATTICE_INC 100
#
#include<type_traits>
#include<tuple>
#include"../tuple.hpp"
#include"utility.hpp"
#include"../iterators/zip_iterator.hpp"
namespace hmLib {
	namespace lattices {
		template<typename... lattices_>
		struct zip_lattice {
			using pack = std::tuple<lattices_&...>;
			using index_type = typename std::common_type<typename lattices_::index_type...>::type;
			using size_type = typename std::common_type<typename lattices_::size_type...>::type;
			using point_type = typename std::common_type<typename lattices_::point_type...>::type;
			using reference = std::tuple<decltype(std::declval<typename std::decay<lattices_>::type>().at(std::declval<point_type>()))...>;
		public:
			template<typename... other_lattices>
			zip_lattice(other_lattices&... Lattices_):Ranges(Lattices_...) {}
			reference at(point_type Pos) {
				return tuple_for_each([Pos](auto& v)->decltype((v.at(Pos))) {return v.at(Pos); }, Ranges);
			}
			template<typename... others>
			reference at(index_type Index, others... Others) { return at(point_type{ Index,Others... }); }
			reference ref(point_type Pos) { return tuple_for_each([Pos](auto& v)->decltype((v[Pos])) {return v[Pos]; }, Ranges); }
			template<typename... others>
			reference ref(index_type Index, others... Others) { return ref(point_type{ Index,Others... }); }
			reference operator[](point_type Pos) { return ref(Pos); }
		private:
			pack Ranges;
		};
		template<typename... lattices_>
		auto make_zip_lattice(lattices_&... Lattices_) {
			return zip_lattice<typename std::decay<lattices_>::type...>(Lattices_...);
		}
	}
}
#
#endif
