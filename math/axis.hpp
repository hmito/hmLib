#ifndef HMLIB_MATH_AXIS_INC
#define HMLIB_MATH_AXIS_INC 100
#
#include<utility>
#include<iterator>
#include<type_traits>
#include<cmath>
#include"../exceptions.hpp"
namespace hmLib {
	namespace math {
		namespace grid_policy {
			constexpr struct round_grid_tag {} round_grid;
			constexpr struct floor_grid_tag {} floor_grid;
			constexpr struct ceil_grid_tag {} ceil_grid;
			namespace detail {
				template<typename grid_policy, int log10_index_threshold>
				struct grid_adjuster {};
				template<int log10_index_threshold>
				struct grid_adjuster<round_grid_tag, log10_index_threshold> {
					inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
					template<typename index_type>
					static index_type index_cast(double index_) { return static_cast<index_type>(std::round(index_)); }
					template<typename index_type>
					static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index-0.5, Index+0.5); }
					template<typename value_type, typename difference_type>
					static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value-Interval/2.0, Value+Interval/2.0); }
				};
				template<int log10_index_threshold>
				struct grid_adjuster<floor_grid_tag, log10_index_threshold> {
					inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
					template<typename index_type>
					static index_type index_cast(double index_) { return static_cast<index_type>(std::floor(index_+index_threshold())); }
					template<typename index_type>
					static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index+0.0, Index+1.0); }
					template<typename value_type, typename difference_type>
					static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value, Value+Interval); }
				};
				template<int log10_index_threshold>
				struct grid_adjuster<ceil_grid_tag, log10_index_threshold> {
					inline static double index_threshold() { return std::pow(10, log10_index_threshold); }
					template<typename index_type>
					static index_type index_cast(double index_) { return static_cast<index_type>(std::ceil(index_-index_threshold())); }
					template<typename index_type>
					static std::pair<double, double> index_range(index_type Index) { return std::pair<double, double>(Index-1.0, Index+0.0); }
					template<typename value_type, typename difference_type>
					static std::pair<value_type, value_type> value_range(value_type Value, difference_type Interval) { return std::pair<value_type, value_type>(Value-Interval, Value); }
				};
			}
		}
		template<typename index_type, int log10_index_threshold = -8>
		struct weighted_index_range {
			using weighted_index = std::pair<index_type, double>;
		private:
			inline double index_threshold() { return std::pow(10, log10_index_threshold); }
		private:
			index_type LowerIndex;
			index_type EndIndex;
			double LowerProb;
			double UpperProb;
			double Weight;
		public:
			struct iterator {
			public:
				using value_type = weighted_index;
				using difference_type = signed int;
				using reference = value_type;
				using pointer = hmLib::clone_ptrproxy<value_type>;
				using iterator_category = std::input_iterator_tag;
			private:
				index_type Index;
				index_type LowerIndex;
				index_type EndIndex;
				double LowerProb;
				double UpperProb;
			public:
				iterator() = default;
				iterator(index_type Index_, index_type LowerIndex_, index_type EndIndex_, double LowerProb_, double UpperProb_)
					: Index(Index_) 
					, LowerIndex(LowerIndex_)
					, EndIndex(EndIndex_)
					, LowerProb(LowerProb_)
					, UpperProb(UpperProb_){
				}
				reference operator*()const {
					if(Index==0) {
						return weighted_index(LowerIndex, LowerProb);
					} else if(LowerIndex+Index==EndIndex-1) {
						return weighted_index(EndIndex-1, UpperProb);
					}
					return weighted_index(Index+LowerIndex, 1.0);
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
		public:
			weighted_index_range()noexcept
				: LowerIndex(0)
				, EndIndex(0)
				, LowerProb(0.0)
				, UpperProb(0.0)
				, Weidth(0.0) {
			}
			weighted_index_range(index_type Index_, double Weidth_)noexcept
				: LowerIndex(Index_)
				, EndIndex(Index_+1)
				, LowerProb(1.0)
				, UpperProb(0.0)
				, Weight(Weidth_) {
			}
			weighted_index_range(index_type LowerIndex_, index_type UpperIndex_, double LowerProb_, double UpperProb_, double Weidth_)noexcept
				: LowerIndex(LowerIndex_)
				, EndIndex(UpperIndex_+1)
				, LowerProb(LowerProb_)
				, UpperProb(UpperProb_)
				, Weidth(Weidth_) {
				if(LowerProb<index_threshold()) {
					if(LowerIndex+2==EndIndex) {
						if(UpperProb<index_threshold()) {
							LowerIndex = 0;
							EndIndex = 0;
							LowerProb = 0.0;
							UpperProb = 0.0;
							Weidth = 0.0;
						}
						LowerProb = UpperProb;
						UpperProb = 0.0;
						++LowerIndex;
					} else {
						LowerProb = 1.0;
						++LowerIndex;
					}
				}else if(UpperProb<index_threshold()) {
					if(LowerIndex+2==EndIndex) {
						UpperProb = 0.0;
						--EndIndex;
					} else {
						UpperProb = 1.0;
						--EndIndex;
					}
				}
			}
			weighted_index operator[](index_type Index)const {
				if(Index==0) {
					return weighted_index(LowerIndex, LowerProb);
				} else if(LowerIndex+Index==EndIndex-1) {
					return weighted_index(EndIndex-1, UpperProb);
				}
				return weighted_index(Index+LowerIndex, 1.0);
			}
			weighted_index at(index_type Index)const {
				hmLib_assert(0<=Index && Index < size(), hmLib::access_exceptions::out_of_range_access, "Out of range access.");
				return operator[](Index);
			}
			bool empty()const { return LowerIndex==EndIndex; }
			unsigned int size()const { return EndIndex-LowerIndex; }
			double weight()const { return Weight; }
			iterator begin()const {
				return iterator(0, LowerIndex, EndIndex, LowerProb, UpperProb);
			}
			iterator end()const {
				return iterator(size(), LowerIndex, EndIndex, LowerProb, UpperProb);
			}
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		};
	}
	template<typename T, typename grid_policy_ = math::grid_policy::round_grid_tag, typename index_type_ = unsigned int, typename calc_type_ = double, int log10_index_threshold_ = -8>
	struct axis {
	private:
		using this_type = axis<T, grid_policy_, index_type_, calc_type_, log10_index_threshold_>;
	public:
		using value_type = T;
		using calc_type = calc_type_;
		using grid_policy = grid_policy_;
		using difference_type = decltype(std::declval<T>() - std::declval<T>());
		using index_type = index_type_;
		static const int log10_index_threshold = log10_index_threshold_;
	private:
		using grid_adjuster = math::grid_policy::detail::grid_adjuster<grid_policy, log10_index_threshold>;
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
			double a;
			double b;
		public:
			iterator()noexcept :Pos(0), a(0.0), b(0.0) {}
			iterator(const iterator&) = default;
			iterator& operator=(const iterator&) = default;
			iterator(iterator&&) = default;
			iterator& operator=(iterator&&) = default;
			iterator(index_type Pos_, double a_, double b_): Pos(Pos_), a(a_), b(b_) {}
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
		using weighted_index_range = math::weighted_index_range<index_type, log10_index_threshold>;
	public:
		axis():a(0), b(0), Size(0) {}
		axis(value_type Lower_, value_type Upper_, index_type Size_):a(static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1)), b(Lower_), Size(Size_) {}
		axis(const this_type&) = default;
		axis& operator=(const this_type&) = default;
		axis(this_type&&) = default;
		axis& operator=(this_type&&) = default;
	public:
		//Val == a*Index + b
		value_type float_at(double FIndex)const { return a*FIndex+b; }
		//Index == (Val-b)/a
		double float_index(value_type Val)const { return static_cast<double>((Val-b)/a);}
	public:
		void assign(value_type Lower_, value_type Upper_, index_type Size_) {
			//a == (Upper-Lower)/(Size - 1)
			//b == Lower
			a = static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1);
			b = Lower_;
			Size = Size_;
		}
		void clear() { Size = 0; }
		index_type size()const { return Size; }
		bool empty()const { return Size == 0; }
		value_type operator[](index_type Index)const {return float_at(Index);}
		value_type at(index_type Index)const {
			hmLib_assert(0<=Index && Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range access.");
			return operator[](Index);
		}
		value_type lower()const { return static_cast<value_type>(b); }
		value_type upper()const { return static_cast<value_type>(a*(size()-1)+b); }
		iterator begin()const { return iterator(0, a, b); }
		iterator end()const { return iterator(Size,a,b); }
		iterator cbegin()const { return begin(); }
		iterator cend()const { return end(); }
	public:
		difference_type interval()const { return static_cast<difference_type>(a); }
		difference_type width()const { return static_cast<difference_type>(a*(size()-1)); }
		value_type grid_lower_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).first; }
		value_type grid_upper_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).second;}
		value_type grid_lower()const { return grid_lower_at(0); }
		value_type grid_upper()const { return grid_upper_at(size()-1); }
		bool inside(value_type Value)const { return grid_lower() <= Value && Value < grid_upper(); }
		bool inside(value_type LowerVal, value_type UpperVal)const { return grid_lower() <= LowerVal && UpperVal < grid_upper(); }
	public:
		friend bool operator==(const this_type& axis1, const this_type& axis2) {
			return axis1.Size==axis2.Size && hmLib::are_equal(axis1.a, axis2.a) && hmLib::are_equal(axis1.b, axis2.b);
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	public:
		index_type index(value_type Val)const {
			hmLib_assert(inside(Val), hmLib::numeric_exceptions::out_of_valuerange, "Requested value is out of [grid_lower, grid_upper).");
			return grid_adjuster::template index_cast<index_type>(float_index(Val));
		}
		weighted_index_range weighted_index(value_type LowerVal, value_type UpperVal)const {
			if(LowerVal > UpperVal)std::swap(LowerVal, UpperVal);
			hmLib_assert(inside(LowerVal, UpperVal), hmLib::numeric_exceptions::out_of_valuerange, "Requested value range is out of [grid_lower, grid_upper).");

			double LowerFIndex = float_index(LowerVal);
			double UpperFIndex = float_index(UpperVal);

			index_type LowerIndex = grid_adjuster::template index_cast<index_type>(LowerFIndex);
			index_type UpperIndex = grid_adjuster::template index_cast<index_type>(UpperFIndex);
			if(LowerIndex == UpperIndex) {
				return weighted_index_range(LowerIndex,UpperFIndex-LowerFIndex);
			}

			return weighted_index_range(
				LowerIndex, UpperIndex, 
				grid_adjuster::index_range(LowerIndex).second - LowerFIndex, 
				UpperFIndex - grid_adjuster::index_range(UpperIndex).first,
				UpperFIndex-LowerFIndex
			);
		}
	public:
		this_type subaxis(index_type LowerIndex, index_type UpperIndex) {
			return this_type(operator[](LowerIndex), operator[](UpperIndex), UpperIndex - LowerIndex + 1);
		}
	private:
		calc_type a;
		calc_type b;
		index_type Size;
	};

	template<typename from_grid_policy_, typename to_grid_policy_, typename index_type_ = unsigned int, int log10_index_threshold_ = -8>
	struct axis_mapper {
	public:
		using from_grid_policy = from_grid_policy_;
		using to_grid_policy = to_grid_policy_;
		using index_type = index_type_;
		using weighted_index_range = math::weighted_index_range<index_type, log10_index_threshold_>;
	private:
		using from_grid_adjuster = math::grid_policy::detail::grid_adjuster<from_grid_policy, log10_index_threshold_>;
		using to_grid_adjuster = math::grid_policy::detail::grid_adjuster<to_grid_policy, log10_index_threshold_>;
	private:
		//to = a*from + b
		//from = (to - b)/a
		double a;
		double b;
		index_type LowerFromIndex;
		index_type UpperFromIndex;
	public:
		axis_mapper() = delete;
		axis_mapper(const axis_mapper&) = default;
		axis_mapper& operator=(const axis_mapper&) = default;
		template<typename from_axis_type, typename to_axis_type>
		axis_mapper(const from_axis_type& from, const to_axis_type& to) {
			a = static_cast<double>(from.upper() - from.lower()) / (to.upper() - to.lower()) * (to.size() - 1) / (from.size() - 1);
			b = static_cast<double>(from.lower() - to.lower()) / (to.upper() - to.lower())* (to.size() - 1);

			double LowerToFIndex = to_grid_adjuster::index_range(0).first;
			double UpperToFIndex = to_grid_adjuster::index_range(to.size()-1).second;
			double LowerFromFIndex = (LowerToFIndex-b)/a;
			double UpperFromFIndex = (UpperToFIndex-b)/a;

			//condition for no violation of the out of range access
			LowerFromIndex = static_cast<index_type>(std::round(std::max(0.0,std::ceil(LowerFromFIndex - from_grid_adjuster::index_range(0).first))));
			UpperFromIndex = static_cast<index_type>(std::round(std::min(from.size()-1.0,std::floor(UpperFromFIndex- from_grid_adjuster::index_range(0).second))));
		}
	public:
		index_type lower()const { return LowerFromIndex; }
		index_type upper()const { return UpperFromIndex; }
		bool inside(double FromFIndex)const { return LowerFromIndex<=FromFIndex && FromFIndex<=UpperFromIndex; }
		index_type operator[](double FromFIndex)const {
			return to_grid_adjuster::template index_cast<index_type>(float_index(FromFIndex));
		}
		index_type index(double FromFIndex)const {
			hmLib_assert(inside(FromFIndex), hmLib::numeric_exceptions::out_of_valuerange, "Requested value is out of [grid_lower, grid_upper).");
			return to_grid_adjuster::template index_cast<index_type>(float_index(FromFIndex));
		}
		double float_index(double FromFIndex)const {
			return a*FromFIndex + b;
		}
		weighted_index_range weighted_index(index_type FromIndex)const {
			hmLib_assert(inside(FromIndex), hmLib::numeric_exceptions::out_of_valuerange, "Requested value range is out of [grid_lower, grid_upper).");

			auto FIndexRange = from_grid_adjuster::index_range(FromIndex);
			double LowerFIndex = float_index(FIndexRange.first);
			double UpperFIndex = float_index(FIndexRange.second);

			index_type LowerIndex = to_grid_adjuster::template index_cast<index_type>(LowerFIndex+to_grid_adjuster::index_threshold());
			index_type UpperIndex = to_grid_adjuster::template index_cast<index_type>(UpperFIndex-to_grid_adjuster::index_threshold());
			if(LowerIndex == UpperIndex) {
				return weighted_index_range(LowerIndex, UpperFIndex-LowerFIndex);
			}

			return weighted_index_range(LowerIndex, UpperIndex, 
				to_grid_adjuster::index_range(LowerIndex).second - LowerFIndex, 
				UpperFIndex - to_grid_adjuster::index_range(UpperIndex).first,
				UpperFIndex-LowerFIndex
			);
		}
	};
	
	template<typename from_axis, typename to_axis>
	auto map_axis(const from_axis& from, const to_axis& to) {
		using from_grid_policy = typename from_axis::grid_policy;
		using to_grid_policy = typename to_axis::grid_policy;
		using index_type = typename from_axis::index_type;

		return axis_mapper<from_grid_policy, to_grid_policy, index_type>(from, to);
	}
	namespace math {
		enum  class make_axis_option { none, gridfit, exclude_upper_boundary, exclude_lower_boundary, exclude_boundary };
	}
	template<typename T,typename grid_policy>
	auto make_axis(T Lower, T Upper, unsigned int Size, grid_policy GridPolicy, math::make_axis_option Opt = math::make_axis_option::none) {
		using value_type = typename std::decay<T>::type;

		switch(Opt) {
		case math::make_axis_option::none:
			return axis<value_type, grid_policy>(Lower, Upper, Size);
		case math::make_axis_option::exclude_upper_boundary:
			return axis<value_type, grid_policy>(
				Lower,
				axis<value_type, grid_policy>(Lower, Upper, Size+1)[Size-1],
				Size
			);
		case math::make_axis_option::exclude_lower_boundary:
			return axis<value_type, grid_policy>(
				axis<value_type, grid_policy>(Lower, Upper, Size+1)[1],
				Upper,
				Size
			);
		case math::make_axis_option::exclude_boundary:
			return axis<value_type, grid_policy>(
				axis<value_type, grid_policy>(Lower, Upper, Size+2)[1],
				axis<value_type, grid_policy>(Lower, Upper, Size+2)[Size],
				Size
			);
		case math::make_axis_option::gridfit:
			using grid_adjuster = math::grid_policy::detail::grid_adjuster<grid_policy,-8>;

			Lower -= (Upper-Lower)*grid_adjuster::index_range(0).first / Size;
			Upper += (Upper-Lower)*grid_adjuster::index_range(0).second / Size;

			return make_axis(Lower, Upper, Size, GridPolicy);
		default:
			return axis<value_type, grid_policy>(Lower, Upper, Size);
		}
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, unsigned int Size, math::make_axis_option Opt = math::make_axis_option::none) {
		return make_axis(Lower, Upper, Size, math::grid_policy::round_grid, Opt);
	}
}
#
#endif
