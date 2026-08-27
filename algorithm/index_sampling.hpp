#ifndef HMLIB_ALGORITHM_INDEXSAMPLING_INC
#define HMLIB_ALGORITHM_INDEXSAMPLING_INC 300
#
#include<concepts>
#include<algorithm>
#include<iterator>
#include<vector>
#include<numeric>
#include<random>
#include"unique_random_integrals.hpp"
#include"../exceptions.hpp"
namespace hmLib{
	template<typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::size_t random_index(std::size_t Size, URBG&& Engine){
		hmLib_assert(Size>0, hmLib::numeric_exceptions::incorrect_arithmetic_request, "Size must be positive integral.");
		return std::uniform_int_distribution<std::size_t>(0,Size-1)(Engine);
	}
	template<typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::vector<std::size_t> random_index(std::size_t Size, std::size_t N, URBG&& Engine){
		hmLib_assert(Size>0, hmLib::numeric_exceptions::incorrect_arithmetic_request, "Size must be positive integral.");
		std::uniform_int_distribution<std::size_t> Dist(0, Size - 1);

		std::vector<std::size_t> Idx;
		Idx.reserve(N);
		for(std::size_t i = 0; i < N; ++i){
			Idx.push_back(Dist(std::forward<URBG>(Engine)));
		}
		return Idx;
	}
    class random_indexer{
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		dist_type Dist;
	public:
		random_indexer()=default;
		random_indexer(std::size_t Size_){reset(Size_);}
	public:
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		std::size_t operator()(URBG&& Engine){
			return Dist(std::forward<URBG>(Engine));
		}
		void reset(std::size_t Size_){
			hmLib_assert(Size_>0, hmLib::numeric_exceptions::incorrect_arithmetic_request, "random_indexer cannot reset with Size=0.");
			Dist.param(dist_type::param_type{0,Size_-1});
		}
	};
	auto make_random_indexer(std::size_t Size_){
		return random_indexer(Size_);
	}

	template<std::forward_iterator WeightIterator, typename URBG,typename weight_type = std::iter_value_t<WeightIterator>>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::size_t roulette_index(WeightIterator Begin, WeightIterator End, URBG&& Engine, weight_type TotalWeight) {
		hmLib_assert(Begin!=End, hmLib::numeric_exceptions::incorrect_arithmetic_request, "WeightIterator must be positive distance.");
		weight_type Selected = std::uniform_real_distribution<weight_type>(0, TotalWeight)(Engine);
		if constexpr(std::random_access_iterator<WeightIterator>){
			for(auto Itr = Begin; Itr!=End; ++Itr) {
				Selected -= *Itr;
				if(Selected <= 0.)return Itr-Begin;
			}
			hmLib_throw(hmLib::numeric_exceptions::incorrect_arithmetic_request, "TotalWeight is smaller than sum of each Weight.");
		}else{
			std::size_t Index = 0;
			for(auto Itr = Begin; Itr!=End; ++Itr) {
				Selected -= *Itr;
				if(Selected <= 0.)return Index;
				++Index;
			}
			hmLib_throw(hmLib::numeric_exceptions::incorrect_arithmetic_request, "TotalWeight is smaller than sum of each Weight.");
		}
	}
	template<std::forward_iterator WeightIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::size_t roulette_index(WeightIterator Begin, WeightIterator End, URBG&& Engine) {
		hmLib_assert(Begin!=End, hmLib::numeric_exceptions::incorrect_arithmetic_request, "WeightIterator must be positive distance.");
		using weight_type = std::iter_value_t<WeightIterator>;
		weight_type TotalWeight = std::accumulate(Begin, End, weight_type{});
		return hmLib::roulette_index<WeightIterator,URBG,weight_type>(Begin, End, std::forward<URBG>(Engine), TotalWeight);
	}
	template<std::forward_iterator WeightIterator, typename URBG, typename weight_type>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::vector<std::size_t> roulette_index(WeightIterator Begin, WeightIterator End,  std::size_t N, URBG&& Engine, weight_type TotalWeight) {
		hmLib_assert(Begin!=End, hmLib::numeric_exceptions::incorrect_arithmetic_request, "WeightIterator must be positive distance.");

		std::vector<std::size_t> Idx;
		Idx.reserve(N);
		for(std::size_t i = 0;i<N;++i){
			weight_type Selected = std::uniform_real_distribution<weight_type>(0., TotalWeight)(Engine);
			if constexpr(std::random_access_iterator<WeightIterator>){
				for(auto Itr = Begin; Itr!=End; ++Itr) {
					Selected -= *Itr;
					if(Selected <= 0.) {
						Idx.push_back(Itr-Begin);
						break;
					}
				}
			}else{
				std::size_t Index = 0;
				for(auto Itr = Begin; Itr!=End; ++Itr) {
					Selected -= *Itr;
					if(Selected <= 0.) {
						Idx.push_back(Index);
						break;
					}
					++Index;
				}
			}
			hmLib_assert(Selected<=0., hmLib::numeric_exceptions::incorrect_arithmetic_request, "Given TotalWeight is lower than sum of each Weight.");
		}
		return Idx;
	}
	template<std::forward_iterator WeightIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::vector<std::size_t> roulette_index(WeightIterator Begin, WeightIterator End, std::size_t N, URBG&& Engine) {
		hmLib_assert(Begin!=End, hmLib::numeric_exceptions::incorrect_arithmetic_request, "WeightIterator must be positive distance.");
		using weight_type = std::iter_value_t<WeightIterator>;
		weight_type TotalWeight = std::accumulate(Begin, End, weight_type{});
		return hmLib::roulette_index(Begin,End,N,Engine,TotalWeight);
	}
	template<typename weight_type>
	class roulette_indexer{
		using dist_type = std::uniform_real_distribution<weight_type>;
	private:
		dist_type Dist;
		std::vector<weight_type> WeightVec;
	public:
		roulette_indexer()=default;
		template<std::forward_iterator WeightIterator>
		roulette_indexer(WeightIterator Begin_,WeightIterator End_){reset(Begin_,End_);}
	public:
		template<typename URB>
		std::size_t operator()(URB&& Engine){
			hmLib_assert(!empty(),hmLib::numeric_exceptions::incorrect_arithmetic_request,"Sampling is requested for empty sampler.");
			return std::distance(WeightVec.begin(),std::lower_bound(WeightVec.begin(),WeightVec.end(),Dist(Engine)));
		}
		void reset(){WeightVec.clear();}
		template<std::forward_iterator WeightIterator>
		void reset(WeightIterator Begin_,WeightIterator End_){
			hmLib_assert(Begin_!=End_, hmLib::numeric_exceptions::incorrect_arithmetic_request, "WeightIterator must be positive distance.");
			WeightVec.clear();
			if constexpr(std::random_access_iterator<WeightIterator>){
				WeightVec.reserve(End_ - Begin_);
			}
			weight_type Val=0;
			for(;Begin_!=End_;++Begin_){
				Val+=*Begin_;
				WeightVec.push_back(Val);
			}
			Dist.param(typename dist_type::param_type(0.0,Val));
		}
		bool empty()const{return WeightVec.empty();}
	};
	template<std::forward_iterator WeightIterator>
	auto make_roulette_indexer(WeightIterator Begin_,WeightIterator End_){
		return roulette_indexer<std::decay_t<decltype(*Begin_)>>(Begin_,End_);
	}

	template<typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::vector<std::size_t> unique_random_index(std::size_t Size,  std::size_t N, URBG&& Engine){
		return algorithm::unique_random_integrals<std::size_t>(N,0,Size,Engine);
	}
	class unique_random_indexer{
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		std::vector<std::size_t> Container;
		dist_type Dist;
	public:
		unique_random_indexer()=default;
		unique_random_indexer(std::size_t Size_){reset(Size_);}
	public:
		template<typename URBG>
		std::size_t operator()(URBG&& Engine){
			hmLib_assert(!Container.empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request,
						"Sampling is requested for empty sampler.");

			std::size_t k = Dist(Engine);
			std::size_t Index = Container[k];

			Container[k] = Container.back();
			Container.pop_back();

			if(!Container.empty()){
				Dist.param(dist_type::param_type(0, Container.size()-1));
			}

			return Index;
		}
		void reset(std::size_t Size_){
			Container.resize(Size_);
			for(std::size_t i = 0; i < Size_; ++i) Container[i] = i;

			Dist.param(dist_type::param_type(0, Size_-1));
		}
		bool empty()const { return Container.empty(); }
		std::size_t size()const{return Container.size();}
	};
	auto make_unique_random_indexer(std::size_t Size){
		return unique_random_indexer(Size);
	}

	template<std::forward_iterator WeightIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	std::vector<std::size_t> unique_roulette_index(WeightIterator Begin, WeightIterator End, std::size_t N, URBG&& Engine) {
		hmLib_assert(Begin != End,hmLib::numeric_exceptions::incorrect_arithmetic_request,"WeightIterator must be positive distance.");

		const std::size_t Size = std::distance(Begin, End);

		hmLib_assert(N <= Size,hmLib::numeric_exceptions::incorrect_arithmetic_request, "N must be <= number of weights.");

		using weight_type = std::iter_value_t<WeightIterator>;

		// A-ES 法のキーを生成
		std::vector<std::pair<double, std::size_t>> Keys;
		Keys.reserve(Size);

		std::uniform_real_distribution<double> Dist(0.0, 1.0);

		std::size_t idx = 0;
		for(auto Itr = Begin; Itr != End; ++Itr, ++idx){
			weight_type w = *Itr;
			hmLib_assert(w > 0,
				hmLib::numeric_exceptions::invalid_initialvalue,
				"Weight must be positive.");

			double U = Dist(Engine);
			double key = std::pow(U, 1.0 / static_cast<double>(w));
			Keys.emplace_back(key, idx);
		}

		// 上位 N 個を選ぶ（キーが大きい順）
		std::nth_element(Keys.begin(), Keys.begin() + N, Keys.end(),[](auto& a, auto& b){ return a.first > b.first; });

   		Keys.resize(N);

		// index のみ抽出
		std::vector<std::size_t> Result;
		Result.reserve(N);
		for(auto& kv : Keys){
			Result.push_back(kv.second);
		}

		return Result;
	}
	template<typename weight_type>
	class unique_roulette_indexer {
	private:
		std::vector<weight_type> CWeight;
	public:
		unique_roulette_indexer() = default;
		template<std::forward_iterator WeightIterator>
		unique_roulette_indexer(WeightIterator Begin, WeightIterator End){
			reset(Begin, End);
		}
		template<std::forward_iterator WeightIterator>
		void reset(WeightIterator Begin, WeightIterator End){
			CWeight.clear();

			weight_type sum = 0;
			for(; Begin != End; ++Begin){
				hmLib_assert(*Begin > 0,hmLib::numeric_exceptions::invalid_initialvalue,"Weight must be positive.");

				sum += static_cast<weight_type>(*Begin);
				CWeight.push_back(sum);
			}
		}
		bool empty() const { return CWeight.empty();}
		std::size_t size() const { return CWeight.size(); }
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		std::size_t operator()(URBG& Engine){
			hmLib_assert(!empty(),hmLib::numeric_exceptions::incorrect_arithmetic_request,"Sampling is requested for empty sampler.");

			weight_type Total = CWeight.back();
			std::uniform_real_distribution<weight_type> Dist(0, Total);
			weight_type Selected = Dist(Engine);

			// 二分探索
			auto itr = std::lower_bound(CWeight.begin(), CWeight.end(), Selected);
			std::size_t Index = std::distance(CWeight.begin(), itr);

			// 重み w_i を CWeight の差分から求める
			weight_type Removed = (Index == 0 ? CWeight[0] : CWeight[Index] - CWeight[Index - 1]);

			// CWeight-sum を更新（重みを 0 にする）
			for(std::size_t i = Index; i < CWeight.size(); ++i){
				CWeight[i] -= Removed;
			}

			return Index;
		}
	};
	template<std::forward_iterator WeightIterator>
	auto make_unique_roulette_indexer(WeightIterator WeightBegin, WeightIterator WeightEnd){
		using weight_type = std::iter_value_t<WeightIterator>;
		return unique_roulette_indexer<weight_type>(WeightBegin,WeightEnd);
	}
}
#
#endif
