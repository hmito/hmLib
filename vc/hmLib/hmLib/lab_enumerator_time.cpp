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
	End = std::unique(Beg, End);
	std::remove_if(Beg, End, [](int val){return val % 2 != 0; });
	std::sort(Beg, End);
	std::for_each(Beg, End, [](int val){return val % 2 != 0; });
	return std::accumulate(Beg, End,0);
}
template<typename iterator>
int func_itr2(iterator Beg, iterator End){
	int Ans = 0;
	for(iterator Itr=Beg; Itr != End; ++Itr){
		if(*Itr % 2 == 0){
			Ans += *Itr;
			*Itr += 1;
		}
	}
	for(iterator Itr = Beg; Itr != End; ++Itr){
		if(*Itr % 3 == 0){
			Ans -= *Itr;
			*Itr += 2;
		}
	}
	for(iterator Itr = Beg; Itr != End; ++Itr){
		if(*Itr % 5 == 0){
			Ans += *Itr / 5;
		}
	}

	return Ans;
}

namespace hmenum = hmLib::enumerators;
using this_enumerator = hmLib::enumerator<int,
	hmLib::mutable_range_enumerator_tag,
	hmenum::forward_traversal_ability, 
	hmenum::sort_ability,
	hmenum::shuffle_ability,
	hmenum::all_of_ability,
	hmenum::generate_ability,
	hmenum::fill_ability,
	hmenum::unique_ability,
	hmenum::remove_ability,
	hmenum::count_ability,
	hmenum::for_each_ability,
	hmenum::minmax_ability,
	hmenum::accumulate_ability<int>
>;
template<typename iterator>
int func_enm(iterator Beg, iterator End){
	return func_enm(this_enumerator::make_enumerator(Beg, End));
}
int func_enm(this_enumerator& Enm){
	Enm.unique_back();
	Enm.remove_back_if([](int val){return val % 2 != 0; });
	Enm.sort_back();
	Enm.for_each_back([](int val){val += 3; });
	return Enm.accumulate_back(0);
}

template<typename iterator>
int func_enm2(iterator Beg, iterator End){
	return func_enm2(this_enumerator::make_enumerator(Beg, End));
}
int func_enm2(this_enumerator& Enm){
	int Ans = 0;
	for(; Enm; ++Enm){
		if(*Enm % 2 == 0){
			Ans += *Enm;
			*Enm += 1;
		}
	}
	Enm.reset();

	for(; Enm; ++Enm){
		if(*Enm % 3 == 0){
			Ans -= *Enm;
			*Enm += 2;
		}
	}
	Enm.reset();

	for(; Enm; ++Enm){
		if(*Enm % 5 == 0){
			Ans += *Enm/5;
		}
	}

	return Ans;
}
template<typename iterator>
int func_any(iterator Beg, iterator End){
	return func_any(boost::any_range<int, boost::random_access_traversal_tag>(Beg,End));
}
int func_any(boost::any_range<int, boost::random_access_traversal_tag> Range){
	boost::unique(Range);
	boost::remove_if(Range, [](int val){return val % 2 != 0; });
	boost::sort(Range);
	boost::for_each(Range, [](int val){val += 3; });
	return boost::accumulate(Range, 0);
}
template<typename iterator>
int func_any2(iterator Beg, iterator End){
	return func_any2(boost::any_range<int, boost::random_access_traversal_tag>(Beg, End));
}
int func_any2(boost::any_range<int, boost::random_access_traversal_tag> Range){
	auto Itr = Range.begin();

	int Ans = 0;
	for(; Itr!=Range.end(); ++Itr){
		if(*Itr % 2 == 0){
			Ans += *Itr;
			*Itr += 1;
		}
	}

	Itr = Range.begin();
	for(; Itr != Range.end(); ++Itr){
		if(*Itr % 3 == 0){
			Ans -= *Itr;
			*Itr += 2;
		}
	}
	Itr = Range.begin();

	for(; Itr != Range.end(); ++Itr){
		if(*Itr % 5 == 0){
			Ans += *Itr / 5;
		}
	}

	return Ans;
}

int main(void){
	std::vector<int> OriginalArray{9,2,6,3,5,5,3,3,1,4,6,8,9,2,2};

	using clock = std::chrono::high_resolution_clock;
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;

	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	std::cout << "------" << std::endl;
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_itr2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_enm2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;

	}
	{
		auto Time1 = clock::now();
		for(unsigned int i = 0; i < 1000; ++i){
			auto Array = OriginalArray;
			func_any2(std::begin(Array), std::end(Array));
		}
		auto Time2 = clock::now();
		std::cout << (Time2 - Time1).count() << std::endl;
	}

	system("pause");
	return 0;
}
