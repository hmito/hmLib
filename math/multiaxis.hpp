#ifndef HMLIB_MATH_MULTIAXIS_INC
#define HMLIB_MATH_MULTIAXIS_INC 100
#
#include<iterator>
#include<array>
#include"axis.hpp"
#include"../clone_ptr.hpp"
#include"../lattices/indexer.hpp"
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
		using axis_type = axis<T, grid_policy_, index_type_, calc_type_, log10_index_threshold_>;
		using calc_type = calc_type_;
		using grid_policy = grid_policy_;
		using indexer_type = lattices::indexer<dim_>;
		using point_type = typename indexer_type::point_type;
		using extent_type = typename indexer_type::extent_type;
		using size_type  = typename indexer_type::size_type;
	public:
		struct iterator {
			using value_type = value_type;
			using reference = const value_type;
			using pointer = clone_ptr<value_type>;
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

		};
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
