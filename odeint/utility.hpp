#ifndef HMLIB_ODEINT_UTILITY_INC
#define HMLIB_ODEINT_UTILITY_INC 100
#
#include<utility>
#include<type_traits>
#include<boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include"../utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename system_>
		struct base_stepper {
		private:
			template<typename T>
			static auto check(T)->decltype(typename system_::base_stepper{});
			static auto check(...)->typename std::decay<system_>::type;
		public:
			using type = decltype(check(std::declval<typename std::decay<system_>::type>()));
		};
		namespace detail {
			template<typename stepper_,typename system_, typename state_type, typename time_type>
			struct can_initialize {
			private:
				template<typename T>
				static auto check(T)->decltype(
					std::declval<T>().initialize(
						std::declval<system_>(),
						std::declval<state_type>(),
						std::declval<time_type>, 
						std::declval<time_type>
					), 
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<stepper_>()));
				static constexpr bool value = type::value;
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_, bool can_initialize = can_initialize<stepper_,system_,state_type_,time_type_>::value>
			struct try_initialize_impl {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {
					Stepper.initialize(std::forward<sys>(Sys),std::forward<state_type>(State), Time, dT);
				}
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct try_initialize_impl <stepper_,system_,state_type_, time_type_,false>{
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {}
			};
		}
		template<typename stepper, typename sys, typename state_type, typename time_type>
		void try_initialize(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {
			detail::try_initialize_impl<typename std::decay<stepper>::type, typename std::decay<sys>::type, typename std::decay<state_type>::type, typename std::decay<time_type>::type>()(Stepper, std::forward<sys>(Sys), std::forward<state_type>(State), Time, dT);
		}
	}
}
#
#endif
