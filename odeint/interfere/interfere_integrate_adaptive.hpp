#ifndef HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATEADAPTIVE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATEADAPTIVE_INC 100
#
#include <utility>
#include <boost/throw_exception.hpp>

#include <boost/math/tools/roots.hpp>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/util/bind.hpp>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/copy.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include "interfere_type.hpp"
#include "../utility.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			/*
			* integrate_adaptive for simple stepper is basically an integrate_const + some last step
			*/
			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_adaptive(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				while (boost::numeric::odeint::detail::less_with_sign(start_time, end_time, dt)) {
					ifrsys.validate(start_state, start_time, start_state);
					obs(start_state, start_time);
					if (boost::numeric::odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)) {
						dt = end_time - start_time;
					}

					auto ifr = ifrsys.ready(st, start_state, start_time, dt, start_state);
					if (ifr == interfere_request::terminate) {
						return start_time;
					}else if(ifr == interfere_request::initialize){
						try_initialize(st, ifrsys, start_state, start_time, dt);
					}

//					boost::numeric::odeint::controlled_step_result res;
					Time pdt = dt;
					do {
						st.do_step(ifrsys, start_state, start_time, dt);

						if (ifrsys.valid_step(start_state, start_time + dt))break;

						pdt /= 2;
						dt = pdt;
					} while (dt > time_error/2);
					start_time += dt;
				}
				ifrsys.validate(start_state, start_time, start_state);
				obs(start_state, start_time);
				return start_time;
			}


			/*
			* integrate adaptive for controlled stepper
			*/
			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_adaptive(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time &start_time, Time end_time, Time &dt, Time time_error,
				Observer observer, boost::numeric::odeint::controlled_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

				while(boost::numeric::odeint::detail::less_with_sign(start_time, end_time, dt)) {
					ifrsys.validate(start_state, start_time, start_state);
					obs(start_state, start_time);
					if(boost::numeric::odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)) {
						dt = end_time - start_time;
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
					} while(dt > time_error/2);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker
				}
				ifrsys.validate(start_state, start_time, start_state);
				obs(start_state, start_time);
				return start_time;
			}


			/*
			* integrate adaptive for dense output steppers
			*
			* step size control is used if the stepper supports it
			*/
			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_adaptive(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::dense_output_stepper_tag) {

				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				ifrsys.validate(start_state, start_time, start_state);
				st.initialize(start_state, start_time, dt);
				obs(st.current_state(), st.current_time());

				while(boost::numeric::odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step())) {
					while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time,st.current_time_step())) {   
//						obs(st.current_state(), st.current_time());

						//make sure we don't go beyond the end_time
						auto ifr = ifrsys.ready(st, st.current_state(), st.current_time(), st.current_time_step(), start_state);
						if (ifr == interfere_request::terminate) {
							return st.current_time();
						} else if(ifr == interfere_request::initialize){
							st.initialize(start_state, st.current_time(), st.current_time_step());
						}
						start_time = st.current_time();
						st.do_step(system);

						if (!ifrsys.valid_step(st.current_state(), ifrsys.current_time())) {
							auto AnsPair = boost::math::tools::bisect(
								[&](Time t){
									st.calc_state(t, start_state);
									return ifrsys.valid_step(start_state, t);
								},
								start_time, st.current_time(), [=](Time a, Time b) {return b - a < time_error; }
							);
							start_time = (AnsPair.first + AnsPair.second) / 2.;
							st.calc_state(start_time, start_state);
							st.initialize(start_state, start_time, st.current_time_step());
							obs(start_state, st.current_time());
						}else {
							obs(st.current_state(), st.current_time());
						}
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}


				obs(st.current_state(), st.current_time());

				// overwrite start_state with the final point
				boost::numeric::odeint::copy(st.current_state(), start_state);
				return st.current_time();
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		Time interfere_integrate_adaptive(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Time time_error, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, time_error, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		Time interfere_integrate_adaptive(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Time time_error) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, time_error, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
