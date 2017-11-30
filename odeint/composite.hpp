#ifndef HMLIB_ODEINT_COMPOSITE_INC
#define HMLIB_ODEINT_COMPOSITE_INC 100
#
#include <type_traits>
#include "../utility.hpp"
namespace hmLib {
	namespace odeint {
		namespace composite{
			template<typename condition_>
			struct is_condition{
			private:
				template<typename T>
				static auto check(T t)->decltype(std::declval<T>().condition(), std::true_type{});
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<condition_>()));
				static constexpr bool value = type::value;
			};
			template<typename require_>
			struct is_require{
			private:
				template<typename T>
				static auto check(T t)->decltype(std::declval<T>().require(std::declval<typename T::state_type>(),std::declval<typename T::state_type>(),std::declval<typename T::time_type>()), std::true_type{});
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<require_>()));
				static constexpr bool value = type::value;
			};

			template<typename state_type_, typename time_type_ = double >
			struct greater_equal_condition {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			private:
				bool check(const state_type& x, time_type t) const{ return val <= x; }
			public:
				void update(const state_type& x, time_type t) { return prev = check(x,t); }
				bool valid(const state_type& x, time_type t) const { return prev == check(x,t); }
				void update(state_type& x, time_type& t, const state_type& vx, time_type vt, const state_type& ux, time_type ut) { update(ux, ut); }
				bool condition()const{return prev;}
			};
			
			template<typename state_type_, typename time_type_ = double >
			struct greater_equal_require {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			private:
				bool check(const state_type& x, time_type t) const { return val < x; }
			public:
				void update(const state_type& x,time_type t) { return prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { return !prev || check(x, t)); }
				void update(state_type& x, time_type& t, const state_type& vx, time_type vt, const state_type& ux, time_type ut) {
					if(!check(x,t)) x = val;
					update(x, t);
				}
				void require(const state_type& x, state_type& dx, time_type t) const { if(prev) dx = std::max(0,dx); }
			};

			template<typename condition_, typename true_system_, typename false_system_, typename state_type_ = typename condition_::state_type, typename time_type_= typename condition_::time_type>
			struct ifelse_system{
				using condition = condition_;
				using true_system = true_system_;
				using false_system = false_system_;
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				condition Condition;
				true_system TrueSystem;
				false_system FalseSystem;
			public:
				void update(const state_type& x, time_type t){
					Condition.update(x,t);
					if(Condition.condition()){
						try_update(TrueSystem, x, t);
					}else{
						try_update(FalseSystem, x, t);
					}
				}
				bool valid(const state_type& x, time_type t)const{
					if(!Condition.valid(x,t))return false;

					if(Condition.condition()){
						return try_valid(TrueSystem, x, t);
					}else{
						return try_valid(FalseSystem, x, t);						
					}
				}
				void update(state_type& x, time_type& t, const state_type& vx, time_type vt, const state_type& ux, time_type ut){
					Condition.update(x,t,vx,vt,ux,ut);
					if(Condition.condition()){
						try_update(TrueSystem, x, t, vx, vt, ux, ut);
					}else{
						try_update(FalseSystem, x, t, vx, vt, ux, ut);
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
				base_system System;
				require Require;
			public:
				void update(const state_type& x, time_type t){
					try_update(System, x, t);
					Require.update(x, t);
				}
				bool valid(const state_type& x, time_type t)const{
					if(!try_valid(System,x,t))return false;
					return Require.valid(x,t);
				}
				void update(state_type& x, time_type& t, const state_type& vx, time_type vt, const state_type& ux, time_type ut){
					try_update(System, x, t, vx, vt, ux, ut);
					Require.update(x,t, vx, vt, ux, ut);
				}
				void operator()(const state_type& x, state_type& dx, time_type t){
					System(x,dx,t);
					Require.require(x,dx,t);
				}
			};
		}

		template<typename condition, typename sys, typename... others>
		auto system_switch(condition&& Cond, sys&& Sys, others... Others){
			static_assert(composites::is_condition<std::decay<condition>::type>::value, "condition is required.");
			return composites::ifelse_system<typename std::decay<condition>::type, typename std::decay<sys>::type, decltype(system_switch(others...))>(std::forward<condition>(Cond),std::forward<sys>(Sys), std::move(system_switch(Others...)));
		}
		template<typename sys>
		auto system_switch(sys Sys){return Sys;}

		template<typename sys, typename require, hmLib_static_restrict(composites::is_require<typename std::decay<require>::type>::value)>
		auto operator&(sys&& Sys, require&& Require){
			return composites::composite_system<typename std::decay<sys>::type,typename std::decay<require>::type>(std::forward<sys>(Sys),std::forward<require>(Require));
		}
	}
}
#
#endif
