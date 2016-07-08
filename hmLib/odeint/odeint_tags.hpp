#ifndef HMLIB_ODEINT_ODEINTTAGS_INC
#define HMLIB_ODEINT_ODEINTTAGS_INC 100
#
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
namespace hmLib{
	namespace odeint{
		using stepper_tag = boost::numeric::odeint::stepper_tag;
		using error_stepper_tag=boost::numeric::odeint::error_stepper_tag;
		using explicit_error_stepper_tag = boost::numeric::odeint::explicit_error_stepper_tag;
		using explicit_error_stepper_fsal_tag = boost::numeric::odeint::explicit_error_stepper_fsal_tag;
		using controlled_stepper_tag = boost::numeric::odeint::controlled_stepper_tag;
		using explicit_controlled_stepper_tag = boost::numeric::odeint::explicit_controlled_stepper_tag;
		using explicit_controlled_stepper_fsal_tag = boost::numeric::odeint::explicit_controlled_stepper_fsal_tag;
		using dense_output_stepper_tag = boost::numeric::odeint::dense_output_stepper_tag;
	}
}
#
#endif
