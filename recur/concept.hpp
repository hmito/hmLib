#ifndef HMLIB_RECUR_STEPPERCATEGORY_INC
#define HMLIB_RECUR_STEPPERCATEGORY_INC 100
#
#include<concepts>
#include<type_traits>
namespace hmLib{
	namespace recur{
		template<typename observer_type, typename state_type, typename time_type>
		concept observer = requires(observer_type obs, const state_type& x, time_type t){
			{obs(x,t)};
		};
		template<typename breaker_type, typename state_type, typename time_type>
		concept breaker = requires(breaker_type brk, const state_type& x, time_type t){
			{brk(x,t)} -> std::convertible_to<bool>;
		};
		template<typename stepper_type, typename system_type, typename state_type>
		concept naive_stepper = requires(stepper_type st, system_type sys, state_type x){
			{st.do_step(sys,x)};
		} && !requires(stepper_type st, system_type sys, std::add_const_t<std::decay_t<state_type>> x){
			{st.do_step(sys,x)};
		};
		template<typename stepper_type, typename system_type, typename state_type, typename time_type>
		concept stepper = requires(stepper_type st, system_type sys, state_type x, time_type t){
			{st.do_step(sys,x,t)};
		} && !requires(stepper_type st, system_type sys, std::add_const_t<std::decay_t<state_type>> x, time_type t){
			{st.do_step(sys,x,t)};
		} && !requires(stepper_type st, system_type sys, state_type x,  std::add_const_t<std::decay_t<time_type>> t){
			{st.do_step(sys,x,t)};
		};
		template<typename stepper_type, typename system_type, typename state_type, typename time_type>
		concept error_stepper = requires(stepper_type st, system_type sys, state_type x, time_type t){
			{st.try_step(sys,x,t)} -> std::convertible_to<bool>;
		} && !requires(stepper_type st, system_type sys, std::add_const_t<std::decay_t<state_type>> x, time_type t){
			{st.do_step(sys,x,t)};
		} && !requires(stepper_type st, system_type sys, state_type x,  std::add_const_t<std::decay_t<time_type>> t){
			{st.do_step(sys,x,t)};
		};
	}
}
#
#endif
