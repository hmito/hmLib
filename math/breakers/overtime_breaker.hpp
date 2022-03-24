#ifndef HMLIB_BREAKERS_OVERTIMEBREAKER_INC
#define HMLIB_BREAKERS_OVERTIMEBREAKER_INC 100
#
namespace hmLib {
    namespace breakers{
        template<typename time_type>
        struct overtime_breaker {
        private:
            time_type limit;
        public:
            overtime_breaker(time_type limit_):limit(limit_){}
            template<typename state_type>
            bool operator()(const state_type&, time_type t)const {
                return limit <= t;
            }
        };
    }
}
#
#endif
