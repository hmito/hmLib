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
	using this_enumerator = hmLib::enumerator<hmLib::enumerator_traits<hmLib::range_enumerator_tag, int>, hmLib::enumerators::forward_traversal_abilities>;
	auto EnumeratorImpl = this_enumerator::impl<int*>();
	this_enumerator* Ref = &EnumeratorImpl;

	system("pause");
	return 0;
}
