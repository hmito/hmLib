#ifndef HMLIB_ITERATORS_INTEGERITERATOR_INC
#define HMLIB_ITERATORS_INTEGERITERATOR_INC 100
#
#include<utility>
#include<iterator>
namespace hmLib {
	template<typename T>
	struct integer_iterator {
	private:
		using this_type = integer_iterator<T>;
	public:
		using value_type = T;
		using difference_type = decltype(std::declval<value_type>() - std::declval<value_type>());
		using pointer = void;
		using reference = value_type;
		using iterator_category = std::random_access_iterator_tag;
	private:
		value_type Pos;
	public:
		integer_iterator() :Pos(0) {}
		integer_iterator(const this_type&) = default;
		integer_iterator& operator=(const this_type&) = default;
		integer_iterator(this_type&&) = default;
		integer_iterator& operator=(this_type&&) = default;
		integer_iterator(value_type Pos_) : Pos(Pos_) {}
	public:
		value_type operator*()const { return Pos; }
		value_type operator[](difference_type Val)const { return Pos; }
		this_type& operator++() { ++Pos; return *this; }
		this_type operator++(int) {
			this_type Itr = *this;
			++(*this);
			return Itr;
		}
		this_type& operator--() { --Pos; return *this; }
		this_type operator--(int) {
			this_type Itr = *this;
			--(*this);
			return Itr;
		}
		this_type& operator+=(difference_type Val) {
			Pos += Val;
			return *this;
		}
		this_type& operator-=(difference_type Val) {
			Pos -= Val;
			return *this;
		}
		friend this_type operator+(const this_type& Itr, difference_type Val) {
			this_type Ans(Itr);
			Ans += Val;
			return Ans;
		}
		friend this_type operator+(difference_type Val, const this_type& Itr) {
			return Itr + Val;
		}
		friend this_type operator-(const this_type& Itr, difference_type Val) {
			this_type Ans(Itr);
			Ans -= Val;
			return Ans;
		}
		friend difference_type operator-(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos - Itr2.Pos;
		}
		friend bool operator==(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos == Itr2.Pos;
		}
		friend bool operator!=(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos != Itr2.Pos;
		}
		friend bool operator<=(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos <= Itr2.Pos;
		}
		friend bool operator<(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos < Itr2.Pos;
		}
		friend bool operator>=(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos >= Itr2.Pos;
		}
		friend bool operator>(const this_type& Itr1, const this_type& Itr2) {
			return Itr1.Pos > Itr2.Pos;
		}
	public:
		value_type value()const { return Pos; }
	};

	template<typename T>
	auto make_integer_iterator(T Val_) { return integer_iterator<T>(Val_); }

	template<typename T>
	struct integer_range {
		using iterator = integer_iterator<T>;
	private:
		T Min;
		T Max;
	public:
		integer_range(T Min_, T Max_) :Min(Min_), Max(Max_) {}
		iterator begin()const { return iterator(Min); }
		iterator end()const { return iterator(Max + 1); }
		iterator cbegin()const { return begin(); }
		iterator cend()const { return end(); }
	};

	template<typename T>
	auto make_integer_range(T Min_, T Max_) { return integer_range<T>(Min_, Max_); }

	using int_iterator = integer_iterator<int>;
	using signed_int_iterator = integer_iterator<signed int>;
	using unsigned_int_iterator = integer_iterator<unsigned int>;

	using int_range = integer_range<int>;
	using signed_int_range = integer_range<signed int>;
	using unsigned_int_range = integer_range<unsigned int>;
}
#
#endif
