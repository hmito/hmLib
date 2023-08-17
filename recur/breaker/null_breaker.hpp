#ifndef HMLIB_RECUR_BREAKER_NULLBREAKER_INC
#define HMLIB_RECUR_BREAKER_NULLBREAKER_INC 100
#
namespace hmLib {
	namespace recur{
		struct null_breaker {
			template<typename state_type,typename time_type>
			constexpr bool operator()(const state_type&, time_type)const noexcept{
				return false;
			}
		};
	}
}
#
#endif
