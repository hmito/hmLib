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

namespace hmLib {
	namespace odeint {
		namespace detail {
			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;
				TIme ostart_time = start_time
				const Time time_step = dt;
				int step = 0;

				while (boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time + time_step), end_time, dt)) {
					ifrsys.validate(start_state, time, start_state);
					obs(start_state, time);

					Time next_time = time + time_step;

					//adaptive copy with no observation
					{
						while (boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
							ifrsys.validate(start_state, start_time, start_state);

							if (boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
								dt = next_time - start_time;
							}

							auto ifr = ifrsys.ready(st, start_state, start_time, dt, start_state);
							if (ifr == interfere_request::terminate) {
								return start_time;
							}
							else if (ifr == interfere_request::initialize) {
								try_initialize(st, ifrsys, start_state, start_time, dt);
							}

							boost::numeric::odeint::controlled_step_result res;
							Time pdt = dt;
							do {
								st.do_step(ifrsys, start_state, start_time, dt);

								if (ifrsys.valid_step(start_state, start_time + dt))break;

								pdt /= 2;
								dt = pdt;
							} while (dt > time_error / 2);
							start_time += dt;
						}
					}

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = ostart_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * time_step;
				}
				ifrsys.validate(start_state, time, start_state);
				obs(start_state, time);

				return time;
			}

			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, InterfereSystem ifrsys, , State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::controlled_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;
				const Time time_step = dt;
				int step = 0;

				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+time_step), end_time, dt)) {
					ifrsys.validate(start_state, time, start_state);
					obs(start_state, time);

					Time next_time = time + time_step;

					//adaptive copy with no observation
					{
						boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

						while (boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
							ifrsys.validate(start_state, start_time, start_state);

							if (boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
								dt = next_time - start_time;
							}

							auto ifr = ifrsys.ready(st, start_state, start_time, dt, start_state);
							if (ifr == interfere_request::terminate) {
								return start_time;
							}
							else if (ifr == interfere_request::initialize) {
								try_initialize(st, ifrsys, start_state, start_time, dt);
							}

							boost::numeric::odeint::controlled_step_result res;
							Time pdt = dt;
							Time rstart_time = start_time;
							do {
								res = st.try_step(ifrsys, start_state, start_time, dt);

								if (res == boost::numeric::odeint::controlled_step_result::fail) {
									pdt = dt;
									// check number of failed steps
									fail_checker();
									continue;
								}
								if (ifrsys.valid_step(start_state, start_time))break;

								pdt /= 2;
								dt = pdt;
								start_time = rstart_time;
							} while (dt > time_error / 2);

							fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker
						}
					}

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * time_step;
				}
				ifrsys.validate(start_state, time, start_state);
				obs(start_state, time);

				return time;
			}

			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, InterfereSystem ifrsys, , State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::dense_output_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;

				ifrsys.validate(start_state, start_time, start_state);
				st.initialize(start_state, time, dt);
				obs(start_state, time);

				time += dt;
				int obs_step(1);
				
				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+dt), end_time, dt)) {
					Time prev_time = st.current_time();

					// we have not reached the end, do another real step
					if (boost::numeric::odeint::detail::less_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time, st.current_time_step())) {
						//make sure we don't go beyond the end_time
						auto ifr = ifrsys.ready(st, st.current_state(), st.current_time(), st.current_time_step(), start_state);
						if (ifr == interfere_request::terminate) {
							return st.current_time();
						}
						else if (ifr == interfere_request::initialize) {
							st.initilize(start_state, st.current_time(), st.current_time_step());
						}
						st.do_step(ifrsys);
					}else{
						// do the last step ending exactly on the end point
						auto ifr = ifrsys.ready(st, st.current_state(), st.current_time(), end_time - st.current_time(), start_state);
						if (ifr == interfere_request::terminate) {
							return st.current_time();
						}
						else if (ifr == interfere_request::initialize) {
							st.initilize(start_state, st.current_time(), end_time - st.current_time()));
						}
						else {
							st.initialize(st.current_state(), st.current_time(), end_time - st.current_time());
						}
						st.do_step(ifrsys);
					}

					if (!ifrsys.valid_step(st.current_state(), st.current_time()) {
						auto AnsPair = boost::math::tools::bisect(
							[&](Time t) {
								st.calc_state(t, start_state);
								return ifrsys.valid_step(start_state, t);
							},
							prev_time, st.current_time(), [=](Time a, Time b) {return b - a < time_error; }
						);

						prev_time = (AnsPair.first + AnsPair.second) / 2.;

						while (boost::numeric::odeint::detail::less_eq_with_sign(time, prev_time, dt)) {
							st.calc_state(time, start_state);
							ifrsys.validate(start_state, time, start_state);
							obs(start_state, time);
							++obs_step;
							// direct computation of the time avoids error propagation happening when using time += dt
							// we need clumsy type analysis to get boost units working here
							time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(obs_step) * dt;
						}

						st.calc_state(prev_time, start_state);
						st.initilize(start_state, prev_time, st.current_time_step());
					}
					else {
						while (boost::numeric::odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
							st.calc_state(time, start_state);
							ifrsys.validate(start_state, time, start_state);
							obs(start_state, time);
							++obs_step;
							// direct computation of the time avoids error propagation happening when using time += dt
							// we need clumsy type analysis to get boost units working here
							time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(obs_step) * dt;
						}
					}
				}

				return time;
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		Time interfere_integrate_const(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Time time_error, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, system, start_state, start_time, end_time, dt, time_error, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		Time interfere_integrate_const(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Time time_error) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, system, start_state, start_time, end_time, dt, time_error, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
