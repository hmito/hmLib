#ifndef HMLIB_ODEINT_INC
#define HMLIB_ODEINT_INC 100
#
#include<boost/numeric/odeint.hpp>
namespace hmLib{
	namespace odeint{
		template<typename system>
		struct stepper{};
		template<typename stepper, typename system, typename state,typename time, typename interrupt_observer>
		void interruptable_integrate_const(stepper Stepper, system System, state &State,time Start, time End, time dt, interrupt_observer Observer){
			using stepper_category = typename stepper::type::stepper_category;

			using namespace boost::numeric::odeint;

		}

		
	}
}
#
#endif
