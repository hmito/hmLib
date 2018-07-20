#ifndef HMLIB_SEQUENCE_INTERFEREPROGRESS_INC
#define HMLIB_SEQUENCE_INTERFEREPROGRESS_INC 100
#
namespace hmLib{
    namespace sequence{
        enum class interfere_request{
            none, terminate
        };
        template<typename interfere_system_t, typename state_t, typename time_t>
        time_t interfere_progress(interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep){
			while(BeginStep != EndStep){
				if(System(State, BeginStep++) == interfere_request::terminate)break;
			}
            return BeginStep;
        }
        template<typename interfere_system_t, typename state_t, typename time_t, typename observer>
        time_t interfere_progress(interfere_system_t& System, state_t& State, time_t BeginStep, time_t EndStep, observer Observer){
			while(BeginStep != EndStep){
				if(System(State, BeginStep++) == interfere_request::terminate)break;
				Observer(State, BeginStep);
			}
            return BeginStep;
        }
    }
}
#
#endif
