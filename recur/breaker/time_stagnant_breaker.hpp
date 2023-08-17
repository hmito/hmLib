#ifndef HMLIB_RECUR_BREAKER_TIMESTAGNANTBREAKER_INC
#define HMLIB_RECUR_BREAKER_TIMESTAGNANTBREAKER_INC 100
#
#include"../../exceptions.hpp"
namespace hmLib {
    namespace recur{
		template<typename time_type>
		struct time_stagnant_breaker {
			time_type last;
			unsigned int count;
			unsigned int limit;
		public:
			time_stagnant__breaker(time_type IniTime_, unsigned int limit_ = 1) 
				: last(IniTime_), count(0), limit(limit_) {}
			template<typename state_type>
			bool operator()(const state_type&, time_type t) {
				if (last == t) {
					return (++count >= limit);
				}				
				last = t;
				count = 0;
				return false;
			}
			void reset(time_type IniTime_ ) { last = IniTime_; count = 0; }
		};
	}
}
#
#endif
