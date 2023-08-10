#ifndef HMLIB_RECUR_BREAKER_WATCHDOGBREAKER_INC
#define HMLIB_RECUR_BREAKER_WATCHDOGBREAKER_INC 100
#
#include"../../exceptions.hpp"
namespace hmLib {
    namespace recur{
		template<typename time_type>
		struct watchdog_throw {
			time_type Last;
			unsigned int Fail;
			unsigned int MaxFail;
		public:
			watchdog_throw(unsigned int MaxFail_ = 50,time_type IniTime_ = 0) :Last(IniTime_), Fail(0), MaxFail(MaxFail_) {}
			template<typename state_type>
			bool operator()(const state_type&, time_type Time) {
				if (Last == Time) {
					if (++Fail >= MaxFail)hmLib_throw(hmLib::exceptions::exception, "recursive cannot detect time update multiple times.");
				} else {
					Last = Time;
					Fail = 0;
				}
				return false;
			}
			void reset(time_type IniTime_ = 0) { Last = IniTime_; Fail = 0; }
		};
		template<typename time_type>
		struct watchdog_breaker {
			time_type Last;
			unsigned int Fail;
			unsigned int MaxFail;
		public:
			watchdog_breaker(unsigned int MaxFail_ = 50,time_type IniTime_ = 0) :Last(IniTime_), Fail(0), MaxFail(MaxFail_) {}
			bool operator()(const state_type&, time_type Time) {
				if (Last == Time) {
					return (++Fail >= MaxFail);
				}				
				Last = Time;
				Fail = 0;
				return false;
			}
			void reset(time_type IniTime_ = 0) { Last = IniTime_; Fail = 0; }
		};
	}
}
#
#endif
