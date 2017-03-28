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
		lattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		lattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		template<typename... others>
		lattice_view(iterator_base Begin_, iterator_base End_, size_type Size_, others... Others_)
			: Base(nullptr)
			, Begin(Begin_)
			, Pos()
			, Indexer(Size_, Others_...){
			hmLib_assert(std::distance(Begin_, End_) <= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range,"The given range is smaller than the lattice size.");
		}
		lattice_view(iterator_base Begin_, iterator_base End_, point_type Size_)
			: Base(nullptr)
			, Begin(Begin_)
			, Pos()
			, Indexer(Size_) {
			hmLib_assert(std::distance(Begin_, End_) <= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
	private:
		lattice_view(this_type& Ref, point_type Pos_, point_type Size_, point_type Gap_, diff_type Base_)
			: Base(Ref)
			, Begin(Ref.Begin)
			, Pos(Pos_)
			, Indexer(Size_, Gap_, Base_) {
		}
	public:
		//!Return reference of the elemtn at the given point
		reference at(const point_type& Point_){ return Begin[Indexer.index(Point_)]; }
		//!Return const_reference of the elemtn at the given point
		const_reference at(const point_type& Point_)const { return Begin[Indexer.index(Point_)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference at(diff_type Pos_, others... Others_){ return Begin[Indexer.index(Pos_, Others_...)]; }
		//!Return const_reference of the elemtn at the given elements point
		template<typename... others>
		const_reference at(diff_type Pos_, others... Others_)const { return Begin[Indexer.index(Pos_, Others_...)]; }
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_){	return Begin[Indexer.index(Point_)]; }
		//!Return const_reference of the elemtn at the given point
		const_reference operator[](const point_type& Point_)const { return Begin[Indexer.index(Point_)]; }
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const{ return Indexer.lattice_size(); }
		//!Get point_type Size
		point_type size()const{ return Indexer.size(); }
		//!Get size of a certain axis
		template<unsigned int  req_dim_>
		size_type axis_size()const{ return Indexer.axis_size<req_dim_>(); }
		//!Return Point from Index value
		point_type index_to_point(diff_type Index) {
			return lattices::index_to_point(Index, Indexer.size())+Pos;
		}
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
		locator locate(const point_type& Point_){ return locator(base_lattice(), Point_ + Pos); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(diff_type Pos_, others... Others_){
			return locate(lattices::make_point(Pos_, Others_...));
		}
		//!Return locator of (0,0,0...)
		locator front_locate(){return Pos; }
		//!Return locator of (size-1)
		locator back_locate(){return locate(size() + Pos + lattices::make_filled_point(-1)); }
	public:
		this_type sublattice(point_type Point_, point_type Size_){
			diff_type Base = 0;
			point_type Gap;
			point_type ThisSize = Indexer.size();
			diff_type Step = 1;
			diff_type OStep = 1;
			for (unsigned int i = 0; i < dim(); ++i) {
				Gap[i] = OStep - Step;
				Step = Step*Size[i] + Gap[i];
				OStep *= ThisSize[i];
			}
			return this_type(*this, Point_+Pos, Size_, Gap, lattices::point_to_index(Point_+Pos, base_lattice().size()));
		}
	private:
		this_type& base_lattice() { return (Base == nullptr ? *this : Base); }
	private:
		//!pointer to theoriginal (i.e. first order) lattice
		this_type* Base;
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Indexer of the focal lattice
		indexer Indexer;
	};

	template<typename lattice_t>
	lattice_t sublattice(lattices::basic_locator<lattice_t> Lat1, lattices::basic_locator<lattice_t> Lat2) {
		return Lat1.get_lattice().sublattice(Lat1.get_point(), Lat2.get_point() - Lat1.get_point() + lattices::make_filled_point<lattice_t::dim()>(1));
	}

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
