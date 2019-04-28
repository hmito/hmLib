#ifndef HMLIB_ODEINT_INTEGRATE_INTEGRATECONST_INC
#define HMLIB_ODEINT_INTEGRATE_INTEGRATECONST_INC 100
#
#include"../stepper_categories.hpp"
#include<boost/numeric/odeint/integrate/null_observer.hpp>
#include<boost/numeric/odeint/integrate/integrate_const.hpp>
namespace hmLib {
	namespace odeint {
		namespace detail {
			namespace boost_odeint = boost::numeric::odeint;

			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, stepper_tag tag
			) {
				return boost_odeint::integrate_const(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, controlled_stepper_tag tag
			) {
				return boost_odeint::integrate_const(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, dense_output_stepper_tag tag
			) {
				return boost_odeint::integrate_const(stepper, system, start_state, start_time, end_time, dt, observer, tag);
			}

			//forward declaration for integrate_const with adaptive stepper
			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_adaptive(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, adaptive_stepper_tag tag);

			template< class Stepper, class System, class State, class Time, class Observer >
			size_t integrate_const(
				Stepper stepper, System system, State& start_state,
				Time start_time, Time end_time, Time dt,
				Observer observer, adaptive_stepper_tag tag
			) {
				typename boost_odeint::unwrap_reference< Observer >::type& obs = observer;

				Time time = start_time;
				const Time time_step = dt;
				int real_steps = 0;
				int step = 0;

				while (boost_odeint::detail::less_eq_with_sign(static_cast<Time>(time + time_step), end_time, dt))
				{
					obs(start_state, time);
					// integrate_adaptive_checked uses the given checker to throw if an overflow occurs
					real_steps += integrate_adaptive(stepper, system, start_state, time,
						static_cast<Time>(time + time_step), dt,
						boost_odeint::null_observer(), adaptive_stepper_tag());
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					step++;
					time = start_time + static_cast<typename boost_odeint::unit_value_type<Time>::type>(step) * time_step;
				}
				obs(start_state, time);

				return real_steps;
			}
		}

		template< class Stepper, class System, class State, class Time, class Observer >
		size_t integrate_const(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt, Time time_error, Observer observer) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, time_error, observer, stepper_category());
		}

		template< class Stepper, class System, class State, class Time>
		size_t integrate_const(Stepper stepper, System sys, State& start_state, Time start_time, Time end_time, Time dt, Time time_error) {
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::integrate_const(stepper, sys, start_state, start_time, end_time, dt, time_error, boost::numeric::odeint::null_observer(), stepper_category());
		}
	}
}
#
#endif
