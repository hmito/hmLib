#ifndef HMLIB_ODEINT_VALIDATE_NONAPPENDIXOBSERVER_INC
#define HMLIB_ODEINT_VALIDATE_NONAPPENDIXOBSERVER_INC 100
#
#include<type_traits>
namespace hmLib {
	namespace odeint {
		template<typename base_observer_>
		struct non_appendix_observer {
			non_appendix_observer(base_observer_ obs_) :obs(obs_) {}
			template<typename state_type, typename time_type>
			void operator()(const state_type& x, time_type t) {
				obs(std::get<0>(x), t);
			}
		private:
			base_observer_ obs;
		};
		template<typename base_observer_>
		auto make_non_appendix_observer(base_observer_&& obs_) {
			using base_observer = typename std::decay<base_observer_>::type;
			return non_appendix_observer< base_observer>(std::forward < base_observer_>(obs_));
		}
	}
}
#
#endif
