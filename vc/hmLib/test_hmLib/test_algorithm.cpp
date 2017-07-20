#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include "../../../hmLib/algorithm.hpp"
#include "../../../random.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{
	TEST_CLASS(test_algorithm){
public:
	TEST_METHOD(random_sample){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);
		int val = 0;
		std::generate(Con.begin(), Con.end(), [&](){return val++; });

		auto Ans = hmLib::algorithm::random_sample(Con.begin(), Con.end(), hmLib::random::default_engine());

		hmLib::algorithm::random_sample(Con.begin(), Con.end(), Con2.begin(), Con2.end(), hmLib::random::default_engine());

		auto itr = hmLib::algorithm::random_sample(Con.begin(), Con.end(), Con2.begin(), Con2.size(), hmLib::random::default_engine());
	}
	TEST_METHOD(random_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::algorithm::make_random_sampler(Con.begin(), Con.end());

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

		auto Ans = hmLib::algorithm::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1+val); }, hmLib::random::default_engine());

		hmLib::algorithm::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); }, Con2.begin(), Con2.end(), hmLib::random::default_engine());

		auto itr = hmLib::algorithm::roulette_sample(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); }, Con2.begin(), Con2.size(), hmLib::random::default_engine());
	}
	TEST_METHOD(roulette_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::algorithm::make_roulette_sampler(Con.begin(), Con.end(), [](int val){return 1.0 / (1 + val); });

		for(auto& Val : Con2){
			Val = *Sampler(hmLib::random::default_engine());
		}
	}
	TEST_METHOD(shuffle_sampler){
		using container = std::vector<int>;
		using iterator = container::iterator;

		container Con(1000);
		container Con2(1000);

		auto Sampler = hmLib::algorithm::make_shuffle_sampler(Con.begin(), Con.end(), hmLib::random::default_engine());

		for(auto& Val : Con2){
			Val = *Sampler();
		}
	}
	};
}
