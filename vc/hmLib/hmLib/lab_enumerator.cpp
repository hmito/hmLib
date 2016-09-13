#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/enumerators.hpp>

int main(void){
	int Array[5] = {9,2,6,3,5};
	using this_enumerator = hmLib::random_access_enumerator<int,hmLib::enumerators::find_ability,hmLib::enumerators::sort_ability>;

	auto EnumeratorImpl = this_enumerator::make_enumerator(std::begin(Array), std::end(Array));
	this_enumerator& Ref = EnumeratorImpl;

	Ref.sort([](int val1, int val2){return val1 > val2; });
	for(; Ref; ++Ref){
		std::cout << *Ref<<std::endl;
	}

	system("pause");
	return 0;
}
