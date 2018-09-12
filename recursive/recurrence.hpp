#ifndef HMLIB_RECURSIVE_PROGRESS_INC
#define HMLIB_RECURSIVE_PROGRESS_INC 100
#
namespace hmLib{
	namespace recursive {
		template<typename system_type,typename state_type, typename time_type>
		void recurrence(system_type& System, state_type& State, time_type BeginStep, time_type EndStep){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
			}
		}
		template<typename system_type, typename state_type, typename time_type, typename observer>
		void recurrence(system_type& System, state_type& State, time_type BeginStep, time_type EndStep, observer Observer){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
				Observer(State, BeginStep);
			}
		}
	}
}
#
#endif
