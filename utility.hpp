#ifndef HMLIB_UTILITY_INC
#define HMLIB_UTILITY_INC 100
#
#include<utility>
namespace hmLib{
	namespace utility{
		extern void* enabler;
	}
}
#define hmLib_static_restrict(condition) typename std::enable_if<condition>::type*& = hmLib::utility::enabler
namespace hmLib{
	template<typename T, typename U>
	T exchange(T& obj, U&& new_val) {
		T old_val = std::move(obj);
		obj = std::forward<U>(new_val);
		return old_val;
	}

	//static size wrapper
	namespace detail{
		template<typename container>
		struct static_size_impl{
			static constexpr std::size_t static_size(){return  container::static_size();}
		};
		template<typename T, std::size_t N>
		struct static_size_impl<std::array<T, N>>{
			static constexpr std::size_t static_size(){return N;}
		};
		template<typename T, std::size_t N>
		struct static_size_impl<T[N]>{
			static constexpr std::size_t static_size(){return N;}
		};
	}
	//static size generalized for std::array and raw array types.
	template<typename container>
	constexpr std::size_t static_size(){
		return detail::static_size_impl<container>::static_size();
	}
	template<typename T>
	inline bool are_equal(T v1, T v2, double tolerance = 1e-8) {
		return v1+tolerance >= v2 && v2+tolerance >= v1;
	}

	template<typename T>
	void hash_combine(size_t& seed, const T& v) {
		seed ^= hash_value(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}
#
#endif
