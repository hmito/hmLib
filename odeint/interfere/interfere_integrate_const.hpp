#ifndef HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATECONST_INC
#define HMLIB_ODEINT_INTERFERE_INTERFEREINTEGRATECONST_INC 100
#
#include <cmath>
#include <boost/numeric/odeint/util/unit_helper.hpp>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/util/bind.hpp>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/copy.hpp>
#include <boost/numeric/odeint/integrate/null_observer.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include "interfere_type.hpp"
#include "../utility.hpp"
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
				namespace interferes = hmLib::odeint::interferes;

				interfere_request req;

				Time pdt = 0;
				if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}

				const Time original_start_time = start_time;
				const Time time_step = dt;
				unsigned int step_cnt = 0;
				Time next_time = static_cast<Time>(start_time+ time_step);

				while (boost::numeric::odeint::detail::less_eq_with_sign(next_time, end_time, dt)) {
					//Until the start time reach to the end time.
					//	i.e., start_time < next_time
					//	dt is just used for checking sign.
					while(boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
						//if start_time + dt > next_time
						if(boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
							dt = next_time - start_time;
						}

						//copy previous state & step
						pdt = dt;
						typename boost::numeric::odeint::unwrap_reference< State >::type post_state = start_state;
						do {
							st.do_step(ifrsys, start_state, start_time, dt);

							//interfere excute
							if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, dt, start_state)) {
								obs(start_state, start_time);
								return start_time;
							}

							//if itnerfere did not request restep
							if(!interferes::detail::is_restep_requested(req))break;

							if(!interferes::detail::should_use_dt_on_restep(req)) {
								dt /= 2;
							}
							//reset state
							start_state = post_state;
						} while(dt > time_error);
						if(std::abs(dt)<=time_error) {
							obs(start_state, start_time);
							return start_time;
						}

						start_time += dt;
						//reset time step
						dt = pdt;
					}

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					start_time = original_start_time + time_step*(++step_cnt);
					next_time = start_time + time_step;
					obs(start_state, start_time);
				}

				next_time = end_time;

				//Until the start time reach to the end time.
				//	i.e., start_time < next_time
				//	dt is just used for checking sign.
				while(boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
					//if start_time + dt > next_time
					if(boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
						dt = next_time - start_time;
					}

					//copy previous state & step
					pdt = dt;
					typename boost::numeric::odeint::unwrap_reference< State >::type post_state = start_state;
					do {
						st.do_step(ifrsys, start_state, start_time, dt);

						//interfere excute
						if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, dt, start_state)) {
							obs(start_state, start_time);
							return start_time;
						}

						//if itnerfere did not request restep
						if(!interferes::detail::is_restep_requested(req))break;

						if(!interferes::detail::should_use_dt_on_restep(req)) {
							dt /= 2;
						}
						//reset state
						start_state = post_state;
					} while(std::abs(dt) > time_error);
					if(std::abs(dt)<=time_error) {
						obs(start_state, start_time);
						return start_time;
					}

					start_time += dt;
					//reset time step
					dt = pdt;
				}

				// direct computation of the time avoids error propagation happening when using time += dt
				// we need clumsy type analysis to get boost units working here
				start_time = end_time;
				obs(start_state, start_time);

				return start_time;
			}

			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::controlled_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;

				interfere_request req;
				boost::numeric::odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

				Time pdt = 0;
				if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}

				const Time original_start_time = start_time;
				const Time time_step = dt;
				unsigned int step_cnt = 0;
				Time next_time = static_cast<Time>(start_time+ time_step);

				while(boost::numeric::odeint::detail::less_eq_with_sign(next_time, end_time, dt)) {
					while(boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
						//if start_time+dt > next_time, set dt as next_time-start_time
						if(boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
							dt = next_time - start_time;
						}

						boost::numeric::odeint::controlled_step_result res;
						//copy previous state & step
						Time ptime = start_time;
						typename boost::numeric::odeint::unwrap_reference< State >::type post_state = start_state;
						do {
							//Note: dt can be over-written by controlled stepper.
							pdt = dt;
							res = st.try_step(ifrsys, start_state, start_time, dt);

							if(res == boost::numeric::odeint::controlled_step_result::fail) {
								// check number of failed steps
								fail_checker();
								continue;
							}

							//ready for next step.
							//	use pdt becauese dt is just a suggestion from stepper regardless of the validity of the step.
							if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
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
							start_state = post_state;
							start_time = ptime;
						} while(pdt > time_error);

						if(std::abs(pdt)<=time_error) {
							obs(start_state, start_time);
							return start_time;
						}

						// if we reach here, the step was successful -> reset fail checker
						fail_checker.reset();
					}

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					start_time = original_start_time + time_step*(++step_cnt);
					next_time = start_time + time_step;
					obs(start_state, start_time);
				}

				next_time = end_time;
				while(boost::numeric::odeint::detail::less_with_sign(start_time, next_time, dt)) {
					//if start_time+dt > next_time, set dt as next_time-start_time
					if(boost::numeric::odeint::detail::less_with_sign(next_time, static_cast<Time>(start_time + dt), dt)) {
						dt = next_time - start_time;
					}

					boost::numeric::odeint::controlled_step_result res;
					//copy previous state & step
					Time ptime = start_time;
					typename boost::numeric::odeint::unwrap_reference< Stepper >::type post_state = st;
					do {
						//Note: dt can be over-written by controlled stepper.
						pdt = dt;
						res = st.try_step(ifrsys, start_state, start_time, dt);

						if(res == boost::numeric::odeint::controlled_step_result::fail) {
							// check number of failed steps
							fail_checker();
							continue;
						}

						//ready for next step.
						//	use pdt becauese dt is just a suggestion from stepper regardless of the validity of the step.
						if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
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
						st = post_state;
						start_time = ptime;
					} while(pdt > time_error);
					if(std::abs(pdt)<=time_error) {
						obs(start_state, start_time);
						return start_time;
					}

					// if we reach here, the step was successful -> reset fail checker
					fail_checker.reset();
				}

				// direct computation of the time avoids error propagation happening when using time += dt
				// we need clumsy type analysis to get boost units working here
				start_time = end_time;
				obs(start_state, start_time);

				return start_time;
			}

			template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
			Time interfere_integrate_const(
				Stepper stepper, InterfereSystem ifrsys, State &start_state,
				Time start_time, Time end_time, Time dt, Time time_error,
				Observer observer, boost::numeric::odeint::dense_output_stepper_tag
			) {
				typename boost::numeric::odeint::unwrap_reference< Observer >::type &obs = observer;
				typename boost::numeric::odeint::unwrap_reference< Stepper >::type &st = stepper;
				interfere_request req;

				st.initialize(start_state, start_time, dt);

				//ready for next step.
				Time pdt = 0;
				if(interferes::detail::interfere_and_excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
					return start_time;
				}
				if(interferes::detail::is_restep_requested(req)) {
					return start_time;
				}

				const Time observe_time_step = dt;
				const Time original_start_time = start_time;
				unsigned int observe_count = 0;
				Time observe_time = start_time + observe_time_step*(++observe_count);
				typename boost::numeric::odeint::unwrap_reference< State >::type observe_state = start_state;

				//if current_time < end_time
				while(boost::numeric::odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step())) {
					//if current_time + current_dt < end_time
					while(boost::numeric::odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time, st.current_time_step())) {
						//step
						auto time_range = st.do_step(ifrsys);

						//ready for next step.
						start_time = st.current_time();
						pdt = time_range.second - time_range.first;

						//get interfere request (not excute)
						req = ifrsys.interfere(st.current_state(), start_time, pdt, start_state);

						//restep is requested
						if(interferes::detail::is_restep_requested(req) && std::abs(time_range.second - time_range.first) > time_error) {
							while(true) {
								if(interferes::detail::should_use_dt_on_restep(req)) {
									start_time = time_range.first + pdt;
								} else {
									start_time = (time_range.first + time_range.second)/2;
								}
								st.calc_state(start_time, start_state);
								pdt = start_time - time_range.first;

								//get interfere request (not excute)
								req = ifrsys.interfere(start_state, start_time, pdt, start_state);
								if(req==interfere_request::none) {
									time_range.first = start_time;
								} else if(interferes::detail::is_restep_requested(req)) {
									time_range.second = start_time;
								} else {
									//observe by using calc_state as long as observe_time < start_time
									while(boost::numeric::odeint::detail::less_eq_with_sign(observe_time, start_time, observe_time_step)) {
										st.calc_state(observe_time, observe_state);
										obs(observe_state, observe_time);
										observe_time = start_time + observe_time_step*(++observe_count);
									}

									//excute request
									if(interferes::detail::excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
										obs(st.current_state(), st.current_time());
										return start_time;
									}
									break;
								}

								//end condition: use time_range.first for avoiding invalid state
								if(std::abs(time_range.second - time_range.first) <= time_error) {
									if(std::abs(time_range.first-start_time)<=time_error) {
										st.calc_state(start_time, start_state);
										st.initialize(start_state, start_time, st.current_time_step());
										obs(st.current_state(), st.current_time());
										return start_time;
									}

									start_time = time_range.first;

									//observe by using calc_state as long as observe_time < start_time
									while(boost::numeric::odeint::detail::less_eq_with_sign(observe_time, start_time, observe_time_step)) {
										st.calc_state(observe_time, observe_state);
										obs(observe_state, observe_time);
										observe_time = start_time + observe_time_step*(++observe_count);
									}

									st.calc_state(start_time, start_state);
									st.initialize(start_state, start_time, st.current_time_step());
									break;
								}
							}
						}
						else{
							//observe by using calc_state as long as observe_time < start_time
							while(boost::numeric::odeint::detail::less_eq_with_sign(observe_time, start_time, observe_time_step)) {
								st.calc_state(observe_time, observe_state);
								obs(observe_state, observe_time);
								observe_time = start_time + observe_time_step*(++observe_count);
							}

							//excute request
							if(interferes::detail::excute(req, st, ifrsys, start_state, start_time, pdt, start_state)) {
								obs(st.current_state(), st.current_time());
								return start_time;
							}
						}
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}

				// overwrite start_state with the final point
				start_state = st.current_state();
				return st.current_time();
			}
		}

		template< class Stepper, class InterfereSystem, class State, class Time, class Observer >
		Time interfere_integrate_const(Stepper stepper, InterfereSystem ifrsys, State &start_state, Time start_time, Time end_time, Time dt, Time time_error, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, ifrsys, start_state, start_time, end_time, dt, time_error, observer, stepper_category());
		}

		template< class Stepper, class InterfereSystem, class State, class Time>
		Time interfere_integrate_const(Stepper stepper, InterfereSystem ifrsys, State &start_state, Time start_time, Time end_time, Time dt, Time time_error) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::interfere_integrate_const(stepper, ifrsys, start_state, start_time, end_time, dt, time_error, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
