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
			template<typename state_type_, typename time_type_ = double >
			struct range_boundary {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				state_type lower_val;
				state_type upper_val;
				char prev; //0: mid, 1: upper, -1: lower
			public:
				range_boundary(const state_type& lower_val_, const state_type& upper_val_) :lower_val(lower_val_), upper_val(upper_val_) {}
				range_boundary(state_type&& lower_val_, const state_type& upper_val_) :lower_val(std::move(lower_val_)), upper_val(upper_val_) {}
				range_boundary(const state_type& lower_val_, state_type&& upper_val_) :lower_val(lower_val_), upper_val(std::move(upper_val_)) {}
				range_boundary(state_type&& lower_val_, state_type&& upper_val_) :lower_val(std::move(lower_val_)), upper_val(std::move(upper_val_)) {}
			public:
				void update(const state_type& x, time_type t) { prev = check(x, t); }
				bool valid(const state_type& x, time_type t) const { char now = check(x, t); return now == prev || now==0;}
				void validate(state_type& x, time_type t) {
					if(upper_val <= x) x = upper_val;
					else if(x <= lower_val) x = lower_val;
				}
				void require(const state_type& x, state_type& dx, time_type t) const {
					if(prev==1)dx = std::min(0.0, dx);
					else if(prev==-1)dx = std::max(0.0, dx);
				}
			private:
				char check(const state_type& x, time_type t) const { return upper_val <= x ? 1:  x <= lower_val? -1: 0;}
			};
		}
		template<typename state_type, typename time_type = double>
		auto case_greater_equal(state_type&& x) {
			return composite::greater_equal_condition<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}
		template<typename state_type, typename time_type = double>
		auto case_greater(state_type&& x) {
			return composite::greater_condition<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}
		template<typename state_type, typename time_type = double>
		auto case_less_equal(state_type&& x) {
			return composite::less_equal_condition<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}
		template<typename state_type, typename time_type = double>
		auto case_less(state_type&& x) {
			return composite::less_condition<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}

		template<typename state_type, typename time_type = double>
		auto case_lower_boundary(state_type&& x) {
			return composite::lower_boundary<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}
		template<typename state_type, typename time_type = double>
		auto require_lower_boundary(state_type&& x) {
			return composite::lower_boundary<typename std::decay<state_type>::type, time_type>(x);
		}
		template<typename state_type, typename time_type = double>
		auto case_upper_boundary(state_type&& x) {
			return composite::upper_boundary<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}
		template<typename state_type, typename time_type = double>
		auto require_upper_boundary(state_type&& x) {
			return composite::upper_boundary<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(x));
		}

		template<typename state_type, typename time_type = double>
		auto require_range_boundary(state_type&& lower, state_type&& upper) {
			return composite::range_boundary<typename std::decay<state_type>::type, time_type>(std::forward<state_type>(lower), std::forward<state_type>(upper));
		}
	}
}
#
#endif
