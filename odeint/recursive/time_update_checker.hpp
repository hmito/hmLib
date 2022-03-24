#ifndef HMLIB_ODEINT_RECURSIVE_TIMECHECKER_INC
#define HMLIB_ODEINT_RECURSIVE_TIMECHECKER_INC 100
#
#include"../../exceptions.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			template<typename time_type>
			struct time_update_checker {
				time_type Last;
				unsigned int Fail;
				unsigned int MaxFail;
			public:
				time_update_checker(time_type IniTime_,unsigned int MaxFail_ = 50) :Last(IniTime_), Fail(0), MaxFail(MaxFail_) {}
				void operator()(time_type Time) {
					if (Last == Time) {
						++Fail;
						if (Fail >= MaxFail)hmLib_throw(hmLib::exceptions::exception, "recursive cannot detect time update multiple times.");
					}
					else {
						Fail = 0;
					}
					Last = Time;
				}
				void reset(time_type IniTime_) { Last = IniTime_; Fail = 0; }
			};
		}
	}
}
#
#endif

