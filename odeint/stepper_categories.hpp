#ifndef HMLIB_ODEINT_STEPPERCATEGORIES_INC
#define HMLIB_ODEINT_STEPPERCATEGORIES_INC 100
#
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
namespace hmLib {
	namespace odeint {
		//! @brief tag for stepper in boost.odeint.
		using stepper_tag = boost::numeric::odeint::stepper_tag;
		//! @brief tag for error_stepper in boost.odeint.
		using error_stepper_tag = boost::numeric::odeint::error_stepper_tag;
		//! @brief tag for controlled_stepper in boost.odeint.
		using controlled_stepper_tag = boost::numeric::odeint::controlled_stepper_tag;
		//! @brief  tag for dense_output_stepper_tag in boost.odeint.
		using dense_output_stepper_tag = boost::numeric::odeint::dense_output_stepper_tag;

		/*! @brief  tag for adaptive_stepper, which is original stepper in hmLib.
		adaptive_stepper change time step dt "adaptically" and therefore the given dt might be not used.
		adaptive_stepper should have a function bool do_step(system, &state, &time, &dt).
		This function return true if step was not same with given dt.
		This function change state, time and suggested dt as refrerence argument.*/
		struct adaptive_stepper_tag {};
	}
}
#
#endif
