#ifndef HMLIB_ODEINT_VALIDATE_STATEVALIDATESTEPPER_INC
#define HMLIB_ODEINT_VALIDATE_STATEVALIDATESTEPPER_INC 100
#
#include<utility>
#include<boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include<boost/numeric/odeint/util/unwrap_reference.hpp>
#include<boost/numeric/odeint/util/copy.hpp>
#include"../utility.hpp"
#include"../stepper_categories.hpp"
#include"validate_result.hpp"
namespace hmLib {
	namespace odeint{ 
		template<typename base_stepper_,typename appendix_type_>
		struct state_validate_stepper {
			using base_stepper = base_stepper_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair< base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = stepper_tag;
		public:// stepper functions
			state_validate_stepper(base_stepper Stepper_) :st(std::move(Stepper_)) {}
			order_type order()const { return st.order(); }
			template<typename sys_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt) {
				sys.initialize(x.second);
				st.do_step(sys, x.first, t, dt);

				t += dt;
				validate_result res = sys.validate(x.first, t, nx, x.second);
				switch (res) {
				case validate_result::assigned:
					utility::move(std::move(nx), x.first);
					try_initialize(st, sys, x.first, t, dt);
					break;
				case validate_result::resized:
					utility::move(std::move(nx), x.first);
					try_reset(st);
					break;
				default:
					break;
				}
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			base_state_type nx;
		};
		template<typename base_controlled_stepper_, typename appendix_type_>
		struct state_validate_controlled_stepper {
			using base_stepper = base_controlled_stepper_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair< base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = controlled_stepper_tag;
		public:
			state_validate_controlled_stepper(base_stepper Stepper_) :st(std::move(Stepper_)) {}
			template<typename sys_type>
			boost::numeric::odeint::controlled_step_result try_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				sys.initialize(x.second);
				auto result = st.try_step(sys, x.first, t, dt);
				//if not success, don't try interfere because this result will be rejected.
				if (result != boost::numeric::odeint::controlled_step_result::success)return result;

				validate_result res = sys.validate(x.first, t, nx, x.second);
				switch (res) {
				case validate_result::assigned:
					utility::move(std::move(nx), x.first);
					try_initialize(st, sys, x.first, t, dt);
					break;
				case validate_result::resized:
					utility::move(std::move(nx), x.first);
					try_reset(st);
					break;
				default:
					break;
				}
				return result;
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			base_state_type nx;
		};
		template<typename base_dense_output_stepper_, typename appendix_type_>
		struct state_validate_dense_output_stepper {
			using base_stepper = base_dense_output_stepper_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair< base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = dense_output_stepper_tag;
		public:
			state_validate_dense_output_stepper(base_stepper Stepper_) : Result(validate_result::none), st(std::move(Stepper_)) {}
			void reset() {
				try_reset(st);
				Result = validate_result::none;
			}
			void initialize(const state_type& x, time_type t, time_type dt) {
				st.initialize(x.first, t, dt);
				ap = x.second;
				prev_ap = ap;
				Result = validate_result::none;
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				switch (Result) {
				case validate_result::assigned:
					st.initialize(nx, st.current_time(), st.current_time_step());
					break;
				case validate_result::resized:
					try_reset(st);
					st.initialize(nx, st.current_time(), st.current_time_step());
					break;
				default:
					break;
				}
				Result = validate_result::none;

				//system initialize with appendix
				sys.initialize(ap);

				//do step
				auto step_range = st.do_step(sys);

				//state validation
				prev_ap = ap;
				Result = sys.validate(st.current_state(), st.current_time(), nx, ap);
				return step_range;
			}
			void calc_state(time_type t, state_type& x)const {
				st.calc_state(t, x.first);
				x.second = prev_ap;
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type& x)const {
				st.calc_state(t, x.first);
				x.second = prev_ap;
				sys.initialize(x.second);
				auto Result = sys.validate(x.first, t, nx, x.second);
				if (Result != validate_result::none) {
					utility::move(std::move(nx), x.first);
				}
			}
			std::pair<const base_state_type&, appendix_type> current_state()const {
				//validated
				if (Result != validate_result::none)return std::pair<const base_state_type&, appendix_type>(nx, ap);
				return std::pair<const base_state_type&, appendix_type>(st.current_state(), ap);
			}
			time_type current_time()const {
				return st.current_time();
			}
			time_type current_time_step()const {
				return st.current_time_step();
			}
		public:
			base_stepper& base() { return st; }
		private:
			validate_result Result;
			base_stepper st;
			appendix_type ap;
			appendix_type prev_ap;
			base_state_type nx;
		};
		template<typename base_stepper_>
		struct state_validate_stepper< base_stepper_, void>{
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = stepper_tag;
		public:// stepper functions
			state_validate_stepper(base_stepper Stepper_) :st(std::move(Stepper_)) {}
			order_type order()const { return st.order(); }
			template<typename sys_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt) {
				st.do_step(sys, x, t, dt);

				t += dt;
				validate_result res = sys.validate(x, t, nx);
				switch (res) {
				case validate_result::assigned:
					utility::move(std::move(nx), x);
					try_initialize(st, sys, x, t, dt);
					break;
				case validate_result::resized:
					utility::move(std::move(nx), x);
					try_reset(st);
					break;
				default:
					break;
				}
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			state_type nx;
		};
		template<typename base_controlled_stepper_>
		struct state_validate_controlled_stepper< base_controlled_stepper_, void> {
			using base_stepper = base_controlled_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = controlled_stepper_tag;
		public:
			state_validate_controlled_stepper(base_stepper Stepper_) :st(std::move(Stepper_)) {}
			template<typename sys_type>
			boost::numeric::odeint::controlled_step_result try_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				auto result = st.try_step(sys, x, t, dt);
				//if not success, don't try interfere because this result will be rejected.
				if (result != boost::numeric::odeint::controlled_step_result::success)return result;

				validate_result res = sys.validate(x, t, nx);
				switch (res) {
				case validate_result::assigned:
					utility::move(std::move(nx), x);
					try_initialize(st, sys, x, t, dt);
					break;
				case validate_result::resized:
					utility::move(std::move(nx), x);
					try_reset(st);
					break;
				default:
					break;
				}
				return result;
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			state_type nx;
		};
		template<typename base_dense_output_stepper_>
		struct state_validate_dense_output_stepper< base_dense_output_stepper_, void> {
			using base_stepper = base_dense_output_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = dense_output_stepper_tag;
		public:
			state_validate_dense_output_stepper(base_stepper Stepper_) : Result(validate_result::none), st(std::move(Stepper_)) {}
			void reset() {
				try_reset(st);
				Result = validate_result::none;
			}
			void initialize(const state_type& x, time_type t, time_type dt) {
				st.initialize(x, t, dt);
				Result = validate_result::none;
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				switch (Result) {
				case validate_result::assigned:
					st.initialize(nx, st.current_time(), st.current_time_step());
					break;
				case validate_result::resized:
					try_reset(st);
					st.initialize(nx, st.current_time(), st.current_time_step());
					break;
				default:
					break;
				}
				auto step_range = st.do_step(sys);

				Result = sys.validate(st.current_state(), st.current_time(), nx);
				return step_range;
			}
			void calc_state(time_type t, state_type& x)const {
				st.calc_state(t, x);
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type& x)const {
				st.calc_state(t, x);
				auto Result = sys.validate(x, t, nx);
				if (Result != validate_result::none) {
					utility::move(nx, x);
				}
			}
			const state_type& current_state()const {
				//validated
				if (Result!=validate_result::none)return nx;
				return st.current_state();
			}
			time_type current_time()const {
				return st.current_time();
			}
			time_type current_time_step()const {
				return st.current_time_step();
			}
		public:
			base_stepper& base() { return st; }
		private:
			validate_result Result;
			base_stepper st;
			state_type nx;
		};

		namespace detail {
			template<typename appendix_, typename stepper_>
			auto make_state_validate_impl(const stepper_& Stepper_, stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return state_validate_stepper<stepper, appendix_>(Stepper_);
			}
			template<typename appendix_, typename stepper_>
			auto make_state_validate_impl(const stepper_& Stepper_, controlled_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return state_validate_controlled_stepper<stepper, appendix_>(Stepper_);
			}
			template<typename appendix_, typename stepper_>
			auto make_state_validate_impl(const stepper_& Stepper_, dense_output_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return state_validate_dense_output_stepper<stepper, appendix_>(Stepper_);
			}
			template<typename appendix_, typename stepper_>
			auto make_state_validate_impl(const stepper_& Stepper_, ...) {
				//static_assert(false, "given stepper type is not supported in make_state_validate.");
			}
		}
		template<typename appendix_, typename stepper_>
		auto make_state_validate(const stepper_& Stepper_= stepper_()) {
			using stepper = typename boost::numeric::odeint::unwrap_reference<stepper_>::type;
			using stepper_category = typename stepper::stepper_category;
			return detail::make_state_validate_impl<appendix_>(Stepper_, stepper_category());
		}
	}
}
#
#endif
