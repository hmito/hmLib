#ifndef HMLIB_ODEINT_BREAKER_BREAKSKIPPER_INC
#define HMLIB_ODEINT_BREAKER_BREAKSKIPPER_INC 100
#
#include<type_traits>
namespace hmLib{
	namespace odeint{
		template<typename breaker>
		class break_skipper {
			breaker brk;
			unsigned int cnt;
			unsigned int interval;
		public:
			template<typename breaker_>
			break_skipper(breaker_&& brk_, unsigned int interval_) :brk(std::forward<observer_>(brk_)), cnt(0), interval(interval_) {}
			void reset() { cnt = 0; }
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if (++cnt >= interval) {
					cnt = 0;
					return brk(x, t);
				}
				return false;
			}
		};
		template<typename breaker>
		auto break_skip(breaker&& brk, unsigned int interval) { return break_skipper<typename std::decay<breaker>::type>(std::forward<breaker>(brk), interval); }
	}
}
#
#endif
