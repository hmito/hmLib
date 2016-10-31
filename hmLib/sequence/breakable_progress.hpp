#ifndef HMLIB_BREAKABLEPROGRESS_INC
#define HMLIB_BREAKABLEPROGRESS_INC 100
#
namespace hmLib{
	namespace sequence{
		template<typename system, typename state, typename breakable_observer>
		unsigned int breakable_progress(system System, state& State, unsigned int BeginStep, unsigned int EndStep, breakable_observer Observer){
			for(; BeginStep != EndStep; ++BeginStep){
				System(State, BeginStep);
				if(Observer(State, BeginStep))break;
			}
			return BeginStep;
		}
	}
}
#
#endif
