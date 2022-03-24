#ifndef HMLIB_ODEINT_OBSERVER_OBSERVESKIPPER_INC
#define HMLIB_ODEINT_OBSERVER_OBSERVESKIPPER_INC 100
#
#include<type_traits>
namespace hmLib {
	namespace odeint {
		template<typename observer>
		class observe_skipper {
			observer Obs;
			unsigned int cnt;
			unsigned int interval;
		public:
			template<typename observer_>
			observe_skipper(observer_&& Obs_, unsigned int interval_) :Obs(std::forward<observer_>(Obs_)), cnt(0), interval(interval_) {}
			void reset() { cnt = 0; }
			template<typename state_type, typename time_type>
			void operator()(const state_type& x, time_type t) {
				if (++cnt >= interval) {
					cnt = 0;
					Obs(x, t);
				}
			}
		};
		template<typename observer>
		auto observe_skip(observer&& obs, unsigned int interval) { return observe_skipper<typename std::decay<observer>::type>(std::forward<observer>(obs), interval); }
	}
}
#
#endif
