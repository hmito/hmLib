#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#define HMLIB_ALGORITHM_SAMPLING_INC 300
#
#include<algorithm>
#include<numeric>
#include<vector>
#include<random>
#include"../exceptions.hpp"
namespace hmLib{
	template<typename InputIterator, typename RandEngine>
	InputIterator random_sample(InputIterator Begin,InputIterator End, RandEngine&& Engine){
		if(Begin == End)return End;
		return std::next(Begin, std::uniform_int_distribution<int>(0,std::distance(Begin,End)-1)(Engine));
	}
	template<typename InputIterator, typename OutputIterator, typename RandEngine>
	void random_sample(InputIterator Begin,InputIterator End,OutputIterator OutBegin,OutputIterator OutEnd, RandEngine&& Engine){
		if(Begin == End)return;

		std::uniform_int_distribution<int> Dist(0, std::distance(Begin, End) - 1);
		while(OutBegin!=OutEnd){
			*OutBegin++ = *std::next(Begin, Dist(Engine));
		}
	}
	template<typename InputIterator, typename OutputIterator, typename RandEngine>
	OutputIterator random_sample(InputIterator Begin,InputIterator End, OutputIterator Out, unsigned int n, RandEngine&& Engine){
		if(Begin == End)return Out;

		std::uniform_int_distribution<int> Dist(0, std::distance(Begin, End) - 1);
		for(unsigned int i = 0; i < n; ++i){
			*Out++ = *std::next(Begin, Dist(Engine));
		}
		return Out;
	}

	template<typename InputIterator>
	class random_sampler{
		using dist_type = std::uniform_int_distribution<int>;
	private:
		InputIterator Begin;
		dist_type Dist;
	public:
		random_sampler(){}
		random_sampler(InputIterator Begin_,InputIterator End_){sync(Begin_,End_);}
	public:
		template<typename RandEngine>
		InputIterator operator()(RandEngine Engine){
			return std::next(Begin, Dist(Engine));
		}
		void reset(InputIterator Begin_,InputIterator End_){
			Begin=Begin_;

			dist_type::param_type prm(0, std::distance(Begin_, End_)-1);
			Dist.param(prm);
		}
	};
	template<typename InputIterator>
	random_sampler<InputIterator> make_random_sampler(InputIterator Begin_,InputIterator End_){
		return random_sampler<InputIterator>(Begin_,End_);
	}

	template<typename InputIterator, typename fnAssess, typename RandEngine>
	InputIterator roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess, RandEngine&& Engine){
		if(Begin == End)return End;

		double TotalFitness = 0;
		std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

		double SelectFitness =std::uniform_real_distribution<double>(0.,TotalFitness)(Engine);
		for(;Begin!=End; ++Begin){
			SelectFitness-= FnAssess(*Begin);
			if(SelectFitness <= 0.)break;
		}
		return Begin;
	}
	template<typename InputIterator, typename OutputIterator, typename fnAssess, typename RandEngine>
	void roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess,OutputIterator OutBegin,OutputIterator OutEnd, RandEngine&& Engine){
		if(Begin == End)return;

		double TotalFitness = 0;
		std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

		std::uniform_real_distribution<double> Dist(0., TotalFitness);

		while(OutBegin != OutEnd){
			double SelectFitness = Dist(Engine);
			auto Itr = Begin;
			for(; Itr != End; ++Itr){
				SelectFitness -= FnAssess(*Itr);
				if(SelectFitness <= 0.)break;
			}
			*OutBegin++ = *Itr;
		}
	}
	template<typename InputIterator, typename OutputIterator, typename fnAssess, typename RandEngine>
	OutputIterator roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess, OutputIterator Out, unsigned int n, RandEngine&& Engine){
		if(Begin == End)return Out;

		double TotalFitness = 0;
		std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

		std::uniform_real_distribution<double> Dist(0., TotalFitness);

		for(unsigned int i = 0; i < n; ++i){
			double SelectFitness = Dist(Engine);
			auto Itr = Begin;
			for(; Itr != End; ++Itr){
				SelectFitness -= FnAssess(*Itr);
				if(SelectFitness <= 0.)break;
			}
			*Out++ = *Itr;
		}

		return Out;
	}

	template<typename InputIterator, typename assess_iterator, typename RandEngine>
	InputIterator roulette_sample(InputIterator Begin, InputIterator End, assess_iterator ABegin, assess_iterator AEnd, RandEngine&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(ABegin, AEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;

		double TotalWeight = std::accumulate(ABegin,AEnd, 0.0);
		double Selected = std::uniform_real_distribution<double>(0., TotalWeight)(Engine);
		for(auto AItr = ABegin; AItr!=AEnd; ++AItr) {
			Selected -= *AItr;
			if(Selected <= 0.)return std::next(Begin, std::distance(ABegin, AItr));
		}
		return End;
	}
	template<typename InputIterator, typename OutputIterator, typename assess_iterator, typename RandEngine>
	void roulette_sample(InputIterator Begin, InputIterator End, assess_iterator ABegin, assess_iterator AEnd, OutputIterator OutBegin, OutputIterator OutEnd, RandEngine&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(ABegin, AEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;

		double TotalWeight = std::accumulate(ABegin, AEnd, 0.0);

		for(; OutBegin!=OutEnd; ++OutBegin) {
			double Selected = std::uniform_real_distribution<double>(0., TotalWeight)(Engine);
			for(auto AItr = ABegin; AItr!=AEnd; ++AItr) {
				Selected -= *AItr;
				if(Selected <= 0.) {
					*(OutBegin++) = std::next(Begin, std::distance(ABegin, AItr));
					break;
				}
			}		
		}
	}
	template<typename InputIterator, typename OutputIterator, typename assess_iterator, typename RandEngine>
	OutputIterator roulette_sample(InputIterator Begin, InputIterator End, assess_iterator ABegin, assess_iterator AEnd, OutputIterator Out, unsigned int n, RandEngine&& Engine) {
		hmLib_assert(std::distance(Begin, End) == std::distance(ABegin, AEnd), hmLib::numeric_exceptions::invalid_initialvalue, "Given pair of iterators has different distance.");
		if(Begin == End)return End;

		double TotalWeight = std::accumulate(ABegin, AEnd, 0.0);
		
		for(unsigned int i = 0; i<n; ++i) {
			double Selected = std::uniform_real_distribution<double>(0., TotalWeight)(Engine);
			for(auto AItr = ABegin; AItr!=AEnd; ++AItr) {
				Selected -= *AItr;
				if(Selected <= 0.) {
					*(Out++) = std::next(Begin, std::distance(ABegin, AItr));
					break;
				}
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
		template<typename RandEngine>
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
	template<typename InputIterator, typename RandEngine>
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
