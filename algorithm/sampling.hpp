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
		return std::next(Begin, std::uniform_int_distribution<int>(0,std::distance(Begin,End)-1)(Engine));
	}
	template<std::forward_iterator Iterator, std::output_iterator OIterator, std::uniform_random_bit_generator URBG>
	OutputIterator random_sample(Iterator Begin,Iterator End, OIterator Out, std::size_t n, URBG&& Engine){
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
		dist_type Dist;
	public:
		random_sampler()=default;
		random_sampler(Iterator Begin_,Iterator End_){reset(Begin_,End_);}
	public:
		template<typename RandEngine>
		Iterator operator()(RandEngine Engine){
			hmLib_assert(!empty(), hmLib::numeric_exceptions::incorrect_arithmetic_request, "Sampling is requested for empty sampler.");
			return std::next(Begin, Dist(Engine));
		}
		void reset(Iterator Begin_,Iterator End_){
			Begin = Begin_;
			Dist.param(dist_type::param_type(0,std::distance(Begin_,End_)));
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
			insert(Begin_,End_,WeightBegin_,WeightEnd_);
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
		if(Begin == End)return End;
		return std::next(Begin,roulette_index_sample(WeightBegin,WeightEnd,Engine));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::uniform_random_bit_generator URBG, typename value_type>
	Iterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;
		return std::next(Begin,roulette_index_sample(WeightBegin,WeightEnd,Engine,TotalWeight));
	}
	template<std::forward_iterator Iterator, std::forward_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return Out;
		using value_type = std::iter_value_t<WeightIterator>;
		value_type TotalWeight = std::accumulate(WeightBegin, WeightEnd, value_type{});
		return roulette_index_sample(Begin,End,WeightBegin, WeightEnd,Out,n,Engine,TotalWeight);
	}
	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
			auto Itr = Begin;
			for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
				Selected -= *WeightItr;
				if(Selected <= 0.) {
					Out++ = *Itr;
					break;
				}
				++Itr;
			}
			hmLib_assert(Selected<=0., hmLib::numeric_exceptions::incorrect_arithmetic_request, "Given TotalWeight is lower than sum of each Weight.");
		}
		return Out;
	}
	template<std::random_access_iterator Iterator, std::random_access_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_sample(Iterator Begin, Iterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
			for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
				Selected -= *WeightItr;
				if(Selected <= 0.) {
					Out++ = *std::next(Begin, std::distance(WeightBegin, WeightItr));
					break;
				}
			}
			hmLib_assert(Selected<=0., hmLib::numeric_exceptions::incorrect_arithmetic_request, "Given TotalWeight is lower than sum of each Weight.");
		}
		return Out;
	}
	template<std::forward_iterator Iterator, typename value_type = double>
	class roulette_sampler{
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
		template<std::forward_iterator WeightIterator>
		void reset(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(WeightBeg_, WeightEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
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
	};
	template<typename Iterator, typename WeightIterator>
	auto make_roulette_sampler(Iterator Begin_, Iterator End_, WeightIterator WeightBeg_, WeightIterator WeightEnd_) {
		return roulette_sampler<Iterator,std::decay_t(decltype(*WeightBeg_))>(Begin_, End_, WeightBeg_, WeightEnd_);
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

	template<typename InputIterator>
	class random_removal_sampler {
		typedef typename std::vector<InputIterator>::iterator iterator;
	private:
		std::vector<InputIterator> Vec;
		bool IsShuffled;
	public:
		random_removal_sampler() {}
		random_removal_sampler(InputIterator Begin_, InputIterator End_) { reset(Begin_, End_); }
	public:
		template<typename RandEngine>
		InputIterator operator()(RandEngine&& Engine) {
			if(!IsShuffled) {
				std::shuffle(Vec.begin(), Vec.end(), std::forward<RandEngine>(Engine));
				IsShuffled = true;
			}
			auto Itr = Vec.back();
			Vec.pop_back();
			return Itr;
		}
		void reset() { Vec.clear(); }
		void reset(InputIterator Begin_, InputIterator End_) {
			Vec.assign(std::distance(Begin_,End_),InputIterator());
			for(auto& Itr: Vec) { Itr = Begin_++; }
			IsShuffled = false;
		}
		void push(InputIterator Begin_, InputIterator End_) {
			for(; Begin_!=End_; ++Begin_) {
				Vec.push_back(Begin_);
			}
			IsShuffled = false;
		}
		bool empty()const { return Vec.empty(); }
		unsigned int size()const{return Vec.size();}
	};
	template<typename InputIterator>
	random_removal_sampler<InputIterator> make_random_removal_sampler(InputIterator Begin_, InputIterator End_) {
		return random_removal_sampler<InputIterator>(Begin_, End_);
	}

	template<typename InputIterator>
	class roulette_removal_sampler {
		struct assess_itr {
			InputIterator Itr;
			double Val;
		public:
			assess_itr(InputIterator Itr_, double Val_):Itr(Itr_), Val(Val_) {}
			bool operator>(const assess_itr& My_)const { return Val<My_.Val; }
		};
	private:
		std::vector<assess_itr> AssessVec;
		double AssessTotal;
		bool IsSorted;
	public:
		roulette_removal_sampler() {}
		template<typename fnAssess>
		roulette_removal_sampler(InputIterator Begin_, InputIterator End_, fnAssess&& FnAssess_) { reset(Begin_, End_, std::forward<fnAssess>(FnAssess_)); }
		template<typename assess_iterator>
		roulette_removal_sampler(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) { reset(Begin_, End_, ABeg_, AEnd_); }
	public:
		template<typename RandEngine>
		InputIterator operator()(RandEngine&& Engine) {
			if(!IsSorted) {
				std::sort(AssessVec.begin(), AssessVec.end());
				IsSorted = true;
			}

			double Terget = std::uniform_real_distribution<double>(0., assess_total())(Engine);
			double Value = 0.0;
			
			auto Itr = AssessVec.begin();
			for(; Itr != AssessVec.end(); ++Itr) {
				Value += Itr->Val;
				if(Terget<Value)break;
			}

			auto Ans = Itr->Itr;
			AssessTotal -= Itr->Val;
			AssessVec.erase(Itr);

			return Ans;
		}
		void reset() {
			AssessTotal = 0.;
			AssessVec.clear();
		}
		template<typename fnAssess>
		void reset(InputIterator Begin_, InputIterator End_, fnAssess&& FnAssess_) {
			reset();
			while(Begin_!=End_) {
				double Assess = FnAssess_(*Begin_);
				AssessTotal += Assess;
				AssessVec.push_back(assess_itr(Begin_++, Assess));
			}
			IsSorted = false;
		}
		template<typename assess_iterator>
		void reset(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(ABeg_, AEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			reset();
			while(Begin_!=End_) {
				double Assess = *ABeg_;
				AssessTotal += Assess;
				AssessVec.push_back(assess_itr(Begin_++, Assess));
			}
			IsSorted = false;
		}
		template<typename fnAssess>
		void push(InputIterator Begin_, InputIterator End_, fnAssess&& FnAssess_) {
			while(Begin_!=End_) {
				double Assess = FnAssess_(*Begin_);
				AssessTotal += Assess;
				AssessVec.push_back(assess_itr(Begin_++, Assess));
			}
			IsSorted = false;
		}
		template<typename assess_iterator>
		void push(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(ABeg_, AEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			while(Begin_!=End_) {
				double Assess = *ABeg_;
				AssessTotal += Assess;
				AssessVec.push_back(assess_itr(Begin_++, Assess));
			}
			IsSorted = false;
		}
		bool empty()const { return AssessVec.empty(); }
		unsigned int size()const { return AssessVec.size(); }
	private:
		double assess_total() { return AssessTotal; }
	};
	template<typename InputIterator, typename fnAssess>
	roulette_removal_sampler<InputIterator> make_roulette_removal_sampler(InputIterator Begin_, InputIterator End_, fnAssess&& FnAssess_) {
		return roulette_removal_sampler<InputIterator>(Begin_, End_, std::forward<fnAssess>(FnAssess_));
	}
	template<typename InputIterator, typename assess_iterator>
	roulette_removal_sampler<InputIterator> make_roulette_removal_sampler(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
		return roulette_removal_sampler<InputIterator>(Begin_, End_,  ABeg_, AEnd_);
	}
}
#
#endif
