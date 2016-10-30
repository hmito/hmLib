#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include <hmLib/chain.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_chain){
public:
	TEST_METHOD(element){
		using my_chain = chain<double>;

		my_chain::element Elm1;
		Assert::IsTrue(Elm1, L"Elm cannot be written.");

		*Elm1 = 5.2;

		Assert::AreEqual(5.2, *Elm1);
	}
	TEST_METHOD(construct){
		chain<double> Cir;
		Assert::IsTrue(Cir.empty(), L"empty function error.");
		Assert::AreEqual(0u, Cir.size(), L"size function error.");
	}
	TEST_METHOD(push_back){
		using my_chain = chain<double>;
		my_chain::element Elm1;
		*Elm1 = 3.5;

		my_chain Cir;
		Cir.push_back(Elm1);
		Assert::IsFalse(Elm1, L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(),L"fron function error");
		
		my_chain::element Elm2;
		*Elm2 = 4.5;
		Cir.push_back(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(), L"front function error");
		Assert::AreEqual(4.5, Cir.back(), L"back function error");
	}
	TEST_METHOD(push_front){
		using my_chain = chain<double>;
		my_chain::element Elm1;
		*Elm1 = 3.5;

		my_chain Cir;
		Cir.push_front(Elm1);
		Assert::IsFalse(Elm1, L"elm can use after it is inserted");

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(1u, Cir.size(), L"size function error.");

		Assert::AreEqual(3.5, Cir.front(), L"fron function error");
		Assert::AreEqual(3.5, Cir.back(), L"back function error");

		my_chain::element Elm2;
		*Elm2 = 4.5;
		Cir.push_front(Elm2);

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(2u, Cir.size(), L"size function error.");

		Assert::AreEqual(4.5, Cir.front(), L"back function error");
		Assert::AreEqual(3.5, Cir.back(), L"front function error");
	}
	TEST_METHOD(push_back_multi){
		using my_chain = chain<int>;
		my_chain Cir;

		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
			Cir.push_back(Elm[i]);
		}

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(10u, Cir.size(), L"size function error.");

		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(i * 5, *Itr, L"");
			++Itr;
		}
		Assert::IsTrue(Itr==Cir.end(), L"itr == error");
	}
	TEST_METHOD(push_front_multi){
		using my_chain = chain<int>;
		my_chain Cir;

		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
			Cir.push_front(Elm[i]);
		}

		Assert::IsFalse(Cir.empty(), L"empty function error.");
		Assert::AreEqual(10u, Cir.size(), L"size function error.");

		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual((10-i - 1) * 5, *Itr, L"");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");
	}
	TEST_METHOD(insert){
		using my_chain = chain<int>;
		my_chain Cir;

		//setup elements
		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
		}

		//push back
		for(int i = 0; i < 5; ++i){
			Cir.push_back(Elm[i*2+1]);
		}

		//insert
		auto IItr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual(i * 2, std::distance(Cir.begin(), IItr), L"Place error");
			IItr = Cir.insert(IItr, Elm[i * 2]);
			Assert::AreEqual(*Elm[i * 2], *IItr, L"Not returned same element");
			std::advance(IItr, 2);
		}

		//check all elements
		auto Itr = Cir.begin();
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(i * 5, *Itr, L"Inserted order error");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");
	}
	TEST_METHOD(erase){
		using my_chain = chain<int>;
		my_chain Cir;

		//setup elements
		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
		}

		//push back
		for(int i = 0; i < 10; ++i){
			Cir.push_back(Elm[i]);
		}

		//erase
		auto IItr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual(i, std::distance(Cir.begin(), IItr), L"Place error");
			IItr = Cir.erase(IItr);
			++IItr;
		}

		//check all elements
		auto Itr = Cir.begin();
		for(int i = 0; i < 5; ++i){
			Assert::AreEqual((2*i+1) * 5, *Itr, L"Erased order error");
			++Itr;
		}
		Assert::IsTrue(Itr == Cir.end(), L"itr == error");

		//check elements
		for(int i = 0; i < 10; ++i){
			Assert::AreEqual(static_cast<bool>(Elm[i]), i % 2 == 0, L"Release element error");
		}
	}
	TEST_METHOD(clear){
		using my_chain = chain<int>;
		my_chain Cir;

		//setup elements
		my_chain::element Elm[10];
		for(int i = 0; i < 10; ++i){
			*(Elm[i]) = i * 5;
		}

		//push back
		for(int i = 0; i < 10; ++i){
			Cir.push_back(Elm[i]);
		}

		//clear
		Cir.clear();

		Assert::IsTrue(Cir.empty(), L"empty error after clear.");
		Assert::AreEqual(0u,Cir.size(), L"size error after clear.");

		//check elements
		for(int i = 0; i < 10; ++i){
			Assert::IsTrue(static_cast<bool>(Elm[i]), L"Release element error");
		}
	}
	};
	TEST_CLASS(test_sorted_chain){
	public:
		TEST_METHOD(element){
			using my_chain = sorted_chain<double>;

			my_chain::element Elm1;
			Assert::IsTrue(Elm1, L"Elm cannot be written.");

			*Elm1 = 5.2;

			Assert::AreEqual(5.2, *Elm1);
		}
		TEST_METHOD(construct){
			sorted_chain<double> Cir;
			Assert::IsTrue(Cir.empty(), L"empty function error.");
		}
		TEST_METHOD(push_and_next){
			using my_chain = sorted_chain<double>;
			my_chain::element Elm1;
			*Elm1 = 3.5;

			my_chain Cir;
			Cir.push(Elm1);
			Assert::IsFalse(Elm1, L"elm can use after it is inserted");
			Assert::IsFalse(Cir.empty(), L"empty function error.");

			Assert::AreEqual(3.5, Cir.next(), L"fron function error");

			my_chain::element Elm2;
			*Elm2 = 2.5;
			Cir.push(Elm2);

			Assert::IsFalse(Cir.empty(), L"empty function error.");
			Assert::AreEqual(2.5, Cir.next(), L"front function error");
		}
		TEST_METHOD(push_multi){
			using my_chain = sorted_chain<int>;
			my_chain Cir;

			my_chain::element Elm[10];
			for(int i = 0; i < 10; ++i){
				*(Elm[i]) = i * 5;
			}
			for(int i = 0; i < 5; ++i){
				Cir.push(Elm[i * 2]);
			}
			for(int i = 0; i < 5; ++i){
				Cir.push(Elm[10 - i * 2 - 1]);
			}

			auto Itr = Cir.begin();
			for(int i = 0; i < 10; ++i){
				Assert::AreEqual(i * 5, *Itr, L"order error");
				++Itr;
			}
			Assert::IsTrue(Itr == Cir.end(), L"itr == error");
		}
		TEST_METHOD(push_multi2){
			using my_chain = sorted_chain<int>;
			my_chain Cir;

			my_chain::element Elm[10];
			for(int i = 0; i < 10; ++i){
				*(Elm[i]) = i * 5;
			}
			for(int i = 0; i < 5; ++i){
				Cir.push(Elm[10 - i * 2 - 2]);
			}
			for(int i = 0; i < 5; ++i){
				Cir.push(Elm[i * 2 + 1]);
			}

			auto Itr = Cir.begin();
			for(int i = 0; i < 10; ++i){
				Assert::AreEqual(i * 5, *Itr, L"order error");
				++Itr;
			}
			Assert::IsTrue(Itr == Cir.end(), L"itr == error");
		}
		TEST_METHOD(push_and_pop){
			using my_chain = sorted_chain<int>;
			my_chain Cir;

			//setup elements
			my_chain::element Elm[10];
			for(int i = 0; i < 10; ++i){
				*(Elm[i]) = i * 5;
			}

			Assert::IsTrue(Cir.empty(), L"empty before all pop");

			//push
			for(int i = 0; i < 10; ++i){
				Cir.push(Elm[i]);
			}

			//pop
			for(int i = 0; i < 10; ++i){
				Assert::AreEqual(i*5, Cir.next(), L"next number error");
				Assert::IsFalse(Cir.empty(), L"empty before all pop");
				Cir.pop();
			}
			Assert::IsTrue(Cir.empty(), L"empty before all pop");

			//push
			for(int i = 0; i < 10; ++i){
				Cir.push(Elm[9-i]);
			}

			//pop
			for(int i = 0; i < 10; ++i){
				Assert::AreEqual(i * 5, Cir.next(), L"next number error");
				Assert::IsFalse(Cir.empty(), L"empty before all pop");
				Cir.pop();
			}
			Assert::IsTrue(Cir.empty(), L"empty before all pop");
		}
		TEST_METHOD(clear){
			using my_chain = sorted_chain<int>;
			my_chain Cir;

			//setup elements
			my_chain::element Elm[10];
			for(int i = 0; i < 10; ++i){
				*(Elm[i]) = i * 5;
			}

			//push back
			for(int i = 0; i < 10; ++i){
				Cir.push(Elm[i]);
			}

			//clear
			Cir.clear();

			Assert::IsTrue(Cir.empty(), L"empty error after clear.");

			//check elements
			for(int i = 0; i < 10; ++i){
				Assert::IsTrue(static_cast<bool>(Elm[i]), L"Release element error");
			}
		}
	};
}
