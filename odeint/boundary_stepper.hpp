#ifndef HMLIB_ODEINT_BOUNDARYSTEPPER_INC
#define HMLIB_ODEINT_BOUNDARYSTEPPER_INC 100
#
#include <functional>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>
#include "composite_stepper.hpp"
#include "../varray.hpp"
namespace hmLib {
	namespace odeint {
		template<typename element>
		struct inviolable_boundary {};
		template<typename element>
		struct reachable_boundary {};
		template<typename element>
		struct exceedable_boundary {};

		namespace composite{
			template<typename state_type_, typename value_type_ = double>
			struct greater_equal_condition {
				using state_type = state_type_;
				using value_type = value_type_;
			private:
				state_type val;
				bool prev;
			public:
				bool condition(const state_type& x, value_type t) const{ return val <= x; }
				bool update(const state_type& x, value_type t) { return prev = condition(x,t); }
				bool valid(const state_type& x, value_type t) const { return prev == condition(x,t); }
				bool update(state_type& x, value_type& t, const state_type& vx, value_type vt, const state_type& ux, value_type ut) { update(ux, ut); }
			};
			template<typename state_type_, typename value_type_ = double>
			struct greater_equal_require {
				using state_type = state_type_;
				using value_type = value_type_;
			private:
				state_type val;
				bool prev;
			public:
				bool condition(const state_type& x, value_type t) const { return val <= x; }
				bool update(const state_type& x, value_type t) { return prev = condition(x, t); }
				bool valid(const state_type& x, value_type t) const { return !(prev  && !condition(x, t)); }
				bool update(state_type& x, value_type& t, const state_type& vx, value_type vt, const state_type& ux, value_type ut) {
					if(!valid(ux, ut)) {
						x = val;
					}
					update(x, t);
				}
				void retuire(const state_type& x, state_type& dx, value_type t) const { return val <= x; }
			};
		}
		template<typename element, typename functor>
		struct float_boundary {
		private:
			element Val;			
			functor Fn;
			bool valid(const element& olde, const element& newe) { return Fn(olde, Val) && !Fn(newe, Val); }
			bool gradient(element& de, const element& e);
		};
		template<typename element>
		using less_boundary = float_boundary<element, std::less<element>>;
		template<typename element>
		using less_equal_boundary = float_boundary<element, std::less_equal<element>>;
		template<typename element>
		using greater_boundary = float_boundary<element, std::greater<element>>;
		template<typename element>
		using greater_equal_boundary = float_boundary<element, std::greater_equal<element>>;

		template<typename T, std::size_t N, typename functor>
		struct varray_axis_boundary {
			using value_t = hmLib::varray<T, N>;
			std::size_t n;
			T val;
			functor Fn;
			bool valid(const value_t& olde, const value_t& newe) { return Fn(olde[n], val) && !Fn(newe[n], val); }
			bool gradient(value_t& de, const value_t& e);
		};

		template<typename system_, typename... boundaries>
		void make_boundary_system(system_ Sys, boundaries... Boundaries);

		template<typename system, typename boundary, typename... others>
		struct boundary_system{
			using state_type = state_type_;

			bool is_exceed(const state_type& State, time_type Time, const state_type& NewState, time_type NewTime);
			void exceed(state_type& State, time_type& Time, state_type state& Lower, time_type LowerTime, const state_type& Upper, time_type UpperTime);
		};

		template<typename state_type_, typename condition, typename true_system, typename false_system = std::nullptr_t>
		struct ifelse_system {
			using state_type = state_type_;
			using value_type = double;
		private:
			condition Condition;
			true_system True;
			false_system False;
		public:
			void update(const state_type& x, value_type t) {
				Condition.update(x, t);
				
				if(IsActive) {
					//TrueがただのSystemならここは不要な処理
					//SFINAEで連鎖させるか決める必要
					True.update(x, t);
				} else {
					False.update(x, t);
				}
			}
			void update(state_type& x, value_type& t, const state_type& vx, value_type vt, const state_type& ux, value_type ut) const {
				if(IsActive != Condition(x, t)) {
					Condition.
				}

				if(IsActive) {
					return True.update(x, t);
				} else {
					return False.update(x, t);
				}
			}
			void operator()(const state_type& x, state_type& dx, value_type t)
			bool valid(const state_type& x, double t) const{
				if(IsActive != Condition(x, t))return true;
				
				if(IsActive) {
					return True.valid(x, t);
				} else {
					return False.valid(x, t);
				}
			}
		};
		template<typename... other_systems>
		struct system_composite{};


		template

		using composite_system = hmLib::odeint::system_composite<
			hmLib::odeint::system_if<condition, system1, system2>,
			hmLib::odeint::system_if<condition_x, system_xborder>,
			hmLib::odeint::system_if<condition_y, system_yborder>
		>;
	}
}
#
#endif
