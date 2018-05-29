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
#include"indexer.hpp"
#include"torus_iterator.hpp"
#include"torus_locator.hpp"

namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct torus_sublattice_view {
	private:
		using this_type = torus_sublattice_view<iterator_, dim_>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using index_type = lattices::index_type;
		using reference = typename std::iterator_traits<iterator_base>::reference;
		using pointer = typename std::iterator_traits<iterator_base>::pointer;
	public:
		using indexer = lattices::indexer<dim_>;
		using locator = lattices::basic_torus_locator<iterator_base, indexer>;
		using iterator = lattices::basic_torus_sub_iterator<iterator_base, indexer>;
		using view = torus_sublattice_view<iterator_base, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		torus_sublattice_view() = default;
		torus_sublattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_sublattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<other_iterator_, iterator_base>::value && !std::is_same<other_iterator_, iterator_base>::value, std::nullptr_t>::type CanConvertible = nullptr>
		torus_sublattice_view(const torus_sublattice_view<other_iterator_, dim_>& Other):Begin(Other.Begin), OriginalIndexer(Other.OriginalIndexer), SubIndexer(Other.SubIndexer), Pos(Other.Pos) {}
		torus_sublattice_view(iterator_base Begin_, const extent_type& OriginalExtent_, const point_type& OriginalPos_, const extent_type& SubExtent_)
			: Begin(Begin_)
			, OriginalIndexer(OriginalExtent_)
			, Pos(OriginalPos_)
			, SubIndexer(SubExtent_) {
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_)const { return Begin[OriginalIndexer.torus_index(Point_+Pos)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_)const { return at(lattices::point(Pos_, Others_...)); }
		//!Return reference of the elemtn at the given point
		reference ref(const point_type& Point_)const { return Begin[OriginalIndexer.torus_index(Point_+Pos)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_)const { return operator[](lattices::point(Pos_, Others_...)); }
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_)const { return ref(Point_); }
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return SubIndexer.lattice_size(); }
		//!Get point_type Extent
		const extent_type& extent()const { return SubIndexer.extent(); }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return SubIndexer.point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return SubIndexer.index(Point_); }
		//!Return reference of the elemtn at the given Index, i.e., at(Pos) == index_at(point_to_index(Pos));
		reference index_at(index_type Index_)const { return at(index_to_point(Index_)); }
		//!Return reference of the elemtn at the given Index, i.e., ref(Pos) == index_ref(point_to_index(Pos));
		reference index_ref(index_type Index_)const { return ref(index_to_point(Index_)); }
	public:
		//!Return begin iterator fot the lattice
		iterator begin() const { return iterator(Begin, OriginalIndexer, Pos, SubIndexer.extent(), 0); }
		//!Return end iterator fot the lattice
		iterator end() const { return iterator(Begin, OriginalIndexer, Pos, SubIndexer.extent(), SubIndexer.lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_)const { return locator(Begin, OriginalIndexer, Pos+Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_) const { return locate(lattices::point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate() const { return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate() const { return locate(extent() + point_type(-1)); }
	public:
		view subview(const point_type& Point_, const extent_type& Extent_)const {
			hmLib_assert(hmLib::all_less_equal_than(Extent_, SubIndexer.extent()), lattices::invalid_range, "The given range is smaller than the lattice size.");
			return view(Begin, OriginalIndexer.extent(), Pos+Point_, Extent_);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Indexer of the original lattice
		indexer OriginalIndexer;
		//!Indexer of the sub lattice
		indexer SubIndexer;
		//!Point of sub lattice
		point_type Pos;
	};

	template<typename iterator_, unsigned int dim_>
	struct torus_lattice_view {
	private:
		using this_type = torus_lattice_view<iterator_, dim_>;
	public:
		using iterator_base = iterator_;
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using index_type = lattices::index_type;
		using reference = typename std::iterator_traits<iterator_base>::reference;
		using pointer = typename std::iterator_traits<iterator_base>::pointer;
	public:
		using indexer = lattices::indexer<dim_>;
		using locator = lattices::basic_torus_locator<iterator_base, indexer>;
		using iterator = lattices::basic_torus_iterator<iterator_base, indexer>;
		using view = torus_sublattice_view<iterator_base, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		torus_lattice_view() = default;
		torus_lattice_view(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_lattice_view(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<other_iterator_, iterator_base>::value && !std::is_same<other_iterator_, iterator_base>::value, std::nullptr_t>::type CanConvertible = nullptr>
		torus_lattice_view(const torus_lattice_view<other_iterator_, dim_>& Other):Begin(Other.Begin), Indexer(Other.Indexer) {}
		torus_lattice_view(iterator_base Begin_, const extent_type& Extent_):Begin(Begin_), Indexer(Extent_) {}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_)const { return Begin[Indexer.torus_index(Point_)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_)const { return at(lattices::point(Pos_, Others_...)); }
		//!Return reference of the elemtn at the given point
		reference ref(const point_type& Point_)const { return Begin[Indexer.torus_index(Point_)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_)const { return operator[](lattices::point(Pos_, Others_...)); }
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_)const { return ref(Point_); }
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return Indexer.lattice_size(); }
		//!Get point_type Extent
		const extent_type& extent()const { return Indexer.extent(); }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return Indexer.point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
		//!Return reference of the elemtn at the given Index, i.e., at(Pos) == index_at(point_to_index(Pos));
		reference index_at(index_type Index_)const {
			hmLib_assert(Index_<lattice_size(), lattices::out_of_range_access, "Requested point is out of lattice.");
			return Begin[Index_];
		}
		//!Return reference of the elemtn at the given Index, i.e., ref(Pos) == index_ref(point_to_index(Pos));
		reference index_ref(index_type Index_)const { return Begin[Index_]; }
	public:
		//!Return begin iterator fot the lattice
		iterator begin() const { return iterator(Begin, Indexer, 0); }
		//!Return end iterator fot the lattice
		iterator end() const { return iterator(Begin, Indexer, lattice_size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_)const { return locator(Begin, Indexer, Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_) const { return locate(lattices::point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate() const { return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate() const { return locate(extent() + point_type(-1)); }
	public:
		view subview(const point_type& Point_, const extent_type& Extent_)const {
			hmLib_assert(hmLib::all_less_equal_than(Extent_, Indexer.extent()), lattices::invalid_range, "The given range is smaller than the lattice size.");
			return view(Begin, Indexer.extent(), Point_, Extent_);
		}
	private:
		//!Begin iterator of the original (i.e. first order) lattice
		iterator_base Begin;
		//!Indexer of the focal lattice
		indexer Indexer;
	};

	template<typename iterator, typename element_type, unsigned int dim>
	auto make_torus_lattice_view(iterator Begin, iterator End, const lattices::point_array_type<element_type,dim>& Extent) {
		hmLib_assert(std::distance(Begin, End) >= static_cast<lattices::index_type>(lattices::indexer<dim>(static_cast<lattices::extent_type<dim>>(Extent)).lattice_size()), lattices::invalid_range, "The given range is smaller than the lattice size.");
		return torus_lattice_view<iterator, dim>(Begin, static_cast<lattices::extent_type<dim>>(Extent));
	}
	template<typename iterator, typename... others>
	auto make_torus_lattice_view(iterator Begin, iterator End, lattices::size_type Size, others... Others) {
		return make_torus_lattice_view(Begin, End, lattices::extent(Size, Others...));
	}

	template<typename iterator, typename indexer>
	auto subview(const lattices::basic_torus_locator<iterator, indexer>& Lat1, const lattices::basic_torus_locator<iterator, indexer>& Lat2) {
		return torus_sublattice_view<iterator, indexer::dim()>(
			Lat1.base_begin(),
			Lat1.base_extent(),
			Lat1.base_point(),
			static_cast<lattices::extent_type<indexer::dim()>>(Lat2.base_point() - Lat1.base_point() + typename indexer::point_type(1))
		);
	}
}
#
#endif
