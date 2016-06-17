#ifndef HMLIB_SEQUENCE_INC
#define HMLIB_SEQUENCE_INC 100
#
namespace hmLib{
	namespace sequence{
		template<typename system,typename state>
		void progress(system System, state& State, unsigned int BeginStep, unsigned int EndStep){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
			}
		}
		template<typename system, typename state, typename observer>
		void progress(system System, state& State, unsigned int BeginStep, unsigned int EndStep, observer Observer){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
				Observer(State, BeginStep);
			}
		}
		template<typename system, typename state, typename interruptable_observer>
		void interruptable_progress(system System, state& State, unsigned int BeginStep, unsigned int EndStep, interruptable_observer IntrpObserver){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
				if(IntrpObserver(State, BeginStep))return;
			}
		}
	}
}
#
#endif
