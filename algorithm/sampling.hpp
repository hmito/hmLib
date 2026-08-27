#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#define HMLIB_ALGORITHM_SAMPLING_INC 300
#
#include<algorithm>
#include<iterator>
#include<vector>
#include<numeric>
#include<random>
#include"../exceptions.hpp"
namespace hmLib{
	template<std::forward_iterator Iterator, std::uniform_random_bit_generator URBG>
	InputIterator random_sample(Iterator Begin,Iterator End, URBG&& Engine){
		if(Begin == End)return End;
		return std::next(Begin, std::uniform_int_distribution<std::size_t>(0,std::distance(Begin,End)-1)(Engine));
	}
	template<std::forward_iterator Iterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG>
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
		template<typename RandEngine>
		Iterator operator()(RandEngine Engine){
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
		template<typename RandEngine>
		Iterator operator()(RandEngine Engine){
			hmLib_assert(!empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request, "Sampling is requested for empty sampler.");
			return *std::next(Container.begin(), dist_type(0,Container.size()-1)(Engine));
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
			for(;Begin_!=End_;++Begin){
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

	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::uniform_random_bit_generator URBG>
	Iterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		return std::next(Begin,roulette_index(WeightBegin,WeightEnd,Engine));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::uniform_random_bit_generator URBG, typename value_type>
	Iterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		return std::next(Begin,roulette_index(WeightBegin,WeightEnd,Engine,TotalWeight));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::output_iterator<std::iter_value_t> OutputIterator, std::uniform_random_bit_generator URBG>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t N, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		using value_type = std::iter_value_t<WeightIterator>;
		value_type TotalWeight = std::accumulate(WeightBegin, WeightEnd, value_type{});
		return roulette_index_sample(Begin,End,WeightBegin, WeightEnd,Out,N,Engine,TotalWeight);
	}
	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
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

	template<typename Iterator, typename value_type = double>
	class roulette_sampler;
	template<std::random_access_iterator Iterator, typename value_type>
	class roulette_sampler<Iterator,value_type>{
		using dist_type = std::uniform_real_distribution<value_type>;
	private:
		Iterator Begin;
		std::vector<value_type> Container;
		dist_type Dist;
	public:
		roulette_sampler()=default;
		template<std::forward_iterator WeightIterator>
		roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) { reset(Begin_, End_, WeightBeg_, WeightEnd_); }
		template<std::uniform_random_bit_generator URBG>
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
			value_type Val = 0.0;
			while(Begin_!=End_) {
				Val += *WeightBeg_;
				Container.emplace_back(Val);
				++Begin_;
				++WeightBeg_;
			}
			Dist.param(dist_type::param_type(0.0,Val));
		}
		bool empty()const{return Container.empty();}
	};
	template<std::forward_iterator Iterator, typename value_type>
	class roulette_sampler<Iterator,value_type>{
		using dist_type = std::uniform_real_distribution<value_type>;
	private:
		std::vector<Iterator> ItVec;       // forward_iterator でも高速化するための iterator 配列
		std::vector<value_type> Container;    // 累積重み
		dist_type Dist;

	public:
		roulette_sampler() = default;
		template<std::forward_iterator WeightIterator>
		roulette_sampler(Iterator Begin_, Iterator End_,
						WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			reset(Begin_, End_, WeightBeg_, WeightEnd_);
		}
		template<std::uniform_random_bit_generator URBG>
		Iterator operator()(URBG&& Engine){
			hmLib_assert(!empty(),hmLib::numeric_exceptions::incorrect_arithmetic_request,"Sampling is requested for empty sampler.");

			value_type r = Dist(Engine);
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

			value_type Val = 0.0;

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
		using value_type = std::iter_value_t<WeightIterator>;
		return roulette_sampler<Iterator,value_type>(Begin_, End_, WeightBeg_, WeightEnd_);
	}

	template<std::forward_iterator Iterator, typename value_type = double>
	class aggregate_roulette_sampler{
		struct element{
			value_type value;
			Iterator itr;
			element()=delete;
			element(value_type v):value(v),itr(){}
			element(value_type v, Iterator i):value(v),itr(i){}
			friend bool operator<(const element& e1, const element& e2){return e1.value<e2.value;}
		};
		using dist_type = std::uniform_real_distribution<value_type>;
	private:
		std::vector<element> Container;
		dist_type Dist;
	public:
		aggregate_roulette_sampler()=default;
		template<std::forward_iterator WeightIterator>
		aggregate_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) { reset(Begin_, End_, WeightBeg_, WeightEnd_); }
		template<std::uniform_random_bit_generator URBG>
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
		void insert(Iterator Itr_, value_type Weight_) {
			value_type Val = 0;
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
			value_type Val = 0.0;
			if(!empty())Val = Container.back().value;
			while(Begin_!=End_) {
				Val += *WeightBeg_;
				Container.emplace_back(Val,Begin_);
				++Begin_;
				++WeightBeg_;
			}
			Dist.param(dist_type::param_type(0.0,Val));
		}
		bool empty()const { return Container.empty(); }
		std::size_t size()const{return Container.size();}
	};
	template<typename Iterator, typename WeightIterator>
	auto make_aggregate_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
		return aggregate_roulette_sampler<Iterator,std::decay_t(decltype(*WeightBeg_))>(Begin_, End_, WeightBeg_, WeightEnd_);
	}

	template<std::forward_iterator Iterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG>
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
		template<std::uniform_random_bit_generator URBG>
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

}
#
#endif
