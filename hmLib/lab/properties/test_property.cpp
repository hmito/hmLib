#include<iostream>
#include<hmLib/lab/properties/property.hpp>

class cTest {
private:
	int a;
	int b;
public:
	hmLib::properties::property<int> A;
	hmLib::properties::property<const int&> B;
public:
	cTest() 
		: a(0)
		, b(0)
		, A(a, [this](const int a_)->const int {if(a_>0)a=a_; return a; })
		, B(b){
	}
};

int main() {
	cTest Test;
	Test.A=4;
	std::cout<<Test.B()<<std::endl;

	return 0;
}
