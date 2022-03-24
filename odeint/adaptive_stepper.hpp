#ifndef HMLIB_ODEINT_ADAPTIVESTEPPER_INC
#define HMLIB_ODEINT_ADAPTIVESTEPPER_INC 100
#
#include <boost/numeric/odeint/util/unwrap_reference.hpp>
#include <boost/numeric/odeint/stepper/generation.hpp>
#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/integrate/max_step_checker.hpp>
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
		controlled_stepper just try with given dt, so they always use the given dt although they often fail.
		adaptive_stepper automatically choose optimal dt, so the given dt might be not used.
		*/

		template<typename base_controlled_stepper_>
		struct adaptive_stepper {
			using base_stepper = base_controlled_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = adaptive_stepper_tag;
		public:
			adaptive_stepper(base_stepper st_) :st(std::move(st_)) {}
			template<typename system_type>
			bool do_step(system_type sys, state_type& state, time_type& time, time_type& dt) {
				boost::numeric::odeint::failed_step_checker fail_checker;
				boost::numeric::odeint::controlled_step_result res;

				res = st.try_step(sys, state, time, dt);
				if (res == boost::numeric::odeint::success)return false;

				do{
					res = st.try_step(sys, state, time, dt);
					fail_checker();  // check number of failed steps
				} while (res == boost::numeric::odeint::fail);
				return true;
			}
		private:
			base_stepper st;
		};
		namespace detail{
			template<typename base_stepper_>
			void make_adaptive_impl(const base_stepper_& stepper, ...) {
				//static_assert(false, "adaptive_stepper require controlled stepper as the base stepper.");
			}
			template<typename base_stepper_>
			auto make_adaptive_impl(const base_stepper_& stepper, controlled_stepper_tag) {
				return adaptive_stepper<base_stepper_>(stepper);
			}
		}
		template<typename base_controlled_stepper_>
		auto make_adaptive(const base_controlled_stepper_& stepper = base_controlled_stepper_()) {
			using stepper_category_ = typename boost::numeric::odeint::unwrap_reference<base_controlled_stepper_>::type::stepper_category;
			return detail::make_adaptive_impl(stepper, stepper_category_());
		}		
		template<typename base_stepper_>
		auto make_adaptive(
			typename base_stepper_::value_type abs_error,
			typename base_stepper_::value_type rel_error,
			const base_stepper_& stepper = base_stepper_()) {
			return make_adaptive(boost::numeric::odeint::make_controlled(abs_error, rel_error, stepper));
		}
		template<typename base_stepper_>
		auto make_adaptive(
			typename base_stepper_::value_type abs_error,
			typename base_stepper_::value_type rel_error,
			typename base_stepper_::time_type max_dt,
			const base_stepper_& stepper = base_stepper_()) {
			return make_adaptive(boost::numeric::odeint::make_controlled(abs_error, rel_error, max_dt, stepper));
		}
	}
}


#
#endif
