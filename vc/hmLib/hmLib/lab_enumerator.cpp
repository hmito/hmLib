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



int main(){
	use Use;
	Use.func();
	Use.punc();
	Use.dunc();

	system("pause");
	return 0;
}

//‡@
template <class... Args>
class tuple{};

//‡A
template <class First, class... Body>
class tuple<First, Body...>{
public:
	First value;
	tuple<Body...> child;
};