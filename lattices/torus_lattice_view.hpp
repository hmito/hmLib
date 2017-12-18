#ifndef HMLIB_LATTICES_TORUSLATTICEVIEW_INC
#define HMLIB_LATTICES_TORUSLATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include<numeric>
#include"../algorithm/compare.hpp"
#include"utility.hpp"
#include"exceptions.hpp"
#include"lattice_indexer.hpp"
#include"iterator.hpp"
#include"torus_locator.hpp"

namespace hmLib{
	template<typename iterator_, unsigned int dim_, bool is_const = std::is_const<typename iterator_::value_type>::value>
	struct torus_lattice_view{
		using this_type = torus_lattice_view<iterator_, dim_, is_const>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using const_reference = typename std::add_const<reference>::type;
		using pointer = typename iterator_::pointer;
		using const_pointer = typename std::add_const<pointer>::type;
	public:
		using indexer = lattices::lattice_indexer<dim_>;
		using locator = lattices::basic_torus_locator<iterator_base, indexer>;
		using const_locator = lattices::basic_const_torus_locator<iterator_base, indexer>;
		using iterator = lattices::basic_iterator<this_type>;
		using const_iterator = lattices::basic_const_iterator<this_type>;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		torus_lattice_view() = default;
		torus_lattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_lattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		torus_lattice_view(iterator_base Begin_, iterator_base End_, const extent_type& Extent_)
			: Begin(Begin_)
			, Indexer(Extent_)
			, Pos()
			, Extent(Extent_){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<index_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
		torus_lattice_view(iterator_base Begin_, const indexer& Indexer_, const point_type& Pos_, const extent_type& Extent_)
			: Begin(Begin_)
			, Indexer(Indexer)
			, Pos(Pos_)
			, Extent(Extent_) {
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_){ return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const point_type& Point_)const{ return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_){
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_reference at(index_type Pos_, others... Others_)const{
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_){ return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return const_reference of the elemtn at the given point
		const_reference operator[](const point_type& Point_)const { return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_){
			return operator[](lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point
		template<typename... others>
		const_reference ref(index_type Pos_, others... Others_)const{
			return operator[](lattices::point(Pos_, Others_...));
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return indexer(Extent).lattice_size(); }
		//!Get point_type Size
		const extent_type& extent()const { return Extent; }
		//!Return Point from Index value
		point_type index_to_point(index_type Index)const { return indexer(Extent).point(Index); }
		//!Return Point inside of the torus
		point_type translate_for_torus(const point_type& Point)const{
			return Indexer.translate_for_torus(Point);
		}
	public:
		//!Return begin iterator fot the lattice
		iterator begin(){ return iterator(*this, 0); }
		//!Return end iterator fot the lattice
		iterator end(){ return iterator(*this, lattice_size()); }
		//!Return begin const_iterator fot the lattice
		const_iterator begin()const{ return cbegin(); }
		//!Return end const_iterator fot the lattice
		const_iterator end()const{ return cend(); }
		//!Return begin const_iterator fot the lattice
		const_iterator cbegin()const{ return const_iterator(*this, 0); }
		//!Return end const_iterator fot the lattice
		const_iterator cend()const{ return const_iterator(*this, lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_){ return locator(Begin, Indexer, Point_ + Pos); }
		//!Return const locator of given point
		const_locator locate(const point_type& Point_)const{ return const_locator(Begin, Indexer, Point_ + Pos); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_){ return locate(lattices::point(Pos_, Others_...)); }
		//!Return const locator of given point elements
		template<typename... others>
		const_locator locate(index_type Pos_, others... Others_)const{ return locate(lattices::point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate(){ return locate(point_type(0)); }
		//!Return locator of (0,0,0...)
		const_locator front_locate()const{ return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate(){ return locate(extent() + point_type(-1)); }
		//!Return const locator of (size-1)
		const_locator back_locate()const{ return locate(extent() + point_type(-1)); }
	public:
		this_type subview(const point_type& Point_, const extent_type& Extent_){
			return this_type(Begin, Indexer, Point_, Extent_);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Indexer of the focal lattice
		indexer Indexer;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Size of lattice
		extent_type Extent;
	};
	template<typename iterator_, unsigned int dim_>
	struct torus_lattice_view<iterator_, dim_, true>{
		using this_type = torus_lattice_view<iterator_, dim_, true>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using const_reference = reference;
		using pointer = typename iterator_::pointer;
		using const_pointer = pointer;
	public:
		using indexer = lattices::lattice_indexer<dim_>;
		using locator = lattices::basic_const_torus_locator<iterator_base, indexer>;
		using const_locator = locator;
		using iterator = lattices::basic_const_iterator<this_type>;
		using const_iterator = iterator;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		torus_lattice_view() = default;
		torus_lattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_lattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		torus_lattice_view(iterator_base Begin_, iterator_base End_, const extent_type& Extent_)
			: Begin(Begin_)
			, Indexer(Extent_)
			, Pos()
			, Extent(Extent_){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<index_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
		torus_lattice_view(iterator_base Begin_, const indexer& Indexer_, const point_type& Pos_, const extent_type& Extent_)
			: Begin(Begin_)
			, Indexer(Indexer_)
			, Pos(Pos_)
			, Extent(Extent_){
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_)const{ return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_)const{
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_)const{ return Begin[Indexer.torus_index(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_)const{
			return operator[](lattices::point(Pos_, Others_...));
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return indexer(Extent).lattice_size(); }
		//!Get point_type Size
		const point_type& extent()const { return Extent; }
		//!Return Point from Index value
		point_type index_to_point(index_type Index)const { return indexer(Extent).point(Index); }
		//!Return Point inside of the torus
		point_type translate_for_torus(const point_type& Point)const {
			return Indexer.translate_for_torus(Point);
		}
	public:
		//!Return begin iterator fot the lattice
		iterator begin()const{ return iterator(*this, 0); }
		//!Return end iterator fot the lattice
		iterator end()const{ return iterator(*this, lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_)const{ return locator(Begin, Indexer, Point_ + Pos); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_)const{ return locate(lattices::point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate()const{ return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate()const{ return locate(extent() + point_type(-1)); }
	public:
		this_type subview(const point_type& Point_, const extent_type& Extent_){
			return this_type(Begin, Indexer, Point_, Extent_);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Indexer of the focal lattice
		indexer Indexer;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Size of lattice
		extent_type Extent;
	};

	template<typename iterator, typename... others>
	auto make_torus_lattice_view(iterator Begin, iterator End, lattices::size_type Size, others... Others){
		return torus_lattice_view<iterator, sizeof...(others)>(Begin, End, Size, Others...);
	}
	template<typename iterator, unsigned int dim>
	auto make_torus_lattice_view(iterator Begin, iterator End, const lattices::extent_type<dim>& Extent){
		return torus_lattice_view<iterator, dim>(Begin, End, Extent);
	}

	template<typename iterator, typename indexer>
	auto subview(const lattices::basic_torus_locator<iterator, indexer>& Lat1, const lattices::basic_torus_locator<iterator, indexer>& Lat2){
		return torus_lattice_view<iterator, indexer::dim(), std::is_const<typename iterator::value_type>::value>(
			Lat1.get_base_iterator(), Lat1.get_indexer(), Lat1.raw_point(), Lat2.raw_point() - Lat1.raw_point() + lattices::point<indexer::dim()>(1)
		);
	}
	template<typename iterator, typename indexer>
	auto subview(const lattices::basic_const_torus_locator<iterator, indexer>& Lat1, const lattices::basic_const_torus_locator<iterator, indexer>& Lat2){
		return torus_lattice_view<iterator, indexer::dim(), true>(
			Lat1.get_base_iterator(), Lat1.get_indexer(), Lat1.raw_point(), Lat2.raw_point() - Lat1.raw_point() + lattices::point<indexer::dim()>(1)
		);
	}
}
#
#endif
