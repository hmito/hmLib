#ifndef HMLIB_MATH_MULTIAXIS_INC
#define HMLIB_MATH_MULTIAXIS_INC 100
#
#include<iterator>
#include<array>
#include<algorithm>
#include"axis.hpp"
#include"../clone_ptrproxy.hpp"
#include"../lattices/indexer.hpp"
#include"../exceptions.hpp"
namespace hmLib {
	template<typename T, unsigned int dim_, typename grid_policy_ = math::grid_policy::round_grid_tag, typename index_type_ = unsigned int, typename calc_type_ = double, int log10_index_threshold_ = -8>
	struct multiaxis {
	private:
		using this_type = multiaxis<T, dim_, grid_policy_, index_type_, calc_type_, log10_index_threshold_>;
	public:
		using element_type = T;
		using value_type = varray<element_type, dim_>;
		using reference = const value_type;
		using index_type = index_type_;
		using axis_type = hmLib::axis<T, grid_policy_, index_type_, calc_type_, log10_index_threshold_>;
		using calc_type = calc_type_;
		using grid_policy = grid_policy_;
		using indexer_type = lattices::indexer<dim_>;
		using point_type = typename indexer_type::point_type;
		using extent_type = typename indexer_type::extent_type;
		using size_type  = typename indexer_type::size_type;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		struct iterator {
			using value_type = value_type;
			using reference = const value_type;
			using pointer = clone_ptrproxy<value_type>;
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using iterator_category = std::random_access_iterator_tag;
		private:
			this_type* Ptr;
			index_type Index;
		public:
			iterator()noexcept: Ptr(nullptr), Index(0){}
			iterator(this_type& Ref_, index_type Index_)noexcept : Ptr(&Ref_), Index(Index_) {}
			reference operator*()const { return Ptr->at(calc_point(Index)); }
			reference operator[](index_type n)const { return Ptr->at(calc_point(Index+n)); }
			pointer operator->()const {return pointer(operator*());}
			iterator& operator++() { ++Index; }
			iterator operator++(int) { 
				iterator Itr = *this;
				operator++();
				return Itr;
			}
			iterator& operator--() { --Index; }
			iterator operator--(int) {
				iterator Itr = *this;
				operator--();
				return Itr;
			}
			iterator& operator+=(index_type n) {
				Index += n;
				return *this;
			}
			iterator& operator-=(index_type n) {
				Index -= n;
				return *this;
			}
			friend iterator operator+(const iterator& itr, index_type n) {
				auto itr2 = itr;
				itr2 += n;
				return itr2;
			}
			friend iterator operator+(index_type n, const iterator& itr) {
				auto itr2 = itr;
				itr2 += n;
				return itr2;
			}
			friend iterator operator-(const iterator& itr, index_type n) {
				auto itr2 = itr;
				itr2 -= n;
				return itr2;
			}
			friend difference_type operator-(const iterator& itr1, const iterator& itr2) {
				return itr1.Index - itr2.Index;
			}
			friend bool operator==(const iterator& itr1, const iterator& itr2){
				return itr1.Index == itr2.Index;
			}
			friend bool operator!=(const iterator& itr1, const iterator& itr2){
				return itr1.Index != itr2.Index;
			}
			friend bool operator<(const iterator& itr1, const iterator& itr2) {
				return itr1.Index < itr2.Index;
			}
			friend bool operator<=(const iterator& itr1, const iterator& itr2) {
				return itr1.Index <= itr2.Index;
			}
			friend bool operator>(const iterator& itr1, const iterator& itr2) {
				return itr1.Index > itr2.Index;
			}
			friend bool operator>=(const iterator& itr1, const iterator& itr2) {
				return itr1.Index >= itr2.Index;
			}
		};
		struct locator {
			using value_type = value_type;
			using reference = const value_type;
			using pointer = clone_ptrproxy<value_type>;
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using point_type = typename indexer_type::point_type;
			using extent_type = typename indexer_type::extent_type;
		private:
			const this_type* Ptr;
			point_type Pos;
		public:
			locator() = default;
			locator(const this_type& Ref_, point_type Pos_):Ptr(&Ref_), Pos(Pos_) {}
		public:
			reference operator*()const { return Ptr->ref(Pos); }
			pointer operator->()const { return pointer(operator*()); }
			reference operator[](const point_type& Pos_)const { return Ptr->ref(Pos+Pos_); }
			template<typename... others>
			reference ref(index_type Pos_, others... Others_) const { return Ptr->ref(Pos+point_type{ Pos_, static_cast<index_type>(Others_)... }); }
			reference at(const point_type& Pos_)const { return Ptr->at(Pos+Pos_); }
			template<typename... others>
			reference at(index_type Pos_, others... Others_) const { return Ptr->at(Pos+point_type{ Pos_, static_cast<index_type>(Others_)... }); }
		public:
			locator& operator+=(const point_type& Dif) { Pos += Dif;  return *this; }
			friend locator operator+(const locator& Loc, const point_type& Dif) {
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend locator operator+(const point_type& Dif, const locator& Loc) { return Loc+Dif; }
			locator& operator-=(const point_type& Dif) { Pos -= Dif;  return *this; }
			friend locator operator-(const locator& Loc, const point_type& Dif) {
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend point_type operator-(const locator& Loc1, const locator& Loc2) {
				return Loc1.Pos - Loc2.Pos;
			}
		public:
			friend bool operator==(const locator& Loc1, const locator& Loc2) { return Loc1.Pos == Loc2.Pos; }
			friend bool operator!=(const locator& Loc1, const locator& Loc2) { return !(Loc1 == Loc2); }
		public:
			void set(const point_type& Pos_) { Pos = Pos_; }
			this_type& add(const point_type& Dif) { return operator+=(Dif); }
			template<typename... others>
			this_type& add(index_type Pos_, others... Others_) { return operator+=(point_type{ Pos_, static_cast<index_type>(Others_)... }); }
			this_type plus(const point_type& Dif)const { return *this + Dif; }
			template<typename... others>
			this_type plus(index_type Pos_, others... Others_)const { return *this + point_type{ Pos_, static_cast<index_type>(Others_)... }; }
		};
	public:
		multiaxis() = default;
		template<typename input_iteratro>
		multiaxis(input_iteratro Beg, input_iteratro End) {
			hmLib_assert(std::distance(Beg, End)==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "number of axis is different from dim.");
			std::copy(Beg, End, AxisSet.begin());
		}	
		axis_type axis(index_type n)const { return AxisSet[n]; }
		void assign_axis(index_type n, axis_type Axis) { hmLib_assert(n<dim_,hmLib::access_exceptions::out_of_range_access,"out of axis range.");  AxisSet[n] = Axis; }
		void assign_axis(index_type n, element_type Lower, element_type Upper, index_type Size) { hmLib_assert(n<dim_, hmLib::access_exceptions::out_of_range_access, "out of axis range.");  AxisSet[n].assign(Lower,Upper,Size); }
		template<typename input_iteratro>
		void assign(input_iteratro Beg, input_iteratro End) {
			hmLib_assert(std::distance(Beg,End)==dim_,hmLib::numeric_exceptions::incorrect_arithmetic_request,"number of axis is different from dim.");
			std::copy(Beg, End, AxisSet.begin());
		}
	public:
		reference at(const point_type& p)const {
			value_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].at(p[i]);
			}
			return Val;
		}
		template<typename... others>
		reference at(index_type Pos_, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return at(point_type{ Pos_, static_cast<index_type>(Others_)... });
		}
		reference operator[](const point_type& p)const {
			value_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i][p[i]];
			}
			return Val;
		}
		template<typename... others>
		reference ref(index_type Pos_, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return operator[](point_type{ Pos_, static_cast<index_type>(Others_)... });
		}
	public:
		iterator begin()const { return iterator(*this, 0); }
		iterator cbegin()const { return begin(); }
		iterator end()const { return iterator(*this, Indexer.lattice_size()); }
		iterator cend()const { return end(); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_) const{ return locator(*this, Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_)const {
			return locate(point_type{ Pos_, static_cast<index_type>(Others_)... }); }
		//!Return locator of (0,0,0...)
		locator front_locate()const { return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate() const{ return locate(extent() + point_type(-1)); }
	public:
		//!Get number of elements included in the lattice
		size_type lattice_size()const { return Indexer.lattice_size(); }
		//!Get point_type Size
		const extent_type& extent()const { return Indexer.extent(); }
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return Indexer.point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return Indexer.index(Point_); }
	private:
		std::array<axis_type, dim_> AxisSet;
		indexer_type Indexer;
	};
}
#
#endif
