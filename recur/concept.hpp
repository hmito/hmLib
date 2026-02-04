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
		template<typename system_type, typename state_type>
		concept invariant_system = requires(system_type s, state_type& x) {
			{ s(x) } -> std::same_as<void>;
		};

		template<typename system_type, typename state_type, typename time_type, typename duration_type>
		concept const_step_system =	requires(system_type s, state_type& x, time_type t, duration_type dt) {
			{ s(x, t, dt) } -> std::same_as<void>;
		};

		template<typename system_type, typename state_type, typename time_type, typename duration_type>
		concept adaptive_step_system = requires(system_type s, state_type& x, time_type t, duration_type dt) {
			{ s(x, t, dt) } -> std::convertible_to<duration_type>;
		};
	}
}
#
#endif
