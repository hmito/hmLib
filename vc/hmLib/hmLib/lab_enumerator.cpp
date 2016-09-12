#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/enumerators.hpp>

int main(void){
	int Array[5] = {9,7,5,3,1};
	using this_enumerator = hmLib::enumerator < 
		hmLib::enumerator_traits<hmLib::range_enumerator_tag, int>, 
		hmLib::enumerators::random_access_traversal_ability, 
		hmLib::enumerators::find_ability
	>;

	auto EnumeratorImpl = this_enumerator::make_enumerator(std::begin(Array), std::end(Array));
	this_enumerator& Ref = EnumeratorImpl;

	Ref.find(hmLib::enumerators::cur, 5);
	Ref += 3;
	for(; Ref; ++Ref){
		std::cout << Ref[-1] << std::endl;
	}

	Ref.find(hmLib::enumerators::cur, 5);
	*Ref = 6;

	Ref.reset();
	Ref.advance(hmLib::enumerators::end, -1);

	for(; Ref; ++Ref){
		std::cout << *Ref << std::endl;
	}

	system("pause");
	return 0;
}
