#ifndef HMLIB_ODEINT_STEPPERCATEGORIES_INC
#define HMLIB_ODEINT_STEPPERCATEGORIES_INC 100
#
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
namespace hmLib {
	namespace odeint {
		using stepper_tag = boost::numeric::odeint::stepper_tag;
		using error_stepper_tag = boost::numeric::odeint::error_stepper_tag;
		using controlled_stepper_tag = boost::numeric::odeint::controlled_stepper_tag;
		using dense_output_stepper_tag = boost::numeric::odeint::dense_output_stepper_tag;

		struct semidense_output_stepper_tag: public dense_output_stepper_tag{};
		//template<typename sys_type>
		//void calc_state(sys_type sys, time_type t, state_type& x)const
		//instead of normal calc_state

		struct adaptive_stepper_tag {};
		//bool do_step(system, &state, &time, &dt);
		//return: true if step was not same with given dt state, time and next suggested dt can be updated.
	}
}
#
#endif
