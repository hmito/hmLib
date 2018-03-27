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
				namespace interferes = hmLib::odeint::interferes;

				interfere_request req;

				Time pdt = 0;
				if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}

				//Until the start time reach to the end time.
				//	i.e., start_time < end_time
				//	dt is just used for checking sign.
				while (boost::numeric::odeint::detail::less_with_sign(start_time, end_time, dt)) {
//					ifrsys.validate(start_state, start_time, start_state);
					//if start_time + dt > end_time
					if (boost::numeric::odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)) {
						dt = end_time - start_time;
					}

//					boost::numeric::odeint::controlled_step_result res;
					//copy previous state & step
					pdt = dt;
					State pst = st;
					do {
						st.do_step(ifrsys, start_state, start_time, dt);

						//interfere excute
						if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, dt, start_state)) {
							obs(start_state, start_time);
							return start_time;
						}

						//if itnerfere did not request restep
						if(!interferes::detail::is_restep_requested(req))break;

						if(!interferes::detail::should_use_dt_on_restep(req)) {
							dt /= 2;
						}
						//reset state
						st = pst;
					} while (dt > time_error/2);
					start_time += dt;
					//reset time step
					dt = pdt;

					obs(start_state, start_time);
				}

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

				interfere_request req;
				boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

				 //ready for next step.
				Time pdt = 0;
				if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}
				
				//as long as start_time < end_time
				while(boost::numeric::odeint::detail::less_with_sign(start_time, end_time, dt)) {
					//if start_time+dt > end_time, set dt as end_time-start_time
					if(boost::numeric::odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)) {
						dt = end_time - start_time;
					}

					boost::numeric::odeint::controlled_step_result res;
					//copy previous state & step
					Time ptime = start_time;
					State pst = st;
					do {
						//Note: dt can be over-written by controlled stepper.
						pdt = dt;
						res = st.try_step(ifrsys, start_state, start_time, dt);

						if (res == boost::numeric::odeint::controlled_step_result::fail){
							// check number of failed steps
							fail_checker();
							continue;
						}

						//ready for next step.
						//	use pdt becauese dt is just a suggestion from stepper regardless of the validity of the step.
						if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
							obs(start_state, start_time);
							return start_time;
						}
						
						//if itnerfere did not request restep
						if(!interferes::detail::is_restep_requested(req))break;

						if(!interferes::detail::should_use_dt_on_restep(req)) {
							pdt /= 2;
							dt = pdt;
						} else {
							dt = pdt;
						}

						//reset state & time
						st = pst;
						start_time = ptime;
					} while(pdt > time_error/2);
					obs(start_state, start_time);
					// if we reach here, the step was successful -> reset fail checker
					fail_checker.reset();  
				}

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
				interfere_request req;

				st.initialize(start_state, start_time, dt);

				//ready for next step.
				Time pdt = 0;
				if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}

				//if current_time < end_time
				while(boost::numeric::odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step())) {
					//if current_time + current_dt < end_time
					while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time,st.current_time_step())) {   
//						obs(st.current_state(), st.current_time());
						auto time_range = st.do_step(system);

						//ready for next step.
						start_time = st.current_time();
						pdt = time_range.second - time_range.first;
						if(interferes::detail::interfere_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
							obs(st.current_state(), st.current_time());
							return start_time;
						}

						//restep is requested
						if(interferes::detail::is_restep_requested(req) && time_range.second-time_range.first > time_error) {
							while(true) {
								if(interferes::detail::should_use_dt_on_restep(req)) {
									start_time = time_range.first + pdt;
								} else {
									start_time = (time_range.first + time_range.second)/2;
								}
								st.calc_state(start_time, start_state);
								pdt = start_time - time_range.first;

								req = ifrsys.interfere(start_state, start_time, pdt);
								if(req==interfere_request::none) {
									time_range.first = start_time;
								} else if(interferes::detail::is_restep_requested(req)) {
									time_range.second = start_time;
								} else {
									break;
								}

								//end condition: use time_range.first for avoiding invalid state
								if(time_range.second - time_range.first <= time_error) {
									start_time = time_range.first;
									st.calc_state(start_time, start_state);

									st.initialize(start_state, start_time, st.current_time_step());
									break;
								}
							}
						}
						obs(st.current_state(), st.current_time());
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}

				// overwrite start_state with the final point
				start_state = st.current_state();
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
