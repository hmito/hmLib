#ifndef HMLIB_ODEINT_VALIDATE_INC
#define HMLIB_ODEINT_VALIDATE_INC 100
#
#include"validate/validate_result.hpp"
#include"validate/state_validate_stepper.hpp"
#include"validate/step_validate_stepper.hpp"
#include"validate/non_appendix_observer.hpp"
/*!
@page odeint.validate
@section What is odeint.validate?
boost.odeint provide the easy methods for calculating the ODE system, but they 
don't condiered the situation that the parameter space is bounded by the 
limitation, e.g., x should not be negative or x+y should be less than one.
odeint.validate provides soem helper steppers for applying boost.odeint steppers
to such bounded parameter space.

@section state validate and step validate
There are two types of validate. One is the state_validate, which validate the end point of each step. validate functon is called at each step.

@code
struct state_validatable_system{
	void operator(const state_type& x, state_type& dx, time_type t);
	validate_result validate(const state_type& x, time_type t, state_type& nx);
};
@endcod

@code
struct state_validatable_system{
	void initialize(appendix_type ap);
	void operator(const state_type& x, state_type& dx, time_type t);
	validate_result validate(const state_type& x, time_type t, state_type& nx, appendix_type& ap);
};
@endcod

@code
struct step_validatable_system{
	void operator(const state_type& x, state_type& dx, time_type t);
	bool is_invalid_step(const state_type& x, time_type t);
	validate_result validate(const state_type& x, time_type t, state_type& nx);
	validate_result validate(const state_type& x1, const state_type& x2, time_type t, state_type& nx);
};
@endcod

@code
struct step_validatable_system{
	void initialize(appendix_type ap);
	void operator(const state_type& x, state_type& dx, time_type t);
	bool is_invalid_step(const state_type& x, time_type t);
	validate_result validate(const state_type& x, time_type t, state_type& nx, appendix_type& ap);
	validate_result validate(const state_type& x1, const state_type& x2, time_type t, state_type& nx, appendix_type& ap);
};
@endcod
*/
#
#endif
