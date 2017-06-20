#ifndef HMLIB_ALGORITHM_SORT_INC
#define HMLIB_ALGORITHM_SORT_INC 100
#
#include<utility>
#include<algorithm>
namespace hmLib{
	namespace algorithm{
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
}
#
#endif
