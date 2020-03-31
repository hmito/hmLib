#ifndef HMLIB_ODEINT_INTEGRATE_INTEGRATEADAPTIVEN_INC
#define HMLIB_ODEINT_INTEGRATE_INTEGRATEADAPTIVEN_INC 100
#
#include<boost/numeric/odeint/util/unwrap_reference.hpp>
#include<boost/numeric/odeint/integrate/null_observer.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include"../stepper_categories.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;
			template< class Stepper, class System, class State, class Time, class Observer >
			Time integrate_adaptive_n(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time dt, size_t step_num,
				Observer observer, stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				for (size_t step = 0; step < step_num; ++step) {
					observer(start_state, start_time);
					st.do_step(sys, start_state, start_time, dt);
				}
				observer(start_state, start_time);
				return start_time;
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			Time integrate_adaptive_n(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time dt, size_t step_num,
				Observer observer, controlled_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				boost_odeint::failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails

				for (size_t step = 0; step < step_num; ++step) {
					observer(start_state, start_time);
					boost_odeint::controlled_step_result res;
					do
					{
						res = st.try_step(sys, start_state, start_time, dt);
						fail_checker();  // check number of failed steps
					} while (res == boost_odeint::controlled_step_result::fail);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker
				}
				observer(start_state, start_time);
				return start_time;
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			Time integrate_adaptive_n(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time dt, size_t step_num,
				Observer observer, dense_output_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				st.initialize(start_state, start_time, dt);
				for (size_t step = 0; step < step_num; ++step) {
					obs(st.current_state(), st.current_time());
					st.do_step(sys);
				}
				obs(st.current_state(), st.current_time());

				// overwrite start_state with the final point
				boost_odeint::copy(st.current_state(), start_state);
				return st.current_time();
			}

			template< class Stepper, class System, class State, class Time, class Observer >
			Time integrate_adaptive_n(
				Stepper stepper, System sys, State& start_state,
				Time start_time, Time dt, size_t step_num,
				Observer observer, adaptive_stepper_tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;
				typename boost_odeint::unwrap_reference< Stepper >::type& st = stepper;

				for (size_t step = 0; step < step_num; ++step) {
					obs(start_state, start_time);
					st.do_step(sys, start_state, start_time, dt);
				}
				obs(start_state, start_time);

				return start_time;
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		Time integrate_adaptive_n(Stepper stepper, System sys, State& start_state, Time start_time, Time dt, size_t step_num, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_adaptive_n(stepper, sys, start_state, start_time, dt, step_num, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		Time integrate_adaptive_n(Stepper stepper, System sys, State& start_state, Time start_time, Time dt, size_t step_num) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_adaptive_n(stepper, sys, start_state, start_time,dt, step_num, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
