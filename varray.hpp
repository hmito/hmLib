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
		static constexpr std::size_t static_size() { return N; }
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
		varray() = default;
		varray(std::initializer_list<T> il)noexcept {
			auto out = Arr.begin();

			auto itr = il.begin();

			while(out != Arr.end() && itr != il.end()) {
				*out = *itr;
				++out;
				++itr;
			}

		}
		explicit varray(const T& val) { Arr.fill(val); }
		template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
		explicit varray(const varray<U,N>& other){
			auto oitr = Arr.begin();
			for(auto itr = other.begin(); itr!= other.end(); ++itr, ++oitr) {
				*oitr = static_cast<T>(*itr);
			}
		}
/*		template<typename U, typename std::enable_if<std::is_convertible<U, T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator=(const varray<U, N>& other) {
			if(&other!=this) {
				std::copy(other.begin(), other.end(), Arr.begin());
			}
			return *this;
		}*/
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
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() + std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator+=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v += *(Beg++);
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() - std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator-=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v -= *(Beg++);
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() * std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator*=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v *= *(Beg++);
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() / std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator/=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v /= *(Beg++);
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() % std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator%=(const varray<U, N>& other) {
			auto Beg = other.begin();
			for(auto& v:*this) v %= *(Beg++);
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() + std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator+=(U val) {
			for(auto& v:*this) v += val;
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() - std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator-=(U val) {
			for(auto& v:*this) v -= val; 
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() * std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator*=(U val) {
			for(auto& v:*this) v *= val; 
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() / std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator/=(U val) {
			for(auto& v:*this) v /= val;
			return *this;
		}
		template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() % std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
		this_type& operator%=(U val) {
			for(auto& v:*this) v %= val;
			return *this;
		}
		template<typename U>
		operator varray<U, N>() {
			varray<U, N> Ans;
			auto oItr = Ans.begin();
			auto Itr = begin();
			auto End = end();
			for(; Itr!=End; ++Itr, ++oItr) {
				*(oItr) = *Itr;
			}
			return Ans;
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
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() + std::declval<U>())>
	varray<ans_type, N> operator+(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] + v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() - std::declval<U>())>
	varray<ans_type, N> operator-(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] - v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
	varray<ans_type, N> operator*(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] * v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() / std::declval<U>())>
	varray<ans_type, N> operator/(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] / v2[i];
		}
		return Ans;

	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() % std::declval<U>())>
	varray<ans_type, N> operator%(const varray<T, N>& v1, const varray<U, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] % v2[i];
		}
		return Ans;

	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() + std::declval<U>())>
	varray<ans_type, N> operator+(const varray<T, N>& v1, U v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] + v2;
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() + std::declval<U>())>
	varray<ans_type, N> operator+(U v1, const varray<T, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1 + v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() - std::declval<U>())>
	varray<ans_type, N> operator-(const varray<T, N>& v1, U v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] - v2;
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() - std::declval<U>())>
	varray<ans_type, N> operator-(U v1, const varray<T, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1 - v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
	varray<ans_type, N> operator*(const varray<T, N>& v1, U v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] * v2;
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
	varray<ans_type, N> operator*(U v1, const varray<T, N>& v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1 * v2[i];
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() / std::declval<U>())>
	varray<ans_type, N> operator/(const varray<T, N>& v1, U v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] / v2;
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename ans_type = decltype(std::declval<T>() % std::declval<U>())>
	varray<ans_type, N> operator%(const varray<T, N>& v1, U v2) {
		varray<ans_type, N> Ans;
		for(std::size_t i = 0; i < N; ++i) {
			Ans[i] = v1[i] % v2;
		}
		return Ans;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() == std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator==(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(unsigned int i = 0; i < N; ++i) {
			if(!(v1[i] == v2[i]))return false;
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() == std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator!=(const varray<T, N>& v1, const varray<U, N>& v2) { return !(v1 == v2); }
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator<(const varray<T, N>& v1, const varray<U, N>& v2) { 
		for(unsigned int i = 0; i < N; ++i) {
			if(!(v1[N-1-i] == v2[N-1-i]))return v1[N-1-i]<v2[N-1-i];
		}
		return false;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator<=(const varray<T, N>& v1, const varray<U, N>& v2) { 
		for(unsigned int i = 0; i < N; ++i) {
			if(!(v1[N-1-i] == v2[N-1-i]))return v1[N-1-i]<v2[N-1-i];
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator>(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(unsigned int i = 0; i < N; ++i) {
			if(!(v1[N-1-i] == v2[N-1-i]))return v1[N-1-i]>v2[N-1-i];
		}
		return false;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool operator>=(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(unsigned int i = 0; i < N; ++i) {
			if(!(v1[N-1-i] == v2[N-1-i]))return v1[N-1-i]>v2[N-1-i];
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool all_less_equal_than(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(std::size_t i = 0; i < v1.size(); ++i) {
			if(!(v1[i] <= v2[i]))return false;
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool all_less_than(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(std::size_t i = 0; i < v1.size(); ++i) {
			if(!(v1[i] < v2[i]))return false;
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool all_greater_equal_than(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(std::size_t i = 0; i < v1.size(); ++i) {
			if(!(v1[i] >= v2[i]))return false;
		}
		return true;
	}
	template<typename T, typename U, std::size_t N, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
	bool all_greater_than(const varray<T, N>& v1, const varray<U, N>& v2) {
		for(std::size_t i = 0; i < v1.size(); ++i) {
			if(!(v1[i] > v2[i]))return false;
		}
		return true;
	}
	template<typename T, std::size_t N>
	void swap(varray<T, N>& v1, varray<T, N>& v2)noexcept { v1.swap(v2); }
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

//std namespace:: only for swap
namespace std {
	template<typename T, std::size_t N>
	void swap(hmLib::varray<T, N>& v1, hmLib::varray<T, N>& v2) {
		v1.swap(v2);
	}
}
#
#endif
