#ifndef HMLIB_RECUR_OBSERVER_NULLOBSERVER_INC
#define HMLIB_RECUR_OBSERVER_NULLOBSERVER_INC 100
#
namespace hmLib {
	namespace recur {
		struct null_observer{
			template<typename state_type, typename time_type>
			void operator()(const state_type&, time_type) const{}
		};
	}
}
#
#endif
