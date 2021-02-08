#ifndef HMLIB_ODEINT_OBSERVAERPACK_INC
#define HMLIB_ODEINT_OBSERVAERPACK_INC 100 
#
#include<utility>
#include"../../tuple.hpp"
#include"null_observer.hpp"
namespace hmLib{
	namespace odeint{
		template<typename observer, typename... others>
		struct observer_pack {
			using pack = std::tuple<observer, others...>;
		private:
			pack Pack;
		public:
			observer_pack(observer&& Obs, others&&... Others) :Pack(std::forward<observer>(Obs), std::forward<others>(Others)...) {}
			template<typename state_type, typename time_type>
			void operator()(const state_type& State, time_type Time) {
				hmLib::tuple_foreach(Pack, [x = &State, t = &Time](auto& obs) {obs(x, t); });
			}
		};
		template<typename observer, typename... others>
		auto make_observer_pack(observer&& Obs, others&&... Others) {
			return observer_pack<observer, others...>(std::forward<observer>(Obs),std::forward<others>(Others)...);
		}
	}
}
#
#endif
