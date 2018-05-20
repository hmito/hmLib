#ifndef HMLIB_MATH_MULTIAXIS_INC
#define HMLIB_MATH_MULTIAXIS_INC 100
#
#include<iterator>
#include<array>
#include<vector>
#include<utility>
#include<algorithm>
#include"axis.hpp"
#include"../clone_ptrproxy.hpp"
#include"../lattices/indexer.hpp"
#include"../exceptions.hpp"
namespace hmLib {
	namespace math {
		template<typename index_type_, unsigned int dim_>
		struct weighted_point_range {
			using index_type = index_type_;
			using size_type = std::size_t;
		private:
			using indexer_type = lattices::indexer<dim_>;
		public:
			using point_type = typename indexer_type::point_type;
			using extent_type = typename indexer_type::extent_type;
			using weighted_point_type = std::pair<point_type, double>;
			using waighted_range_container = std::vector<math::weighted_index_range>;
			using waighted_range_iterator = typename waighted_range_container::iterator;
		public:
			struct iterator {
			public:
				using value_type = weighted_point;
				using difference_type = signed int;
				using reference = value_type;
				using pointer = hmLib::clone_ptrproxy<value_type>;
				using iterator_category = std::input_iterator_tag;
			private:
				index_type Index;
				indexer_type Indexer;
				range_iterator Beg;
			public:
				iterator() = default;
				iterator(index_type Index_, range_iterator Beg_)
					: Index(Index_)
					, Beg(Beg_) {
					extent_type Extent;
					for(unsigned int i = 0; i<dim(); ++i) {
						Extent[i] = Beg_[i].size();
					}
					Indexer.resize(Extent);
				}
				reference operator*()const {
					point_type p = Indexer.point(Index);
					weighted_point q;
					q.second = 1.0;
					for(unsigned int i = 0; i<dim(); ++i) {
						auto pair = Beg[i].at(p[i]);
						q.first[i] = pair.first;
						q.second *= pair.second;
					}
					return q;
				}
				pointer operator->()const { return pointer(operator*()); }
				iterator& operator++() { ++Index; return *this; }
				iterator operator++(int) {
					iterator Prev = *this;
					operator++();
					return Prev;
				}
				friend bool operator==(const iterator& itr1, const iterator& itr2) {
					return itr1.Index==itr2.Index;
				}
				friend bool operator!=(const iterator& itr1, const iterator& itr2) {
					return itr1.Index!=itr2.Index;
				}
			};
		private:
			waighted_range_container Range;
			indexer_type Indexer;
			unsigned int Size;
			double Weight;
		public:
			weighted_point_range()noexcept {}
			explicit weighted_point_range(waighted_range_container Range_):Range(std::move(Range_)) {
				hmLib_assert(Range_.size()==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "weighted range cannot create from wieghted range container whose size is not equal to dim.");
				extent_type Extent;
				Size = 1;
				Weight = 1.0;
				for(unsigned int i = 0; i<dim(); ++i) {
					Extent[i] = Range.at(i).size();
					Size *= Extent[i];
					Weight *= Range.at(i).weight();
				}
			}
			weighted_point_type operator[](index_type Index)const {
				point_type p = Indexer.point(Index);
				weighted_point q;
				q.second = 1.0;
				for(unsigned int i = 0; i<dim(); ++i) {
					auto pair = Range.at(p[i]);
					q.first[i] = pair.first;
					q.second *= pair.second;
				}
				return q;
			}
			weighted_point_type at(index_type Index)const {
				hmLib_assert(0<=Index && Index < size(), hmLib::access_exceptions::out_of_range_access, "Out of range access.");
				return operator[](Index);
			}
			bool empty()const { return Range.empty(); }
			unsigned int size()const { return Size; }
			double weight()const { return Weight; }
			iterator begin()const {
				return iterator(0, Range.begin());
			}
			iterator end()const {
				return iterator(size(), Range.begin());
			}
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		};
	}
	template<typename T, unsigned int dim_, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = unsigned int, typename calc_type_ = double>
	struct multiaxis {
	private:
		using this_type = multiaxis<T, dim_, grid_adjuster_, index_type_, calc_type_>;
	public:
		using value_type = T;
		using index_type = index_type_;
		using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
		using axis_type = hmLib::axis<T, grid_adjuster_, index_type_, calc_type_>;
	private:
		using axis_container = std::vector<axis_type>;
		using indexer_type = lattices::indexer<dim_>;
	public:
		using point_type = typename indexer_type::point_type;
		using extent_type = typename indexer_type::extent_type;
		using size_type = typename indexer_type::size_type;
		using value_point_type = varray<value_type, dim_>;
		using float_point_type = varray<double, dim_>;
		using weighted_point_range = math::weighted_point_range<index_type, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		struct iterator {
			using value_type = value_point_type;
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
			using value_type = value_point_type;
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
		template<typename... other_axes>
		multiaxis(axis_type ax, other_axes... others){
			assign(ax, others...);
		}
		template<typename input_iterator>
		multiaxis(input_iterator Beg, input_iterator End) {
			assign(Beg, End);
		}	
		explicit multiaxis(axis_container AxisSet_) {
			assign(AxisSet_);
		}
	public:
		void clear() {
			AxisSet.clear();
			Indexer.resize(extent_type(0));
		}
		bool empty()const { return AxisSet.empty(); }
		template<typename... other_axes>
		void assign(axis_type ax, other_axes... others) {
			static_assert(sizeof...(others)==dim_-1, "argument number is different from dim.");
			AxisSet = axis_container{ ax, others... };

			extent_type Extent;
			for(unsigned int i = 0; i<dim(); ++i) {
				Extent[i] = AxisSet[i].size();
			}
			Indexer.resize(Extent);
		}
		template<typename input_iteratro>
		void assign(input_iteratro Beg, input_iteratro End) {
			hmLib_assert(std::distance(Beg, End)==dim_,hmLib::numeric_exceptions::incorrect_arithmetic_request,"number of axis is different from dim.");
			AxisSet.assign(Beg, End);

			extent_type Extent;
			for(unsigned int i = 0; i<dim(); ++i) {
				Extent[i] = AxisSet[i].size();
			}
			Indexer.resize(Extent);
		}
		void assign(axis_container AxisSet_) {
			hmLib_assert(AxisSet_.size()==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "multiaxis require the axis container with size dim.");
			AxisSet = std::move(AxisSet_);

			extent_type Extent;
			for(unsigned int i = 0; i<dim(); ++i) {
				Extent[i] = AxisSet[i].size();
			}
			Indexer.resize(Extent);
		}
	public:
		const axis_type& axis(index_type n)const { return AxisSet[n]; }
		void assign_axis(index_type n, axis_type Axis) { 
			hmLib_assert(n<dim_, hmLib::access_exceptions::out_of_range_access, "out of axis range.");
			AxisSet[n] = Axis; 

			extent_type Extent = extent();
			Extent[n] = Axis.size();
			Indexer.resize(Extent);
		}
		void assign_axis(index_type n, value_type Lower, value_type Upper, index_type Size) {
			hmLib_assert(n<dim_, hmLib::access_exceptions::out_of_range_access, "out of axis range.");
			AxisSet[n].assign(Lower, Upper, Size);

			extent_type Extent = extent();
			Extent[n] = Size;
			Indexer.resize(Extent);
		}
	public:
		value_point_type at(const point_type& p)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].at(p[i]);
			}
			return Val;
		}
		template<typename... others>
		value_point_type at(index_type Pos_, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return at(point_type{ Pos_, static_cast<index_type>(Others_)... });
		}
		value_point_type operator[](const point_type& p)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i][p[i]];
			}
			return Val;
		}
		template<typename... others>
		value_point_type ref(index_type Pos_, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return operator[](point_type{ Pos_, static_cast<index_type>(Others_)... });
		}
		value_point_type float_at(const float_point_type& fp)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].float_at(fp[i]);
			}
			return Val;
		}
		template<typename... others>
		value_point_type float_at(double Pos_, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for float_at is different from dim.");
			return float_at(float_point_type{ Pos_, static_cast<double>(Others_)... });
		}
	public:
		point_type point(value_point_type p)const {
			point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].index(p[i]);
			}
			return Val;
		}
		float_point_type float_point(value_point_type p)const {
			float_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].float_index(p[i]);
			}
			return Val;
		}
		weighted_point_range weighted_point(value_point_type LowerVal, value_point_type UpperVal)const {
			typename weighted_point_range::waighted_range_container Container;
			for(unsigned int i = 0; i<dim(); ++i) {
				Container.push_back(AxisSet[i].weighted_index(LowerVal[i], UpperVal[i]));;
			}
			return weighted_point_range(std::move(Container));
		}
		value_point_type lower()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].lower();
			}
			return Val;
		}
		value_point_type upper()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].upper();
			}
			return Val;
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
			return locate(point_type{ Pos_, static_cast<index_type>(Others_)... }); 
		}
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
	public:
		friend bool operator==(const this_type& axis1, const this_type& axis2) {
			if(axis1.empty()) {
				if(axis2.empty()) {
					return true;
				} else {
					return false;
				}
			} else if(axis2.empty()) {
				return false;
			}
			for(unsigned int i==0; i<dim(); ++i) {
				if(axis1.AxisSet[i]!=axis2.AxisSet[i])return false;
			}
			return true;
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	private:
		std::vector<axis_type> AxisSet;
		indexer_type Indexer;
	};
	template<typename T, unsigned int dim_, typename grid_adjuster>
	auto make_multiaxis(varray<T, dim_> Lower, varray<T, dim_> Upper, varray<std::size_t, dim_> Size, grid_adjuster GridAdjuster, math::make_axis_option Opt = math::make_axis_option::none) {
		using multiaxis_type = multiaxis<typename std::decay<T>::type, dim_, grid_adjuster>;
		using axis_type = typename multiaxis_type::axis_type;
		using axis_container = typename multiaxis_type::axis_container;

		axis_container Container;
		for(unsigned int i = 0; i<dim_; ++i) {
			Container.push_back(make_axis(Lower[i], Upper[i], Size[i], GridAdjuster, Opt));
		}
		return multiaxis_type(std::move(Container));
	}
	template<typename T>
	auto make_multiaxis(varray<T, dim_> Lower, varray<T, dim_> Upper, varray<std::size_t, dim_> Size, math::make_axis_option Opt = math::make_axis_option::none) {
		return make_multiaxis(Lower, Upper, Size, math::default_grid_adjuster(), Opt);
	}

	template<unsigned int dim_, typename from_grid_adjuster_, typename to_grid_adjuster_, typename index_type_>
	struct multiaxis_mapper {
	private:
		using this_type = multiaxis_mapper<dim_, from_grid_adjuster_, to_grid_adjuster_, index_type_>;
		using from_grid_adjuster = from_grid_adjuster_;
		using to_grid_adjuster = to_grid_adjuster_;
		using index_type = index_type_;
		using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
		using weighted_point_range = math::weighted_point_range<index_type, dim_>;
		using axis_mapper = axis_mapper<from_grid_adjuster_, to_grid_adjuster_, index_type>;
	public:
		using point_type = varray<index_type, dim_>;
		using float_point_type = varray<double, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		multiaxis_mapper() = delete;
		template<typename from_multiaxis_type, typename to_multiaxis_type>
		multiaxis_mapper(const from_multiaxis_type& from, const to_multiaxis_type& to) {
			static_assert(from_multiaxis_type::dim() == dim() && to_multiaxis_type::dim() == dim(), "dim of given axis is different.");
			for(unsigned int i = 0; i<dim(); ++i) {
				MapperSet.emplace_back(from.axis(i), to.axis(i));
			}
		}
	public:
		point_type lower()const {
			point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].lower();
			}
			return Pos;
		}
		point_type upper()const {
			point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].upper();
			}
			return Pos;
		}
		bool inside(float_point_type FromIndex)const {
			for(unsigned int i = 0; i<dim_; ++i) {
				if(!MapperSet[i].inside(FromIndex[i]))return false;
			}
			return true;
		}
		point_type operator[](float_point_type FromIndex)const {
			point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i][FromIndex[i];
			}
			return Pos;
		}
		point_type point(float_point_type FromIndex)const {
			point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].index(FromIndex[i]);
			}
			return Pos;
		}
		float_point_type float_point(float_point_type FromIndex)const {
			float_point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].float_index(FromIndex[i]);
			}
			return Pos;
		}
		weighted_point_range weighted_point(point_type FromIndex)const {
			typename weighted_point_range::waighted_range_container Container;
			for(unsigned int i = 0; i<dim_; ++i) {
				Container.push_back(MapperSet[i].weighted_index(FromIndex[i]));;
			}
			return weighted_point_range(std::move(Container));
		}
	private:
		std::vector<axis_mapper> MapperSet;
	};
	template<typename from_multiaxis, typename to_multiaxis>
	auto map_axis(const from_multiaxis& from, const to_multiaxis& to) {
		using from_grid_adjuster = typename from_multiaxis::grid_adjuster;
		using to_grid_adjuster = typename to_multiaxis::grid_adjuster;
		using index_type = typename from_multiaxis::index_type;

		return multiaxis_mapper<from_grid_adjuster, to_grid_adjuster, index_type>(from, to);
	}
}
#
#endif
