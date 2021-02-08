#ifndef HMLIB_ODEINT_STATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_STATEBREAKOBSERVER_INC 100
#
#include <array>
#include "../utility.hpp"
namespace hmLib{
	namespace odeint{
		//not ready for use: should be changed from old-style breaker (breakable-observer) to separated breaker, but have not changed as container reference style.
		/* 
		namespace detail{
			template<typename state_container_>
			struct same_state_breaker{
			public:
				using state_container = state_container_;
			private:
				state_container& Container;
				unsigned int Cnt;
				unsigned int Interval;
				unsigned int IgnoreNum;
				double Error;
			public:
				same_state_breaker(state_container& Container, unsigned int Interval_,unsigned int IgnoreNum_, double Error_) :Cnt(0), Interval(Interval_), IgnoreNum(IgnoreNum_), Error(Error_){}
				template<typename state_type,typename time_type>
				bool operator()(const state_type& x, time_type t){
					if(++Cnt < Interval)return false;
					Cnt = 0;

					auto Beg = std::rbegin(Container);
					auto End = std::rend(Container);

					if(End - Beg <= IgnoreNum)return false;
					Beg += IgnoreNum;

					for(auto Itr = Beg; Itr != End; ++Itr){
						if(hmLib::odeint::distance_norm_inf(x, Itr->second) < Error){
							return true;
						}
					}

					return false;
				}
			};
			struct cross_state_breaker{
			private:
				unsigned int Cnt;
				unsigned int Interval;
			private:
				template<typename state>
				bool is_cross_segment(const state& a1, const state& a2, const state& b1, const state& b2){
					if(a1[0] >= a2[0]){
						if((a1[0] < b1[0] && a1[0] < b2[0]) || (a2[0] > b1[0] && a2[0] > b2[0])){
							return false;
						}
					} else{
						if((a2[0] < b1[0] && a2[0] < b2[0]) || (a1[0] > b1[0] && a1[0] > b2[0])){
							return false;
						}
					}
					if(a1[1] >= a2[1]){
						if((a1[1] < b1[1] && a1[1] < b2[1]) || (a2[1] > b1[1] && a2[1] > b2[1])){
							return false;
						}
					} else{
						if((a2[1] < b1[1] && a2[1] < b2[1]) || (a1[1] > b1[1] && a1[1] > b2[1])){
							return false;
						}
					}

					if(((a1[0] - a2[0]) * (b1[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b1[0])) *
						((a1[0] - a2[0]) * (b2[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b2[0])) > 0){
						return false;
					}
					if(((b1[0] - b2[0]) * (a1[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a1[0])) *
						((b1[0] - b2[0]) * (a2[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a2[0])) > 0){
						return false;
					}
					return true;
				}
			public:
				cross_state_breaker(unsigned int Interval_):Cnt(0),Interval(Interval_){}
				template<typename state_type, typename time_type>
				bool operator()(const state_type& x, time_type t){
					if(++Cnt < Interval)return false;
					Cnt = 0;

					auto Beg = Observer.rbegin();
					auto End = Observer.rend();

					state y = Beg->second;
					state p = Beg->second;
					++Beg;

					for(; Beg != End; ++Beg){
						if(is_cross_segment(x, y, p, Beg->second))return true;
						p = Beg->second;
					}
					return false;
				}
			};
		}
		*/
	}
}
#
#endif
