#ifndef HMLIB_ALGORITHM_ALGORITHM_INC
#define HMLIB_ALGORITHM_ALGORITHM_INC 100
#
#include<utility>
#include<algorithm>
namespace hmLib{
	/*! @brief Remove elements from the given range with allowing change order of elements (value version).
	This is altenative faster algorithm of std::remove, but after applying this function the elements order can be changed.
	@param [in] Beg Begin iterator of the range
	@param [in] End End iterator of the range
	@param [in] Value The removing value from the range
	@return End of non-removed elemetns */
	template<typename bidirectional_iterator, typename type>
	bidirectional_iterator unstable_remove(bidirectional_iterator Beg, bidirectional_iterator End, const type& Value) {
		while(Beg!=End) {
			if(*Beg==Value) {
				--End;
				while(Beg!=End) {
					if(*End!=Value) {
						*Beg = std::move(*End);
						++Beg;
						break;
					} else {
						--End;
					}
				}
			} else {
				++Beg;
			}
		}
		return Beg;
	}
	/*! @brief Remove elements from the given range with allowing change order of elements (functor version).
	This is altenative faster algorithm of std::remove_if, but after applying this function the elements order can be changed.
	@param [in] Beg Begin iterator of the range
	@param [in] End End iterator of the range
	@param [in] Cond All elemtns are evaluated as Cond(*Beg). If true is returned, the element is removed from the range. 
	@return End of non-removed elemetns */
	template<typename bidirectional_iterator, typename condition>
	bidirectional_iterator unstable_remove_if(bidirectional_iterator Beg, bidirectional_iterator End, condition Cond) {
		while(Beg!=End) {
			if(Cond(*Beg)) {
				--End;
				while(Beg!=End) {
					if(!Cond(*End)) {
						*Beg = std::move(*End);
						++Beg;
						break;
					} else {
						--End;
					}
				}
			} else {
				++Beg;
			}
		}
		return Beg;
	}
	/*! @brief Remove elements from the given range by using swap function instead of move (value version).
	This is altenative algorithm of std::remove, but use std::swap for moving elements.
	This means that the removed elements can be still accessible.
	@param [in] Beg Begin iterator of the range
	@param [in] End End iterator of the range
	@param [in] Value The removing value from the range
	@return End of non-removed elemetns */
	template<typename forward_iterator, typename type>
	forward_iterator swap_remove(forward_iterator Beg, forward_iterator End, const type& Value) {
		for(auto Itr = Beg; Itr!=End; ++Itr) {
			if(*Itr!=Value) {
				if(Beg!=Itr)std::swap(*Beg, *Itr);
				++Beg;
			}
		}
		return Beg;
	}
	/*! @brief Remove elements from the given range by using swap function instead of move (functor version).
	This is altenative algorithm of std::remove_if, but use std::swap for moving elements.
	This means that the removed elements can be still accessible.
	@param [in] Beg Begin iterator of the range
	@param [in] End End iterator of the range
	@param [in] Cond All elemtns are evaluated as Cond(*Beg). If true is returned, the element is removed from the range.
	@return End of non-removed elemetns */
	template<typename forward_iterator, typename condition>
	forward_iterator swap_remove_if(forward_iterator Beg, forward_iterator End, condition Cond) {
		//Itr: condition checked element
		//Beg: end of remaining elements

		auto Itr = Beg;
		//Beg==Itr as long as Cond is false
		while(Itr!=End) {
			if(Cond(*Itr)){
				++Itr;
				break;
			}
			++Beg;
			++Itr;
		}
		//Beg!=Itr
		while(Itr!=End){
			if(!Cond(*Itr)){
				std::swap(*Beg, *Itr);
				++Beg;
			}
			++Itr;
		}
		return Beg;
	}

	template<typename Iterator, typename Transform>
	std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>
	transform_max_element(Iterator Beg, Iterator End, Transform Fn){
		auto Max = std::numeric_limits<decltype(std::declval<Transform>()(*std::declval<Iterator>()))>::lowest();
		Iterator MaxItr = End;

		for(; Beg != End; ++Beg){
			auto Val = Fn(*Beg);
			if(Val > Max){
				Max = Val;
				MaxItr = Beg;
			}
		}
		return std::make_pair(Max,MaxItr);
	}
	template<typename Iterator, typename Transform>
	std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>
	transform_min_element(Iterator Beg, Iterator End, Transform Fn){
		auto Min = std::numeric_limits<decltype(std::declval<Transform>()(*std::declval<Iterator>()))>::max();
		Iterator MinItr = End;

		for(; Beg != End; ++Beg){
			auto Val = Fn(*Beg);
			if(Val < Min){
				Min = Val;
				MinItr = Beg;
			}
		}
		return std::make_pair(Min,MinItr);
	}
	template<typename Iterator, typename Transform>
	std::pair<std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>,std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>>
	transform_minmax_element(Iterator Beg, Iterator End, Transform Fn){
		auto Min = std::numeric_limits<decltype(std::declval<Transform>()(*std::declval<Iterator>()))>::max();
		Iterator MinItr = End;
		auto Max = std::numeric_limits<decltype(std::declval<Transform>()(*std::declval<Iterator>()))>::lowest();
		Iterator MaxItr = End;

		for(; Beg != End; ++Beg) {
			auto Val = Fn(*Beg);
			if(Val < Min){
				Min = Val;
				MinItr = Beg;
			} 
			if(Val > Max){
				Max = Val;
				MaxItr = Beg;
			}
		}
		return std::make_pair(std::make_pair(Min,MinItr), std::make_pair(Max,MaxItr));
	}
	template<typename Iterator, typename Compare>
	std::pair<Iterator, Iterator> widest_equal_range(Iterator Beg, Iterator End, Compare&& Comp){
		auto Pair = std::make_pair(Beg, Beg);
		auto BPair = std::make_pair(End, End);
		unsigned int BDis = 0;

		while(Pair.second != End){
			Pair.first = Pair.second;
			Pair.second = std::upper_bound(Pair.first, End, *(Pair.first), Comp);
			unsigned int Dis = std::distance(Pair.first, Pair.second);

			if(BDis < Dis){
				BDis = Dis;
				BPair = Pair;
			}
		}

		return BPair;
	}
	template<typename Iterator>
	std::pair<Iterator, Iterator> widest_equal_range(Iterator Beg, Iterator End){
		auto Pair = std::make_pair(Beg, Beg);
		auto BPair = std::make_pair(End, End);
		unsigned int BDis = 0;

		while(Pair.second != End){
			Pair.first = Pair.second;
			Pair.second = std::upper_bound(Pair.first, End, *(Pair.first));
			unsigned int Dis = std::distance(Pair.first, Pair.second);

			if(BDis < Dis){
				BDis = Dis;
				BPair = Pair;
			}
		}

		return BPair;
	}

	template<typename T, typename output_iterator, typename internal_cast_type = double>
	output_iterator generate_range(T lower, T upper, unsigned int n, output_iterator out) {
		for(unsigned int i = 0; i < n; ++i) {
			*(out++) = static_cast<internal_cast_type>(lower*(n - 1 - i) + upper *i) / (n - 1);
		}
		return out;
	}
}
#
#endif
