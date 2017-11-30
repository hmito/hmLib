#ifndef HMLIB_ODEINT_COMPOSITE_COMPOSITELOGICAL_INC
#define HMLIB_ODEINT_COMPOSITE_COMPOSITELOGICAL_INC 100
#
namespace hmLib {
	namespace odeint {
		namespace composite {
			template<typename state_type_, typename time_type_, typename condition1, typename condition2>
			struct logical_and_condition {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				condition1 Cond1;
				condition2 Cond2;
			public:
				logical_and_condition(const condition1& Cond1_, const condition2& Cond2_):Cond1(Cond1_),Cond2(Cond2_){}
				logical_and_condition(condition1&& Cond1_, condition2&& Cond2_) :Cond1(std::move(Cond1_)), Cond2(std::move(Cond2_)){}
			public:
				void update(const state_type& x, time_type t) { Cond1.update(x, t); Cond2.update(x, t); }
				bool valid(const state_type& x, time_type t) const { return Cond1.valid(x, t) && Cond2.valid(x, t); }
				void validate(state_type& x, time_type t) { Cond1.validate(x, t); Cond2.validate(x, t); }
				bool condition()const { return Cond1.condition() && Cond2.condition(); }
			};
			template<typename state_type_, typename time_type_, typename condition1, typename condition2>
			struct logical_or_condition {
				using state_type = state_type_;
				using time_type = time_type_;
			private:
				condition1 Cond1;
				condition2 Cond2;
			public:
				logical_or_condition(const condition1& Cond1_, const condition2& Cond2_) :Cond1(Cond1_), Cond2(Cond2_) {}
				logical_or_condition(condition1&& Cond1_, condition2&& Cond2_) :Cond1(std::move(Cond1_)), Cond2(std::move(Cond2_)) {}
			public:
				void update(const state_type& x, time_type t) { Cond1.update(x, t); Cond2.update(x, t); }
				bool valid(const state_type& x, time_type t) const { return Cond1.valid(x, t) && Cond2.valid(x, t); }
				void validate(state_type& x, time_type t) { Cond1.validate(x, t); Cond2.validate(x, t); }
				bool condition()const { return Cond1.condition() || Cond2.condition(); }
			};
		}
		template<typename condition1, typename condition2>
		auto case_logical_and(condition1&& Cond1, condition2&& Cond2) {
			return composite::logical_and_condition<typename condition1::state_type, typename condition1::time_type, typename std::decay<condition1>::type, typename std::decay<condition2>::type>(std::forward<condition1>(Cond1), std::forward<condition2>(Cond2));
		}
		template<typename condition1, typename condition2>
		auto case_logical_or(condition1&& Cond1, condition2&& Cond2) {
			return composite::logical_or_condition<typename condition1::state_type, typename condition1::time_type, typename std::decay<condition1>::type, typename std::decay<condition2>::type>(std::forward<condition1>(Cond1), std::forward<condition2>(Cond2));
		}
	}
}
#
#endif
