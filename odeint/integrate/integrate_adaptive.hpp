#ifndef HMLIB_ODEINT_INTEGRATE_INTEGRATEADAPTIVE_INC
#define HMLIB_ODEINT_INTEGRATE_INTEGRATEADAPTIVE_INC 100
#
#include"../stepper_categories.hpp"
#include<boost/numeric/odeint/integrate/null_observer.hpp>
#include<boost/numeric/odeint/integrate/integrate_adaptive.hpp>
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, stepper_tag tag
			) {
				return boost_odeint::integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, controlled_stepper_tag tag
			) {
				return boost_odeint::integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, dense_output_stepper_tag tag
			) {
				return boost_odeint::integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, adaptive_stepper_tag tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				boost_odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
				size_t count = 0;
				while (boost_odeint::detail::less_with_sign(start_time, end_time, dt))
				{
					obs(start_state, start_time);
					if (boost_odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt))
					{
						dt = end_time - start_time;
					}
					//argument "start_time" will be updated inside of do_step.
					st.do_step(system, start_state, start_time, dt);
					++count;
				}
				obs(start_state, start_time);
				return count;
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		size_t integrate_adaptive(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt, Time time_error, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_adaptive(stepper, sys, start_state, start_time, end_time, dt, time_error, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		size_t integrate_adaptive(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt, Time time_error) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_adaptive(stepper, sys, start_state, start_time, end_time, dt, time_error, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
