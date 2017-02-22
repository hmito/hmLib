#include<memory>
#include<algorithm>
#include<iostream>
#include<vector>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/enumerators.hpp>
#include<chrono>
#include<boost/range/any_range.hpp>
#include<boost/range/algorithm.hpp>
#include<boost/range/numeric.hpp>
template<typename iterator>
int func_itr(iterator Beg, iterator End){
	//重複を除いて
	End = std::unique(Beg, End);
	//奇数を消して
	std::remove_if(Beg, End, [](int val){return val % 2 != 0; });
	//ソートして
	std::sort(Beg, End);

	//先頭からの合計が10を超える最初の値を返す
	int pos = 0;
	for(; Beg != End; ++Beg){
		pos += *Beg;
		if(pos > 10)return *Beg;
	}

	//失敗したら0を返す
	return 0;
}
namespace henum = hmLib::enumerators;
using my_enumerator = hmLib::forward_enumerator<int, henum::unique_ability,henum::remove_ability,henum::sort_ability>;
template<typename iterator>
int func_enm(iterator Beg, iterator End){
	return func_enm(my_enumerator::make_enumerator(Beg, End));
}
int func_enm(my_enumerator& Enm){
	//重複を除いて
	Enm.unique();
	//奇数を消して
	Enm.remove_if([](int val){return val % 2 != 0; });
	//ソートして
	Enm.sort();

	//先頭からの合計が10を超える最初の値を返す
	int pos = 0;
	for(; Enm; ++Enm){
		pos += *Enm;
		if(pos > 10)return *Enm;
	}

	//失敗したら0を返す
	return 0;
}

int main_enumerator(void){
	std::vector<int> Array{9,2,6,3,5,5,3,3,1,4,6,8,9,2,2};

	system("pause");
	return 0;
}
