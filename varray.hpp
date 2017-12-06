#ifndef HMLIB_VARRAY_INC
#define HMLIB_VARRAY_INC 100
#
#include<array>
#include<algorithm>
#include<numeric>
#include<initializer_list>
#include<type_traits>
#include"utility.hpp"
namespace hmLib {
	template<typename T, std::size_t N>
	struct varray {
	private:
		using this_type = varray<T, N>;
		using container = std::array<T, N>;
	public:
		using value_type = typename container::value_type;
		using reference = typename container::reference;
		using const_reference = typename container::const_reference;
		using pointer = typename container::pointer;
		using const_pointer = typename container::const_pointer;
		using size_type = typename container::size_type;
		using difference_type = typename container::difference_type;
	public:
		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;
		using reverse_iterator = typename container::reverse_iterator;
		using const_reverse_iterator = typename container::const_reverse_iterator;
	private:
		container Arr;
	public:
		varray() :Arr() {}
		varray(std::initializer_list<T> il) {
			std::copy(il.begin(), il.end(), Arr.begin());
		}
		explicit varray(const T& val) { Arr.fill(val); }
		template<typename U>
		varray(const varray<U,N>& other){
			std::copy(other.begin(), other.end(), Arr.begin());
		}
		template<typename U>
		this_type& operator=(const varray<U, N>& other) {
			if(&other!=this) {
				std::copy(other.begin(), other.end(), Arr.begin());
			}
			return *this;
		}
		template<typename U>
		varray(varray<U, N>&& other) {
			std::copy(other.begin(), other.end(), Arr.begin());
		}
		template<typename U>
		this_type& operator=(varray<U, N>&& other) {
			if(&other!=this) {
				std::copy(other.begin(), other.end(), Arr.begin());
			}
			return *this;
		}
	public:
		reference at(size_type n) { return Arr.at(n); }
		constexpr const_reference at(size_type n)const{ return Arr.at(n); }
		reference operator[](size_type n) { return Arr[n]; }
		constexpr const_reference operator[](size_type n)const { return Arr[n]; }
		reference front(){ return Arr.front(); }
		constexpr const_reference front() const{ return Arr.front(); }
		reference back() { return Arr.back(); }
		constexpr const_reference back() const { return Arr.back(); }
		T* data()noexcept { return Arr.data(); }
		const T* data()const noexcept { return Arr.data(); }
	public:
		iterator begin() noexcept{ return Arr.begin(); }
		const_iterator begin()const noexcept { return Arr.begin(); }
		const_iterator cbegin()const noexcept { return Arr.cbegin(); }
		iterator end() noexcept { return Arr.end(); }
		const_iterator end()const noexcept { return Arr.end(); }
		const_iterator cend()const noexcept { return Arr.cend(); }
		reverse_iterator rbegin() noexcept { return Arr.rbegin(); }
		const_reverse_iterator rbegin()const noexcept { return Arr.rbegin(); }
		const_reverse_iterator crbegin()const noexcept { return Arr.crbegin(); }
		reverse_iterator rend() noexcept { return Arr.rend(); }
		const_reverse_iterator rend()const noexcept { return Arr.rend(); }
		const_reverse_iterator crend()const noexcept { return Arr.crend(); }
	public:
		constexpr bool empty()const noexcept { return Arr.empty(); }
		constexpr size_type size()const noexcept { return Arr.size(); }
		constexpr size_type max_size()const noexcept { return Arr.max_size(); }
	public:
		void fill(const T& v) { Arr.fill(v); }
		void swap(this_type& other)noexcept { Arr.swap(other.Arr); }
	public:
		this_type operator+()const { return *this; }
		this_type operator-()const { 
			this_type Ans = *this;
			for(auto& v:*this) v = -v;
			return Ans;
		}
		template<typename U>
		this_type& operator+=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v += *(Beg++);
			return *this;
		}
		template<typename U>
		this_type& operator-=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v -= *(Beg++);
			return *this;
		}
		template<typename U>
		this_type& operator*=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v *= *(Beg++);
			return *this;
		}
		template<typename U>
		this_type& operator/=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v /= *(Beg++);
			return *this;
		}
		template<typename U, hmLib_static_restrict(std::is_arithmetic<U>::value)>
		this_type& operator+=(U val) { 
			for(auto& v:*this) v += val;
			return *this;
		}
		template<typename U, hmLib_static_restrict(std::is_arithmetic<U>::value)>
		this_type& operator-=(U val) {
			for(auto& v:*this) v -= val; 
			return *this;
		}
		template<typename U, hmLib_static_restrict(std::is_arithmetic<U>::value)>
		this_type& operator*=(U val) {
			for(auto& v:*this) v *= val; 
			return *this;
		}
		template<typename U, hmLib_static_restrict(std::is_arithmetic<U>::value)>
		this_type& operator/=(U val) {
			for(auto& v:*this) v /= val;
			return *this;
		}
	public:
		T sum()const{
			return std::accumulate(std::next(begin()), end(), front());
		}
		T min()const {
			return *std::min_element(begin(), end());
		}
		T max()const {
			return *std::max_element(begin(), end());
		}
		template<typename func>
		varray<decltype(std::declval<func>()(std::declval<T>())),N> apply(func Fn) {
			varray<decltype(std::declval<func>()(std::declval<T>())), N> Ans;
			std::transform(begin(), end(), Ans.begin(), std::forward<func>(Fn));
			return Ans;
		}
	};
	template<typename T, typename U, std::size_t N>
	bool operator==(const varray<T, N>& v1, const varray<U, N>& v2) { return v1==v2; }
	template<typename T, typename U, std::size_t N>
	bool operator!=(const varray<T, N>& v1, const varray<U, N>& v2) { return v1!=v2; }
	template<typename T, typename U, std::size_t N>
	bool operator<(const varray<T, N>& v1, const varray<U, N>& v2) { return v1<v2; }
	template<typename T, typename U, std::size_t N>
	bool operator<=(const varray<T, N>& v1, const varray<U, N>& v2) { return v1<=v2; }
	template<typename T, typename U, std::size_t N>
	bool operator>(const varray<T, N>& v1, const varray<U, N>& v2) { return v1>v2; }
	template<typename T, typename U, std::size_t N>
	bool operator>=(const varray<T, N>& v1, const varray<U, N>& v2) { return v1>=v2; }
	template<typename T, typename U, std::size_t N>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator+(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans += v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N>
	varray<decltype(std::declval<T>()-std::declval<U>()), N> operator-(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans -= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N>
	varray<decltype(std::declval<T>()-std::declval<U>()), N> operator*(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans *= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N>
	varray<decltype(std::declval<T>()-std::declval<U>()), N> operator/(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans /= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator+(const varray<T, N>& v1, U v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans += v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator+(U v2, const varray<T, N>& v1) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans += v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator-(const varray<T, N>& v1, U v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans -= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator-(U v2, const varray<T, N>& v1) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(-v1);
		Ans += v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator*(const varray<T, N>& v1, U v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans *= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator*(U v2, const varray<T, N>& v1) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans *= v2;
		return Ans;
	}
	template<typename T, typename U, std::size_t N, hmLib_static_restrict(std::is_arithmetic<U>::value)>
	varray<decltype(std::declval<T>()+std::declval<U>()), N> operator/(const varray<T, N>& v1, U v2) {
		varray<decltype(std::declval<T>()+std::declval<U>()), N> Ans(v1);
		Ans /= v2;
		return Ans;
	}
	template<typename T, std::size_t N>
	void swap(varray<T, N>& v1, varray<T, N>& v2)noexcept { std::swap(v1, v2); }
	template<typename T, std::size_t N>
	varray<decltype(std::sin(std::declval<T>())),N> sin(const varray<T, N>& v){
		varray<decltype(std::sin(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::sin(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::cos(std::declval<T>())), N> cos(const varray<T, N>& v) {
		varray<decltype(std::cos(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::cos(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::tan(std::declval<T>())), N> tan(const varray<T, N>& v) {
		varray<decltype(std::tan(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::tan(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::asin(std::declval<T>())), N> asin(const varray<T, N>& v) {
		varray<decltype(std::asin(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::asin(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::acos(std::declval<T>())), N> acos(const varray<T, N>& v) {
		varray<decltype(std::acos(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::acos(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::atan(std::declval<T>())), N> atan(const varray<T, N>& v) {
		varray<decltype(std::atan(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::atan(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::sinh(std::declval<T>())), N> sinh(const varray<T, N>& v) {
		varray<decltype(std::sinh(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::sinh(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::cosh(std::declval<T>())), N> cosh(const varray<T, N>& v) {
		varray<decltype(std::cosh(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::cosh(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::tanh(std::declval<T>())), N> tanh(const varray<T, N>& v) {
		varray<decltype(std::tanh(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::tanh(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::exp(std::declval<T>())), N> exp(const varray<T, N>& v) {
		varray<decltype(std::exp(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::exp(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::log(std::declval<T>())), N> log(const varray<T, N>& v) {
		varray<decltype(std::log(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::log(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::log10(std::declval<T>())), N> log10(const varray<T, N>& v) {
		varray<decltype(std::log10(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::log10(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::sqrt(std::declval<T>())), N> sqrt(const varray<T, N>& v) {
		varray<decltype(std::sqrt(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::sqrt(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	varray<decltype(std::abs(std::declval<T>())), N> abs(const varray<T, N>& v) {
		varray<decltype(std::abs(std::declval<T>())), N> Ans;
		auto itr = v.begin();
		for(auto& a:Ans)a = std::abs(*(itr++));
		return Ans;
	}
	template<typename T, std::size_t N>
	decltype(std::sqrt(std::declval<T>()*std::declval<T>())) norm(const varray<T, N>& v) {
		static_assert(N>0, "norm can be calculated only when N>0.");

		auto Ans = v.front()* v.front();
		for(auto itr = std::next(v.begin()); itr!=v.end(); ++itr) Ans += (*itr)*(*itr);
		return std::sqrt(Ans);
	}
	template<typename T, std::size_t N>
	decltype(std::declval<T>()*std::declval<T>()) norm2(const varray<T, N>& v) {
		static_assert(N>0, "norm can be calculated only when N>0.");

		auto Ans = v.front()* v.front();
		for(auto itr = std::next(v.begin()); itr!=v.end(); ++itr) Ans += (*itr)*(*itr);
		return Ans;
	}
}
#
#endif
