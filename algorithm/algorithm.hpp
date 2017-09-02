#ifndef HMLIB_ALGORITHM_ALGORITHM_INC
#define HMLIB_ALGORITHM_ALGORITHM_INC 100
#
#include<utility>
#include<algorithm>
namespace hmLib{
	template<typename Iterator, typename Transform>
	Iterator transform_max_element(Iterator Beg, Iterator End, Transform&& Fn){
		auto Max = Fn(*Beg);
		Iterator MaxItr = Beg;

		for(++Beg; Beg != End; ++Beg){
			auto Val = Fn(*(Beg++));
			if(Val > Max){
				Max = Val;
				MaxItr = Beg;
			}
		}
		return MaxItr;
	}
	template<typename Iterator, typename Transform>
	Iterator transform_min_element(Iterator Beg, Iterator End, Transform&& Fn){
		auto Min = Fn(*Beg);
		Iterator MinItr = Beg;

		for(++Beg; Beg != End; ++Beg){
			auto Val = Fn(*(Beg++));
			if(Val < Min){
				Min = Val;
				MinItr = Beg;
			}
		}

		return MinItr;
	}
	template<typename Iterator, typename Transform>
	std::pair<Iterator, Iterator> transform_minmax_element(Iterator Beg, Iterator End, Transform&& Fn){
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
		return std::make_pair(MinItr, MaxItr);
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
}
#
#endif
