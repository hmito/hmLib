#ifndef HMLIB_LATTICES_TORUSLATTICE_INC
#define HMLIB_LATTICES_TORUSLATTICE_INC 100
#
#include<vector>
#include<utility>
#include<iterator>
#include<numeric>
#include"utility.hpp"
#include"exceptions.hpp"
#include"indexer.hpp"
#include"torus_iterator.hpp"
#include"torus_locator.hpp"
#include"torus_lattice_view.hpp"

namespace hmLib {
	template<typename T, unsigned int dim_>
	struct torus_lattice {
	private:
		using this_type = torus_lattice<T, dim_>;
		using data_type = std::vector<T>;
		using iterator_base = typename data_type::iterator;
		using const_iterator_base = typename data_type::const_iterator;
	public:
		using point_type = lattices::point_type<dim_>;
		using extent_type = lattices::extent_type<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
	public:
		using value_type = typename std::decay<T>::type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	public:
		using indexer = lattices::indexer<dim_>;
		using locator = lattices::basic_torus_locator<iterator_base, indexer>;
		using const_locator = lattices::basic_torus_locator<const_iterator_base, indexer>;
		using iterator = lattices::basic_torus_iterator<iterator_base, indexer>;
		using const_iterator = lattices::basic_torus_iterator<const_iterator_base, indexer>;
		using view = torus_sublattice_view<iterator_base, dim_>;
		using const_view = torus_sublattice_view<const_iterator_base, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		torus_lattice() = default;
		torus_lattice(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		torus_lattice(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		torus_lattice(const extent_type& Extent_, const_reference IniVal)
			: Indexer(Extent_) {
			Data.assign(Indexer.lattice_size(), IniVal);
		}
		torus_lattice(const extent_type& Extent_, std::initializer_list<value_type> Ini_) {
			resize(Extent_, std::move(Ini_));
		}
		torus_lattice(const extent_type& Extent_, const data_type& Data_) {
			resize(Extent_, Data_);
		}
		torus_lattice(const extent_type& Extent_, data_type&& Data_) {
			resize(Extent_, std::move(Data_));
		}
		template<typename input_iterator>
		torus_lattice(const extent_type& Extent_, input_iterator Begin, input_iterator End) {
			resize(Extent_, Begin, End);
		}
	public:
		//!Return reference of the elemtn at the given point with range check
		reference at(const point_type& Point_) { return Data[Indexer.torus_index(Point_)]; }
		//!Return const_reference of the elemtn at the given point with range check
		const_reference at(const point_type& Point_)const { return Data[Indexer.torus_index(Point_)]; }
		//!Return reference of the elemtn at the given elements point with range check
		template<typename... others>
		reference at(index_type Pos_, others... Others_) {
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point with range check
		template<typename... others>
		const_reference at(index_type Pos_, others... Others_)const {
			return at(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference ref(const point_type& Point_) { return Data[Indexer.torus_index(Point_)]; }
		//!Return const_reference of the elemtn at the given point
		const_reference ref(const point_type& Point_)const { return Data[Indexer.torus_index(Point_)]; }
		//!Return reference of the elemtn at the given elements point
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return const_reference of the elemtn at the given elements point
		template<typename... others>
		const_reference ref(index_type Pos_, others... Others_)const {
			return ref(lattices::point(Pos_, Others_...));
		}
		//!Return reference of the elemtn at the given point
		reference operator[](const point_type& Point_) { return ref(Point_); }
		//!Return const_reference of the elemtn at the given point
		const_reference operator[](const point_type& Point_)const { return ref(Point_); }
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return Indexer.lattice_size(); }
		//!Get point_type Size
		const extent_type& extent()const { return Indexer.extent(); }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return Indexer.point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
		//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
		reference index_at(index_type Index_) { return Data.at(Index_); }
		//!Return reference of the elemtn at the given Index with checking out-of-range, i.e., at(Pos) == index_at(point_to_index(Pos));
		const_reference index_at(index_type Index_)const { return Data.at(Index_); }
		//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
		reference index_ref(index_type Index_) { return Data[Index_]; }
		//!Return reference of the elemtn at the given Index without checking out-of-range, i.e., ref(Pos) == index_ref(point_to_index(Pos));
		const_reference index_ref(index_type Index_)const { return Data[Index_]; }
	public:
		//!Return begin iterator fot the lattice
		iterator begin() { return iterator(Data.begin(), Indexer, 0);}
		//!Return end iterator fot the lattice
		iterator end() { return iterator(Data.begin(), Indexer, Data.size()); }
		//!Return begin const_iterator fot the lattice
		const_iterator begin()const { return cbegin(); }
		//!Return end const_iterator fot the lattice
		const_iterator end()const { return cend(); }
		//!Return begin const_iterator fot the lattice
		const_iterator cbegin()const { return const_iterator(Data.cbegin(), Indexer, Data.size()); }
		//!Return end const_iterator fot the lattice
		const_iterator cend()const { return const_iterator(Data.cbegin(), Indexer, Data.size()); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_) { return locator(Data.begin(), Indexer, Point_); }
		//!Return const locator of given point
		const_locator locate(const point_type& Point_)const { return const_locator(Data.begin(), Indexer, Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(diff_type Pos_, others... Others_) { return locate(lattices::point(Pos_, Others_...)); }
		//!Return const locator of given point elements
		template<typename... others>
		const_locator locate(diff_type Pos_, others... Others_)const { return locate(lattices::point(Pos_, Others_...)); }
		//!Return locator of (0,0,0...)
		locator front_locate() { return locate(point_type(0)); }
		//!Return locator of (0,0,0...)
		const_locator front_locate()const { return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate() { return locate(extent() + point_type(-1)); }
		//!Return const locator of (size-1)
		const_locator back_locate()const { return locate(extent() + point_type(-1)); }
	public:
		view subview(const point_type& Point_, const extent_type& Extent_) {
			hmLib_assert(hmLib::all_less_equal_than(Extent_, Indexer.extent()), lattices::invalid_range, "The given range is smaller than the lattice size.");
			return view(Data.begin(), Indexer.extent(), Point_, Extent_);
		}
		const_view subview(const point_type& Point_, const extent_type& Extent_) const {
			return csubview(Point_, Extent_);
		}
		const_view csubview(const point_type& Point_, const extent_type& Extent_) const {
			hmLib_assert(hmLib::all_less_equal_than(Extent_, Indexer.extent()), lattices::invalid_range, "The given range is smaller than the lattice size.");
			return view(Data.begin(), Indexer.extent(), Point_, Extent_);
		}
	public:
		bool empty()const { return Data.empty(); }
		void resize() {
			Indexer.resize(point_type(0));
			Data.clear();
		}
		void resize(const extent_type& Extent_) {
			Indexer.resize(Extent_);
			Data.assign(Indexer.lattice_size());
		}
		void resize(const extent_type& Extent_, const_reference IniVal) {
			Indexer.resize(Extent_);
			Data.assign(Indexer.lattice_size(), IniVal);
		}
		void resize(const extent_type& Extent_, std::initializer_list<value_type> Ini_) {
			indexer NewIndexer(Extent_);
			hmLib_assert(Ini_.size() >= NewIndexer.lattice_size(), lattices::invalid_range, "The given Data is smaller than the lattice size.");
			Indexer = NewIndexer;
			Data.assign(std::move(Ini_));
		}
		void resize(const extent_type& Extent_, const data_type& Data_) {
			indexer NewIndexer(Extent_);
			hmLib_assert(Data_.size() >= NewIndexer.lattice_size(), lattices::invalid_range, "The given Data is smaller than the lattice size.");
			Indexer = NewIndexer;
			Data = Data_;
		}
		void resize(const extent_type& Extent_, data_type&& Data_) {
			indexer NewIndexer(Extent_);
			hmLib_assert(Data_.size() >= NewIndexer.lattice_size(), lattices::invalid_range, "The given Data is smaller than the lattice size.");
			Indexer = NewIndexer;
			Data = std::move(Data_);
		}
		template<typename input_iterator>
		void resize(const extent_type& Extent_, input_iterator Begin, input_iterator End) {
			indexer NewIndexer(Extent_);
			hmLib_assert(static_cast<size_type>(std::distance(Begin, End)) >= NewIndexer.lattice_size(), lattices::invalid_range, "The given Data is smaller than the lattice size.");
			Indexer = NewIndexer;
			Data.assign(Begin, End);
		}
		const data_type& data()const { return Data; }
		data_type release() {
			auto ReturnData = std::move(Data);
			Indexer.resize(extent_type(0));
			Data.clear();
			return std::move(ReturnData);
		}
	private:
		data_type Data;
		indexer Indexer;
	};

	template<typename T, typename... others>
	auto make_torus_lattice(const T& inival, lattices::size_type Size, others... Others) {
		return torus_lattice<typename std::decay<T>::type, sizeof...(others)>(inival, lattices::extent(Size, Others...));
	}
	template<typename T, typename element_type, unsigned int dim>
	auto make_torus_lattice(const T& inival, const lattices::point_array_type<element_type, dim>& Extent) {
		return torus_lattice<typename std::decay<T>::type, dim>(inival, static_cast<lattices::extent_type<dim>>(Extent));
	}
}
#
#endif
