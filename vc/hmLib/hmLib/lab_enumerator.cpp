//#include<hmLib/enumerator.hpp>
#include<iostream>
template<typename type>
struct func_mixin{
	int i;
	void func(){
		std::cout << "func:";
		static_cast<type*>(this)->print();
	}
};
template<typename type>
struct punc_mixin{
	int k;
	void punc(){
		std::cout << "punc:";
		static_cast<type*>(this)->print();
	}
};
template<typename type>
struct dunc_mixin{
	int k;
	void dunc(){
		std::cout << "dunc:";
		static_cast<type*>(this)->print();
	}
};

template<class... concepts>
struct concept_pack{};
template<class concept, class... concepts>
struct concept_pack<concept, concepts...> :public concept, public concept_pack<concepts...>{};


struct use :public concept_pack< func_mixin<use>, punc_mixin<use>, dunc_mixin<use> >{
	void print(){
		std::cout << "test_use" << std::endl;
	}
};

#include<hmLib/enumerator.hpp>

struct user_interface{
	using my_enumerator = hmLib::enumerator<std::string, hmLib::enumerators::foward_iterator_concept>;

	template<typename iterator>
	void output(iterator Begin, iterator End){
		output_enum(my_enumerator::make_range(Begin, End));
	}
	virtual void output_enum(my_enumerator etr) = 0;
};

int main(){
	use Use;
	Use.func();
	Use.punc();
	Use.dunc();

	system("pause");
	return 0;
}
