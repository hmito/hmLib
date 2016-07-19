#ifndef HMLIB_ODEINT_BREAKABLE_INTEGRATE_INC
#define HMLIB_ODEINT_BREAKABLE_INTEGRATE_INC 100
#
#include<utility>
#include<boost/numeric/odeint.hpp>
namespace hmLib{
	namespace odeint{
		namespace detail{
			/*
			* integrate_adaptive for simple stepper is basically an integrate_const + some last step
			*/
			template< class Stepper, class System, class State, class Time, class BreakableObserver >
			std::pair<Time, size_t> breakable_integrate_adaptive(Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt, BreakableObserver observer, boost::numeric::odeint::stepper_tag
			){
				using namespace boost::numeric::odeint;
				using namespace boost::numeric::odeint::detail;

				unwrap_reference< BreakableObserver >::type &obs = observer;
				unwrap_reference< Stepper >::type &st = stepper;

				Time time = start_time;
				int step = 0;
				// cast time+dt explicitely in case of expression templates (e.g. multiprecision)
				while(less_eq_with_sign(static_cast<Time>(time + dt), end_time, dt)){
					if(obs(start_state, time)){
						return std::make_pair(time, step);
					}
					st.do_step(system, start_state, time, dt);
					// direct computation of the time avoids error propagation happening when using time += dt
					// we need clumsy type analysis to get boost units working here
					++step;
					time = start_time + static_cast< typename unit_value_type<Time>::type >(step) * dt;
				}
				obs(start_state, time);

				return std::make_pair(time,step);
			}


			/*
			* integrate adaptive for controlled stepper
			*/
			template< class Stepper, class System, class State, class Time, class BreakableObserver >
			std::pair<Time,size_t> breakable_integrate_adaptive(Stepper stepper, System system, State &start_state,
				Time &start_time, Time end_time, Time &dt, BreakableObserver observer, boost::numeric::odeint::controlled_stepper_tag
			){
				using namespace boost::numeric::odeint;
				using namespace boost::numeric::odeint::detail;

				typename unwrap_reference< BreakableObserver >::type &obs = observer;
				typename unwrap_reference< Stepper >::type &st = stepper;

				failed_step_checker fail_checker;  // to throw a runtime_error if step size adjustment fails
				size_t count = 0;
				while(less_with_sign(start_time, end_time, dt)){
					if(obs(start_state, start_time))return std::make_pair(start_time,count);
					if(less_with_sign(end_time, static_cast<Time>(start_time + dt), dt)){
						dt = end_time - start_time;
					}

					controlled_step_result res;
					do{
						res = st.try_step(system, start_state, start_time, dt);
						fail_checker();  // check number of failed steps
					} while(res == fail);
					fail_checker.reset();  // if we reach here, the step was successful -> reset fail checker

					++count;
				}
				obs(start_state, start_time);

				return std::make_pair(start_time, count)
			}


			/*
			* integrate adaptive for dense output steppers
			*
			* step size control is used if the stepper supports it
			*/
			template< class Stepper, class System, class State, class Time, class BreakableObserver >
			std::pair<Time, size_t> breakable_integrate_adaptive(Stepper stepper, System system, State &start_state,
				Time start_time, Time end_time, Time dt, BreakableObserver observer, boost::numeric::odeint::dense_output_stepper_tag
			){
				using namespace boost::numeric::odeint;
				using namespace boost::numeric::odeint::detail;

				typename unwrap_reference< BreakableObserver >::type &obs = observer;
				typename unwrap_reference< Stepper >::type &st = stepper;

				size_t count = 0;
				st.initialize(start_state, start_time, dt);

				while(less_with_sign(st.current_time(), end_time, st.current_time_step())){
					while(less_eq_with_sign(static_cast<Time>(st.current_time() + st.current_time_step()),
						end_time,
						st.current_time_step())){   //make sure we don't go beyond the end_time
						if(obs(st.current_state(), st.current_time())){
							// overwrite start_state with the final point
							boost::numeric::odeint::copy(st.current_state(), start_state);
							return std::make_pair(st.current_time(), count);
						}
						st.do_step(system);
						++count;
					}
					// calculate time step to arrive exactly at end time
					st.initialize(st.current_state(), st.current_time(), static_cast<Time>(end_time - st.current_time()));
				}
				obs(st.current_state(), st.current_time());

				// overwrite start_state with the final point
				boost::numeric::odeint::copy(st.current_state(), start_state);

				return std::make_pair(st.current_time(), count);
			}
		}

		template< class Stepper, class System, class State, class Time, class BreakableObserver >
		size_t breakable_integrate_adaptive(Stepper stepper, System system, State &start_state,Time start_time, Time end_time, Time dt,BreakableObserver observer){
			using stepper_category = typename boost::numeric::odeint::unwrap_reference<Stepper>::type::stepper_category;
			return detail::breakable_integrate_adaptive(stepper,system, start_state,start_time, end_time, dt,observer,stepper_category());
		}
	}
}
#
#endif
