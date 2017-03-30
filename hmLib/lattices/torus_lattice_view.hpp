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
		using point_type = lattices::point<dim_>;
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
		template<typename... others>
		torus_lattice_view(iterator_base Begin_, iterator_base End_, index_type Size_, others... Others_)
			: Begin(Begin_)
			, Pos()
			, Size(lattices::make_point(Size_, Others_...))
			, Indexer(lattices::make_point(Size_, Others_...)){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
		torus_lattice_view(iterator_base Begin_, iterator_base End_, const point_type& Size_)
			: Begin(Begin_)
			, Pos()
			, Size(Size_)
			, Indexer(Size_){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
	private:
		torus_lattice_view(iterator_base Begin_, const point_type& Pos_, const point_type& Size_, const indexer& Indexer_)
			: Begin(Begin_)
			, Pos(Pos_)
			, Size(Size_)
			, Indexer(Indexer_){}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_){ return Begin[Indexer.index(make_torus_point(Point_ + Pos))]; }
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const point_type& Point_)const{ return Begin[Indexer.index(make_torus_point(Point_ + Pos))]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(diff_type Pos_, others... Others_){
			return at(lattices::make_point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_reference at(diff_type Pos_, others... Others_)const{
			return at(lattices::make_point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_){ return Begin[Indexer(make_torus_point(Point_ + Pos))]; }
		//!Return const_reference of the elemtn at the given point
		const_reference operator[](const point_type& Point_)const{ return Begin[Indexer(make_torus_point(Point_ + Pos))]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(diff_type Pos_, others... Others_){
			return operator[](lattices::make_point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point
		template<typename... others>
		const_reference ref(diff_type Pos_, others... Others_)const{
			return operator[](lattices::make_point(Pos_, Others_...));
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const{ return std::accumulate(Size.begin(), Size.end(), 1, [](int v1, int v2)->int {return v1*v2; }); }
		//!Get point_type Size
		point_type size()const{ return Size; }
		//!Return Point from Index value
		point_type index_to_point(diff_type Index)const{
			return make_torus_point(lattices::index_to_point(Index, Size) + Pos);
		}
		//!Return Point inside of the torus
		point_type make_torus_point(const point_type& Point)const{
			return lattices::make_torus_point(Point, Indexer.size());
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
		locator locate(const point_type& Point_){ return locator(Begin, Point_ + Pos, Indexer); }
		//!Return const locator of given point
		const_locator locate(const point_type& Point_)const{ return const_locator(Begin, Point_ + Pos, Indexer); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(diff_type Pos_, others... Others_){ return locate(lattices::make_point(Pos_, Others_...)); }
		//!Return const locator of given point elements
		template<typename... others>
		const_locator locate(diff_type Pos_, others... Others_)const{ return locate(lattices::make_point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate(){ return locate(lattices::make_filled_point<dim_>(0)); }
		//!Return locator of (0,0,0...)
		const_locator front_locate()const{ return locate(lattices::make_filled_point<dim_>(0)); }
		//!Return locator of (size-1)
		locator back_locate(){ return locate(Size + lattices::make_filled_point<dim_>(-1)); }
		//!Return const locator of (size-1)
		const_locator back_locate()const{ return locate(Size + lattices::make_filled_point<dim_>(-1)); }
	public:
		this_type sublattice(point_type Point_, point_type Size_){
			return this_type(Begin, Point_, Size_, Indexer);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Size of the lattice
		point_type Size;
		//!Indexer of the focal lattice
		indexer Indexer;
	public:
		friend torus_lattice_view<iterator_, dim_> sublattice(const locator& Lat1, const locator& Lat2){
			return this_type(Lat1.get_base_iterator(), Lat1.point(), Lat2.point() - Lat1.point() + lattices::make_filled_point<dim_>(1), indexer(Lat1.size()));
		}
	};
	template<typename iterator_, unsigned int dim_>
	struct torus_lattice_view<iterator_, dim_, true>{
		using this_type = torus_lattice_view<iterator_, dim_, true>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using pointer = typename iterator_::pointer;
	public:
		using indexer = lattices::lattice_indexer<dim_>;
		using locator = lattices::basic_const_locator<iterator_base, indexer>;
		using iterator = lattices::basic_const_iterator<this_type>;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		torus_lattice_view() = default;
		torus_lattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_lattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		template<typename... others>
		torus_lattice_view(iterator_base Begin_, iterator_base End_, index_type Size_, others... Others_)
			: Begin(Begin_)
			, Pos()
			, Size(lattices::make_point(Size_, Others_...))
			, Indexer(lattices::make_point(Size_, Others_...)){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
		torus_lattice_view(iterator_base Begin_, iterator_base End_, const point_type& Size_)
			: Begin(Begin_)
			, Pos()
			, Size(Size_)
			, Indexer(Size_){
			hmLib_assert(std::distance(Begin_, End_) >= static_cast<diff_type>(lattice_size()), lattices::invalid_view_range, "The given range is smaller than the lattice size.");
		}
	private:
		torus_lattice_view(iterator_base Begin_, const point_type& Pos_, const point_type& Size_, const indexer& Indexer_)
			: Begin(Begin_)
			, Pos(Pos_)
			, Size(Size_)
			, Indexer(Indexer_){}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_)const{ return Begin[Indexer.index(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(diff_type Pos_, others... Others_)const{
			return at(lattices::make_point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_)const{ return Begin[Indexer(Point_ + Pos)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(diff_type Pos_, others... Others_)const{
			return operator[](lattices::make_point(Pos_, Others_...));
		}
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const{ return std::accumulate(Size.begin(), Size.end(), 1, [](int v1, int v2)->int {return v1*v2; }); }
		//!Get point_type Size
		point_type size()const{ return Size; }
		//!Return Point from Index value
		point_type index_to_point(diff_type Index)const{
			return lattices::index_to_point(Index, Size) + Pos;
		}
		//!Return Point inside of the torus
		point_type make_torus_point(const point_type& Point)const{
			return lattices::make_torus_point(Point, Indexer.size());
		}
	public:
		//!Return begin iterator fot the lattice
		iterator begin()const{ return iterator(*this, 0); }
		//!Return end iterator fot the lattice
		iterator end()const{ return iterator(*this, lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_)const{ return locator(Begin, Point_ + Pos, Indexer); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(diff_type Pos_, others... Others_)const{ return locate(lattices::make_point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate()const{ return locate(lattices::make_filled_point<dim_>(0)); }
		//!Return locator of (size-1)
		locator back_locate()const{ return locate(Size + lattices::make_filled_point<dim_>(-1)); }
	public:
		this_type sublattice(point_type Point_, point_type Size_){
			return this_type(Begin, Point_, Size_, Indexer);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Front point on the original (i.e. first order) lattice
		point_type Pos;
		//!Size of the lattice
		point_type Size;
		//!Indexer of the focal lattice
		indexer Indexer;
	public:
		friend torus_lattice_view<iterator_, dim_> sublattice(const locator& Lat1, const locator& Lat2){
			return this_type(Lat1.get_base_iterator(), Lat1.point(), Lat2.point() - Lat1.point() + lattices::make_filled_point<dim_>(1), indexer(Lat1.size()));
		}
	};

	template<typename iterator, typename... others>
	auto make_torus_lattice(iterator Begin, iterator End, others... Others)->torus_lattice_view<iterator, sizeof...(others)>{
		return torus_lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
