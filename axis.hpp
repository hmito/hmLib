#ifndef HMLIB_AXIS_INC
#define HMLIB_AXIS_INC 100
#
#include<utility>
#include<iterator>
#include<cmath>
#include"exceptions.hpp"
namespace hmLib {
	template<typename T>
	struct axis {
	private:
		using this_type = axis<T>;
	public:
		using value_type = T;
		using difference_type = decltype(std::declval<T>() - std::declval<T>());
		using index_type = unsigned long long;
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
			iterator& operator++() { ++Pos; }
			iterator operator++(int) {
				iterator Itr = *this;
				++(*this);
				return Itr;
			}
			iterator& operator--() { --Pos; }
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
		struct index_translator {
			double a;
			double b;
		public:
			index_translator() :a(1.0), b(0.0) {}
			index_translator(const index_translator&) = default;
			index_translator& operator=(const index_translator&) = default;
			index_translator(const this_type& from, const this_type& to) {
				a = static_cast<double>(from.Max - from.Min) / (to.Max - to.Min) * (from.size() - 1) / (to.size() - 1);
				b = (from.Min - to.Min) / (to.Max - to.Min);
			}
		public:
			double index_float(index_type from) {
				return a*from + b;
			}
			index_type round(index_type from) {
				return static_cast<index_type>(std::round(index_float(from)));
			}
			index_type ceil(index_type from) {
				return static_cast<index_type>(std::ceil(index_float()));
			}
			index_type floor(index_type from) {
				return static_cast<index_type>(std::floor(index_float()));
			}
		};
	public:
		value_type min()const { return Min; }
		value_type max()const { return Max; }
		difference_type width()const { return Max - Min; }
		difference_type grid_width()const { return width() / (size()-1); }
		axis<T> subaxis(index_type MinIndex, index_type MaxIndex) {
			return axis<T>(operator[](MinIndex), operator[](MaxIndex), MaxIndex - MinIndex + 1);
		}
	public:
		index_type size()const { return Size; }
		bool empty()const { return Size == 0; }
		value_type operator[](index_type Index)const {
			return (Min*(Size - 1 - Index) + Max*Index) / (Size - 1); 
		}
		value_type at(index_type Index)const {
			hmLib_assert(Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range.");
			return operator[](Index);
		}
	public:
		double find_float_index(value_type Val)const {
			return static_cast<double>(Size - 1)*((Val - Min) / (Max - Min));
		}
		index_type find_ceil_index(value_type Val)const {
			return static_cast<index_type>(std::ceil(find_float_index(Val)));
		}
		index_type find_floor_index(value_type Val)const {
			return static_cast<index_type>(std::floor(find_float_index(Val)));
		}
		index_type find_round_index(value_type Val)const {
			return static_cast<index_type>(std::round(find_float_index(Val)));
		}
		iterator find_ceil(value_type Val)const {
			index_type Index = find_ceil_index(Val);
			hmLib_assert(Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range.");
			return iterator(*this, Index);
		}
		iterator find_floor(value_type Val)const {
			index_type Index = find_floor_index(Val);
			hmLib_assert(Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range.");
			return iterator(*this, Index);
		}
		iterator find_round(value_type Val)const {
			index_type Index = find_round_index(Val);
			hmLib_assert(Index < Size, hmLib::access_exceptions::out_of_range_access, "Out of axis range.");
			return iterator(*this, Index);
		}
	public:
		axis() :Min(0), Max(0), Size(0) {}
		axis(value_type Min_, value_type Max_, index_type Size_) :Min(Min_), Max(Max_), Size(Size_) {}
		axis(const this_type&) = default;
		axis& operator=(const this_type&) = default;
		axis(this_type&&) = default;
		axis& operator=(this_type&&) = default;
	public:
		void assign(value_type Min_, value_type Max_, index_type Size_) {
			Min = Min_;
			Max = Max_;
			Size = Size_;
		}
		void clear() { Size = 0; }
		iterator begin()const { return iterator(*this, 0); }
		iterator end()const { return iterator(*this, Size); }
		iterator cbegin()const { return begin(); }
		iterator cend()const { return end(); }
	private:
		value_type Min;
		value_type Max;
		index_type Size;
	};
	template<typename T>
	typename axis<T>::index_translator make_axis_index_translator(const axis<T>& From, const axis<T>& To) {
		return typename axis<T>::index_translator(From, To);
	}
}
#
#endif
