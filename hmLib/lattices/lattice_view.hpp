#ifndef HMLIB_LATTICES_LATTICEVIEW_INC
#define HMLIB_LATTICES_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include"utility.hpp"
#include"exceptions.hpp"
#include"lattice_indexer.hpp"
#include"iterator.hpp"
#include"locator.hpp"

namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
		using this_type = lattice_view<iterator_, dim_>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using reference = typename iterator_::reference;
		using const_reference = const reference;
		using pointer = typename iterator_::pointer;
		using const_pointer = const pointer;
	public:
		using indexer = lattices::lattice_indexer<dim_>;
		using locator = lattices::basic_locator<this_type>;
		using iterator = lattices::basic_iterator<this_type>;
		using const_iterator = lattices::basic_const_iterator<this_type>;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		lattice_view() = default;
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, size_type Size_, others... Others_)
			: Begin(Begin_)
			, End(End_)
			, Indexer(Size_, Others_...){
		}
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<size_type,diff_type> SizeGap_, others... Others_)
			: Begin(Begin_)
			, End(End_)
			, Indexer(SizeGap_, Others_...) {
		}
		lattice_view(iterator_ Begin_, iterator_ End_, point_type Size_)
			: Begin(Begin_)
			, End(End_)
			, Indexer(Size_) {
		}
	private:
		lattice_view(iterator_ Begin_, iterator_ End_, point_type Size_, point_type Gap_)
			: Begin(Begin_)
			, End(End_)
			, Indexer(Size_,Gap_) {
		}
	public:
		template<typename... others>
		reference at(diff_type Pos_, others... Others_){
			return Begin[Indexer.index(Pos_, Others_...)];
		}
		reference at(const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
		reference operator[](const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
	public:
		iterator begin() { return make_iterator(*this, 0); }
		iterator end() { return make_iterator(*this, 1); }
		const_iterator begin()const{ return make_iterator(*this, 0); }
		const_iterator end()const{ return make_iterator(*this, 1); }
	private:
		indexer Indexer;
		iterator_base Begin;
		point_type Pos;
		point_type Size;
		point_type OriginalSize;
	};
	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
