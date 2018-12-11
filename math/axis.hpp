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
				, Weight(0.0) {
			}
			weighted_index_range(index_type Index_, double Weidth_)noexcept
				: LowerIndex(Index_)
				, EndIndex(Index_+1)
				, LowerProb(1.0)
				, UpperProb(0.0)
				, Weight(Weidth_) {
			}
			weighted_index_range(index_type LowerIndex_, index_type UpperIndex_, double LowerProb_, double UpperProb_, double Weight_, double index_threshold_)noexcept
				: LowerIndex(LowerIndex_)
				, EndIndex(UpperIndex_+1)
				, LowerProb(LowerProb_)
				, UpperProb(UpperProb_)
				, Weight(Weight_) {
				if(LowerProb<index_threshold_) {
					if(LowerIndex+2==EndIndex) {
						if(UpperProb<index_threshold_) {
							LowerIndex = 0;
							EndIndex = 0;
							LowerProb = 0.0;
							UpperProb = 0.0;
							Weight = 0.0;
						}
						LowerProb = UpperProb;
						UpperProb = 0.0;
						++LowerIndex;
					} else {
						LowerProb = 1.0;
						++LowerIndex;
					}
				}else if(UpperProb<index_threshold_) {
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
				hmLib_assert(0<=Index && Index < static_cast<int>(size()), hmLib::access_exceptions::out_of_range_access, "Out of range access.");
				return operator[](Index);
			}
			bool empty()const { return LowerIndex==EndIndex; }
			std::size_t size()const { return static_cast<std::size_t>(EndIndex-LowerIndex); }
			double weight()const { return Weight; }
			iterator begin()const {
				return iterator(0, LowerIndex, EndIndex, LowerProb, UpperProb);
			}
			iterator end()const {
				return iterator(static_cast<int>(size()), LowerIndex, EndIndex, LowerProb, UpperProb);
			}
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		};
	}
	template<typename T, typename grid_adjuster_ = math::default_grid_adjuster, typename index_type_ = int, 
		typename calc_type_ = typename std::conditional<
			std::is_same<
				decltype(std::declval<T>()*std::declval<double>()),
				double
			>::value,double,T
		>::type
	>
	struct axis {
	private:
		using this_type = axis<T, grid_adjuster_, index_type_, calc_type_>;
	public:
		using value_type = T;
		using grid_adjuster = grid_adjuster_;
		using index_type = index_type_;
		using float_index_type = double;
		using size_type = std::size_t;
		using calc_type = calc_type_;
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
		axis():a(0), b(0), Size(0) {}
		axis(value_type Lower_, value_type Upper_, size_type Size_):a((static_cast<calc_type>(Upper_)-static_cast<calc_type>(Lower_))/(Size_ - 1)), b(Lower_), Size(Size_) {}
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
		void assign(value_type Lower_, value_type Upper_, size_type Size_) {
			//a == (Upper-Lower)/(Size - 1)
			//b == Lower
			a = static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1);
			b = Lower_;
			Size = Size_;
		}
		void clear() { Size = 0; }
		bool empty()const { return Size == 0; }
		size_type size()const { return Size; }
		value_type operator[](index_type Index)const {return float_at(Index);}
		value_type at(index_type Index)const {
			hmLib_assert(0<=Index && Index < static_cast<int>(Size), hmLib::access_exceptions::out_of_range_access, "Out of axis range access.");
			return operator[](Index);
		}
		index_type index(value_type Val)const {
			hmLib_assert(inside(Val), hmLib::numeric_exceptions::out_of_valuerange, "Requested value is out of [grid_lower, grid_upper).");
			return grid_adjuster::template index_cast<index_type>(float_index(Val));
		}
		weighted_index_range weighted_index(value_type LowerVal, value_type UpperVal)const {
			if(LowerVal > UpperVal)std::swap(LowerVal, UpperVal);
			hmLib_assert(inside(LowerVal, UpperVal), hmLib::numeric_exceptions::out_of_valuerange, "Requested value range is out of [grid_lower, grid_upper).");

			float_index_type LowerFIndex = float_index(LowerVal);
			float_index_type UpperFIndex = float_index(UpperVal);

			index_type LowerIndex = grid_adjuster::template index_cast<index_type>(LowerFIndex);
			index_type UpperIndex = grid_adjuster::template index_cast<index_type>(UpperFIndex);
			if(LowerIndex == UpperIndex) {
				return weighted_index_range(LowerIndex,UpperFIndex-LowerFIndex);
			}

			return weighted_index_range(
				LowerIndex, UpperIndex, 
				grid_adjuster::index_range(LowerIndex).second - LowerFIndex, 
				UpperFIndex - grid_adjuster::index_range(UpperIndex).first,
				UpperFIndex-LowerFIndex,
				grid_adjuster::index_threshold()
			);
		}
	public:
		iterator begin()const { return iterator(0, a, b); }
		iterator end()const { return iterator(static_cast<int>(Size),a,b); }
		iterator cbegin()const { return begin(); }
		iterator cend()const { return end(); }
	public:
		value_type interval()const { return static_cast<value_type>(a); }
		value_type width()const { return static_cast<value_type>(a*(size()-1)); }
		value_type lower()const { return static_cast<value_type>(b); }
		value_type upper()const { return static_cast<value_type>(a*(size()-1)+b); }
		value_type grid_width()const { return grid_upper() - grid_lower(); }
		value_type grid_lower()const { return grid_lower_at(0); }
		value_type grid_upper()const { return grid_upper_at(static_cast<int>(size())-1); }
		value_type grid_lower_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).first; }
		value_type grid_upper_at(index_type Index)const { return grid_adjuster::value_range(at(Index), interval()).second;}
		bool inside(value_type Value)const { return grid_lower() <= Value && Value < grid_upper(); }
		bool inside(value_type LowerVal, value_type UpperVal)const { return grid_lower() <= LowerVal && UpperVal < grid_upper(); }
	public:
		this_type subaxis(index_type LowerIndex, index_type UpperIndex) {
			return this_type(operator[](LowerIndex), operator[](UpperIndex), UpperIndex - LowerIndex + 1);
		}
		template<typename to_axis>
		auto map_to(const to_axis& to) {
			return map_axis(*this, to);
		}
	public:
		friend bool operator==(const this_type& axis1, const this_type& axis2) {
			return axis1.Size==axis2.Size && hmLib::are_equal(axis1.a, axis2.a) && hmLib::are_equal(axis1.b, axis2.b);
		}
		friend bool operator!=(const this_type& axis1, const this_type& axis2) {
			return !(axis1==axis2);
		}
	private:
		calc_type a;
		calc_type b;
		size_type Size;
	};

	namespace math {
		enum  class make_axis_option { none, gridfit, exclude_upper_boundary, exclude_lower_boundary, exclude_boundary };
	}
	template<typename T,typename grid_adjuster>
	auto make_axis(T Lower, T Upper, std::size_t Size, grid_adjuster GridAdjuster, math::make_axis_option Opt = math::make_axis_option::none) {
		using value_type = typename std::decay<T>::type;

		switch(Opt) {
		case math::make_axis_option::none:
			return axis<value_type, grid_adjuster>(Lower, Upper, Size);
		case math::make_axis_option::exclude_upper_boundary:
			return axis<value_type, grid_adjuster>(
				Lower,
				axis<value_type, grid_adjuster>(Lower, Upper, Size+1)[static_cast<int>(Size)-1],
				Size
			);
		case math::make_axis_option::exclude_lower_boundary:
			return axis<value_type, grid_adjuster>(
				axis<value_type, grid_adjuster>(Lower, Upper, Size+1)[1],
				Upper,
				Size
			);
		case math::make_axis_option::exclude_boundary:
			return axis<value_type, grid_adjuster>(
				axis<value_type, grid_adjuster>(Lower, Upper, Size+2)[1],
				axis<value_type, grid_adjuster>(Lower, Upper, Size+2)[static_cast<int>(Size)],
				Size
			);
		case math::make_axis_option::gridfit:
			Lower -= static_cast<T>((Upper-Lower)*grid_adjuster::index_range(0).first / Size);
			Upper += static_cast<T>((Upper-Lower)*grid_adjuster::index_range(0).second / Size);

			return make_axis(Lower, Upper, Size, GridAdjuster);
		default:
			return axis<value_type, grid_adjuster>(Lower, Upper, Size);
		}
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size, math::make_axis_option Opt = math::make_axis_option::none) {
		return make_axis(Lower, Upper, Size, math::default_grid_adjuster(), Opt);
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
			a = static_cast<index_calc_type>(from.upper() - from.lower()) / static_cast<index_calc_type>((to.upper() - to.lower())) * (to.size() - 1) / (from.size() - 1);
			b = static_cast<index_calc_type>(from.lower() - to.lower()) / static_cast<index_calc_type>((to.upper() - to.lower()))* (to.size() - 1);

			float_index_type LowerToFIndex = to_grid_adjuster::index_range(0).first;
			float_index_type UpperToFIndex = to_grid_adjuster::index_range(to.size()-1).second;
			float_index_type LowerFromFIndex = (LowerToFIndex-b)/a;
			float_index_type UpperFromFIndex = (UpperToFIndex-b)/a;

			//condition for no violation of the out of range access
			LowerFromIndex = static_cast<index_type>(std::round(std::max(0.0, std::ceil(LowerFromFIndex - from_grid_adjuster::index_range(0).first))));
			UpperFromIndex = static_cast<index_type>(std::round(std::min(from.size()-1.0, std::floor(UpperFromFIndex- from_grid_adjuster::index_range(0).second))));
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

			index_type LowerIndex = to_grid_adjuster::template index_cast<index_type>(LowerFIndex+to_grid_adjuster::index_threshold());
			index_type UpperIndex = to_grid_adjuster::template index_cast<index_type>(UpperFIndex-to_grid_adjuster::index_threshold());
			if(LowerIndex == UpperIndex) {
				return weighted_index_range(LowerIndex, UpperFIndex-LowerFIndex);
			}

			return weighted_index_range(LowerIndex, UpperIndex,
				to_grid_adjuster::index_range(LowerIndex).second - LowerFIndex,
				UpperFIndex - to_grid_adjuster::index_range(UpperIndex).first,
				UpperFIndex-LowerFIndex,
				to_grid_adjuster::index_threshold()
			);
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
