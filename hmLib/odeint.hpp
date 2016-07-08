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

			unwrap_reference< interrupt_observer >::type &obs = Observer;
			unwrap_reference< stepper >::type &st = Stepper;

			time Time = Start;
			int Step = 0;
			// cast time+dt explicitely in case of expression templates (e.g. multiprecision)
			while(detail::less_eq_with_sign(static_cast<time>(Time + dt), End, dt)){
				if(obs(State, Time)return Step;
				st.do_step(System, State, Time, dt);

				// direct computation of the time avoids error propagation happening when using time += dt
				// we need clumsy type analysis to get boost units working here
				++Step;
				Time = Start + static_cast< typename unit_value_type<time>::type >(Step) * dt;
			}
			obs(State, Time);

			return Step;
		}

		
	}
}
#
#endif
