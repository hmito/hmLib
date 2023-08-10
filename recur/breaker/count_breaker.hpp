#ifndef HMLIB_RECUR_BREAKER_COUNTBREAKER_INC
#define HMLIB_RECUR_BREAKER_COUNTBREAKER_INC 100
#
namespace hmLib {
    namespace recur{
        template<typename time_type>
        struct count_breaker {
        private:
            unsigned int count;
            unsigned int maxcount;
        public:
            count_breaker(unsigned int maxcount_):maxcount(maxcount_){}
            template<typename state_type>
            bool operator()(const state_type&, time_type t) {
                return ++count >= maxcount;
            }
            void reset(){count = 0;}
        };
    }
}
#
#endif
