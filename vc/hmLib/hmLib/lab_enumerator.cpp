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
	//�d����������
	End = std::unique(Beg, End);
	//���������
	std::remove_if(Beg, End, [](int val){return val % 2 != 0; });
	//�\�[�g����
	std::sort(Beg, End);

	//�擪����̍��v��10�𒴂���ŏ��̒l��Ԃ�
	int pos = 0;
	for(; Beg != End; ++Beg){
		pos += *Beg;
		if(pos > 10)return *Beg;
	}

	//���s������0��Ԃ�
	return 0;
}
namespace henum = hmLib::enumerators;
using my_enumerator = hmLib::forward_enumerator<int, henum::unique_ability,henum::remove_ability,henum::sort_ability>;
template<typename iterator>
int func_enm(iterator Beg, iterator End){
	return func_enm(my_enumerator::make_enumerator(Beg, End));
}
int func_enm(my_enumerator& Enm){
	//�d����������
	Enm.unique();
	//���������
	Enm.remove_if([](int val){return val % 2 != 0; });
	//�\�[�g����
	Enm.sort();

	//�擪����̍��v��10�𒴂���ŏ��̒l��Ԃ�
	int pos = 0;
	for(; Enm; ++Enm){
		pos += *Enm;
		if(pos > 10)return *Enm;
	}

	//���s������0��Ԃ�
	return 0;
}

int main(void){
	std::vector<int> Array{9,2,6,3,5,5,3,3,1,4,6,8,9,2,2};

	system("pause");
	return 0;
}
