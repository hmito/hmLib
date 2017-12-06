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

					bool ans = ifr.do_step(st, system, start_state, time, dt);

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					++step;
					time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * dt;

					if (ans)break;
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
//				int real_steps = 0;
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

							std::pair<bool,boost::numeric::odeint::controlled_step_result> res;
							do {
								res = ifr.try_step(st, system, start_state, step_start_time, dt);
								fail_checker();  // check number of failed steps
							} while((!res.first) && res.second == boost::numeric::odeint::controlled_step_result::fail);
							fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

							if (res.first) {
								if(res.second!=boost::numeric::odeint::controlled_step_result::fail)obs(start_state, step_start_time);
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

				auto inians = ifr.initialize(st,start_state, time, dt);
				obs(start_state, time);
				if (inians) return time;
				time += dt;

				int obs_step(1);
				
				while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(time+dt), end_time, dt)) {
					while(boost::numeric::odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
						if (ifr.calc_state(st, time, start_state))break;
						obs(start_state, time);
						++obs_step;
						// direct computation of the time avoids error propagation happening when using time += dt
						// we need clumsy type analysis to get boost units working here
						time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(obs_step) * dt;
					}
					// we have not reached the end, do another real step
					if(boost::numeric::odeint::detail::less_with_sign(static_cast<Time>(st.current_time()+st.current_time_step()),end_time,st.current_time_step())) {
						while(boost::numeric::odeint::detail::less_eq_with_sign(st.current_time(), time, dt)) {
							if (ifr(st, system) ){
								return st.current_time();
							}
							auto ans = ifr.do_step(st,system);

							if(ans.first) {
								while(boost::numeric::odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
									if (ifr.calc_state(st, time, start_state))break;

									obs(start_state, time);
									++obs_step;
									// direct computation of the time avoids error propagation happening when using time += dt
									// we need clumsy type analysis to get boost units working here
									time = start_time + static_cast<typename boost::numeric::odeint::unit_value_type<Time>::type>(obs_step) * dt;
								}
								return st.current_time();
							}

						}
					} else if(boost::numeric::odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step())) { // do the last step ending exactly on the end point
						if (ifr.initialize(st, st.current_state(), st.current_time(), end_time - st.current_time())) {
							obs(st.current_state(), time);
							return time;
						}

						if (ifr(st, system) ){
							return st.current_time();
						}
						auto ans = ifr.do_step(st, system);

						if (ans.first) {
							while (boost::numeric::odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
								if (ifr.calc_state(st, time, start_state)) {
									obs(start_state, time);
									return time;
								}

								obs(start_state, time);
								++obs_step;
								// direct computation of the time avoids error propagation happening when using time += dt
								// we need clumsy type analysis to get boost units working here
								time = start_time + static_cast<typename boost::numeric::odeint::unit_value_type<Time>::type>(obs_step) * dt;

							}
							return st.current_time();
						}


					}

				}
				// last observation, if we are still in observation interval
				// might happen due to finite precision problems when computing the the time
				if (boost::numeric::odeint::detail::less_eq_with_sign(time, end_time, dt)) {
					ifr.calc_state(st, time, start_state);
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
