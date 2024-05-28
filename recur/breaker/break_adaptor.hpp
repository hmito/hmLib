#ifndef HMLIB_RECUR_BREAKER_BREAKADAPTER_INC
#define HMLIB_RECUR_BREAKER_BREAKADAPTER_INC 100
#
#include<type_traits>
namespace hmLib{
	namespace recur{
		template<typename breaker>
		class break_skipper {
			breaker brk;
			unsigned int cnt;
			unsigned int interval;
		public:
			template<typename breaker_>
			break_skipper(breaker_&& brk_, unsigned int interval_) 
				: brk(std::forward<breaker_>(brk_))
				, cnt(0)
				, interval(interval_) {
			}
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

		template<typename breaker>
		class break_counter {
			breaker brk;
			unsigned int cntbrk;
			unsigned int maxbrk;
		public:
			template<typename breaker_>
			break_counter(breaker_&& brk_, unsigned int maxbrk_) 
				: brk(std::forward<breaker_>(brk_))
				, cntbrk(0)
				, maxbrk(maxbrk_) {
			}
			void reset() { cntbrk = 0; }
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if (brk(x, t)) {
					return ++cntbrk >= maxbrk;
				}
				cntbrk = 0;
				return false;
			}
		};
		template<typename breaker>
		auto break_count(breaker&& brk, unsigned int maxbrk) { return break_counter<typename std::decay<breaker>::type>(std::forward<breaker>(brk), maxbrk); }

		template<typename breaker,typename exception>
		class break_thrower {
			breaker brk;
			exception excp;
		public:
			template<typename breaker_,typename exception_>
			break_thrower(breaker_&& brk_, exception_&& excp_) 
				: brk(std::forward<breaker_>(brk_))
				, excp(std::forward<exception_>(excp_)){
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if(brk(x, t)) throw excp;
				return false;
			}
		};
		template<typename breaker,typename exception>
		auto break_throw(breaker&& brk, exception&& excp) { return break_thrower<typename std::decay<breaker>::type,typename std::decay<exception>::type>(std::forward<breaker>(brk), std::forward<exception>(excp)); }
	}
}
#
#endif
