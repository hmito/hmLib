#ifndef HMLIB_ODEINT_COMPOSITE_FLOATSTATECOMPOSITE_INC
#define HMLIB_ODEINT_COMPOSITE_FLOATSTATECOMPOSITE_INC 100
#
#include<utility>
#include<algorithm>
namespace hmLib {
	namespace odeint {
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
			struct lower_boundary {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			public:
				lower_boundary(const state_type& val_) :val(val_) {}
				lower_boundary(state_type&& val_) :val(std::move(val_)) {}
			public:
				void update(const state_type& x, time_type t) { prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { return prev || !check(x, t); }
				void validate(state_type& x, time_type t) {
					if(check(x, t)) x = val;
				}
				bool condition()const { return prev; }
				void require(const state_type& x, state_type& dx, time_type t) const { if(prev) dx = std::max<state_type>(0, dx); }
			private:
				bool check(const state_type& x, time_type t) const { return val >= x; }
			};
			template<typename state_type_, typename time_type_ = double >
			struct upper_boundary {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type val;
				bool prev;
			public:
				upper_boundary(const state_type& val_) :val(val_) {}
				upper_boundary(state_type&& val_) :val(std::move(val_)) {}
			public:
				void update(const state_type& x, time_type t) { prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { return prev || !check(x, t); }
				void validate(state_type& x, time_type t) {
					if(check(x, t)) x = val;
				}
				bool condition()const { return prev; }
				void require(const state_type& x, state_type& dx, time_type t) const { if(prev) dx = std::min<state_type>(0, dx); }
			private:
				bool check(const state_type& x, time_type t) const { return val <= x; }
			};
		}
		template<typename state_type, typename time_type = double>
		auto case_greater_equal(state_type&& x) {
			return composite::greater_equal_condition<state_type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto case_lower_boundary(state_type&& x) {
			return composite::lower_boundary<state_type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto require_lower_boundary(state_type&& x) {
			return composite::lower_boundary<state_type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto case_upper_boundary(state_type&& x) {
			return composite::upper_boundary<state_type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto require_upper_boundary(state_type&& x) {
			return composite::upper_boundary<state_type, time_type>(x);
		}
	}
}
#
#endif
