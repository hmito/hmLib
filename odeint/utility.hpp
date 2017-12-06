#ifndef HMLIB_ODEINT_UTILITY_INC
#define HMLIB_ODEINT_UTILITY_INC 100
#
#include<utility>
#include<type_traits>
#include<boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include"../utility.hpp"
#include"../type_traits.hpp"
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

		namespace detail {
			template<typename state, hmLib_static_restrict(has_begin_and_end<state>::value)>
			double abs_distance(const state& State1, const state& State2) {
				auto itr1 = State1.begin();
				auto end1 = State1.end();
				auto itr2 = State2.begin();

				double Max = 0;
				for (; itr1 != end1; ++itr1, ++itr2) {
					Max = std::max(Max, boost::numeric::odeint::vector_space_norm_inf<decltype(*itr1)>()(*itr1 - *itr2));
				}

				return Max;
			}
			template<typename state, hmLib_static_restrict(!has_begin_and_end<state>::value)>
			double abs_distance(const state& State1, const state& State2) {
				return boost::numeric::odeint::vector_space_norm_inf<state>()(State1 + (State2 * -1.0));
			}
		}
	}
}
#
#endif
