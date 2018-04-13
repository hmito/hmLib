#ifndef HMLIB_ALGORITHM_ALGORITHM_INC
#define HMLIB_ALGORITHM_ALGORITHM_INC 100
#
#include<utility>
#include<algorithm>
namespace hmLib{
	template<typename Iterator, typename Transform>
	std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>
	transform_max_element(Iterator Beg, Iterator End, Transform Fn){
		auto Max = Fn(*Beg);
		Iterator MaxItr = Beg;

		for(++Beg; Beg != End; ++Beg){
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
		auto Min = Fn(*Beg);
		Iterator MinItr = Beg;

		for(++Beg; Beg != End; ++Beg){
			auto Val = Fn(*Beg);
			if(Val < Min){
				Min = Val;
				MinItr = Beg;
			}
		}
		return std::make_pair(Min,MinItr);
	}
	template<typename Iterator, typename Transform>
	std::pair<
		std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>,
		std::pair<decltype(std::declval<Transform>()(*std::declval<Iterator>())), Iterator>
	>
	transform_minmax_element(Iterator Beg, Iterator End, Transform Fn){
		auto Min = Fn(*Beg);
		Iterator MinItr = Beg;
		auto Max = Fn(*Beg);
		Iterator MaxItr = Beg;

		for(++Beg; Beg != End; ++Beg){
			auto Val = Fn(*(Beg++));
			if(Val < Min){
				Min = Val;
				MinItr = Beg;
			} else if(Val > Max){
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
