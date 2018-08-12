#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../algorithm.hpp"
#include "../../../random.hpp"
#include"../../../iterators/index_access_iterator.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_sample_algorithms){
public:
	TEST_METHOD(random_sample){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		auto Ans = hmLib::random_sample(Con.begin(), Con.end(), hmLib::random::default_engine());

		hmLib::random_sample(Con.begin(), Con.end(), Con2.begin(), Con2.end(), hmLib::random::default_engine());

		auto itr = hmLib::random_sample(Con.begin(), Con.end(), Con2.begin(), Con2.size(), hmLib::random::default_engine());
	}
	TEST_METHOD(random_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::make_random_sampler(Con.begin(), Con.end());

		for(auto& Val : Con2){
			Val = *Sampler(hmLib::random::default_engine());
		}
	}
	TEST_METHOD(roulette_sample){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		auto Ans = hmLib::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1+val); }, hmLib::random::default_engine());

		hmLib::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); }, Con2.begin(), Con2.end(), hmLib::random::default_engine());

		auto itr = hmLib::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); }, Con2.begin(), Con2.size(), hmLib::random::default_engine());
	}
	TEST_METHOD(roulette_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::make_roulette_sampler(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); });

		for(auto& Val : Con2){
			Val = *Sampler(hmLib::random::default_engine());
		}
	}
	TEST_METHOD(shuffle_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::make_random_removal_sampler(Con.begin(), Con.end());

		for(auto& Val : Con2){
			Val = *Sampler(hmLib::random::default_engine());
		}
	}
	};

	TEST_CLASS(test_keep_algorithms) {
	public:
		TEST_METHOD(keep_if_notag) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,4,5,7,-3,0,2,-5,4,9 };

			auto Keeper = hmLib::keep_if(Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(3u, Keeper.size());
			Assert::AreEqual(3, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-5, *Itr++);
		}
		TEST_METHOD(keep_if_element) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,4,5,7,-3,0,2,-5,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::element_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });
			
			Assert::AreEqual(3u, Keeper.size());
			Assert::AreEqual(3,std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-5, *Itr++);
		}
		TEST_METHOD(keep_if_block) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,4,5,7,-3,0,2,-5,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::block_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(3u, Keeper.size());
			Assert::AreEqual(3, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-5, *Itr++);
		}
		TEST_METHOD(keep_if_element2) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ 4,5,7,-1,-3,-2,0,2,-5,-4,-7,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::element_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(6u, Keeper.size());
			Assert::AreEqual(6, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-2, *Itr++);
			Assert::AreEqual(-5, *Itr++);
			Assert::AreEqual(-4, *Itr++);
			Assert::AreEqual(-7, *Itr++);
		}
		TEST_METHOD(keep_if_block2) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ 4,5,7,-1,-3,-2,0,2,-5,-4,-7,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::block_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(6u, Keeper.size());
			Assert::AreEqual(6, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-2, *Itr++);
			Assert::AreEqual(-5, *Itr++);
			Assert::AreEqual(-4, *Itr++);
			Assert::AreEqual(-7, *Itr++);
		}
		TEST_METHOD(keep_if_element3) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,-3,-2, 4, 5, 7, 0, 2, 4, 9,-5,-4,-7};

			auto Keeper = hmLib::keep_if(hmLib::algorithm::element_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(6u, Keeper.size());
			Assert::AreEqual(6, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-2, *Itr++);
			Assert::AreEqual(-5, *Itr++);
			Assert::AreEqual(-4, *Itr++);
			Assert::AreEqual(-7, *Itr++);
		}
		TEST_METHOD(keep_if_block3) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,-3,-2,4,5,7,0,2,4,9 ,-5,-4,-7 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::block_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(6u, Keeper.size());
			Assert::AreEqual(6, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-2, *Itr++);
			Assert::AreEqual(-5, *Itr++);
			Assert::AreEqual(-4, *Itr++);
			Assert::AreEqual(-7, *Itr++);
		}
		TEST_METHOD(keep_if_elementc) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,4,5,7,-3,0,2,-5,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::element_keep_tag(), Con.cbegin(), Con.cend(), [](int v) {return v<0; });

			Assert::AreEqual(3u, Keeper.size());
			Assert::AreEqual(3, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-5, *Itr++);
		}
		TEST_METHOD(keep_if_blockc) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ -1,4,5,7,-3,0,2,-5,4,9 };

			auto Keeper = hmLib::keep_if(hmLib::algorithm::block_keep_tag(), Con.cbegin(), Con.cend(), [](int v) {return v<0; });

			Assert::AreEqual(3u, Keeper.size());
			Assert::AreEqual(3, std::distance(Keeper.begin(), Keeper.end()));

			auto Itr = Keeper.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-5, *Itr++);
		}
		TEST_METHOD(keep_if_block_push_back) {
			hmLib::algorithm::block_index_keeper<int> Keeper;

			std::vector<int> Index{ 2,3,6,7,8,10,12,14,15,16,17,18,19 };

			for(auto i:Index) {
				Keeper.push_back(i);
			}

			Assert::AreEqual(Index.size(), Keeper.size());

			unsigned int Cnt = 0;
			for(auto i:Keeper) {
				Assert::AreEqual(Index[Cnt], i);
				++Cnt;
			}
			Assert::AreEqual(Cnt, Index.size());
		}
		TEST_METHOD(keep_if_block_with_index_access_iterator) {
			using container = std::vector<int>;
			using iterator = container::iterator;

			container Con{ 4,5,7,-1,-3,-2,0,2,-5,-4,-7,4,9 };

			auto Keeper = hmLib::keep_index_if(hmLib::algorithm::block_keep_tag(), Con.begin(), Con.end(), [](int v) {return v<0; });

			Assert::AreEqual(6u, Keeper.size());
			Assert::AreEqual(6, std::distance(Keeper.begin(), Keeper.end()));

			auto Range = hmLib::make_index_access_range(Con.begin(), Keeper.begin(), Keeper.end());
			auto Itr = Range.begin();

			Assert::AreEqual(-1, *Itr++);
			Assert::AreEqual(-3, *Itr++);
			Assert::AreEqual(-2, *Itr++);
			Assert::AreEqual(-5, *Itr++);
			Assert::AreEqual(-4, *Itr++);
			Assert::AreEqual(-7, *Itr++);
		}
	};
}
