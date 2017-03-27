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
		//!Return reference of the elemtn at the given point
		reference at(const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference at(diff_type Pos_, others... Others_){
			return Begin[Indexer.index(Pos_, Others_...)];
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_){
			return Begin[Indexer.index(Point_)];
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const{ return Indexer.lattice_size(); }
		//!Get point_type Size
		point_type size()const{ return Indexer.size(); }
		//!Get size of a certain axis
		template<unsigned int  req_dim_>
		size_type axis_size()const{ return Indexer.axis_size<req_dim_>(); }
	public:
		//!Return begin iterator fot the lattice
		iterator begin() { return iterator(*this, 0); }
		//!Return end iterator fot the lattice
		iterator end() { return iterator(*this, Indexer.lattice_size()); }
		//!Return begin const_iterator fot the lattice
		const_iterator begin()const{ return const_iterator(*this, 0); }
		//!Return end const_iterator fot the lattice
		const_iterator end()const{ return const_iterator(*this, Indexer.lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_){ return locator(Base == nullptr ? *this : Base, Pos_ + Pos); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(diff_type Pos_, others... Others_){
			return locate(lattices::make_point(Pos_, Others_...));
		}
		//!Return locator of (0,0,0...)
		locator front_locate(){point_type Point; Point.fill(0);  return locate(Point); }
		//!Return locator of (size-1)
		locator back_locate(){point_type Point; Point.fill(-1); return locate(size() + Point); }
	public:
		this_type sublattice(point_type Point_, point_type Size_){

		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Size of the original (i.e. first order) lattice
		point_type Size;
		//!Indexer of the focal lattice
		indexer Indexer;
	};

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
