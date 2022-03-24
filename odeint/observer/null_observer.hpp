#ifndef HMLIB_ODEINT_OBSERVER_NULLOBSERVER_INC
#define HMLIB_ODEINT_OBSERVER_NULLOBSERVER_INC 100
#
namespace hmLib {
	namespace odeint {
		struct null_observer{
			template<typename state_type, typename time_type>
			void operator()(const state_type&, time_type) const{}
		};
	}
}
#
#endif
