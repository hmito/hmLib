#ifndef HMLIB_ODEINT_ADAPTIVESTEPPER_INC
#define HMLIB_ODEINT_ADAPTIVESTEPPER_INC 100
#
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
#include <boost/numeric/odeint/stepper/controlled_runge_kutta.hpp>
#include"stepper_categories.hpp"
namespace hmLib {
	namespace odeint {
		/*
		requirement of "adaptive_stepper" concept
		struct adaptive_stepper{
			using state_type = ???;
			using deriv_type = ???;
			using time_type = ???;
			using order_type = ???;
			using stepper_category = adaptive_stepper_tag;
		public:
			template<typename system_type>
			bool do_step(system_type sys, state_type& state, time_type& time, time_type& dt)
		};
		*/

		template<typename base_stepper_, typename stepper_tag_ = typename boost::numeric::odeint::unwrap_reference<base_stepper_>::type::stepper_category>
		struct adaptive_stepper {
			static_assert(!std::is_base_of<controlled_stepper_tag, stepper_tag_>::value, "adaptive_stepper can use a stepper whose stepper_category is controlled_stepper_tag as the base_stepper.");
		};
		template<typename base_stepper_>
		struct adaptive_stepper<base_stepper_, controlled_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using stepper_category = adaptive_stepper_tag;
		public:
			adaptive_stepper(base_stepper st_) :st(std::move(st_)) {}
			template<typename system_type>
			bool do_step(system_type sys, state_type& state, time_type& time, time_type& dt) {
				boost::numeric::odeint::failed_step_checker fail_checker;
				boost::numeric::odeint::controlled_step_result res;

				res = st.try_step(system, start_state, start_time, dt);
				if (res == boost::numeric::odeint::success)return false;

				do{
					res = st.try_step(system, start_state, start_time, dt);
					fail_checker();  // check number of failed steps
				} while (res == fail);
				return true;
			}
		private:
			base_stepper st;
		};

		template<typename base_stepper_>
		auto make_adaptive(const base_stepper_& stepper = Stepper()) {
			return adaptive_stepper<base_stepper_>(stepper);
		}		
		template<typename base_stepper_>
		auto make_adaptive(
			typename base_stepper_::value_type abs_error,
			typename base_stepper_::value_type rel_error,
			const base_stepper_& stepper = Stepper()) {
			return make_adaptive(boost::numeric::odeint::make_controlled(abs_error, rel_error, stepper));
		}
		template<typename base_stepper_>
		auto make_adaptive(
			typename base_stepper_::value_type abs_error,
			typename base_stepper_::value_type rel_error,
			typename base_stepper_::time_type max_dt,
			const base_stepper_& stepper = Stepper()) {
			return make_adaptive(boost::numeric::odeint::make_controlled(abs_error, rel_error, max_dt, stepper));
		}
	}
}


#
#endif
