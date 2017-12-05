#ifndef HMLIB_ODEINT_COMPOSITE_COMPOSITESYSTEM_INC
#define HMLIB_ODEINT_COMPOSITE_COMPOSITESYSTEM_INC 100
#
#include <utility>
#include <type_traits>
#include "../../utility.hpp"
namespace hmLib {
	namespace odeint {
		/*
		composite system
			update will called once before stepper start to call system operator().
			valid will checked after each step.
			validate will called if the step might not be valid.
		*/
		namespace composite {
			template<typename condition_>
			struct is_condition {
			private:
				template<typename T>
				static auto check(T t)->decltype(
					std::declval<T>().update(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().validate(
						std::declval<typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().valid(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().condition(),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<typename std::decay<condition_>::type>()));
				static constexpr bool value = type::value;
			};
			template<typename require_>
			struct is_require {
			private:
				template<typename T>
				static auto check(T t)->decltype(
					std::declval<T>().update(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().validate(
						std::declval<typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().valid(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().require(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<typename std::decay<require_>::type>()));
				static constexpr bool value = type::value;
			};
			template<typename sys_>
			struct is_composite_system {
			private:
				template<typename T>
				static auto check(T t)->decltype(
					std::declval<T>().update(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().validate(
						std::declval<typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>().valid(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::time_type>()
					),
					std::declval<T>()(
						std::declval<const typename T::state_type&>(),
						std::declval<typename T::state_type&>(),
						std::declval<typename T::time_type>()
						),
					std::true_type{}
				);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<typename std::decay<sys_>::type>()));
				static constexpr bool value = type::value;
			};

			namespace detail {
				template<typename T, bool IsCmpSys = is_composite_system<T>::value>
				struct apply_for_composite_system {
					template<typename U, typename state_type, typename time_type>
					static void update(U& v, const state_type& x, time_type t) {
						v.update(x, t);
					}
					template<typename U, typename state_type, typename time_type>
					static void validate(U& v, state_type& x, time_type t) {
						v.validate(x, t);
					}
					template<typename U, typename state_type, typename time_type>
					static bool valid(U& v, const state_type& x, time_type t) {
						return v.valid(x, t);
					}
				};
				template<typename T>
				struct apply_for_composite_system<T, false> {
					template<typename U, typename state_type, typename time_type>
					static void update(U& v, const state_type& x, time_type t) {
					}
					template<typename U, typename state_type, typename time_type>
					static void validate(U& v, state_type& x, time_type t) {
					}
					template<typename U, typename state_type, typename time_type>
					static bool valid(U& v, const state_type& x, time_type t) {
						return true;
					}
				};

				template<typename system_>
				struct has_base_system {
				private:
					template<typename T>
					static auto check(T)->decltype(std::declval<typename T::base_system>(), std::true_type{});
					static auto check(...)->std::false_type;
				public:
					using type = decltype(check(std::declval<system_>()));
					static constexpr bool value = type::value;
				};
				template<typename system_, bool has_base_system_ = has_base_system<system_>::value>
				struct get_base_system {
					template<typename T>
					auto& operator()(T&& Sys) { return Sys; }
				};
				template<typename system_>
				struct get_base_system<system_,true> {
					template<typename T>
					auto& operator()(T&& Sys) { return Sys.base(); }
				};
			}

			template<typename condition_, typename true_system_, typename false_system_, typename state_type_ = typename condition_::state_type, typename time_type_ = typename condition_::time_type>
			struct ifelse_system {
				using condition = condition_;
				using true_system = true_system_;
				using false_system = false_system_;
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				using true_applier = detail::apply_for_composite_system<true_system_>;
				using false_applier = detail::apply_for_composite_system<false_system_>;
			private:
				condition Condition;
				true_system TrueSystem;
				false_system FalseSystem;
			public:
				ifelse_system(condition Condition_, const true_system& TrueSystem_, const false_system& FalseSystem_) : Condition(std::move(Condition_)), TrueSystem(TrueSystem_), FalseSystem(FalseSystem_) {}
				ifelse_system(condition Condition_, true_system&& TrueSystem_, const false_system& FalseSystem_) : Condition(std::move(Condition_)), TrueSystem(std::move(TrueSystem_)), FalseSystem(std::move(FalseSystem_)) {}
				ifelse_system(condition Condition_, const true_system& TrueSystem_, false_system&& FalseSystem_) : Condition(std::move(Condition_)), TrueSystem(TrueSystem_), FalseSystem(FalseSystem_) {}
				ifelse_system(condition Condition_, true_system&& TrueSystem_, false_system&& FalseSystem_) : Condition(std::move(Condition_)), TrueSystem(std::move(TrueSystem_)), FalseSystem(std::move(FalseSystem_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Condition.update(x, t);
					if(Condition.condition()) {
						true_applier::update(TrueSystem, x, t);
					} else {
						false_applier::update(FalseSystem, x, t);
					}
				}
				bool valid(const state_type& x, time_type t) {
					if(!Condition.valid(x, t))return false;

					if(Condition.condition()) {
						return true_applier::valid(TrueSystem, x, t);
					} else {
						return false_applier::valid(FalseSystem, x, t);
					}
				}
				void validate(state_type& x, time_type t) {
					Condition.validate(x, t);
					if(Condition.condition()) {
						true_applier::validate(TrueSystem, x, t);
					} else {
						false_applier::validate(FalseSystem, x, t);
					}
				}
				void operator()(const state_type& x, state_type& dx, time_type t) {
					if(Condition.condition()) {
						TrueSystem(x, dx, t);
					} else {
						FalseSystem(x, dx, t);
					}
				}
			public:
				true_system& true_base() { return TrueSystem; }
				false_system& false_base() { return FalseSystem; }
			};

			template<typename system_, typename require_, typename state_type_ = typename require_::state_type, typename time_type_ = typename require_::time_type>
			struct composite_system {
				using base_system = system_;
				using require = require_;
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				using applier = detail::apply_for_composite_system<system_>;
			private:
				base_system System;
				require Require;
			public:
				composite_system(const base_system& System_, require Require_) : System(System_), Require(std::move(Require_)) {}
				composite_system(base_system&& System_, require Require_) : System(std::move(System_)), Require(std::move(Require_)) {}
			public:
				void update(const state_type& x, time_type t) {
					applier::update(System, x, t);
					Require.update(x, t);
				}
				bool valid(const state_type& x, time_type t) {
					if(!applier::valid(System, x, t))return false;
					return Require.valid(x, t);
				}
				void validate(state_type& x, time_type t) {
					applier::validate(System, x, t);
					Require.validate(x, t);
				}
				void operator()(const state_type& x, state_type& dx, time_type t) {
					System(x, dx, t);
					Require.require(x, dx, t);
				}
			public:
				auto& base() { return detail::get_base_system<base_system>()(System); }
			};
		}

		template<typename condition, typename sys, typename... others>
		auto system_switch(condition&& Cond, sys&& Sys, others... Others) {
			static_assert(composite::is_condition<condition>::value, "condition object is required.");
			return composite::ifelse_system<typename std::decay<condition>::type, typename std::decay<sys>::type, decltype(system_switch(Others...))>(
				std::forward<condition>(Cond),
				std::forward<sys>(Sys),
				std::move(system_switch(Others...))
				);
		}
		template<typename sys>
		auto system_switch(sys Sys) { return Sys; }

		template<typename sys, typename require, typename... others>
		auto system_compose(sys&& Sys, require&& Require, others... Others) {
			static_assert(composite::is_require<typename std::decay<require>::type>::value, "require object is required.");
			return system_compose(
				composite::composite_system<typename std::decay<sys>::type, typename std::decay<require>::type>(
					std::forward<sys>(Sys),
					std::forward<require>(Require)
					),
				Others...
			);
		}
		template<typename sys, typename require>
		auto system_compose(sys&& Sys, require&& Require) {
			static_assert(composite::is_require<typename std::decay<require>::type>::value, "require object is required.");
			return composite::composite_system<typename std::decay<sys>::type, typename std::decay<require>::type>(
				std::forward<sys>(Sys),
				std::forward<require>(Require)
				);
		}

	}
}
#
#endif
