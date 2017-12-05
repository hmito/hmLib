#ifndef HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATEADAPTIVE_INC
#define HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATEADAPTIVE_INC 100
#
#include <utility>
#include <boost/throw_exception.hpp>

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
			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_adaptive(
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
					auto ans = ifr.do_step(st, system, start_state, time, dt);
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					++step;
					time = start_time + static_cast< typename boost::numeric::odeint::unit_value_type<Time>::type >(step) * dt;


					if (ans) {
						obs(start_state, time);
						return time;
					}
				}

				if(boost::numeric::odeint::detail::less_with_sign(time, end_time, dt)) {   //make a last step to end exactly at end_time
					ifr.do_step(st,system, start_state, time, end_time - time);
					steps++;
					obs(start_state, end_time);
				}
				return end_time;
			}


			/*
			* integrate adaptive for controlled stepper
			*/
			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_adaptive(
				Stepper stepper, System system, State &start_state,
				Time &start_time, Time end_time, Time &dt,
				Interferer interferer, Observer observer, boost::numeric::odeint::controlled_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Interferer >::type &ifr = interferer;
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

				while(boost::numeric::odeint::detail::less_with_sign(start_time, end_time, dt)) {
					obs(start_state, start_time);
					if(boost::numeric::odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)) {
						dt = end_time - start_time;
					}

					std::pair<bool,boost::numeric::odeint::controlled_step_result> res;
					do {
						res = ifr.try_step(st,system, start_state, start_time, dt);
						fail_checker();  // check number of failed steps
					} while(!res.first && res.second == boost::numeric::odeint::controlled_step_result::fail);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

					if (res.first) {
						if (res.second != boost::numeric::odeint::controlled_step_result::fail) {
							obs(start_state, start_time);
						}
						return start_time;
					}
				}

				obs(start_state, start_time);

				return start_time;
			}


			/*
			* integrate adaptive for dense output steppers
			*
			* step size control is used if the stepper supports it
			*/
			template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
			Time interfere_integrate_adaptive(
				Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt,
				Interferer interferer, Observer observer, boost::numeric::odeint::dense_output_stepper_tag) {

				typename boost::numeric::odeint::unwrap_reference< Interferer >::type &ifr = interferer;
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				auto inians = ifr.initialize(st,start_state, start_time, dt);
				obs(st.current_state(), st.current_time());
				if (inians)return start_time;

				while(boost::numeric::odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step())) {
					while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time,st.current_time_step())) {   
						//make sure we don't go beyond the end_time
						auto ans = ifr.do_step(st,system);

						if (ans.first) {
							// overwrite start_state with the final point
							start_state = st.current_state();

							return st.current_time();
						}
						obs(st.current_state(), st.current_time());
					}
					// calculate time step to arrive exactly at end time
					if (ifr.initialize(st, st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()))) {
						break;
					}
				}

				// overwrite start_state with the final point
				start_state = st.current_state();

				return st.current_time();
			}
		}

		template< class Stepper, class System, class State, class Time, class Interferer, class Observer >
		Time interfere_integrate_adaptive(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Interferer interferer, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, interferer, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time, class Interferer >
		Time interfere_integrate_adaptive(Stepper stepper, System system, State &start_state, Time start_time, Time end_time, Time dt, Interferer interferer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_adaptive(stepper, system, start_state, start_time, end_time, dt, interferer, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
