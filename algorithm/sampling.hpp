#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#define HMLIB_ALGORITHM_SAMPLING_INC 300
#
#include<algorithm>
#include<iterator>
#include<vector>
#include<numeric>
#include<random>
#include"../exceptions.hpp"
#include"index_sampling.hpp"
namespace hmLib{
	template<std::forward_iterator Iterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	Iterator random_sample(Iterator Begin,Iterator End, URBG&& Engine){
		if(Begin == End)return End;
		return std::next(Begin, std::uniform_int_distribution<std::size_t>(0,std::distance(Begin,End)-1)(Engine));
	}
	template<std::forward_iterator Iterator, std::output_iterator<std::iter_value_t<Iterator>> OutputIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	OutputIterator random_sample(Iterator Begin,Iterator End, OutputIterator Out, std::size_t n, URBG&& Engine){
		if(Begin == End)return Out;

		std::uniform_int_distribution<int> Dist(0, std::distance(Begin, End) - 1);
		for(std::size_t i = 0; i < n; ++i){
			*Out++ = *std::next(Begin, Dist(Engine));
		}
		return Out;
	}
	template<std::forward_iterator Iterator>
	class random_sampler{
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		Iterator Begin;
		random_indexer Indexer;
	public:
		random_sampler()=default;
		random_sampler(Iterator Begin_,Iterator End_){reset(Begin_,End_);}
	public:
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			return std::next(Begin, Indexer(Engine));
		}
		void reset(Iterator Begin_,Iterator End_){
			Begin = Begin_;
			Indexer.reset(std::distance(Begin_,End_));
		}
	};
	template<std::forward_iterator Iterator>
	auto make_random_sampler(Iterator Begin_,Iterator End_){
		return random_sampler<Iterator>(Begin_,End_);
	}
	template<std::forward_iterator Iterator>
	class aggregate_random_sampler{
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		std::vector<Iterator> Container;
	public:
		aggregate_random_sampler()=default;
		aggregate_random_sampler(Iterator Begin_,Iterator End_){reset(Begin_,End_);}
	public:
		template<typename URBG>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request, "Sampling is requested for empty sampler.");
			return Container[dist_type(0, Container.size()-1)(Engine)];
		}
		void reset(){Container.clear();}
		void reset(Iterator Begin_,Iterator End_){
			Container.clear();
			insert(Begin_,End_);
		}
		void insert(Iterator Itr_) {
			Container.push_back(Itr_);
		}
		void insert(Iterator Begin_, Iterator End_) {
			if constexpr(std::random_access_iterator<Iterator>){
				Container.reserve(Container.size() + End_-Begin_);
			}
			for(;Begin_!=End_;++Begin_){
				Container.push_back(Begin_);
			}
		}
		bool empty()const { return Container.empty(); }
		std::size_t size()const{return Container.size();}
	};
	template<std::forward_iterator Iterator>
	auto make_aggregate_random_sampler(Iterator Begin_,Iterator End_){
		return aggregate_random_sampler<Iterator>(Begin_,End_);
	}

	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	Iterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		return std::next(Begin,roulette_index(WeightBegin,WeightEnd,std::forward<URBG>(Engine)));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, typename URBG, typename weight_type>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	Iterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine, weight_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		return std::next(Begin,roulette_index(WeightBegin,WeightEnd,std::forward<URBG>(Engine),TotalWeight));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::output_iterator<std::iter_value_t<Iterator>> OutputIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t N, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		using weight_type = std::iter_value_t<WeightIterator>;
		weight_type TotalWeight = std::accumulate(WeightBegin, WeightEnd, weight_type{});
		return roulette_index(WeightBegin, WeightEnd,Out,N,std::forward<URBG>(Engine),TotalWeight);
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::output_iterator<std::iter_value_t<Iterator>> OutputIterator, typename URBG, typename weight_type>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, weight_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			weight_type Selected = std::uniform_real_distribution<weight_type>(0., TotalWeight)(Engine);
			if constexpr(std::random_access_iterator<Iterator> && std::random_access_iterator<WeightIterator>){
				for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
					Selected -= *WeightItr;
					if(Selected <= 0.) {
						Out++ = *std::next(Begin, std::distance(WeightBegin, WeightItr));
						break;
					}
				}
			}else{
				auto Itr = Begin;
				for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
					Selected -= *WeightItr;
					if(Selected <= 0.) {
						Out++ = *Itr;
						break;
					}
					++Itr;
				}
			}
			hmLib_assert(Selected<=0., hmLib::numeric_exceptions::incorrect_arithmetic_request, "Given TotalWeight is lower than sum of each Weight.");
		}
		return Out;
	}

	template<typename Iterator, typename weight_type = double>
	class roulette_sampler;
	template<std::random_access_iterator Iterator, typename weight_type>
	class roulette_sampler<Iterator,weight_type>{
		using dist_type = std::uniform_real_distribution<weight_type>;
	private:
		Iterator Begin;
		std::vector<weight_type> Container;
		dist_type Dist;
	public:
		roulette_sampler()=default;
		template<std::forward_iterator WeightIterator>
		roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) { reset(Begin_, End_, WeightBeg_, WeightEnd_); }
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request, "Sampling is requested for empty sampler.");
			return std::next(Begin,
				std::distance(Container.begin(), std::lower_bound(Container.begin(),Container.end(),Dist(Engine)))
			);
		}
		void reset(){Container.clear();}
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(WeightBeg_, WeightEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			Begin = Begin_;
			Container.clear();
			if constexpr(std::random_access_iterator<Iterator>){
				Container.reserve(End_-Begin_);
			}
			weight_type Val = 0.0;
			while(Begin_!=End_) {
				Val += *WeightBeg_;
				Container.emplace_back(Val);
				++Begin_;
				++WeightBeg_;
			}
			Dist.param(typename dist_type::param_type(0.0,Val));
		}
		bool empty()const{return Container.empty();}
	};
	template<std::forward_iterator Iterator, typename weight_type>
	class roulette_sampler<Iterator,weight_type>{
		using dist_type = std::uniform_real_distribution<weight_type>;
	private:
		std::vector<Iterator> ItVec;       // forward_iterator でも高速化するための iterator 配列
		std::vector<weight_type> Container;    // 累積重み
		dist_type Dist;

	public:
		roulette_sampler() = default;
		template<std::forward_iterator WeightIterator>
		roulette_sampler(Iterator Begin_, Iterator End_,
						WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			reset(Begin_, End_, WeightBeg_, WeightEnd_);
		}
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!empty(),hmLib::numeric_exceptions::incorrect_arithmetic_request,"Sampling is requested for empty sampler.");

			weight_type r = Dist(Engine);
			auto pitr = std::lower_bound(Container.begin(), Container.end(), r);
			std::size_t Index = std::distance(Container.begin(), pitr);

			return ItVec[Index];   // forward_iterator でも O(1)
		}
		void reset(){
			ItVec.clear();
			Container.clear();
		}
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_,WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(WeightBeg_, WeightEnd_),hmLib::numeric_exceptions::invalid_initialvalue,"Given pair of iterators has different distance.");

			ItVec.clear();
			Container.clear();

			weight_type Val = 0.0;

			while(Begin_ != End_) {
				ItVec.push_back(Begin_);   // forward_iterator でも高速化できる
				Val += *WeightBeg_;
				Container.emplace_back(Val);

				++Begin_;
				++WeightBeg_;
			}

			Dist.param(dist_type::param_type(0.0, Val));
		}
		bool empty() const { return Container.empty(); }
	};
	template<typename Iterator, typename WeightIterator>
	auto make_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
		using weight_type = std::iter_value_t<WeightIterator>;
		return roulette_sampler<Iterator,weight_type>(Begin_, End_, WeightBeg_, WeightEnd_);
	}

	template<std::forward_iterator Iterator, typename weight_type = double>
	class aggregate_roulette_sampler{
		struct element{
			weight_type value;
			Iterator itr;
			element()=delete;
			element(weight_type v):value(v),itr(){}
			element(weight_type v, Iterator i):value(v),itr(i){}
			friend bool operator<(const element& e1, const element& e2){return e1.value<e2.value;}
		};
		using dist_type = std::uniform_real_distribution<weight_type>;
	private:
		std::vector<element> Container;
		dist_type Dist;
	public:
		aggregate_roulette_sampler()=default;
		template<std::forward_iterator WeightIterator>
		aggregate_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) { reset(Begin_, End_, WeightBeg_, WeightEnd_); }
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request, "Sampling is requested for empty sampler.");
			return std::lower_bound(Container.begin(),Container.end(),element{Dist(Engine)})->itr;
		}
		void reset() { Container.clear(); }
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(WeightBeg_, WeightEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			Container.clear();
			insert(Begin_,End_,WeightBeg_,WeightEnd_);
		}
		void insert(Iterator Itr_, weight_type Weight_) {
			weight_type Val = 0;
			if(!empty())Val = Container.back().value;
			Val+=Weight_;
			Container.emplace_back(Val,Itr_);
			Dist.param(dist_type::param_type(0.0,Val));
		}
		template<std::forward_iterator WeightIterator>
		void insert(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(WeightBeg_, WeightEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			if constexpr(std::random_access_iterator<Iterator>){
				Container.reserve(Container.size() + End_-Begin_);
			}
			weight_type Val = 0.0;
			if(!empty())Val = Container.back().value;
			while(Begin_!=End_) {
				Val += *WeightBeg_;
				Container.emplace_back(Val,Begin_);
				++Begin_;
				++WeightBeg_;
			}
			Dist.param(typename dist_type::param_type(0.0,Val));
		}
		bool empty()const { return Container.empty(); }
		std::size_t size()const{return Container.size();}
	};
	template<typename Iterator, typename WeightIterator>
	auto make_aggregate_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
		return aggregate_roulette_sampler<Iterator,std::iter_value_t<WeightIterator>>(Begin_, End_, WeightBeg_, WeightEnd_);
	}

	template<std::forward_iterator Iterator, std::output_iterator<std::iter_value_t<Iterator>> OutputIterator, typename URBG>
	requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
	OutputIterator unique_random_sample(Iterator Begin,Iterator End, OutputIterator Out, std::size_t N, URBG&& Engine){
		std::size_t Size = std::distance(Begin, End);
		if(Size == 0 || N == 0) return Out;
		hmLib_assert(Size>=N, hmLib::numeric_exceptions::incorrect_arithmetic_request, "More sampling than size is requested.");

		std::vector<std::size_t> idxs = unique_random_index(Size, N, Engine);

		std::sort(idxs.begin(), idxs.end());

		Iterator itr = Begin;
		std::size_t cur = 0;

		for(std::size_t k = 0; k < N; ++k){
			std::size_t target = idxs[k];

			while(cur < target){
				++itr;
				++cur;
			}

			*Out++ = *itr;
		}

		return Out;
	}
	template<std::forward_iterator Iterator>
	class unique_random_sampler {
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		std::vector<Iterator> Container;
		dist_type Dist;
	public:
		unique_random_sampler() = default;
		unique_random_sampler(Iterator Begin, Iterator End) {
			reset(Begin, End);
		}
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!Container.empty(),hmLib::numeric_exceptions::incorrect_arithmetic_request,"Sampling is requested for empty sampler.");

			std::size_t k = Dist(Engine);
			Iterator itr = Container[k];

			Container[k] = Container.back();
			Container.pop_back();

			if(!Container.empty()){
				Dist.param(dist_type::param_type(0, Container.size()-1));
			}

			return itr;
		}
		void reset(Iterator Begin, Iterator End){
			Container.clear();
			for(auto it = Begin; it != End; ++it){
				Container.push_back(it);
			}
			if(!Container.empty()){
				Dist.param(dist_type::param_type(0, Container.size()-1));
			}
		}
		bool empty() const { return Container.empty(); }
		std::size_t size() const { return Container.size(); }
	};
	template<std::forward_iterator Iterator>
	auto make_unique_random_sampler(Iterator Begin_,Iterator End_){
		return unique_random_sampler<Iterator>(Begin_,End_);
	}

	template<std::forward_iterator Iterator,std::forward_iterator WeightIterator,std::output_iterator<std::iter_value_t<Iterator>> OutputIterator,typename URBG>
	OutputIterator unique_roulette_sample(Iterator Begin, Iterator End,	WeightIterator WeightBeg, WeightIterator WeightEnd,	OutputIterator Out,	std::size_t N,URBG&& Engine){
		using weight_type = std::iter_value_t<WeightIterator>;

		std::size_t Size = std::distance(Begin, End);
		hmLib_assert(Size == std::distance(WeightBeg, WeightEnd),hmLib::numeric_exceptions::invalid_initialvalue,"Given pair of iterators has different distance.");
		if(Size == 0 || N == 0) return Out;
		hmLib_assert(Size>=N, hmLib::numeric_exceptions::incorrect_arithmetic_request, "More sampling than size is requested.");

		auto idxs = unique_roulette_index(WeightBeg, WeightEnd, N, Engine);

		// 3. forward_iterator を next で進めるために index を昇順に
		std::sort(idxs.begin(), idxs.end());

		// 4. forward_iterator を next で進めて値を取り出す
		Iterator itr = Begin;
		std::size_t cur = 0;

		for(std::size_t k = 0; k < N; ++k){
			std::size_t target = idxs[k];

			while(cur < target){
				++itr;
				++cur;
			}

			*Out++ = *itr;
		}

		return Out;
	}
	template<typename Iterator, typename weight_type = double>
	class unique_roulette_sampler;
	template<std::random_access_iterator Iterator, typename weight_type>
	class unique_roulette_sampler<Iterator, weight_type>{
	private:
		unique_roulette_indexer<weight_type> Indexer;
		Iterator Begin;
	public:
		unique_roulette_sampler() = default;
		template<std::forward_iterator WeightIterator>
		unique_roulette_sampler(Iterator Begin_, Iterator End_,WeightIterator WeightBegin, WeightIterator WeightEnd){
			reset(Begin_, End_, WeightBegin, WeightEnd);
		}
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_,WeightIterator WeightBegin, WeightIterator WeightEnd){
			Begin = Begin_;
			Indexer.reset(WeightBegin, WeightEnd);
		}
		template<typename URBG>
		Iterator operator()(URBG&& Engine){
			std::size_t idx = Indexer(Engine);
			return Begin + idx;   // O(1)
		}
		bool empty() const { return Indexer.empty(); }
		std::size_t size() const { return Indexer.size(); }
	};
	template<std::forward_iterator Iterator, typename weight_type>
	class unique_roulette_sampler<Iterator, weight_type>{
	private:
		unique_roulette_indexer<weight_type> Indexer;
		std::vector<Iterator> ItVec;
	public:
		unique_roulette_sampler() = default;
		template<std::forward_iterator WeightIterator>
		unique_roulette_sampler(Iterator Begin_, Iterator End_,WeightIterator WeightBegin, WeightIterator WeightEnd){
			reset(Begin_, End_, WeightBegin, WeightEnd);
		}
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_,WeightIterator WeightBegin, WeightIterator WeightEnd){
			ItVec.clear();
			for(auto itr = Begin_; itr != End_; ++itr){
				ItVec.push_back(itr);
			}
			Indexer.reset(WeightBegin, WeightEnd);
		}
		template<typename URBG>
		requires std::uniform_random_bit_generator<std::remove_reference_t<URBG>>
		Iterator operator()(URBG&& Engine){
			std::size_t idx = Indexer(Engine);
			return ItVec[idx];   // O(1)
		}
		bool empty() const { return Indexer.empty(); }
		std::size_t size() const { return Indexer.size(); }
	};
	template<typename Iterator, typename WeightIterator>
	auto make_unique_roulette_sampler(Iterator Begin, Iterator End,WeightIterator WB, WeightIterator WE){
		using weight_type = std::iter_value_t<WeightIterator>;
		return unique_roulette_sampler<Iterator, weight_type>(Begin, End, WB, WE);
	}
}
#
#endif
