#ifndef HMLIB_ODEINT_INTERFERE_BREAKABLEINTEGRATEADAPTIVE_INC
#define HMLIB_ODEINT_INTERFERE_BREAKABLEINTEGRATEADAPTIVE_INC 100
#
#include <stdexcept>
#include <tuple>
#include <boost/throw_exception.hpp>
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/util/unit_helper.hpp>
#include <boost/numeric/odeint/util/copy.hpp>
#include <boost/numeric/odeint/util/detail/less_with_sign.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/integrate/null_observer.hpp>
#include "../stepper_categories.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;

			//forward declaration for breakable_integrate_adaptive for simple stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_const(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, stepper_tag);

			//integrate_adaptive for simple stepper is basically an integrate_const + some last step
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, stepper_tag)
			{
				auto ans = detail::breakable_integrate_const(stepper, sys, start_state, start_time, end_time, dt, observer, stepper_tag());
				//if break is detected, end immidiately.
				if (std::get<0>(ans))return ans;


				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				size_t steps = std::get<1>(ans);

				//make remaining small step.
				Time end = start_time + dt * steps;
				if (boost_odeint::detail::less_with_sign(end, end_time, dt)){
					//make a last step to end exactly at end_time
					st.do_step(sys, start_state, end, end_time - end);
					steps++;
					obs(start_state, end_time);

					return std::make_tuple(breaker(start_state, end_time), steps, end_time);
				}

				//last break check has been done in breakable_integrate_const
				return std::make_tuple(false, steps, end_time);
			}

			//integrate adaptive for controlled stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, controlled_stepper_tag
			)
			{
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				// check breaker condition with the initial state
				bool IsBreak = breaker(start_state, start_time));

				boost_odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
				size_t step = 0;
				while (!IsBreak && boost_odeint::detail::less_with_sign(start_time, end_time, dt))
				{
					obs(start_state, start_time);
					if (boost_odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt))
					{
						dt = end_time - start_time;
					}

					boost_odeint::controlled_step_result res;
					do
					{
						res = st.try_step(sys, start_state, start_time, dt);
						fail_checker();  // check number of failed steps
					} while (res == fail);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

					++step;

					IsBreak = breaker(start_state, start_time));
				}
				obs(start_state, start_time);

				return std::make_tuple(IsBreak, step, start_time);
			}

			//integrate adaptive for dense output stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, dense_output_stepper_tag)
			{
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				// check breaker condition with the initial state
				bool IsBreak = breaker(start_state, start_time));

				size_t count = 0;
				st.initialize(start_state, start_time, dt);

				while (!IsBreak && boost_odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step()))
				{
					while (!IsBreak && boost_odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()),
						end_time,
						st.current_time_step()))
					{   //make sure we don't go beyond the end_time
						obs(st.current_state(), st.current_time());
						st.do_step(sys);
						++count;
						IsBreak = breaker(st.current_state(), st.current_time());
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}
				obs(st.current_state(), st.current_time());
				// overwrite start_state with the final point
				boost_odeint::copy(st.current_state(), start_state);
				return std::make_tuple(IsBreak, count, st.current_time());
			}

			//integrate adaptive for dense output stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, semidense_output_stepper_tag)
			{
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				// check breaker condition with the initial state
				bool IsBreak = breaker(start_state, start_time));

				size_t count = 0;
				st.initialize(start_state, start_time, dt);

				while (!IsBreak && boost_odeint::detail::less_with_sign(st.current_time(), end_time, st.current_time_step()))
				{
					while (!IsBreak && boost_odeint::detail::less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()),
						end_time,
						st.current_time_step()))
					{   //make sure we don't go beyond the end_time
						obs(st.current_state(), st.current_time());
						st.do_step(sys);
						++count;
						IsBreak = breaker(st.current_state(), st.current_time());
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}
				obs(st.current_state(), st.current_time());
				// overwrite start_state with the final point
				boost_odeint::copy(st.current_state(), start_state);
				return std::make_tuple(IsBreak, count, st.current_time());
			}

			//integrate adaptive for adaptive stepper
			template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
			std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time end_time, Time dt, Breaker breaker,
				Observer observer, adaptive_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				// check breaker condition with the initial state
				bool IsBreak = breaker(start_state, start_time));

				boost_odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
				size_t count = 0;
				while (!IsBreak && boost_odeint::detail::less_with_sign(start_time, end_time, dt))
				{
					obs(start_state, start_time);
					if (boost_odeint::detail::less_with_sign(end_time, static_cast<Time>(start_time + dt), dt))
					{
						dt = end_time - start_time;
					}
					//argument "start_time" will be updated inside of do_step.
					st.do_step(sys, start_state, start_time, dt);
					++count;

					IsBreak = breaker(start_state, start_time));
				}
				obs(start_state, start_time);
				return std::make_tuple(IsBreak, count, start_time);
			}
		} 

		template< class Stepper, class System, class State, class Time, class Breaker, class Observer >
		std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
			Stepper stepper, System sys, State& start_state,
			Time start_time, Time end_time, Time dt,
			Breaker breaker, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::breakable_integrate_adaptive(stepper, sys, start_state, start_time, end_time, dt, breaker, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time, class Breaker>
		std::tuple<bool, size_t, Time> breakable_integrate_adaptive(
			Stepper stepper, System sys, State& start_state,
			Time start_time, Time end_time, Time dt,
			Breaker breaker) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::breakable_integrate_adaptive(stepper, sys, start_state, start_time, end_time, dt, breaker, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
} 
#
#endif
