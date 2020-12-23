#ifndef HMLIB_ALGORITHM_NUMERIC_INC
#define HMLIB_ALGORITHM_NUMERIC_INC 100
#
#include<algorithm>
#include"../utility.hpp"
#include"../type_traits.hpp"
namespace hmLib{
	template<typename InputIterator, typename OutputIterator, typename T>
	T partial_accumulate(InputIterator first, InputIterator last, OutputIterator out, T init){
		for(; first != last; ++first){
			init += *first;
			*(out++) = init;
		}
		return init;
	}
	template<typename InputIterator, typename OutputIterator, typename T, typename Op>
	T partial_accumulate(InputIterator first, InputIterator last, OutputIterator out, T init, Op op){
		for(; first != last; ++first){
			init = op(init, *first);
			*(out++) = init;
		}
		return init;
	}
	template<typename Container, typename T = decltype(*std::begin(std::declval<Container>()) * *std::begin(std::declval<Container>())), typename std::enable_if<hmLib::is_range<Container>::value>::type*& = hmLib::utility::enabler>
	T norm2(const Container & c, T ini = 0) {
		auto end = std::end(c);
		for(auto itr = std::begin(c); itr != end; ++itr) {
			ini += (*itr) * (*itr);
		}
		return ini;
	}
	template<typename Container, typename T = decltype(std::sqrt(*std::begin(std::declval<Container>())* *std::begin(std::declval<Container>()))), typename std::enable_if<hmLib::is_range<Container>::value>::type*& = hmLib::utility::enabler>
	T norm(const Container& c, T ini = 0) {
		return std::sqrt(norm2(c,ini));
	}
}
#
#endif
