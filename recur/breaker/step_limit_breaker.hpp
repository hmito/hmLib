#ifndef HMLIB_RECUR_BREAKER_STEPLIMITBREAKER_INC
#define HMLIB_RECUR_BREAKER_STEPLIMITBREAKER_INC 100
#
namespace hmLib {
	namespace recur{
		struct step_limit_breaker {
		private:
			unsigned int count;
			unsigned int limit;
		public:
			step_limit_breaker(unsigned int limit_):limit(limit_){}
			template<typename state_type,typename time_type>
			bool operator()(const state_type&, time_type t) {
				return ++count >= limit_;
			}
			void reset(){count = 0;}
			unsigned int get_count()const{ return count; }
			unsigned int get_limit()const{ return limit; }
		};
	}
}
#
#endif
