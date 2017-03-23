#ifndef HMLIB_LATTICES_LATTICEVIEW_INC
#define HMLIB_LATTICES_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include"utility.hpp"
#include"exceptions.hpp"
#include"iterator_base.hpp"
#include"lattice_indexer.hpp"
namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
		using this_type = lattice_view<iterator_, dim_>;
	public:
		using indexer = lattices::lattice_indexer<dim_>;
		using point_type = lattices::point<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using iterator = lattices::iterator_base<iterator_, this_type, dim_>;
		using raw_iterator = iterator_;
		struct locator{};
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
		reference at(index_type Pos_, others... Others_){
			return Begin[Indexer.index(Pos_, Others_...)];
		}
		reference at(const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
		reference operator[](const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
		iterator begin() { return make_iterator(*this, 0); }
		iterator end() { return make_iterator(*this, 1); }
	public:
		this_type sublattice(point_type Pos, point_type Size) {

		}
	public:
		template<typename... others>
		index_type raw_index(index_type Pos_, others... Others_)const { return Indexer.index(Pos_, Others_...); }
		index_type raw_index(const point& Point_)const { Indexer.index(Point_); }
		raw_iterator raw_begin(){ return Lower.raw_begin(); }
		raw_iterator raw_end(){ return Lower.raw_end(); }
	private:
		raw_iterator Begin;
		raw_iterator End;
		point_type Pos;
		point_type Size;
		point_type OriginalSize;
		indexer Indexer;
	};
	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
