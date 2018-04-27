#ifndef HMLIB_ODEINT_EQSTATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_EQSTATEBREAKOBSERVER_INC 100
#
#include <array>
#include <boost/geometry.hpp>
#include "container_observer.hpp"
#include "break_observer.hpp"
#include "../utility.hpp"
namespace hmLib{
	namespace odeint{
		namespace detail{
			template<typename state_, typename time_>
			struct eqstate_breaker{
			public:
				using state = state_;
				using time = time_;
			private:
				unsigned int Cnt;
				unsigned int Interval;
				unsigned int IgnoreNum;
				double Error;
			public:
				eqstate_breaker(unsigned int Interval_,unsigned int IgnoreNum_, double Error_) :Cnt(0), Interval(Interval_), IgnoreNum(IgnoreNum_), Error(Error_){}
				bool operator()(const state& x, time t, container_observer<state,time>& Observer){
					if(++Cnt < Interval)return false;
					Cnt = 0;

					auto Beg = Observer.rbegin();
					auto End = Observer.rend();

					if(End - Beg <= IgnoreNum)return false;
					Beg += IgnoreNum;

					for(auto Itr = Beg; Itr != End; ++Itr){
						if(hmLib::odeint::detail::abs_distance(x, Itr->second) < Error){
							return true;
						}
					}

					return false;
				}
			};
			template<typename time_>
			struct eqstate_breaker < std::array<double, 2> , time_>{
				using state = std::array<double, 2>;
				using time = time_;
			private:
				unsigned int Cnt;
				unsigned int Interval;
			private:
				bool is_cross_segment(std::array<double, 2> a1, std::array<double, 2> a2, std::array<double, 2> b1, std::array<double, 2> b2){
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
				eqstate_breaker(unsigned int Interval_):Cnt(0),Interval(Interval_){}
				bool operator()(const state& x, time t, container_observer<state, time>& Observer){
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
		template<typename state_, typename time_=double>
		using eqstate_break_observer = observer_based_break_observer<container_observer<state_, time_>, detail::eqstate_breaker<state_,time_> >;
	}
}
#
#endif
