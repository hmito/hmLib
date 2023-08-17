#ifndef HMLIB_RECUR_BREAKER_TIMELIMITBREAKER_INC
#define HMLIB_RECUR_BREAKER_TIMELIMITBREAKER_INC 100
#
namespace hmLib {
	namespace recur{
		template<typename time_type>
		struct time_limit_breaker {
		private:
			time_type limit;
		public:
			time_limit_breaker(time_type limit_):limit(limit_){}
			template<typename state_type,typename other_time_type>
			bool operator()(const state_type&, other_time_type t)const {
				return limit <= t;
			}
			time_type get_limit()const{ return limit; }
		};
	}
}
#
#endif
