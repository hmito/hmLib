#ifndef HMLIB_MATH_MULTIAXIS_INC
#define HMLIB_MATH_MULTIAXIS_INC 101
#
#include<iterator>
#include<array>
#include<utility>
#include<algorithm>
#include<initializer_list>
#include"axis.hpp"
#include"../lattices/indexer.hpp"
#include"../exceptions.hpp"
#include"../clone_ptrproxy.hpp"
namespace hmLib {
	namespace math {
		template<typename index_type_, unsigned int dim_>
		struct weighted_point_range {
			using index_type = index_type_;
			using size_type = std::size_t;
			constexpr static unsigned int dim() { return dim_; }
		private:
			using indexer_type = lattices::indexer<dim_>;
		public:
			using point_type = typename indexer_type::point_type;
			using extent_type = typename indexer_type::extent_type;
			using weighted_point_type = std::pair<point_type, double>;	//point - proportion of the point
			using waighted_range_container = std::vector<math::weighted_index_range<index_type>>;
			using waighted_range_iterator = typename waighted_range_container::const_iterator;
		public:
			struct iterator {
			public:
				using value_type = weighted_point_type;
				using difference_type = signed int;
				using reference = value_type;
				using pointer = hmLib::clone_ptrproxy<value_type>;
				using iterator_category = std::input_iterator_tag;
			private:
				index_type Index;
				indexer_type Indexer;
				waighted_range_iterator Beg;
			public:
				iterator() = default;
				iterator(index_type Index_, waighted_range_iterator Beg_)
					: Index(Index_)
					, Beg(Beg_) {
					extent_type Extent;
					for(unsigned int i = 0; i<dim(); ++i) {
						Extent[i] = Beg_[i].size();
					}
					Indexer.resize(Extent);
				}
			public:
				point_type point()const { return base_point(); }
				extent_type extent()const { return base_extent(); }
				index_type index()const { return base_index(); }
			public:
				point_type base_point()const { return Indexer.calc_point(Index); }
				extent_type base_extent()const { return Indexer.extent(); }
				index_type base_index()const { return Index; }
			public:
				reference operator*()const {
					point_type p = Indexer.point(Index);
					weighted_point_type q;
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
			explicit weighted_point_range(waighted_range_container Range_){
				hmLib_assert(Range_.size()==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "weighted range cannot create from wieghted range container whose size is not equal to dim.");
				Range = std::move(Range_);
				extent_type Extent;
				Size = 1;
				Weight = 1.0;
				for(unsigned int i = 0; i<dim(); ++i) {
					Extent[i] = Range.at(i).size();
					Size *= static_cast<unsigned int>(Extent[i]);
					Weight *= Range.at(i).volume();
				}
			}
			weighted_point_type operator[](index_type Index)const {
				point_type p = Indexer.point(Index);
				weighted_point_type q;
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
			double volume()const { return Weight; }
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
	template<typename T, unsigned int dim_, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = int, 
		typename calc_type_ = typename std::conditional<std::is_same<decltype(std::declval<T>()*std::declval<double>()), double>::value, double, T>::type
	>
	struct multiaxis {
	private:
		using this_type = multiaxis<T, dim_, grid_adjuster_, index_type_, calc_type_>;
	public:
		using value_type = T;
		using grid_adjuster = grid_adjuster_;
		using index_type = index_type_;
		using size_type = std::size_t;
		using calc_type = calc_type_;
	public:
		using axis_type = hmLib::axis<T, grid_adjuster, index_type, calc_type>;
		using axis_container = std::array<axis_type,dim_>;
		using indexer_type = lattices::indexer<dim_>;
		using point_type = typename indexer_type::point_type;
		using extent_type = typename indexer_type::extent_type;
		using value_point_type = varray<value_type, dim_>;
		using float_point_type = varray<double, dim_>;
		using weighted_point_range = math::weighted_point_range<index_type, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public://locator
		struct locator {
			using value_type = value_point_type;
			using reference = const value_type;
			using pointer = clone_ptrproxy<const value_type>;
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
		multiaxis(std::initializer_list<axis_type> axlist)noexcept: AxisSet(std::move(axlist)) { }
		template<typename input_iterator>
		multiaxis(input_iterator Beg, input_iterator End) {
			assign(Beg, End);
		}
	public:
		void assign(std::initializer_list<axis_type> axlist) {
			assign(axlist.begin(), axlist.end());
		}
		template<typename input_iteratro>
		void assign(input_iteratro Beg, input_iteratro End) {
			hmLib_assert(std::distance(Beg, End)==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "number of axis is different from dim.");
			std::copy(Beg, End, AxisSet.begin());
		}
		axis_type& axis(index_type n) { return AxisSet[n]; }
		const axis_type& axis(index_type n)const { return AxisSet[n]; }
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
		value_point_type ref(const point_type& p) const {
			return operator[](p);
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
		point_type point(value_point_type p)const {
			point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].index(p[i]);
			}
			return Val;
		}
		template<typename... others>
		point_type point(value_type pv, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return point(value_point_type{ pv, static_cast<value_type>(Others_)... });
		}
		float_point_type float_point(value_point_type p)const {
			float_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].float_index(p[i]);
			}
			return Val;
		}
		template<typename... others>
		float_point_type float_point(value_type pv, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return float_point(value_point_type{ pv, static_cast<value_type>(Others_)... });
		}
		weighted_point_range weighted_point(value_point_type LowerVal, value_point_type UpperVal)const {
			typename weighted_point_range::waighted_range_container Container;
			for(unsigned int i = 0; i<dim(); ++i) {
				Container.push_back(AxisSet[i].weighted_index(LowerVal[i], UpperVal[i]));;
			}
			return weighted_point_range(std::move(Container));
		}
		point_type max_point()const {
			point_type Pos;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Pos[i] = AxisSet[i].max_index();
			}
			return Pos;
		}
		point_type min_point()const {
			point_type Pos;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Pos[i] = AxisSet[i].min_index();
			}
			return Pos;
		}
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_) const{ return locator(*this, Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_)const {
			return locate(point_type{ Pos_, static_cast<index_type>(Others_)... }); 
		}
	public:
		value_point_type interval()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].interval();
			}
			return Val;
		}
		value_point_type origin()const{
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].origin();
			}
			return Val;
		}
		value_point_type width()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].width();
			}
			return Val;
		}
		value_point_type grid_lower_at(point_type Pos)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_lower_at(Pos[i]);
			}
			return Val;
		}
		value_point_type grid_upper_at(point_type Pos)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_upper_at(Pos[i]);
			}
			return Val;
		}
		bool inside(value_point_type Val)const {
			return true;
		}
		bool inside(value_type LowerVal, value_type UpperVal)const {
			return true;
		}
	public:
		template<typename to_multiaxis>
		auto map_to(const to_multiaxis& to)const {
			return map_multiaxis(*this, to);
		}
	public:
		friend bool operator==(const this_type& axis1, const this_type& axis2) {
			for(unsigned int i=0; i<dim(); ++i) {
				if(axis1.AxisSet[i]!=axis2.AxisSet[i])return false;
			}
			return true;
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	private:
		axis_container AxisSet;
	};

	template<typename T, unsigned int dim_, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = int,
		typename calc_type_ = typename std::conditional<std::is_same<decltype(std::declval<T>()*std::declval<double>()),double>::value, double, T>::type
	>
	struct lattice_axis {
	private:
		using this_type = lattice_axis<T, dim_, grid_adjuster_, index_type_, calc_type_>;
	public:
		using value_type = T;
		using grid_adjuster = grid_adjuster_;
		using index_type = index_type_;
		using size_type = std::size_t;
		using calc_type = calc_type_;
	public:
		using axis_type = hmLib::range_axis<T, grid_adjuster, index_type, calc_type>;
		using axis_container = std::array<axis_type, dim_>;
		using indexer_type = lattices::indexer<dim_>;
		using point_type = typename indexer_type::point_type;
		using extent_type = typename indexer_type::extent_type;
		using value_point_type = varray<value_type, dim_>;
		using float_point_type = varray<double, dim_>;
		using weighted_point_range = math::weighted_point_range<index_type, dim_>;
	public:
		static constexpr unsigned int dim() { return dim_; }
	public:
		struct iterator {
			using value_type = value_point_type;
			using reference = const value_type;
			using pointer = clone_ptrproxy<const value_type>;
			using difference_type = decltype(std::declval<index_type>()-std::declval<index_type>());
			using iterator_category = std::random_access_iterator_tag;
		private:
			const this_type* Ptr;
			index_type Index;
		public:
			iterator()noexcept: Ptr(nullptr), Index(0) {}
			iterator(const this_type& Ref_, index_type Index_)noexcept: Ptr(&Ref_), Index(Index_) {}
			reference operator*()const { return Ptr->at(Ptr->index_to_point(Index)); }
			reference operator[](index_type n)const { return Ptr->at(Ptr->index_to_point(Index+n)); }
			pointer operator->()const { return pointer(operator*()); }
			iterator& operator++() { ++Index; return *this; }
			iterator operator++(int) {
				iterator Itr = *this;
				operator++();
				return Itr;
			}
			iterator& operator--() { --Index; return *this; }
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
			friend bool operator==(const iterator& itr1, const iterator& itr2) {
				return itr1.Index == itr2.Index;
			}
			friend bool operator!=(const iterator& itr1, const iterator& itr2) {
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
			using pointer = clone_ptrproxy<const value_type>;
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
		lattice_axis() = default;
		lattice_axis(std::initializer_list<axis_type> axlist)noexcept: AxisSet(std::move(axlist)) { }
		template<typename input_iterator>
		lattice_axis(input_iterator Beg, input_iterator End) {
			assign(Beg, End);
		}
	public:
		void assign(std::initializer_list<axis_type> axlist) {
			assign(axlist.begin(), axlist.end());
		}
		template<typename input_iteratro>
		void assign(input_iteratro Beg, input_iteratro End) {
			hmLib_assert(std::distance(Beg, End)==dim_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "number of axis is different from dim.");
			std::copy(Beg, End, AxisSet.begin());
		}
		size_type lattice_size()const {
			size_type s = 1;
			for(int i = 0; i<static_cast<int>(dim()); ++i) s *= AxisSet[i].size();
			return s;
		}
		extent_type extent()const {
			extent_type e;
			for(int i = 0; i<static_cast<int>(dim()); ++i) e[i] = AxisSet[i].size();
			return e;
		}
		indexer_type indexer()const { return indexer_type(extent()); }
		axis_type& axis(index_type n) { return AxisSet[n]; }
		const axis_type& axis(index_type n)const { return AxisSet[n]; }
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
		value_point_type ref(const point_type& p) const {
			return operator[](p);
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
		point_type point(value_point_type p)const {
			point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].index(p[i]);
			}
			return Val;
		}
		template<typename... others>
		point_type point(value_type pv, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return point(value_point_type{ pv, static_cast<value_type>(Others_)... });
		}
		float_point_type float_point(value_point_type p)const {
			float_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].float_index(p[i]);
			}
			return Val;
		}
		template<typename... others>
		float_point_type float_point(value_type pv, others... Others_) const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return float_point(value_point_type{ pv, static_cast<value_type>(Others_)... });
		}
		weighted_point_range weighted_point(value_point_type LowerVal, value_point_type UpperVal)const {
			typename weighted_point_range::waighted_range_container Container;
			for(unsigned int i = 0; i<dim(); ++i) {
				Container.push_back(AxisSet[i].weighted_index(LowerVal[i], UpperVal[i]));;
			}
			return weighted_point_range(std::move(Container));
		}
	public:
		iterator begin()const { return iterator(*this, 0); }
		iterator cbegin()const { return begin(); }
		iterator end()const { return iterator(*this, lattice_size()); }
		iterator cend()const { return end(); }
	public:
		//!Return locator of given point
		locator locate(const point_type& Point_) const { return locator(*this, Point_); }
		//!Return locator of given point elements
		template<typename... others>
		locator locate(index_type Pos_, others... Others_)const {
			return locate(point_type{ Pos_, static_cast<index_type>(Others_)... });
		}
		//!Return locator of (0,0,0...)
		locator front_locate()const { return locate(point_type(0)); }
		//!Return locator of (size-1)
		locator back_locate() const { return locate(extent() + point_type(-1)); }
	public:
		value_point_type interval()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].interval();
			}
			return Val;
		}
		value_point_type width()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].width();
			}
			return Val;
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
		value_point_type grid_width()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_width();
			}
			return Val;
		}
		value_point_type grid_lower()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_lower();
			}
			return Val;
		}
		value_point_type grid_upper()const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_upper();
			}
			return Val;
		}
		value_point_type grid_lower_at(point_type Pos)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_lower_at(Pos[i]);
			}
			return Val;
		}
		value_point_type grid_upper_at(point_type Pos)const {
			value_point_type Val;
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				Val[i] = AxisSet[i].grid_upper_at(Pos[i]);
			}
			return Val;
		}
		bool inside(value_point_type Val)const {
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				if(!AxisSet[i].inside(Val[i]))return false;
			}
			return true;
		}
		bool inside(value_type LowerVal, value_type UpperVal)const {
			for(unsigned int i = 0; i<AxisSet.size(); ++i) {
				if(!AxisSet[i].inside(LowerVal[i], UpperVal[i]))return false;
			}
			return true;
		}
	public:
		//!Convert from index to point
		point_type index_to_point(index_type Index_)const { return indexer().calc_point(Index_); }
		//!Convert from point to index
		index_type point_to_index(point_type Point_)const { return indexer().calc_index(Point_); }
		value_type index_at(index_type Index)const { return at(index_to_point(Index)); }
		value_type index_ref(index_type Index)const { return ref(index_to_point(Index)); }
	public:
		template<typename to_multiaxis>
		auto map_to(const to_multiaxis& to)const {
			return map_multiaxis(*this, to);
		}
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
			for(unsigned int i = 0; i<dim(); ++i) {
				if(axis1.AxisSet[i]!=axis2.AxisSet[i])return false;
			}
			return true;
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	private:
		axis_container AxisSet;
	};
	template<typename T, unsigned int dim_, typename grid_adjuster>
	auto make_lattice_axis(varray<T, dim_> Lower, varray<T, dim_> Upper, varray<std::size_t, dim_> Size, grid_adjuster GridAdjuster, math::range_axis_option Opt = math::range_axis_option::none) {
		using multiaxis_type = multiaxis<typename std::decay<T>::type, dim_, grid_adjuster>;
		//using axis_type = typename multiaxis_type::axis_type;
		using axis_container = typename multiaxis_type::axis_container;

		axis_container Container;
		for(unsigned int i = 0; i<dim_; ++i) {
			Container.push_back(make_axis(Lower[i], Upper[i], Size[i], GridAdjuster, Opt));
		}
		return multiaxis_type(Container.begin(), Container.end());
	}
	template<typename T, unsigned int dim_>
	auto make_lattice_axis(varray<T, dim_> Lower, varray<T, dim_> Upper, varray<std::size_t, dim_> Size, math::range_axis_option Opt = math::range_axis_option::none) {
		return make_multiaxis(Lower, Upper, Size, math::default_grid_adjuster(), Opt);
	}
	template<typename T, typename grid_adjuster_, typename index_type_, typename calc_type_, typename... others>
	auto bind_axis(axis<T, grid_adjuster_, index_type_, calc_type_> Axis, others... Others) {
		return multiaxis<T, 1+sizeof...(others), grid_adjuster_, index_type_, calc_type_>{
			Axis, Others...
		};
	}
	template<typename T, typename grid_adjuster_, typename index_type_, typename calc_type_, typename... others>
	auto bind_range_axis(range_axis<T, grid_adjuster_, index_type_, calc_type_> Axis, others... Others) {
		return lattice_axis<T, 1+sizeof...(others), grid_adjuster_, index_type_, calc_type_>{
			Axis, Others...
		};
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
		point_type point(float_point_type FromIndex)const {
			point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].index(FromIndex[i]);
			}
			return Pos;
		}
		template<typename... others>
		point_type point(double FromIndex, others... Others)const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return point(float_point_type{ FromIndex , static_cast<double>(Others)... });
		}
		float_point_type float_point(float_point_type FromIndex)const {
			float_point_type Pos;
			for(unsigned int i = 0; i<dim_; ++i) {
				Pos[i] = MapperSet[i].float_index(FromIndex[i]);
			}
			return Pos;
		}
		template<typename... others>
		float_point_type float_point(double FromIndex, others... Others)const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return float_point(float_point_type{ FromIndex , static_cast<double>(Others)... });
		}
		weighted_point_range weighted_point(point_type FromIndex)const {
			typename weighted_point_range::waighted_range_container Container;
			for(unsigned int i = 0; i<dim_; ++i) {
				Container.push_back(MapperSet[i].weighted_index(FromIndex[i]));;
			}
			return weighted_point_range(std::move(Container));
		}
		template<typename... others>
		weighted_point_range weighted_point(index_type FromIndex, others... Others)const {
			static_assert(sizeof...(others)==dim_-1, "argument number for at is different from dim.");
			return weighted_point(point_type{ FromIndex , static_cast<index_type>(Others)... });
		}
	private:
		std::vector<axis_mapper> MapperSet;
	};
	template<typename from_multiaxis, typename to_multiaxis>
	auto map_multiaxis(const from_multiaxis& from, const to_multiaxis& to) {
		using from_grid_adjuster = typename from_multiaxis::grid_adjuster;
		using to_grid_adjuster = typename to_multiaxis::grid_adjuster;
		using index_type = typename from_multiaxis::index_type;

		return multiaxis_mapper<from_multiaxis::dim(), from_grid_adjuster, to_grid_adjuster, index_type>(from, to);
	}
}
#
#endif
