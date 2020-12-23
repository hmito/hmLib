#ifndef HMLIB_ODEINT_UTILITY_INC
#define HMLIB_ODEINT_UTILITY_INC 100
#
#include<utility>
#include<type_traits>
#include<boost/numeric/odeint/util/copy.hpp>
#include<boost/numeric/odeint/util/resize.hpp>
#include<boost/numeric/odeint/util/resizer.hpp>
#include<boost/numeric/odeint/util/is_resizeable.hpp>
#include<boost/numeric/odeint/util/same_size.hpp>
#include<boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include<boost/numeric/odeint/algebra/array_algebra.hpp>
#include<boost/numeric/odeint/algebra/range_algebra.hpp>
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
			struct can_system_initialize {
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
			template<typename stepper_, typename state_type, typename time_type>
			struct can_normal_initialize {
			private:
				template<typename T>
				static auto check(T)->decltype(
					std::declval<T>().initialize(
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
			template<typename stepper_, typename system_, typename state_type_, typename time_type_,
				bool can_system_initialize = can_system_initialize<stepper_, system_, state_type_, time_type_>::value,
				bool can_normal_initialize = can_normal_initialize<stepper_, state_type_, time_type_>::value>
			struct try_initialize_impl {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {}
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct try_initialize_impl <stepper_,system_,state_type_, time_type_,true, false>{
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {
					Stepper.initialize(std::forward<sys>(Sys), std::forward<state_type>(State), Time, dT);
				}
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct try_initialize_impl <stepper_, system_, state_type_, time_type_, true, true> {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {
					Stepper.initialize(std::forward<sys>(Sys), std::forward<state_type>(State), Time, dT);
				}
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct try_initialize_impl <stepper_, system_, state_type_, time_type_, false, true> {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys&& Sys, state_type&& State, time_type Time, time_type dT) {
					Stepper.initialize(std::forward<state_type>(State), Time, dT);
				}
			};
		}
		template<typename stepper, typename sys, typename state_type, typename time_type>
		void try_initialize(stepper& Stepper, sys& Sys, state_type& State, time_type Time, time_type dT) {
			detail::try_initialize_impl<typename std::decay<stepper>::type, typename std::decay<sys>::type, typename std::decay<state_type>::type, typename std::decay<time_type>::type>()(Stepper, std::forward<sys>(Sys), std::forward<state_type>(State), Time, dT);
		}
		namespace detail {
			template<typename stepper_, typename state_type>
			struct can_adjust_size {
			private:
				template<typename T>
				static auto check(T)->decltype(
					std::declval<T>().adjust_size(std::declval<state_type>()),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<stepper_>()));
				static constexpr bool value = type::value;
			};
			template<typename stepper_, typename state_type_, bool can_adjust_size = can_adjust_size<stepper_, state_type_>::value>
			struct try_adjust_size_impl {
				template<typename stepper, typename state_type>
				void operator()(stepper& Stepper, state_type&& State) {
					Stepper.adjust_size(std::forward<state_type>(State));
				}
			};
			template<typename stepper_, typename state_type_>
			struct try_adjust_size_impl <stepper_, state_type_, false> {
				template<typename stepper, typename state_type>
				void operator()(stepper& Stepper, state_type&& State) {}
			};
		}
		template<typename stepper, typename state_type>
		void try_adjust_size(stepper& Stepper, state_type& State) {
			detail::try_adjust_size_impl<typename std::decay<stepper>::type, typename std::decay<state_type>::type>()(Stepper, std::forward<state_type>(State));
		}
		namespace detail {
			template<typename stepper_>
			struct can_reset {
			private:
				template<typename T>
				static auto check(T)->decltype(
					std::declval<T>().reset(),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<stepper_>()));
				static constexpr bool value = type::value;
			};
			template<typename stepper_, bool can_reset = can_reset<stepper_>::value>
			struct try_reset_impl {
				template<typename stepper>
				void operator()(stepper& Stepper) {Stepper.reset();}
			};
			template<typename stepper_>
			struct try_reset_impl <stepper_, false> {
				template<typename stepper>
				void operator()(stepper& Stepper) {}
			};
		}
		template<typename stepper>
		void try_reset(stepper& Stepper) {
			detail::try_reset_impl<typename std::decay<stepper>::type>()(Stepper);
		}

		namespace detail {
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct can_extended_calc_state {
			private:
				template<typename T>
				static auto check(T)->decltype(
					std::declval<T>().calc_state(
						std::declval<system_&>(),
						std::declval<time_type_>,
						std::declval<state_type_&>
					),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<stepper_>()));
				static constexpr bool value = type::value;
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_,
				bool extend_calc_state = can_extended_calc_state<stepper_, system_, state_type_, time_type_>::value>
			struct calc_state_impl {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys& Sys,time_type Time, state_type& State) {
					Stepper.calc_state(Time, State);
				}
			};
			template<typename stepper_, typename system_, typename state_type_, typename time_type_>
			struct calc_state_impl <stepper_, system_, state_type_, time_type_, true> {
				template<typename stepper, typename sys, typename state_type, typename time_type>
				void operator()(stepper& Stepper, sys& Sys, time_type Time, state_type& State) {
					Stepper.calc_state(Sys,Time, State);
				}
			};
		}
		template<typename stepper, typename sys, typename time_type, typename state_type>
		void calc_state(stepper& Stepper, sys& Sys, time_type Time, state_type& State) {
			detail::calc_state_impl<typename std::decay<stepper>::type, typename std::decay<sys>::type, typename std::decay<state_type>::type, typename std::decay<time_type>::type>()(Stepper, Sys, Time, State);
		}

		namespace utility {
			template<typename state_type>
			void copy(const state_type& from, state_type& to) {
				boost::numeric::odeint::copy(from, to);
			}
			template<typename state_type>
			void move(state_type&& from, state_type& to) {
				//using std::move;
				to = std::move(from);
			}
			template<typename state_type>
			void swap(state_type& x1, state_type& x2) {
				//using std::swap;
				std::swap(x1, x2);
			}
			template<typename state_type>
			void resize(const state_type& from, state_type& to, boost::true_type) {
				if(!boost::numeric::odeint::same_size(from, to)) {
					boost::numeric::odeint::resize(to, from);
				}
			}
			template<typename state_type>
			void resize(const state_type& from, state_type& to, boost::false_type) {}
			template<typename state_type>
			void resize(const state_type& from, state_type& to) {
				using is_resizable = typename boost::numeric::odeint::template is_resizeable<state_type>::type;
				resize(from, to, is_resizable());
			}
			template<typename state_type>
			void resize_and_copy(const state_type& from, state_type& to) {
				resize(from, to);
				copy(from, to);
			}
			template<typename state_type>
			void resize_and_swap(state_type& from, state_type& to) {
				resize(from, to);
				swap(from, to);
			}
		}

		namespace detail {
			template<typename algebra_check>
			struct norm_inf_distance_impl {
				template<typename state_type,typename algebra_type,typename operations_type>
				auto operator()(const state_type& v1, const state_type& v2, state_type& out, algebra_type, operations_type) {
					algebra_type().for_each3(out, v1, v2, typename operations_type::template scale_sum2<double, double>(1.0, -1.0));
					return algebra_type().norm_inf(out);
				}
				template<typename state_type, typename algebra_type, typename operations_type>
				auto operator()(const state_type& v1, const state_type& v2, algebra_type, operations_type) {
					state_type out;
					utility::resize(v1, out);
					return operator()<state_type, algebra_type, operations_type>(v1, v2, out, algebra_type(), operations_type());
				}
			};
			template<>
			struct norm_inf_distance_impl<boost::numeric::odeint::range_algebra> {
				template<typename state_type, typename algebra_type, typename operations_type>
				auto operator()(const state_type& v1, const state_type& v2, state_type& out, algebra_type, operations_type) {
					auto itr1 = boost::begin(v1);
					auto end1 = boost::end(v1);
					auto itr2 = boost::begin(v2);
					auto itro = boost::begin(out);
					using error_type = decltype(*itr1 - *itr2);

					error_type error(0);

					using std::max;
					using std::abs;
					while(itr1 != end1) {
						typename operations_type::template scale_sum2<double, double>(1.0, -1.0)(*itro, *itr1, *itr2);
						error = max(error, abs(*itro));
						++itr1;
						++itr2;
						++itro;
					}

					return error;
				}
				template<typename state_type, typename algebra_type, typename operations_type>
				auto operator()(const state_type& v1, const state_type& v2, algebra_type, operations_type) {
					auto itr1 = boost::begin(v1);
					auto end1 = boost::end(v1);
					auto itr2 = boost::begin(v2);
					using error_type = decltype(*itr1 - *itr2);
					error_type error(0);

					using std::max;
					using std::abs;
					while(itr1 != end1) {
						error_type val(0);
						typename operations_type::template scale_sum2<double, double>(1.0, -1.0)(val, *itr1, *itr2);
						error = max(error, abs(val));
						++itr1;
						++itr2;
					}

					return error;
				}
			};
		}
		template<typename state_type,
			typename algebra_type = typename boost::numeric::odeint::algebra_dispatcher<state_type>::algebra_type,
			typename operations_type = typename boost::numeric::odeint::operations_dispatcher<state_type>::operations_type>
		auto distance_norm_inf(const state_type& v1, const state_type& v2, state_type & out) {
			return detail::norm_inf_distance_impl<algebra_type>()(v1, v2, out, algebra_type(), operations_type());
		}
		template<typename state_type,
			typename algebra_type = typename boost::numeric::odeint::algebra_dispatcher<state_type>::algebra_type,
			typename operations_type = typename boost::numeric::odeint::operations_dispatcher<state_type>::operations_type>
		auto distance_norm_inf(const state_type& v1, const state_type& v2) {
			return detail::norm_inf_distance_impl<algebra_type>()(v1, v2, algebra_type(), operations_type());
		}
	}
}
#
#endif
