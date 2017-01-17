#ifndef HMLIB_ALGORITHM_NUMERIC_INC
#define HMLIB_ALGORITHM_NUMERIC_INC 100
#
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
}
#
#endif
