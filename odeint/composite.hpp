#ifndef HMLIB_ODEINT_COMPOSITE_INC
#define HMLIB_ODEINT_COMPOSITE_INC 100
#
#include <vector>
#include <utility>
#include <type_traits>
#include "../utility.hpp"
namespace hmLib {
	namespace odeint {
		namespace composite{
			template<typename condition_>
			struct is_condition{
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
				using type = decltype(check(std::declval<condition_>()));
				static constexpr bool value = type::value;
			};
			template<typename require_>
			struct is_require{
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
				using type = decltype(check(std::declval<require_>()));
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
				using type = decltype(check(std::declval<sys_>()));
				static constexpr bool value = type::value;
			};

			namespace detail {
				template<typename T, bool IsCmpSys = is_composite_system<T>::value>
				struct apply_for_composite_system{
					template<typename U, typename state_type, typename time_type>
					static void update(U v, const state_type& x, time_type t) {
						v.update(x, t);
					}
					template<typename U, typename state_type, typename time_type>
					static void validate(U v,  state_type& x, time_type t) {
						v.validate(x, t);
					}
					template<typename U, typename state_type, typename time_type>
					static bool valid(U v, const state_type& x, time_type t) {
						return v.valid(x, t);
					}
				};
				template<typename T>
				struct apply_for_composite_system<T,false> {
					template<typename U, typename state_type, typename time_type>
					static void update(U v, const state_type& x, time_type t) {
					}
					template<typename U, typename state_type, typename time_type>
					static void validate(U v, state_type& x, time_type t) {
					}
					template<typename U, typename state_type, typename time_type>
					static bool valid(U v, const state_type& x, time_type t) {
						return true;
					}
				};
			}

			template<typename condition_, typename true_system_, typename false_system_, typename state_type_ = typename condition_::state_type, typename time_type_= typename condition_::time_type>
			struct ifelse_system{
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
				void update(const state_type& x, time_type t){
					Condition.update(x,t);
					if(Condition.condition()){
						true_applier::update(TrueSystem, x, t);
					}else{
						false_applier::update(FalseSystem, x, t);
					}
				}
				bool valid(const state_type& x, time_type t)const{
					if(!Condition.valid(x,t))return false;

					if(Condition.condition()){
						return true_applier::valid(TrueSystem, x, t);
					}else{
						return false_applier::valid(FalseSystem, x, t);
					}
				}
				void validate(state_type& x, time_type t){
					Condition.validate(x,t);
					if(Condition.condition()){
						true_applier::validate(TrueSystem, x, t);
					}else{
						false_applier::validate(FalseSystem, x, t);
					}
				}
				void operator()(const state_type& x, state_type& dx, time_type t){
					if(Condition.condition()){
						TrueSystem(x,dx,t);
					}else{
						FalseSystem(x,dx,t);
					}
				}
			};

			template<typename system_, typename require_, typename state_type_ = typename require_::state_type, typename time_type_= typename require_::time_type>
			struct composite_system{
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
				composite_system(const base_system& System_, require Require_): System(System_), Require(std::move(Require_)){}
				composite_system(base_system&& System_, require Require_) : System(std::move(System_)), Require(std::move(Require_)) {}
			public:
				void update(const state_type& x, time_type t){
					applier::update(System, x, t);
					Require.update(x, t);
				}
				bool valid(const state_type& x, time_type t)const{
					if(!applier::valid(System,x,t))return false;
					return Require.valid(x,t);
				}
				void validate(state_type& x, time_type t){
					applier::validate(System, x, t);
					Require.validate(x,t);
				}
				void operator()(const state_type& x, state_type& dx, time_type t){
					System(x,dx,t);
					Require.require(x,dx,t);
				}
			};
		}
		
		template<typename condition, typename sys, typename... others>
		auto system_switch(condition&& Cond, sys&& Sys, others... Others){
			static_assert(composite::is_condition<std::decay<condition>::type>::value, "condition object is required.");
			return composite::ifelse_system<typename std::decay<condition>::type, typename std::decay<sys>::type, decltype(system_switch(Others...))>(
				std::forward<condition>(Cond),
				std::forward<sys>(Sys), 
				std::move(system_switch(Others...))
			);
		}
		template<typename sys>
		auto system_switch(sys Sys){return Sys;}

		template<typename sys, typename require, typename... others>
		auto system_compose(sys&& Sys, require&& Require, others... Others){
			static_assert(composite::is_require<typename std::decay<require>::type>::value, "require object is required.");
			return system_compose(
				composite::composite_system<typename std::decay<sys>::type,typename std::decay<require>::type>(
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

		namespace composite {
			template<typename state_type_, typename time_type_ = double >
			struct greater_equal_condition {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			public:
				greater_equal_condition(const state_type& val_) :val(val_) {}
				greater_equal_condition(state_type&& val_) :val(std::move(val_)) {}
			public:
				void update(const state_type& x, time_type t) { prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { return prev == check(x, t); }
				void validate(state_type& x, time_type t) {}
				bool condition()const { return prev; }
			private:
				bool check(const state_type& x, time_type t) const { return val <= x; }
			};

			template<typename state_type_, typename time_type_ = double >
			struct greater_equal_require {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			public:
				greater_equal_require(const state_type& val_) :val(val_) {}
				greater_equal_require(state_type&& val_) :val(std::move(val_)) {}
			public:
				void update(const state_type& x, time_type t) { prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { return !prev || check(x, t); }
				void validate(state_type& x, time_type t) {
					if(!check(x, t)) x = val;
				}
				void require(const state_type& x, state_type& dx, time_type t) const { if(!prev) dx = std::max<state_type>(0, dx); }
			private:
				bool check(const state_type& x, time_type t) const { return val < x; }
			};
		}
		template<typename state_type, typename time_type = double>
		auto case_greater_equal(state_type&& x) {
			return composite::greater_equal_condition<state_type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto require_greater_equal(state_type&& x) {
			return composite::greater_equal_require<state_type, time_type>(x);
		}

		namespace composite {
			template<typename state_type_, typename time_type_, typename condition_>
			struct state_at_condition {
				using state_type = state_type_;
				using time_type = time_type_;
				using condition_type = condition_;
				using element_type = typename condition_type::state_type;
			private:
				std::size_t n;
				condition_type Cond;
			public:
				state_at_condition(std::size_t n_, condition_type  Cond_) :n(n_), Cond(std::move(Cond_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Cond.update(x[n], t);
				}
				bool valid(const state_type& x, time_type t) const {
					return Cond.valid(x[n], t);
				}
				void validate(state_type& x, time_type t) {
					Cond.validate(x[n], t);
				}
				bool condition() const {
					return Cond.condition();
				}
			};
			template<typename state_type_, typename time_type_, typename require_>
			struct state_at_require {
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
				using element_type = typename require_type::state_type;
			private:
				std::size_t n;
				require_type Req;
			public:
				state_at_require(std::size_t n_, require_type  Req_) :n(n_), Req(std::move(Req_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Req.update(x[n],t);
				}
				bool valid(const state_type& x, time_type t) const {
					return Req.valid(x[n], t);
				}
				void validate(state_type& x, time_type t) {
					Req.validate(x[n], t);
				}
				void require(const state_type& x, state_type& dx, time_type t) const { 
					Req.require(x[n], dx[n], t);
				}
			};
			template<typename state_type_, typename time_type_, typename require_>
			struct state_for_each_require {
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
				using element_type = typename require_type::state_type;
			private:
				std::vector<require_type> Req;
			public:
				state_for_each_require(std::size_t n, require_type  Req_){
					Req.reserve(n);
					for(std::size_t i = 0; i<n; ++i) {
						Req.push_back(Req_);
					}
				}
			public:
				void update(const state_type& x, time_type t) {
					auto ritr = boost::begin(Req);
					auto itr = boost::begin(x);
					auto end = boost::end(x);
					while(itr!=end) {
						(*ritr++).update(*itr++, t);
					}
				}
				bool valid(const state_type& x, time_type t) const {
					auto ritr = boost::begin(Req);
					auto itr = boost::begin(x);
					auto end = boost::end(x);
					while(itr!=end) {
						if(!(*ritr++).valid(*itr++, t))return false;
					}
					return true;
				}
				void validate(state_type& x, time_type t) {
					auto ritr = boost::begin(Req);
					auto itr = boost::begin(x);
					auto end = boost::end(x);
					while(itr!=end) {
						(*ritr++).validate(*itr++, t);
					}
				}
				void require(const state_type& x, state_type& dx, time_type t) const {
					auto ritr = boost::begin(Req);
					auto itr = boost::begin(x);
					auto end = boost::end(x);
					auto ditr = boost::begin(dx);
					while(itr!=end) {
						(*ritr++).require(*itr++, *ditr++, t);
					}
				}
			};
		}
		template<typename state_type, typename type, typename time_type = double, hmLib_static_restrict(composite::is_require<type>::value)>
		auto state_at(std::size_t n, type&& Req) {
			return composite::state_at_require<state_type, time_type, typename std::decay<type>::type>(n, std::forward<type>(Req));
		}
		template<typename state_type, typename type, typename time_type = double, hmLib_static_restrict(composite::is_condition<type>::value)>
		auto state_at(std::size_t n, type&& Cond) {
			return composite::state_at_condition<state_type, time_type, typename std::decay<type>::type>(n, std::forward<type>(Cond));
		}
		template<typename state_type, typename require, typename time_type = double>
		auto state_for_each(std::size_t n, require&& Req) {
			return composite::state_for_each_require<state_type, time_type, typename std::decay<require>::type>(n, std::forward<require>(Req));
		}
	}
}
#
#endif
