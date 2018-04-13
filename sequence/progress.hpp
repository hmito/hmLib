#ifndef HMLIB_SEQUENCE_PROGRESS_INC
#define HMLIB_SEQUENCE_PROGRESS_INC 100
#
namespace hmLib{
	namespace sequence{
		template<typename system_type,typename state_type, typename time_type>
		void progress(system_type& System, state_type& State, time_type BeginStep, time_type EndStep){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
			}
		}
		template<typename system_type, typename state_type, typename time_type, typename observer>
		void progress(system_type& System, state_type& State, time_type BeginStep, time_type EndStep, observer Observer){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
				Observer(State, BeginStep);
			}
		}
	}
}
#
#endif
