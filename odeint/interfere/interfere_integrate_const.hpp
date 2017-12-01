#ifndef HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATECONST_INC
#define HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATECONST_INC 100
#
#include <boost/numeric/odeint/util/unit_helper.hpp>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/util/bind.hpp>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/copy.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>

#include "interfere_type.hpp"
#include "utility.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt,
				Interferer interferer, Observer observer, boost::numeric::odeint::stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Interferer >::type &ifr = interferer;
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;
				int step = 0;
				// cast time+dt explicitely in case of expression templates (e.g. multiprecision)
				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+dt), end_time, dt)) {
					obs(start_state, time);
					st.do_step(system, start_state, time, dt);
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					++step;
					time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * dt;

					auto ifrans = ifr(start_state, start_state, time);
					if(ifrans==interfere_type::interfere) {
						try_initialize<Stepper, System, State, Time>(stepper, system, start_state, time, dt);
					} else if(ifrans == interfere_type::terminate) {
						try_initialize<Stepper, System, State, Time>(stepper, system, start_state, time, dt);
						break;
					}
				}
				obs(start_state, time);

				return time;
			}



			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt,
				Interferer interferer, Observer observer, boost::numeric::odeint::controlled_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Interferer >::type &ifr = interferer;
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;
				const Time time_step = dt;
				int real_steps = 0;
				int step = 0;

				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+time_step), end_time, dt)) {
					obs(start_state, time);

					// integrate_adaptive_checked uses the given checker to throw if an overflow occurs
					//real_steps += detail::interfere_integrate_adaptive(stepper, system, start_state, time,
					//	static_cast<Time>(time + time_step), dt, interferer,
					//	null_observer(), controlled_stepper_tag());

					//adaptive copy
					{
						Time step_start_time = time;
						Time step_end_time = time + time_step;
						boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

						while(boost::numeric::odeint::detail::less_with_sign(step_start_time, step_end_time, dt)) {
							if(boost::numeric::odeint::detail::less_with_sign(step_end_time, static_cast<Time>(step_start_time + dt), dt)) {
								dt = end_time - step_start_time;
							}

							boost::numeric::odeint::controlled_step_result res;
							do {
								res = st.try_step(system, start_state, step_start_time, dt);
								fail_checker();  // check number of failed steps
							} while(res == fail);
							fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

							auto ifrans = ifr(start_state, start_state, step_start_time);
							if(ifrans!=interfere_type::interfere) {
								try_initialize<Stepper, System, State, Time>(stepper, system, start_state, step_start_time, dt);
							} else if(ifrans == interfere_type::terminate) {
								try_initialize<Stepper, System, State, Time>(stepper, system, start_state, step_start_time, dt);
								obs(start_state, step_start_time);
								return step_start_time;
							}
						}
					}

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * time_step;
				}
				obs(start_state, time);

				return time;
			}


			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt,
				Interferer interferer, Observer observer, boost::numeric::odeint::dense_output_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Interferer >::type &ifr = interferer;
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;

				st.initialize(start_state, time, dt);
				obs(start_state, time);
				time += dt;

				int obs_step(1);
				
				interfere_type ifrans = interfere_type::ignore;

				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+dt), end_time, dt)) {
					while(boost::numeric::odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
						st.calc_state(time, start_state);
						ifrans = ifr(start_state, start_state, time);
						obs(start_state, time);

						if(ifrans==interfere_type::terminate) {
							st.initilize(start_state, time, dt);
							return time;
						}

						++obs_step;
						// direct computation of the time avoids error propagation happening when using time += dt
						// we need clumsy type analysis to get boost units working here
						time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(obs_step) * dt;
					}
					// we have not reached the end, do another real step
					if(boost::numeric::odeint::detail::less_with_sign(static_cast<Time>(st.current_time()+st.current_time_step()),end_time,st.current_time_step())) {
						while(less_eq_with_sign(st.current_time(), time, dt)) {
							st.do_step(system);

							ifrans = ifr(st.current_state(), start_state, st.current_time());
							if(ifrans == interfere_type::interfere) {
								st.initialize(start_state, st.current_time(), dt);
							} else if(ifrans == interfere_type::terminate) {
								st.initialize(start_state, st.current_time(), dt);
								obs(start_state, st.current_time());
								return st.current_time();
							}
						}
					} else if(less_with_sign(st.current_time(), end_time, st.current_time_step())) { // do the last step ending exactly on the end point
						st.initialize(st.current_state(), st.current_time(), end_time - st.current_time());
						st.do_step(system);

						ifrans = ifr(st.current_state(), start_state, st.current_time());
						if(ifrans == interfere_type::interfere) {
							st.initialize(start_state, st.current_time(), end_time - st.current_time());
						} else if(ifrans == interfere_type::terminate) {
							st.initialize(start_state, st.current_time(), end_time - st.current_time());
							obs(start_state, st.current_time());
							return st.current_time();
						}
					}

				}
				// last observation, if we are still in observation interval
				// might happen due to finite precision problems when computing the the time
				if(less_eq_with_sign(time, end_time, dt)) {
					st.calc_state(time, start_state);
					ifr(start_state, start_state, time);
					obs(start_state, time);
				}

				return time;
			}
		}

		template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
		Time interfere_integrate_const(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Interferer interferer, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, system, start_state, start_time, end_time, dt, interferer, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time, class Interferer >
		Time interfere_integrate_const(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Interferer interferer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, system, start_state, start_time, end_time, dt, interferer, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
