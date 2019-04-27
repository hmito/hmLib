#ifndef HMLIB_ODEINT_INTEGRATE_BRAKEABLEINTEGRATEADAPTIVE_INC
#define HMLIB_ODEINT_INTEGRATE_BRAKEABLEINTEGRATEADAPTIVE_INC 100
#
#include <stdexcept>

#include <boost/throw_exception.hpp>

#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/integrate/detail/integrate_const.hpp>
#include <boost/numeric/odeint/util/bind.hpp>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/copy.hpp>

#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>


#include <iostream>

namespace hmLib {
	namespace odeint {
		namespace detail {

			// forward declaration
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t brakeable_integrate_const(Stepper stepper, System system, State& start_state, Time start_time, Time end_time, Time dt, Observer observer, boost::numeric::odeint::stepper_tag);

			/*
				* integrate_adaptive for simple stepper is basically an integrate_const + some last step
				*/
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t brakeable_integrate_adaptive(Stepper stepper, System system, State& start_state,Time start_time, Time end_time, Time dt,Observer observer, boost::numeric::odeint::stepper_tag)
			{
				size_t steps = boost::numeric::odeint::detail::integrate_const(stepper, system, start_state, start_time,
					end_time, dt, observer, stepper_tag());
				typename boost::numeric::odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type& st = stepper;

				Time end = start_time + dt * steps;
				if (boost::numeric::odeint::detail::less_with_sign(end, end_time, dt))
				{   //make a last step to end exactly at end_time
					st.do_step(system, start_state, end, end_time - end);
					steps++;
					obs(start_state, end_time);
				}
				return steps;
			}


			/*
				* integrate adaptive for controlled stepper
				*/
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State & start_state,
				Time & start_time, Time end_time, Time & dt,
				Observer observer, boost::numeric::odeint::controlled_stepper_tag
			)
			{
				typename odeint::unwrap_reference< Observer >::type& obs = observer;
				typename odeint::unwrap_reference< Stepper >::type& st = stepper;

				failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
				size_t count = 0;
				while (less_with_sign(start_time, end_time, dt))
				{
					obs(start_state, start_time);
					if (less_with_sign(end_time, static_cast<Time>(start_time + dt), dt))
					{
						dt = end_time - start_time;
					}

					controlled_step_result res;
					do
					{
						res = st.try_step(system, start_state, start_time, dt);
						fail_checker();  // check number of failed steps
					} while (res == fail);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

					++count;
				}
				obs(start_state, start_time);
				return count;
			}


			/*
				* integrate adaptive for dense output steppers
				*
				* step size control is used if the stepper supports it
				*/
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State & start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, boost::numeric::odeint::dense_output_stepper_tag)
			{
				typename odeint::unwrap_reference< Observer >::type& obs = observer;
				typename odeint::unwrap_reference< Stepper >::type& st = stepper;

				size_t count = 0;
				st.initialize(start_state, start_time, dt);

				while (less_with_sign(st.current_time(), end_time, st.current_time_step()))
				{
					while (less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()),
						end_time,
						st.current_time_step()))
					{   //make sure we don't go beyond the end_time
						obs(st.current_state(), st.current_time());
						st.do_step(system);
						++count;
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}
				obs(st.current_state(), st.current_time());
				// overwrite start_state with the final point
				boost::numeric::odeint::copy(st.current_state(), start_state);
				return count;
			}
		}
	}
}
#
#endif
