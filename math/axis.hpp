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
		namespace detail {
			struct exclude_upper_boundary_tag {};
			struct exclude_lower_boundary_tag {};
			struct include_boundary_tag {};
			struct exclude_boundary_tag {};
		}
		namespace boundary {
			constexpr detail::exclude_upper_boundary_tag exclude_upper_boundary;
			constexpr detail::exclude_lower_boundary_tag exclude_lower_boundary;
			constexpr detail::include_boundary_tag include_boundary;
			constexpr detail::exclude_boundary_tag exclude_boundary;
		}

		namespace detail {
			struct floor_index_tag {};
			struct ceil_index_tag {};
			struct round_index_tag {};
		}
		namespace grid {
			constexpr detail::floor_index_tag floor_grid;
			constexpr detail::ceil_index_tag ceil_grid;
			constexpr detail::round_index_tag round_grid;
			template<typename grid_policy>
			struct grid_adjuster{};
		}
		namespace detail {

		}
		namespace detail {
			template<typename index_type, typename index_translate_tag>
			struct translate_traits {};
			template<typename index_type>
			struct translate_traits<index_type, detail::floor_index_tag> {
				static index_type index(double v) { return static_cast<index_type>(std::floor(v)); }
				static std::pair<double, double> range(index_type Index) { return std::pair<double, double>(Index+0.0,Index+1.0); }
			};
			template<typename index_type>
			struct translate_traits<index_type, detail::ceil_index_tag> {
				static index_type index(double v) { return static_cast<index_type>(std::ceil(v)); }
				static std::pair<double, double> range(index_type Index) { return std::pair<double, double>(Index-1.0, Index+0.0); }
			};
			template<typename index_type>
			struct translate_traits<index_type, detail::round_index_tag> {
				static index_type index(double v) { return static_cast<index_type>(std::round(v)); }
				static std::pair<double, double> range(index_type Index) { return std::pair<double, double>(Index-0.5, Index+0.5); }
			};
		}
	}
	template<typename T, typename grid_policy_, typename calc_type_ = T, typename index_type_ = unsigned int>
	struct axis {
	private:
		using this_type = axis<T, grid_policy_, calc_type_, index_type_>;
	public:
		using value_type = T;
		using calc_type = calc_type_;
		using grid_policy = grid_policy_;
		using difference_type = decltype(std::declval<T>() - std::declval<T>());
		using index_type = index_type_;
	private:
		using grid_adjuster = math::grid::grid_adjuster<grid_policy>;
	public:
		struct iterator {
		public:
			using value_type = typename this_type::value_type;
			using difference_type = int;
			using pointer = value_type;
			using reference = value_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			const this_type* pAxis;
			index_type Pos;
		public:
			iterator() :pAxis(0), Pos(0) {}
			iterator(const iterator&) = default;
			iterator& operator=(const iterator&) = default;
			iterator(iterator&&) = default;
			iterator& operator=(iterator&&) = default;
			iterator(const this_type& Axis_, index_type Pos_) : pAxis(&Axis_), Pos(Pos_) {}
		public:
			value_type operator*()const { return (*pAxis)[Pos]; }
			value_type operator[](difference_type Val)const { return (*pAxis)[Pos + Val]; }
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
		struct index_range {
			using index_and_proportion = std::pair<index_type, double>;
		private:
			index_type LowerIndex;
			index_type UpperIndex;
			double LowerProb;
			double MidProb;
			double UpperProb;
		public:
			struct iterator {
			public:
				using value_type = index_and_proportion;
				using difference_type = signed int;
				using reference = void;
				using pointer = void;
				using iterator_category = std::input_iterator_tag;
			private:
				index_type Index;
				const index_range* pRange;
			public:
				iterator() = default;
				iterator(index_type Index_, const index_range& Range_):Index(Index_), pRange(&Range_) {}
				value_type operator*()const {return (*pRange)[Index]; }
//				const value_type* operator->()const { return &(*pRange)[Index]; }
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
			index_range()noexcept : LowerIndex(1), UpperIndex(0), LowerProb(0.0), MidProb(0.0), UpperProb(0.0) {};
			explicit index_range(index_type Index_)
				: LowerIndex(Index_)
				, UpperIndex(Index_)
				, LowerProb(1.0)
				, MidProb(0.0)
				, UpperProb(0.0) {
			}
			index_range(index_type LowerIndex_, index_type UpperIndex_, double LowerProb_, double MidProb_, double UpperProb_)
				: LowerIndex(LowerIndex_)
				, UpperIndex(UpperIndex_)
				, LowerProb(LowerProb_)
				, MidProb(MidProb_)
				, UpperProb(UpperProb_) {
			}
			index_and_proportion operator[](index_type Index)const{
				if(Index==0) {
					return index_and_proportion(LowerIndex, LowerProb);
				} else if(LowerIndex+Index==UpperIndex) {
					return index_and_proportion(UpperIndex, UpperProb);
				}
				return index_and_proportion(Index+LowerIndex, MidProb);
			}
			index_and_proportion at(index_type Index)const{
				hmLib_assert(0<=Index && Index < size(), hmLib::access_exceptions::out_of_range_access, "Out of range access.");
				return operator[](Index);
			}
			bool empty()const { return LowerIndex>UpperIndex; }
			std::size_t size()const { return UpperIndex-LowerIndex+1; }
			iterator begin()const {
				return iterator(0, *this);
			}
			iterator end()const {
				return iterator(size(), *this);
			}
			iterator cbegin()const { return begin(); }
			iterator cend()const { return end(); }
		};
		template<typename grid_index_from_tag, typename grid_index_to_tag>
		struct index_translator {
		private:
			//to = a*from + b
			//from = (to - b)/a
			double a;
			double b;
			index_type UpperIndex;
			index_type LowerIndex;
		public:
			index_translator() :a(1.0), b(0.0) {}
			index_translator(const index_translator&) = default;
			index_translator& operator=(const index_translator&) = default;
			index_translator(const this_type& from, const this_type& to) {
				a = static_cast<double>(from.upper() - from.lower()) / (to.upper() - to.lower()) * (to.size() - 1) / (from.size() - 1);
				b = (from.lower() - to.lower()) / (to.upper() - to.lower())* (to.size() - 1);
			}
		public:
			double grid_float_at(double from)const {
				return a*from + b;
			}
			index_range grid_round_at(index_type Index)const{
				auto IndexRange = math::detail::translate_traits<index_type, index_translate_tag>::range(Index);
				double LowerFIndex = grid_float_index_at(IndexRange.first);
				double UpperFIndex = grid_float_index_at(IndexRange.second);

				if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

				index_type LowerIndex = static_cast<index_type>(std::ceil(LowerFIndex));
				index_type UpperIndex = static_cast<index_type>(std::ceil(UpperFIndex));
				if(LowerIndex == UpperIndex) {
					return index_range(LowerIndex);
				}
				//out of range is now ignored.
				//It is not considered the case like LowerFIndex in negative or Upper index is larger than Size.
				return index_type(LowerIndex, UpperIndex, (LowerIndex - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex + 1.0)/(UpperFIndex-LowerFIndex));
			}
			index_range grid_ceil_at(index_type Index)const {
				auto IndexRange = math::detail::translate_traits<index_type, index_translate_tag>::range(Index);
				double LowerFIndex = grid_float_index_at(IndexRange.first);
				double UpperFIndex = grid_float_index_at(IndexRange.second);

				if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

				index_type LowerIndex = static_cast<index_type>(std::ceil(LowerFIndex));
				index_type UpperIndex = static_cast<index_type>(std::ceil(UpperFIndex));
				if(LowerIndex == UpperIndex) {
					return index_range(LowerIndex);
				}

				return index_type(LowerIndex, UpperIndex, (LowerIndex - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex + 1.0)/(UpperFIndex-LowerFIndex));
			}
			index_range grid_floor_at(index_type Index)const {
				auto IndexRange = math::detail::translate_traits<index_type, index_translate_tag>::range(Index);
				double LowerFIndex = grid_float_index_at(IndexRange.first);
				double UpperFIndex = grid_float_index_at(IndexRange.second);

				if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

				index_type LowerIndex = static_cast<index_type>(std::ceil(LowerFIndex));
				index_type UpperIndex = static_cast<index_type>(std::ceil(UpperFIndex));
				if(LowerIndex == UpperIndex) {
					return index_range(LowerIndex);
				}

				return index_type(LowerIndex, UpperIndex, (LowerIndex - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex + 1.0)/(UpperFIndex-LowerFIndex));
			}
		};
	public:
		value_type lower()const { return static_cast<value_type>(b); }
		value_type upper()const { return static_cast<value_type>(a*(size()-1)+b); }
		difference_type step()const { return static_cast<difference_type>(a); }
		difference_type width()const { return static_cast<difference_type>(a*(size()-1)); }
		value_type grid_lower()const {return lower()}
		difference_type grid_width()const { return static_cast<difference_type>(a); }
		axis<T, index_type, calc_type> subaxis(index_type LowerIndex, index_type UpperIndex) {
			return axis<T>(operator[](LowerIndex), operator[](UpperIndex), UpperIndex - LowerIndex + 1);
		}
	public:
		index_type size()const { return Size; }
		bool empty()const { return Size == 0; }
		value_type operator[](index_type Index)const {
			return a*Index + b;
			//a == (Upper-Lower)/(Size - 1)
			//b == Lower
			//return (Lower*(Size - 1 - Index) + Upper*Index) / (Size - 1);
		}
		value_type at(index_type Index)const {
			hmLib_assert(0<=Index && Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range access.");
			return operator[](Index);
		}
	public:
		double grid_float_index(value_type Val)const {
			return static_cast<double>((Val-b)/a);
			//Val == a*Index + b
			//			return static_cast<double>(Size - 1)*((Val - Lower) / (Upper - Lower));
		}
		index_type grid_round_index(value_type Val)const {
			return static_cast<index_type>(std::round(grid_float_index(Val)));
		}
		index_type grid_ceil_index(value_type Val)const {
			return static_cast<index_type>(std::ceil(grid_float_index(Val)));
		}
		index_type grid_floor_index(value_type Val)const {
			return static_cast<index_type>(std::floor(grid_float_index(Val)));
		}
		index_range grid_round_range(value_type LowerVal, value_type UpperVal)const {
			double LowerFIndex = grid_float_index(LowerVal);
			double UpperFIndex = grid_float_index(UpperVal);

			if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

			index_type LowerIndex = static_cast<index_type>(std::round(LowerFIndex));
			index_type UpperIndex = static_cast<index_type>(std::round(UpperFIndex));
			if(LowerIndex == UpperIndex) {
				return index_range(LowerIndex);
			}

			return index_range(LowerIndex, UpperIndex, (LowerIndex + 0.5 - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex + 0.5)/(UpperFIndex-LowerFIndex));
		}
		index_range grid_ceil_range(value_type LowerVal, value_type UpperVal)const{
			double LowerFIndex = grid_float_index(LowerVal);
			double UpperFIndex = grid_float_index(UpperVal);

			if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

			index_type LowerIndex = static_cast<index_type>(std::ceil(LowerFIndex));
			index_type UpperIndex = static_cast<index_type>(std::ceil(UpperFIndex));
			if(LowerIndex == UpperIndex) {
				return index_range(LowerIndex);
			}

			return index_range(LowerIndex, UpperIndex, (LowerIndex - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex + 1.0)/(UpperFIndex-LowerFIndex));
		}
		index_range grid_floor_range(value_type LowerVal, value_type UpperVal)const {
			double LowerFIndex = grid_float_index(LowerVal);
			double UpperFIndex = grid_float_index(UpperVal);

			if(LowerFIndex>UpperFIndex) std::swap(LowerFIndex, UpperFIndex);

			index_type LowerIndex = static_cast<index_type>(std::floor(LowerFIndex));
			index_type UpperIndex = static_cast<index_type>(std::floor(UpperFIndex));
			if(LowerIndex == UpperIndex) {
				return index_range(LowerIndex);
			}

			return index_range(LowerIndex, UpperIndex, (LowerIndex + 1.0 - LowerFIndex)/(UpperFIndex-LowerFIndex), 1.0/(UpperFIndex-LowerFIndex), (UpperFIndex - UpperIndex)/(UpperFIndex-LowerFIndex));
		}
		value_type grid_round(value_type Val)const {
			return at(grid_round_index(Val));
		}
		value_type grid_ceil(value_type Val)const {
			return at(grid_ceil_index(Val));
		}
		value_type grid_floor(value_type Val)const {
			return at(grid_floor_index(Val));
		}
	public:
		friend index_translator<math::detail::floor_index_tag, math::detail::floor_index_tag> axis_translate(const this_type& from, const this_type& to)const { return index_translator<math::detail::floor_index_tag, math::detail::floor_index_tag>(from, to); }
		template<typename grid_index_tag>
		friend index_translator<grid_index_tag, grid_index_tag> axis_translate(const this_type& from, const this_type& to, grid_index_tag)const { return index_translator<grid_index_tag, grid_index_tag>(from, to); }
		template<typename grid_index_from_tag, typename grid_idnex_to_tag>
		friend index_translator<grid_index_from_tag, grid_idnex_to_tag> axis_translate(const this_type& from, const this_type& to, grid_index_from_tag, grid_idnex_to_tag)const { return index_translator<grid_index_from_tag, grid_idnex_to_tag>(from, to); }
	public:
		axis() :a(0), b(0), Size(0) {}
		axis(value_type Lower_, value_type Upper_, index_type Size_) :a(static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1)), b(Lower_), Size(Size_) {}
		axis(const this_type&) = default;
		axis& operator=(const this_type&) = default;
		axis(this_type&&) = default;
		axis& operator=(this_type&&) = default;
	public:
		void assign(value_type Lower_, value_type Upper_, index_type Size_) {
			//a == (Upper-Lower)/(Size - 1)
			//b == Lower
			a = static_cast<calc_type>(Upper_-Lower_)/(Size_ - 1);
			b = Lower_;
			Size = Size_;
		}
		void clear() { Size = 0; }
		iterator begin()const { return iterator(*this, 0); }
		iterator end()const { return iterator(*this, Size); }
		iterator cbegin()const { return begin(); }
		iterator cend()const { return end(); }
	private:
		calc_type a;
		calc_type b;
		index_type Size;
	};
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size) {
		return make_axis(std::forward<T>(Lower), std::forward<T>(Upper), Size, math::include_boundary_tag());
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size, math::include_boundary_tag) {
		using value_type = typename std::decay<T>::type;
		using calc_type = typename std::conditional<std::is_integral<value_type>::value, double, value_type>::type;
		return axis<value_type, calc_type>(Lower, Upper, Size);
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size, math::exclude_upper_boundary_tag) {
		using value_type = typename std::decay<T>::type;
		using calc_type = typename std::conditional<std::is_integral<value_type>::value, double, value_type>::type;
		return axis<value_type, calc_type>(Lower, axis<value_type, calc_type>(Lower,Upper,Size+1)[Size-1], Size);
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size, math::exclude_lower_boundary_tag) {
		using value_type = typename std::decay<T>::type;
		using calc_type = typename std::conditional<std::is_integral<value_type>::value, double, value_type>::type;
		return axis<value_type, calc_type>(axis<value_type, calc_type>(Lower, Upper, Size+1)[1], Upper, Size);
	}
	template<typename T>
	auto make_axis(T Lower, T Upper, std::size_t Size, math::exclude_boundary_tag) {
		using value_type = typename std::decay<T>::type;
		using calc_type = typename std::conditional<std::is_integral<value_type>::value, double, value_type>::type;
		return axis<value_type, calc_type>(axis<value_type, calc_type>(Lower, Upper, Size+2)[1], axis<value_type, calc_type>(Lower, Upper, Size+2)[Size], Size);
	}
}
#
#endif
