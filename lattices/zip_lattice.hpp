#ifndef HMLIB_LATTICES_ZIPLATTICE_INC
#define HMLIB_LATTICES_ZIPLATTICE_INC 100
#
#include<type_traits>
#include<tuple>
#include"../tuple.hpp"
#include"utility.hpp"
namespace hmLib {
	namespace lattices {
		template<typename... lattices_>
		struct zip_lattice {
			using pack = std::tuple<lattices_&...>;
			using reference = std::tuple<decltype(std::declval<typename std::decay<lattices_>::type>().at(std::declval<point_type>()))...>;
		public:
			template<typename... other_lattices>
			zip_lattice(other_lattices&... Lattices_):Ranges(Lattices_...) {
			}
			reference operator[](point_type Pos) { return tuple_for_each([Pos](auto& v)->decltype((v[Pos])) {return v[Pos]; }, Ranges); }
			reference at(point_type Pos) {
				return tuple_for_each([Pos](auto& v)->decltype((v.at(Pos))) {return v.at(Pos); }, Ranges);
			}
			size_type size() { return Size; }
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
