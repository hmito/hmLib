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
	std::size_t random_index_sample(Iterator Begin,Iterator End, URBG&& Engine){
		if(Begin == End)return 0;
		return std::uniform_int_distribution<std::size_t>(0,std::distance(Begin,End)-1)(Engine);
	}
	template<std::forward_iterator Iterator, std::output_iterator OIterator, std::uniform_random_bit_generator URBG>
	OIterator random_index_sample(Iterator Begin,Iterator End, OIterator Out, std::size_t n, URBG&& Engine){
		if(Begin == End)return Out;

		std::uniform_int_distribution<std::size_t> Dist(0, std::distance(Begin, End) - 1);
		for(std::size_t i = 0; i < n; ++i){
			*Out++ = Dist(Engine);
		}
		return Out;
	}
	class random_index_sampler{
		using dist_type = std::uniform_int_distribution<std::size_t>;
	private:
		dist_type Dist;
	public:
		random_index_sampler()=default;
		template<std::forward_iterator Iterator>
		random_index_sampler(Iterator Begin_,Iterator End_){reset(Begin_,End_);}
	public:
		template<typename std::uniform_random_bit_generator URBG>
		std::size_t operator()(URBG Engine){
			return Dist(Engine);
		}
		template<std::forward_iterator Iterator>
		void reset(Iterator Begin_,Iterator End_){
			if(Begin_==End_){
				dist_type::param_type prm(0, 0);
				Dist.param(prm);
			}else{
				dist_type::param_type prm(0, std::distance(Begin_, End_)-1);
				Dist.param(prm);
			}
		}
	};
	template<typename InputIterator>
	auto make_random_index_sampler(InputIterator Begin_,InputIterator End_){
		return random_index_sampler<InputIterator>(Begin_,End_);
	}

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
		random_index_sampler IndexSampler;
	public:
		random_sampler()=default;
		random_sampler(Iterator Begin_,Iterator End_){reset(Begin_,End_);}
	public:
		template<typename RandEngine>
		Iterator operator()(RandEngine Engine){
			return std::next(Begin, IndexSampler(Engine));
		}
		void reset(Iterator Begin_,Iterator End_){
			Begin=Begin_;
			IndexSampler.reset(Begin_,End_);
		}
	};
	template<std::forward_iterator Iterator>
	auto make_random_sampler(Iterator Begin_,Iterator End_){
		return random_sampler<Iterator>(Begin_,End_);
	}

	template<std::forward_iterator Iterator, std::uniform_random_bit_generator URBG>
	std::size_t roulette_index_sample(Iterator Begin, Iterator End, URBG&& Engine) {
		if(Begin==End)return 0;
		using value_type = std::iter_value_t<Iterator>;
		value_type TotalWeight = std::accumulate(Begin, End, value_type{});
		return roulette_index_sample(Begin, End, std::forward<URBG>(Engine), TotalWeight);
	}
	template<std::forward_iterator Iterator, std::uniform_random_bit_generator URBG, typename value_type>
	std::size_t roulette_index_sample(Iterator Begin, Iterator End, URBG&& Engine, value_type TotalWeight) {
		if(Begin==End)return 0;
		value_type Selected = std::uniform_real_distribution<value_type>(0, TotalWeight)(Engine);
		for(auto Itr = Begin; Itr!=End; ++Itr) {
			Selected -= *Itr;
			if(Selected <= 0.)return std::distance(Begin, Itr);
		}
		return std::distance(Begin, End);
	}
	template<std::forward_iterator Iterator, std::output_iterator OIterator, std::uniform_random_bit_generator URBG>
	OutputIterator roulette_index_sample(Iterator Begin, Iterator End, OIterator Out, std::size_t n, URBG&& Engine) {
		if(Begin==End)return Out;
		using value_type = std::iter_value_t<Iterator>;
		value_type TotalWeight = std::accumulate(Begin, End, value_type{});
		return roulette_index_sample(Begin,End,Out,n,Engine,TotalWeight);
	}
	template<std::forward_iterator Iterator, std::output_iterator OIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_index_sample(Iterator Begin, Iterator End, OIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeigh) {
		if(Begin==End)return Out;
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
			for(auto Itr = Begin; Itr!=End; ++Itr) {
				Selected -= *Itr;
				if(Selected <= 0.) {
					Out++ = std::distance(Begin, Itr);
					break;
				}
			}
			if(Selected>0){
				*Out++  = std::distance(Begin, End);
			}
		}
		return Out;
	}
	template<typename value_type>
	class roulette_index_sampler{
	private:
		std::vector<value_type> WeightVec;
	public:
		roulette_index_sampler()=default;
		roulette_index_sampler(InputIterator Begin_,InputIterator End_){reset(Begin_,End_);}
	public:
		template<std::uniform_random_bit_generator URB>
		std::size_t operator()(URB&& Engine){
			if(WeightVec.empty())return 0;
			return std::distance(WeightVec.begin(),std::lower_bound(
				WeightVec.begin(),WeightVec.end(),std::uniform_real_distribution<value_type>(0,WeightVec.back())(Engine)
			));
		}
		void reset(){WeightVec.clear();}
		template<std::forward_iterator Iterator>
		void reset(Iterator Begin_,Iterator End_){
			WeightVec.clear();
			value_type Val=0;
			for(;Begin_!=End_;++Begin_){
				Val+=*Begin_;
				WeightVec.push_back(Val);
			}
		}
		template<std::random_access_iterator Iterator>
		void reset(Iterator Begin_,Iterator End_){
			WeightVec.clear();
			WeightVec.reserve(End_-Begin_);
			value_type Val=0;
			for(;Begin_!=End_;++Begin_){
				Val+=*Begin_;
				WeightVec.push_back(Val);
			}
		}
	};
	template<std::forward_iterator Iterator>
	auto make_roulette_index_sampler(Iterator Begin_,Iterator End_){
		return roulette_index_sampler<std::decay_t<decltype(*Begin_)>>(Begin_,End_);
	}

	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::uniform_random_bit_generator URBG>
	SampleIterator roulette_sample(SampleIterator Begin, SampleIterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;
		return std::next(Begin,roulette_index_sample(WeightBegin,WeightEnd,Engine));
	}
	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::uniform_random_bit_generator URBG, typename value_type>
	SampleIterator roulette_sample(SampleIterator Begin, SampleIterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;
		return std::next(Begin,roulette_index_sample(WeightBegin,WeightEnd,Engine,TotalWeight));
	}
	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG>
	OutputIterator roulette_sample(SampleIterator Begin, SampleIterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return Out;
		using value_type = std::iter_value_t<WeightIterator>;
		value_type TotalWeight = std::accumulate(WeightBegin, WeightEnd, value_type{});
		return roulette_index_sample(Begin,End,WeightBegin, WeightEnd,Out,n,Engine,TotalWeight);
	}
	template<std::forward_iterator SampleIterator, std::forward_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_sample(SampleIterator Begin, SampleIterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
			auto Itr = Begin;
			for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
				Selected -= *WeightItr;
				if(Selected <= 0.) {
					Out++ = Itr;
					break;
				}
				++Itr;
			}
			if(Selected>0){
				*Out++  = End;
			}
		}
		return Out;
	}
	template<std::random_access_iterator SampleIterator, std::random_access_iterator WeightIterator, std::output_iterator OutputIterator, std::uniform_random_bit_generator URBG, typename value_type>
	OutputIterator roulette_sample(SampleIterator Begin, SampleIterator End, WeightIterator WeightBegin, WeightIterator WeightEnd, OutputIterator Out, std::size_t n, URBG&& Engine, value_type TotalWeight) {
		hmLib_assert(std::distance(Begin, End) == std::distance(WeightBegin, WeightEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		for(std::size_t i = 0;i<n;++i){
			value_type Selected = std::uniform_real_distribution<value_type>(0., TotalWeight)(Engine);
			for(auto WeightItr = WeightBegin; WeightItr!=WeightEnd; ++WeightItr) {
				Selected -= *WeightItr;
				if(Selected <= 0.) {
					Out++ = std::next(Begin, std::distance(WeightBegin, WeightItr));
					break;
				}
			}
			if(Selected>0){
				*Out++  = End;
			}
		}
		return Out;
	}
	template<typename InputIterator>
	class roulette_sampler{
		struct assess_itr{
			InputIterator Itr;
			double Val;
		public:
			assess_itr(double Val_):Val(Val_){}
			assess_itr(InputIterator Itr_,double Val_):Itr(Itr_),Val(Val_){}
			bool operator<(const assess_itr& My_)const{return Val<My_.Val;}
		};
	private:
		std::vector<assess_itr> AssessVec;
	public:
		roulette_sampler(){}
		template<typename fnAssess>
		roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess&& FnAssess_){reset(Begin_,End_, std::forward<fnAssess>(FnAssess_));}
		template<typename assess_iterator>
		roulette_sampler(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) { reset(Begin_, End_, ABeg_, AEnd_); }
	public:
		template<typename RandEngine>
		InputIterator operator()(RandEngine&& Engine){
			return std::lower_bound(
				AssessVec.begin()
				,AssessVec.end()
				,assess_itr(InputIterator(), std::uniform_real_distribution<double>(0.,assess_total())(Engine))
			)->Itr;
		}
		void reset() { AssessVec.clear(); }
		template<typename fnAssess>
		void reset(InputIterator Begin_,InputIterator End_, fnAssess&& FnAssess_){
			double Val=0.;
			AssessVec.clear();
			while(Begin_!=End_){
				Val+=FnAssess_(*Begin_);
				AssessVec.push_back(assess_itr(Begin_++,Val));
			}
		}
		template<typename assess_iterator>
		void reset(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(ABeg_, AEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			double Val = 0.;
			AssessVec.clear();
			while(Begin_!=End_) {
				Val += *ABeg_;
				AssessVec.push_back(assess_itr(Begin_, Val));
				++Begin_;
				++ABeg_;
			}
		}
		template<typename fnAssess>
		void push(InputIterator Begin_, InputIterator End_, fnAssess&& FnAssess_) {
			double Val = 0.0;
			if(!empty())Val = AssessVec.back().Val;
			while(Begin_!=End_) {
				Val += FnAssess_(*Begin_);
				AssessVec.push_back(assess_itr(Begin_++, Val));
			}
		}
		template<typename assess_iterator>
		void push(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
			hmLib_assert(std::distance(Begin_, End_) == std::distance(ABeg_, AEnd_), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
			double Val = 0.0;
			if(!empty())Val = AssessVec.back().Val;
			while(Begin_!=End_) {
				Val += *ABeg_;
				AssessVec.push_back(assess_itr(Begin_, Val));
				++Begin_;
				++ABeg_;
			}
		}
		bool empty()const { return AssessVec.empty(); }
		unsigned int size()const{return AssessVec.size();}
	private:
		double assess_total() { if(empty())return 0.0; return AssessVec.back().Val; }
	};
	template<typename InputIterator,typename fnAssess>
	roulette_sampler<InputIterator> make_roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess&& FnAssess_){
		return roulette_sampler<InputIterator>(Begin_,End_,std::forward<fnAssess>(FnAssess_));
	}
	template<typename InputIterator, typename assess_iterator>
	roulette_sampler<InputIterator> make_roulette_sampler(InputIterator Begin_, InputIterator End_, assess_iterator ABeg_, assess_iterator AEnd_) {
		return roulette_sampler<InputIterator>(Begin_, End_, ABeg_, AEnd_);
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
