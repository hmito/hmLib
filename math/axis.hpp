#ifndef HMLIB_MATH_AXIS_INC
#define HMLIB_MATH_AXIS_INC 100
#
#include<utility>
#include<iterator>
#include<type_traits>
#include<cmath>
#include"../utility.hpp"
#include"../exceptions.hpp"
#include"../clone_ptrproxy.hpp"
namespace hmLib {
	namespace math {
		namespace grid_policy {
			struct round_grid_tag {};
			struct floor_grid_tag {};
			struct ceil_grid_tag {};
		}
		template<typename grid_policy, int log10_index_threshold>
		struct grid_adjuster {};
		template<int log10_index_threshold>
		struct grid_adjuster<grid_policy::round_grid_tag, log10_index_threshold> {
			inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
			template<typename index_type>
			static index_type index_cast(double index_) { return static_cast<index_type>(std::round(index_)); }
			inline static std::pair<double, double> castable_index_range(double FIndexLower, double FIndexUpper) { return std::make_pair(FIndexLower + 0.50, FIndexUpper + 0.50 -index_threshold()); }
			template<typename index_type>
			static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index-0.5, Index+0.5); }
			template<typename value_type, typename difference_type>
			static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value-Interval/2.0, Value+Interval/2.0); }
			grid_adjuster() = default;
		};
		template<int log10_index_threshold>
		struct grid_adjuster<grid_policy::floor_grid_tag, log10_index_threshold> {
			inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
			template<typename index_type>
			static index_type index_cast(double index_) { return static_cast<index_type>(std::floor(index_+index_threshold())); }
			inline static std::pair<double, double> castable_index_range(double FIndexLower, double FIndexUpper) { return std::make_pair(FIndexLower, FIndexUpper -index_threshold()); }
			template<typename index_type>
			static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index+0.0, Index+1.0); }
			template<typename value_type, typename difference_type>
			static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value, Value+Interval); }
			grid_adjuster() = default;
		};
		template<int log10_index_threshold>
		struct grid_adjuster<grid_policy::ceil_grid_tag, log10_index_threshold> {
			inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
			template<typename index_type>
			static index_type index_cast(double index_) { return static_cast<index_type>(std::ceil(index_-index_threshold())); }
			inline static std::pair<double, double> castable_index_range(double FIndexLower, double FIndexUpper) { return std::make_pair(FIndexLower + 1.0 + index_threshold(), FIndexUpper + 1.0); }
			template<typename index_type>
			static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index-1.0, Index+0.0); }
			template<typename value_type, typename difference_type>
			static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value-Interval, Value); }
			grid_adjuster() = default;
		};

		template<int log10_index_threshold>
		using round_grid_adjuster = grid_adjuster<grid_policy::round_grid_tag,log10_index_threshold>;
		template<int log10_index_threshold>
		using floor_grid_adjuster = grid_adjuster<grid_policy::floor_grid_tag, log10_index_threshold>;
		template<int log10_index_threshold>
		using ceil_grid_adjuster = grid_adjuster<grid_policy::ceil_grid_tag, log10_index_threshold>;
		using default_grid_adjuster = round_grid_adjuster<-8>;

		template<typename index_type_>
		struct weighted_index_range {
			using index_type = index_type_;
			using weighted_index = std::pair<index_type, double>;
		private:
			double LowerCIndex;	//Castable index: [0.0, 1.0) is the range 
			double UpperCIndex;	//	static_cast<index_type>(CIndex) == Index <=> Same Index
			std::size_t Size;
		public:
			struct iterator {
			public:
				using value_type = weighted_index;
				using difference_type = signed int;
				using reference = value_type;
				using pointer = hmLib::clone_ptrproxy<value_type>;
				using iterator_category = std::random_access_iterator_tag;
			private:
				index_type Index;
				double LowerCIndex;
				double UpperCIndex;
			public:
				iterator() = default;
				iterator(index_type Index_, double LowerCIndex_, double UpperCIndex_)
					: Index(Index_)
					, LowerCIndex(LowerCIndex_)
					, UpperCIndex(UpperCIndex_){
				}
				reference operator*()const {
					if(Index == 0) {
						return weighted_index(static_cast<index_type>(LowerCIndex), std::min(1.0, (std::ceil(LowerCIndex) - LowerCIndex)/(UpperCIndex-LowerCIndex)));
					} else {
						return weighted_index(static_cast<index_type>(LowerCIndex)+Index, std::min(1.0, UpperCIndex - (std::floor(LowerCIndex) + Index))/(UpperCIndex-LowerCIndex));
					}
				}
				reference operator[](difference_type n) { return *(*this+n); }
				pointer operator->()const { return pointer(operator*()); }
				iterator& operator++() { ++Index; return *this; }
				iterator operator++(int) {
					iterator Prev = *this;
					operator++();
					return Prev;
				}
				iterator& operator--() { --Index; return *this; }
				iterator operator--(int) {
					iterator Prev = *this;
					operator--();
					return Prev;
				}
				iterator& operator+=(difference_type n) {
					Index += n;
					return *this;
				}
				iterator& operator-=(difference_type n) {
					Index -= n;
					return *this;
				}
				friend iterator operator+(const iterator& itr1, difference_type n) {
					auto ans = itr1;
					ans += n;
					return ans;
				}
				friend iterator operator+(difference_type n, const iterator& itr1) {
					auto ans = itr1;
					ans += n;
					return ans;
				}
				friend iterator operator-(const iterator& itr1, difference_type n) {
					auto ans = itr1;
					ans -= n;
					return ans;
				}
				friend difference_type operator-(const iterator& itr1, const iterator& itr2) {
					return itr1.Index - itr2.Index;
				}
				friend bool operator==(const iterator& itr1, const iterator& itr2) {
					return itr1.Index==itr2.Index;
				}
				friend bool operator!=(const iterator& itr1, const iterator& itr2) {
					return itr1.Index!=itr2.Index;
				}
				friend bool operator<(const iterator& itr1, const iterator& itr2) {
					return itr1.Index<itr2.Index;
				}
				friend bool operator<=(const iterator& itr1, const iterator& itr2) {
					return itr1.Index<=itr2.Index;
				}
				friend bool operator>(const iterator& itr1, const iterator& itr2) {
					return itr1.Index>itr2.Index;
				}
				friend bool operator>=(const iterator& itr1, const iterator& itr2) {
					return itr1.Index>=itr2.Index;
				}
			};
		public:
			weighted_index_range()noexcept: LowerCIndex(0.0), UpperCIndex(0.0), Size(0){}
			weighted_index_range(double LowerCIndex_, double UpperCIndex_)noexcept: LowerCIndex(LowerCIndex_), UpperCIndex(UpperCIndex_)
				, Size(static_cast<index_type>(UpperCIndex) -static_cast<index_type>(LowerCIndex)+1){
				if(LowerCIndex > UpperCIndex) {
					Size = 0;
				}
			}
			weighted_index operator[](index_type Index)const {
				if(Index == 0) {
					return weighted_index(static_cast<index_type>(LowerCIndex), std::min(1.0, (std::ceil(LowerCIndex) - LowerCIndex)/(UpperCIndex-LowerCIndex)));
				} else {
					return weighted_index(static_cast<index_type>(LowerCIndex)+Index, std::min(1.0, UpperCIndex - (std::floor(LowerCIndex) + Index))/(UpperCIndex-LowerCIndex));
				}
			}
			weighted_index at(index_type Index)const {
				hmLib_assert(0<=Index && Index < static_cast<int>(size()), hmLib::access_exceptions::out_of_range_access, "Out of range access.");
				return operator[](Index);
			}
			bool empty()const { return Size==0; }
			std::size_t size()const { return Size; }
			double volume()const { return UpperCIndex-LowerCIndex; }
			iterator begin()const {
				return iterator(0, LowerCIndex, UpperCIndex);
			}
			iterator end()const {
				return iterator(static_cast<int>(size()), LowerCIndex, UpperCIndex);
			}
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		};
	}
	template<typename T, typename grid_adjuster_, typename index_type_,typename calc_type_> 
	struct range_axis;

	template<typename T, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = int, 
		typename calc_type_ = typename std::conditional<std::is_same<decltype(std::declval<T>()*std::declval<double>()), double>::value, double, T>::type
	>
	struct axis {
	private:
		using this_type = axis<T, grid_adjuster_, index_type_, calc_type_>;
	public:
		using value_type = T;
		using grid_adjuster = grid_adjuster_;
		using index_type = index_type_;
		using float_index_type = double;
		using calc_type = calc_type_;
		using range_type = range_axis<T, grid_adjuster_, index_type_, calc_type_>;
	public:
		struct iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using pointer = value_type;
			using reference = value_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			index_type Pos;
			calc_type a;
			calc_type b;
		public:
			iterator()noexcept :Pos(0), a(0.0), b(0.0) {}
			iterator(const iterator&) = default;
			iterator& operator=(const iterator&) = default;
			iterator(iterator&&) = default;
			iterator& operator=(iterator&&) = default;
			iterator(index_type Pos_, calc_type a_, calc_type b_): Pos(Pos_), a(a_), b(b_) {}
		public:
			value_type operator*()const { return a*Pos+b; ; }
			value_type operator[](difference_type Val)const { return a*(Pos+Val)+b; }
			iterator& operator++() {
				++Pos; 
				return *this;
			}
			iterator operator++(int) {
				iterator Itr = *this;
				++(*this);
				return Itr;
			}
			iterator& operator--() {
				--Pos;
				return *this;
			}
			iterator operator--(int) {
				iterator Itr = *this;
				--(*this);
				return Itr;
			}
			iterator& operator+=(difference_type Val) {
				Pos += Val;
				return *this;
			}
			iterator& operator-=(difference_type Val) {
				Pos -= Val;
				return *this;
			}
			friend iterator operator+(const iterator& Itr, difference_type Val) {
				iterator Ans(Itr);
				Ans += Val;
				return Ans;
			}
			friend iterator operator+(difference_type Val, const iterator& Itr) {
				return Itr + Val;
			}
			friend iterator operator-(const iterator& Itr, difference_type Val) {
				iterator Ans(Itr);
				Ans -= Val;
				return Ans;
			}
			friend difference_type operator-(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos - Itr2.Pos;
			}
			friend bool operator==(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos == Itr2.Pos;
			}
			friend bool operator!=(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos != Itr2.Pos;
			}
			friend bool operator<=(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos <= Itr2.Pos;
			}
			friend bool operator<(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos < Itr2.Pos;
			}
			friend bool operator>=(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos >= Itr2.Pos;
			}
			friend bool operator>(const iterator& Itr1, const iterator& Itr2) {
				return Itr1.Pos > Itr2.Pos;
			}
		public:
			index_type index()const { return Pos; }
		};
		using weighted_index_range = math::weighted_index_range<index_type>;
	public:
		axis():a(0), b(0){}
		axis(value_type Interval_, value_type Origin_):a(static_cast<calc_type>(Interval_)), b(static_cast<calc_type>(Origin_)){}
		axis(const this_type&) = default;
		axis& operator=(const this_type& Other) = default;
		axis(this_type&&) = default;
		axis& operator=(this_type&&) = default;
	public:
		//Val == a*Index + b
		value_type float_at(float_index_type FIndex)const { return a*FIndex+b; }
		//Index == (Val-b)/a
		float_index_type float_index(value_type Val)const { return static_cast<float_index_type>((Val-b)/a);}
	public:
		void assign(value_type Interval_, value_type Origin_) {
			//a == (Upper-Lower)/(Size - 1)
			//b == Lower
			a = static_cast<calc_type>(Interval_);
			b = static_cast<calc_type>(Origin_);
		}
		value_type operator[](index_type Index)const {return float_at(Index);}
		value_type at(index_type Index)const {return operator[](Index);}
		index_type index(value_type Val)const {return grid_adjuster::template index_cast<index_type>(float_index(Val));}
		index_type max_index()const { return std::numeric_limits<index_type>::max(); }
		index_type min_index()const { return std::numeric_limits<index_type>::min(); }
		weighted_index_range weighted_index(value_type LowerVal, value_type UpperVal)const {
			if(LowerVal > UpperVal)std::swap(LowerVal, UpperVal);
			auto CIndex = grid_adjuster::castable_index_range(float_index(LowerVal), float_index(UpperVal));
			return weighted_index_range(CIndex.first, CIndex.second);
		}
	public:
		iterator itr_at(index_type Index_)const { return iterator(0, a, b); }
	public:
		value_type interval()const { return static_cast<value_type>(a); }
		value_type origin()const { return static_cast<value_type>(b); }
		value_type grid_lower_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).first; }
		value_type grid_upper_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).second;}
		bool inside(value_type Value)const { return true; }
		bool inside(value_type LowerVal, value_type UpperVal)const { return true; }
	public:
		range_type range(index_type LowerIndex, index_type UpperIndex) {
			return range_type(operator[](LowerIndex), operator[](UpperIndex), UpperIndex - LowerIndex + 1);
		}
		template<typename to_axis>
		auto map_to(const to_axis& to) {
			return map_axis(*this, to);
		}
	public:
		friend bool operator==(const this_type& axis1, const this_type& axis2) {
			return hmLib::are_equal(axis1.a, axis2.a) && hmLib::are_equal(axis1.b, axis2.b);
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	private:
		calc_type a;
		calc_type b;
	};

	template<typename T, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = int,
		typename calc_type_ = typename std::conditional<std::is_same<decltype(std::declval<T>()*std::declval<double>()),double>::value, double, T>::type
	>
	struct range_axis {
		private:
			using this_type = range_axis<T, grid_adjuster_, index_type_, calc_type_>;
			using base_type = axis<T, grid_adjuster_, index_type_, calc_type_>;
		public:
			using value_type = typename base_type::value_type;
			using grid_adjuster = typename base_type::grid_adjuster;
			using index_type = typename base_type::index_type;
			using float_index_type = typename base_type::float_index_type;
			using calc_type = typename base_type::calc_type;
			using size_type = std::size_t;
		public:
			using iterator = typename base_type::iterator;
			using weighted_index_range = typename base_type::weighted_index_range;
		public:
			range_axis():Axis(), Size(0) {}
			range_axis(value_type Lower_, value_type Upper_, size_type Size_)
				: Axis((static_cast<calc_type>(Upper_)-static_cast<calc_type>(Lower_))/(Size_ - 1), Lower_)
				, Size(Size_) {
			}
			range_axis(const this_type&) = default;
			range_axis& operator=(const this_type& Other) = default;
			range_axis(this_type&&) = default;
			range_axis& operator=(this_type&&) = default;
		public:
			//Val == a*Index + b
			value_type float_at(float_index_type FIndex)const { return Axis.float_at(FIndex); }
			//Index == (Val-b)/a
			float_index_type float_index(value_type Val)const { return Axis.float_index(Val); }
		public:
			void assign(value_type Lower_, value_type Upper_, size_type Size_) {
				//a == (Upper-Lower)/(Size - 1)
				//b == Lower
				Axis.assign(static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1), Lower_);
				Size = Size_;
			}
			void clear() { Size = 0; }
			bool empty()const { return Size == 0; }
			size_type size()const { return Size; }
			value_type operator[](index_type Index)const { return Axis[Index]; }
			value_type at(index_type Index)const {
				hmLib_assert(0<=Index && Index < static_cast<int>(Size), hmLib::access_exceptions::out_of_range_access, "Out of axis range access.");
				return operator[](Index);
			}
			index_type index(value_type Val)const {
				hmLib_assert(inside(Val), hmLib::numeric_exceptions::out_of_valuerange, "Requested value is out of [grid_lower, grid_upper).");
				return Axis.index(Val);
			}
			index_type max_index()const { return static_cast<index_type>(size()-1); }
			index_type min_index()const { return 0; }
			weighted_index_range weighted_index(value_type LowerVal, value_type UpperVal)const {
				if(LowerVal > UpperVal)std::swap(LowerVal, UpperVal);
				hmLib_assert(inside(LowerVal, UpperVal), hmLib::numeric_exceptions::out_of_valuerange, "Requested value range is out of [grid_lower, grid_upper).");
				return Axis.weighted_index(LowerVal, UpperVal);
			}
		public:
			iterator begin()const { return Axis.itr_at(0); }
			iterator end()const { return Axis.itr_at(Size); }
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		public:
			value_type interval()const { return Axis.interval(); }
			value_type origin()const { return Axis.origin(); }
			value_type width()const { return Axis.interval()*(size()-1); }
			value_type lower()const { return Axis[0]; }
			value_type upper()const { return Axis[static_cast<index_type>(size()-1)]; }
			value_type grid_width()const { return grid_upper() - grid_lower(); }
			value_type grid_lower()const { return grid_lower_at(0); }
			value_type grid_upper()const { return grid_upper_at(static_cast<int>(size())-1); }
			value_type grid_lower_at(index_type Index)const { return Axis.grid_lower_at(Index); }
			value_type grid_upper_at(index_type Index)const { return Axis.grid_upper_at(Index); }
			bool inside(value_type Value)const { return grid_lower() <= Value && Value < grid_upper(); }
			bool inside(value_type LowerVal, value_type UpperVal)const { return grid_lower() <= LowerVal && UpperVal < grid_upper(); }
		public:
			this_type range(index_type LowerIndex, index_type UpperIndex) {
				return this_type(operator[](LowerIndex), operator[](UpperIndex), UpperIndex - LowerIndex + 1);
			}
			template<typename to_axis>
			auto map_to(const to_axis& to) {
				return map_axis(*this, to);
			}
		public:
			friend bool operator==(const this_type& axis1, const this_type& axis2) {
				return axis1.Size==axis2.Size && axis1.Axis == axis2.Axis;
			}
			friend bool operator!=(const this_type& axis1, const this_type& axis2) {
				return !(axis1==axis2);
			}
		private:
			base_type Axis;
			size_type Size;
	};

	namespace math {
		enum  class range_axis_option { none, gridfit, exclude_upper_boundary, exclude_lower_boundary, exclude_boundary };
	}
	template<typename T,typename grid_adjuster>
	auto make_range_axis(T Lower, T Upper, std::size_t Size, grid_adjuster GridAdjuster, math::range_axis_option Opt = math::range_axis_option::none) {
		using value_type = typename std::decay<T>::type;

		switch(Opt) {
		case math::range_axis_option::none:
			return range_axis<value_type, grid_adjuster>(Lower, Upper, Size);
		case math::range_axis_option::exclude_upper_boundary:
			return range_axis<value_type, grid_adjuster>(
				Lower,
				range_axis<value_type, grid_adjuster>(Lower, Upper, Size+1)[static_cast<int>(Size)-1],
				Size
			);
		case math::range_axis_option::exclude_lower_boundary:
			return range_axis<value_type, grid_adjuster>(
				range_axis<value_type, grid_adjuster>(Lower, Upper, Size+1)[1],
				Upper,
				Size
			);
		case math::range_axis_option::exclude_boundary:
			return range_axis<value_type, grid_adjuster>(
				range_axis<value_type, grid_adjuster>(Lower, Upper, Size+2)[1],
				range_axis<value_type, grid_adjuster>(Lower, Upper, Size+2)[static_cast<int>(Size)],
				Size
			);
		case math::range_axis_option::gridfit:
			Lower -= static_cast<T>((Upper-Lower)*grid_adjuster::index_range(0).first / Size);
			Upper += static_cast<T>((Upper-Lower)*grid_adjuster::index_range(0).second / Size);

			return make_range_axis(Lower, Upper, Size, GridAdjuster);
		default:
			return range_axis<value_type, grid_adjuster>(Lower, Upper, Size);
		}
	}
	template<typename T>
	auto make_range_axis(T Lower, T Upper, std::size_t Size, math::range_axis_option Opt = math::range_axis_option::none) {
		return make_range_axis(Lower, Upper, Size, math::default_grid_adjuster(), Opt);
	}

	template<typename from_grid_adjuster_, typename to_grid_adjuster_, typename index_type_, typename index_calc_type_ = double>
	struct axis_mapper {
	public:
		using index_type = index_type_;
		using float_index_type = double;
		using weighted_index_range = math::weighted_index_range<index_type>;
		using index_calc_type = index_calc_type_;
	private:
		using from_grid_adjuster = from_grid_adjuster_;
		using to_grid_adjuster = to_grid_adjuster_;
	private:
		//to = a*from + b
		//from = (to - b)/a
		index_calc_type a;
		index_calc_type b;
		index_type LowerFromIndex;
		index_type UpperFromIndex;
	public:
		axis_mapper() = delete;
		template<typename from_axis_type, typename to_axis_type>
		axis_mapper(const from_axis_type& from, const to_axis_type& to) {
			a = static_cast<index_calc_type>(from.interval()) / static_cast<index_calc_type>(to.interval());
			b = static_cast<index_calc_type>(from.origin() - to.origin())/ static_cast<index_calc_type>(to.interval());

			float_index_type LowerToFIndex = to_grid_adjuster::index_range(to.min_index()).first;
			float_index_type UpperToFIndex = to_grid_adjuster::index_range(to.max_index()).second;
			float_index_type LowerFromFIndex = (LowerToFIndex-b)/a;
			float_index_type UpperFromFIndex = (UpperToFIndex-b)/a;

			//condition for no violation of the out of range access
			LowerFromIndex = std::max(static_cast<index_type>(from.min_index()), static_cast<index_type>(std::round(std::ceil(LowerFromFIndex - from_grid_adjuster::index_range(0).first ))));
			UpperFromIndex = std::min(static_cast<index_type>(from.max_index()), static_cast<index_type>(std::round(std::floor(UpperFromFIndex- from_grid_adjuster::index_range(0).second))));
		}
	public:
		index_type lower()const { return LowerFromIndex; }
		index_type upper()const { return UpperFromIndex; }
		bool inside(float_index_type FromFIndex)const { return LowerFromIndex<=FromFIndex && FromFIndex<=UpperFromIndex; }
		index_type operator[](float_index_type FromFIndex)const {
			return to_grid_adjuster::template index_cast<index_type>(float_index(FromFIndex));
		}
		index_type index(float_index_type FromFIndex)const {
			hmLib_assert(inside(FromFIndex), hmLib::numeric_exceptions::out_of_valuerange, "Requested value "+std::to_string(FromFIndex) + " is out of ["+std::to_string(LowerFromIndex)+", "+std::to_string(UpperFromIndex)+"]");
			return to_grid_adjuster::template index_cast<index_type>(float_index(FromFIndex));
		}
		float_index_type float_index(float_index_type FromFIndex)const {
			return a*FromFIndex + b;
		}
		weighted_index_range weighted_index(index_type FromIndex)const {
			hmLib_assert(inside(FromIndex), hmLib::numeric_exceptions::out_of_valuerange, "Requested value "+std::to_string(FromIndex) + " is out of ["+std::to_string(LowerFromIndex)+", "+std::to_string(UpperFromIndex)+"]");

			auto FIndexRange = from_grid_adjuster::index_range(FromIndex);

			float_index_type LowerFIndex = float_index(FIndexRange.first);
			float_index_type UpperFIndex = float_index(FIndexRange.second);

			if(LowerFIndex > UpperFIndex)std::swap(LowerFIndex, UpperFIndex);
			auto CIndex = to_grid_adjuster::castable_index_range(LowerFIndex, UpperFIndex);
			return weighted_index_range(CIndex.first, CIndex.second);
		}
	};
	template<typename from_axis, typename to_axis>
	auto map_axis(const from_axis& from, const to_axis& to) {
		using from_grid_adjuster = typename from_axis::grid_adjuster;
		using to_grid_adjuster = typename to_axis::grid_adjuster;
		using index_type = typename from_axis::index_type;

		return axis_mapper<from_grid_adjuster, to_grid_adjuster, index_type>(from, to);
	}
}
#
#endif
