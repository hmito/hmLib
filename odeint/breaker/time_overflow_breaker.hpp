#ifndef HMLIB_ODEINT_BREAKER_TIMEOVERFLOWBREAKER_INC
#define HMLIB_ODEINT_BREAKER_TIMEOVERFLOWBREAKER_INC 100
#
namespace hmLib {
	namespace odeint {
		template<typename time_type = double>
		struct time_overflow_breaker {
		private:
			time_type overflow;
		public:
			time_overflow_breaker(time_type overflow_):overflow(overflow_){}
			template<typename state_type>
			bool operator()(const state_type&, time_type t) {
				return overflow <= t;
			}
		};
	}
}
#
#endif
