#ifndef HMLIB_ODEINT_BREAKER_BREAKERPACK_INC
#define HMLIB_ODEINT_BREAKER_BREAKERPACK_INC 100 
#
#include<utility>
#include"../../tuple.hpp"
#include"../observer/null_observer.hpp"
namespace hmLib{
	namespace odeint{
		template<typename breaker, typename... others>
		struct breaker_pack {
			using pack = std::tuple<breaker, others...>;
		private:
			pack Pack;
		public:
			template<typename breaker_, typename... others_>
			breaker_pack(breaker_ Brk, others_... Others) :Pack(std::forward<breaker_>(Brk), std::forward<others_>(Others)...) {}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& State, time_type Time) {
				return hmLib::tuple_reduce(Pack, false, [x = &State, t = &Time](bool Ini, auto& brk) {return Ini || brk(x, t); });
			}
		};
		template<typename breaker, typename... others>
		auto make_breaker_pack(breaker&& Brk, others&&... Others) {
			return breaker_pack<breaker, others...>(std::forward<breaker>(Brk),std::forward<others>(Others)...);
		}
	}
}
#
#endif
