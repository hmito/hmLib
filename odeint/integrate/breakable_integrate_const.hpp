#ifndef HMLIB_ODEINT_INTERFERE_BREAKABLEINTEGRATECONST_INC
#define HMLIB_ODEINT_INTERFERE_BREAKABLEINTEGRATECONST_INC 100
#
#include <tuple>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/unit_helper.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include <boost/numeric/odeint/integrate/null_observer.hpp>
#include "../stepper_categories.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;

			//breakable integrate const for simple stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, stepper_tag
			){
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				// check breaker condition with the initial state
				bool IsBreak = breaker(start_state, start_time);

				Time time = start_time;
				int step = 0;


				// cast time+dt explicitely in case of expression templates (e.g. multiprecision)
				while (!IsBreak && boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + dt), end_time, dt))
				{
					obs(start_state, time);
					st.do_step(sys, start_state, time, dt);

					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					++step;
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(step) * dt;

					// check breaker condition
					IsBreak = breaker(start_state, time);
				}
				obs(start_state, time);

				return std::make_tuple(IsBreak, step, time);
			}


			// forward declaration for breakable integrate const for controlled_stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, controlled_stepper_tag
			);

			//breakable integrate const for controlled_stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, controlled_stepper_tag
			)
			{
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;

				Time time = start_time;
				const Time time_step = dt;
				int real_steps = 0;
				int step = 0;

				// check breaker condition with the initial state
				if (breaker(start_state, start_time)) {
					obs(start_state, start_time);
					return std::make_tuple(true, real_steps, start_time);
				}

				while (boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + time_step), end_time, dt)){
					obs(start_state, time);
					// integrate_adaptive_checked uses the given checker to throw if an overflow occurs
					auto ans = breakable_integrate_adaptive(stepper, sys, start_state,
						time, static_cast<Time>(time + time_step), dt, breaker,
						boost_odeint::null_observer(), controlled_stepper_tag());

					//if break is detected, finish immidiately
					if (std::get<0>(ans)) {
						return ans;
					}

					real_steps += std::get<1>(ans);
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(step) * time_step;
				}
				obs(start_state, time);

				return std::make_tuple(false, real_steps, time);
			}


			//breakable integrate const for dense_output_stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, dense_output_stepper_tag
			){
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				Time time = start_time;
				int obs_step(1);
				int real_step(0);

				st.initialize(start_state, time, dt);
				obs(start_state, time);
				bool IsBreak = breaker(start_state, time);
				bool IsLast = false;
				if (IsBreak) {
					return std::make_tuple(true, real_step, time);
				}

				time += dt;

				//continue if next observe time is before end_time
				do {
					//as long as additional observation is possible (always fail at first time)
					while (boost_odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
						st.calc_state(time, start_state);
						obs(start_state, time);
						++obs_step;
						// direct computation of the time avoids error propagation happening when using time += dt
						// we need clumsy type analysis to get boost units working here
						time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(obs_step) * dt;
					}

					//continue if the current time is before next observe time
					//	i.e., addtional step is required for next observation
					while (!IsLast && boost_odeint::detail::less_eq_with_sign(st.current_time(), time, dt)) {
						//if overshoot will not occur at the next step
						if (boost_odeint::detail::less_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time, st.current_time_step())) {
							st.do_step(sys);
							++real_step;
							IsBreak = breaker(st.current_state(), st.current_time());
							IsLast = IsBreak;
						} else {
							// do the last step ending exactly on the end point
							st.initialize(st.current_state(), st.current_time(), end_time - st.current_time());
							st.do_step(sys);
							++real_step;
							IsBreak = breaker(st.current_state(), st.current_time());
							IsLast = true;
						}
					}
				} while (!IsLast && boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time), end_time, dt));

				//as long as additional observation is possible
				while (boost_odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
					st.calc_state(time, start_state);
					obs(start_state, time);
					++obs_step;
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(obs_step) * dt;
				}
				
				return std::make_tuple(IsBreak, real_step, st.current_time());
			}

			//breakable integrate const for dense_output_stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, semidense_output_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				Time time = start_time;
				int obs_step(1);
				int real_step(0);

				st.initialize(start_state, time, dt);
				obs(start_state, time);
				bool IsBreak = breaker(start_state, time);
				bool IsLast = false;
				if (IsBreak) {
					return std::make_tuple(true, real_step, time);
				}

				time += dt;

				//continue if next observe time is before end_time
				do {
					//as long as additional observation is possible (always fail at first time)
					while (boost_odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
						st.calc_state(sys,time, start_state);
						obs(start_state, time);
						++obs_step;
						// direct computation of the time avoids error propagation happening when using time += dt
						// we need clumsy type analysis to get boost units working here
						time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(obs_step) * dt;
					}

					//continue if the current time is before next observe time
					//	i.e., addtional step is required for next observation
					while (!IsLast && boost_odeint::detail::less_eq_with_sign(st.current_time(), time, dt)) {
						//if overshoot will not occur at the next step
						if (boost_odeint::detail::less_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()), end_time, st.current_time_step())) {
							st.do_step(sys);
							++real_step;
							IsBreak = breaker(st.current_state(), st.current_time());
							IsLast = IsBreak;
						}
						else {
							// do the last step ending exactly on the end point
							st.initialize(st.current_state(), st.current_time(), end_time - st.current_time());
							st.do_step(sys);
							++real_step;
							IsBreak = breaker(st.current_state(), st.current_time());
							IsLast = true;
						}
					}
				} while (!IsLast && boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time), end_time, dt));

				//as long as additional observation is possible
				while (boost_odeint::detail::less_eq_with_sign(time, st.current_time(), dt)) {
					st.calc_state(sys, time, start_state);
					obs(start_state, time);
					++obs_step;
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(obs_step) * dt;
				}

				return std::make_tuple(IsBreak, real_step, st.current_time());
			}

			// forward declaration for breakable integrate const for adaptive_stepper_tag
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, adaptive_stepper_tag
			);

			//breakable integrate const for dense_output_stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, adaptive_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;

				Time time = start_time;
				const Time time_step = dt;
				int real_steps = 0;
				int step = 0;

				// check breaker condition with the initial state
				if (breaker(start_state, start_time)) {
					obs(start_state, start_time);
					return std::make_tuple(true, real_steps, start_time);
				}

				while (boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + time_step), end_time, dt)) {
					obs(start_state, time);
					// integrate_adaptive_checked uses the given checker to throw if an overflow occurs
					auto ans = breakable_integrate_adaptive(stepper, sys, start_state,
						time, static_cast<Time>(time + time_step), dt, breaker,
						boost_odeint::null_observer(), adaptive_stepper_tag());

					//if break is detected, finish immidiately
					if (std::get<0>(ans)) {
						return ans;
					}

					real_steps += std::get<1>(ans);
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(step) * time_step;
				}
				obs(start_state, time);

				return std::make_tuple(false, real_steps, time);
			}
		}

		template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
		std::tuple<bool, size_t, Time> breakable_integrate_const(
			Stepper stepper, System sys, State& start_state,
			Time start_time, Time end_time, Time dt, 
			Breaker breaker, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::breakable_integrate_const(stepper, sys, start_state, start_time, end_time, dt, breaker, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time, class Breaker>
		std::tuple<bool, size_t, Time> breakable_integrate_const(
			Stepper stepper, System sys, State& start_state,
			Time start_time, Time end_time, Time dt,
			Breaker breaker) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::breakable_integrate_const(stepper, sys, start_state, start_time, end_time, dt, breaker, boost::numeric::odeint::null_observer(), stepper_category());
		}

	}
}
#
#endif
