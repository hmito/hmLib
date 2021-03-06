#ifndef HMLIB_ODEINT_INTEGRATE_INTEGRATECONST_INC
#define HMLIB_ODEINT_INTEGRATE_INTEGRATECONST_INC 100
#
#include<boost/numeric/odeint/integrate/null_observer.hpp>
#include<boost/numeric/odeint/integrate/integrate_const.hpp>
#include"../stepper_categories.hpp"
#include"../utility.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;

			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, stepper_tag
			) {
				return boost_odeint::detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, observer, stepper_tag());
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, controlled_stepper_tag
			) {
				return boost_odeint::detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, observer, controlled_stepper_tag());
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, dense_output_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				Time time = start_time;

				st.initialize(start_state, time, dt);
				obs(start_state, time);
				time += dt;

				int obs_step(1);
				int real_step(0);

				while (boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + dt), end_time, dt))
				{
					while (boost_odeint::detail::less_eq_with_sign(time, st.current_time(), dt))
					{
						calc_state(st, sys, time, start_state);
						obs(start_state, time);
						++obs_step;
						// direct computation of the time avoids error propagation happening when using time += dt
						// we need clumsy type analysis to get boost units working here
						time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(obs_step) * dt;
					}
					// we have not reached the end, do another real step
					if (boost_odeint::detail::less_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()),
						end_time,
						st.current_time_step())){
						while (boost_odeint::detail::less_eq_with_sign(st.current_time(), time, dt))
						{
							st.do_step(sys);
							++real_step;
						}
					}
					else if (boost_odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step()))
					{ // do the last step ending exactly on the end point
						st.initialize(st.current_state(), st.current_time(), end_time - st.current_time());
						st.do_step(sys);
						++real_step;
					}

				}
				// last observation, if we are still in observation interval
				// might happen due to finite precision problems when computing the the time
				if (boost_odeint::detail::less_eq_with_sign(time, end_time, dt)){
					calc_state(st, sys, time, start_state);
					obs(start_state, time);
				}

				return real_step;
			}

			//forward declaration for integrate_const with adaptive stepper
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, adaptive_stepper_tag);

			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, adaptive_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;

				Time time = start_time;
				const Time time_step = dt;
				int real_steps = 0;
				int step = 0;

				while (boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + time_step), end_time, dt))
				{
					obs(start_state, time);
					// integrate_adaptive_checked uses the given checker to throw if an overflow occurs
					real_steps += integrate_adaptive(stepper, sys, start_state, 
						time, static_cast<Time>(time + time_step), dt,
						boost_odeint::null_observer(), adaptive_stepper_tag());
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(step) * time_step;
				}
				obs(start_state, time);

				return real_steps;
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		size_t integrate_const(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		size_t integrate_const(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
