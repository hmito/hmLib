#ifndef HMLIB_ODEINT_VALIDATE_STEPVALIDATESTEPPER_INC
#define HMLIB_ODEINT_VALIDATE_STEPVALIDATESTEPPER_INC 100
#
#include<cmath>
#include<boost/numeric/odeint/integrate/max_step_checker.hpp>
#include<boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include<boost/numeric/odeint/util/resizer.hpp>
#include<boost/numeric/odeint/util/unwrap_reference.hpp>
#include<boost/numeric/odeint/util/copy.hpp>
#include"../utility.hpp"
#include"../stepper_categories.hpp"
#include"validate_result.hpp"
namespace hmLib {
	namespace odeint {
		template<typename base_stepper_>
		struct step_validate_stepper{
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = adaptive_stepper_tag;
		public:// stepper functions
			step_validate_stepper(base_stepper st_, double abs_error_, unsigned int max_binary_trial_) :st(std::move(st_)), abs_error(abs_error_), max_binary_trial(max_binary_trial_){}
			order_type order()const { return st.order(); }
			template<typename sys_type>
			bool do_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				detail::resize_and_copy(x, x0);
				time_type t0 = t;

				st.do_step(sys, x, t, dt);
				t += dt;
				if (sys.is_invalid_step(x, t)) {
					//initial state is invalid
					if (sys.is_invalid_step(x0, t0)) {
						validate_result res = sys.validate(x0, x0, t0, x);
						t = t0;
						switch (res) {
						case validate_result::assigned:
							try_initialize(st, sys, x, t, dt);
							break;
						case validate_result::resized:
							try_reset(st);
							break;
						default:
							detail::move(std::move(x0), x);
							try_initialize(st, sys, x, t, dt);
							break;
						}
						return true;
					}

					//binary search
					time_type nt1 = t0;
					time_type nt2 = t;
					detail::resize_and_copy(x, nx1);
					detail::resize_and_copy(x, nx2);
					time_type ndt = dt;

					for (unsigned int i = 0; i < max_binary_trial; ++i) {
						detail::copy(x0, x);
						ndt = (nt2 + nt1 - 2*t0) / 2;
						try_initialize(st, sys, x, t0, ndt);
						st.do_step(sys, x, t0, ndt);

						if (sys.is_invalid_step(x, t0 + ndt)) {
							detail::copy(x, nx2);
							nt2 = t0 + ndt;
						}
						else {
							detail::copy(x, nx1);
							nt1 = t0 + ndt;
						}

						//error value check
						if (detail::maximum_absolute_error<state_type, algebra_type, operations_type>(nx1, nx2) < abs_error || nt1 == nt2)break;
					}

					//update arguments by new validated state
					t = nt1;
					validate_result res = sys.validate(nx1, nx2, t, x);
					switch (res) {
					case validate_result::assigned:
						try_initialize(st, sys, x, t, dt);
						break;
					case validate_result::resized:
						try_reset(st);
						break;
					default:
						detail::move(std::move(nx2), x);
						try_initialize(st, sys, x, t, dt);
						break;
					}

					return true;
				}

				validate_result res = sys.validate(x, t, nx1);
				switch (res) {
				case validate_result::assigned:
					detail::move(std::move(nx1), x);
					try_initialize(st, sys, x, t, dt);
					break;
				case validate_result::resized:
					detail::move(std::move(nx1), x);
					try_reset(st);
					break;
				}

				return false;
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			double abs_error;
			unsigned int max_binary_trial;
			state_type x0;
			state_type nx1;
			state_type nx2;
		};
		template<typename base_controlled_stepper_>
		struct step_validate_controlled_stepper {
			using base_stepper = base_controlled_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = adaptive_stepper_tag;
		public:// stepper functions
			step_validate_controlled_stepper(base_stepper st_, double abs_error_, unsigned int max_binary_trial_) :st(std::move(st_)), abs_error(abs_error_), max_binary_trial(max_binary_trial_) {}
			template<typename sys_type>
			bool do_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				detail::resize_and_copy(x, x0);
				time_type t0 = t;

				boost::numeric::odeint::failed_step_checker fail_checker;
				boost::numeric::odeint::controlled_step_result tryres;
				do {
					tryres = st.try_step(sys, x, t, dt);
					fail_checker();  // check number of failed steps
				} while (tryres == boost::numeric::odeint::controlled_step_result::fail);
				fail_checker.reset();

				if (sys.is_invalid_step(x, t)) {
					//initial state is invalid
					if (sys.is_invalid_step(x0, t0)) {
						validate_result res = sys.validate(x0, x0, t0, x);
						t = t0;
						switch (res) {
						case validate_result::assigned:
							try_initialize(st, sys, x, t, dt);
							break;
						case validate_result::resized:
							try_reset(st);
							break;
						default:
							detail::move(std::move(x0), x);
							try_initialize(st, sys, x, t, dt);
							break;
						}
						return true;
					}

					//binary search
					time_type nt1 = t0;
					time_type nt2 = t;
					detail::resize_and_copy(x, nx1);
					detail::resize_and_copy(x, nx2);
					time_type ndt = dt;

					for (unsigned int i = 0; i < max_binary_trial; ++i) {
						detail::copy(x0, x);
						t = t0;
						ndt = (nt2 + nt1 - 2 * t) / 2;
						try_initialize(st, sys, x, t, ndt);
						do {
							tryres = st.try_step(sys, x, t, ndt);
							fail_checker();  // check number of failed steps
						} while (tryres == boost::numeric::odeint::controlled_step_result::fail);
						fail_checker.reset();

						if (sys.is_invalid_step(x, t)) {
							detail::copy(x, nx2);
							nt2 = t;
						}
						else {
							detail::copy(x, nx1);
							nt1 = t;
						}

						//error value check
						if (detail::maximum_absolute_error<state_type, algebra_type, operations_type>(nx1, nx2) < abs_error || nt1 == nt2)break;
					}

					//update arguments by new validated state
					t = nt1;
					validate_result res = sys.validate(nx1, nx2, t, x);
					switch (res) {
					case validate_result::assigned:
						try_initialize(st, sys, x, t, dt);
						break;
					case validate_result::resized:
						try_reset(st);
						break;
					default:
						detail::move(std::move(nx2), x);
						try_initialize(st, sys, x, t, dt);
						break;
					}

					return true;
				}

				validate_result res = sys.validate(x, t, nx1);
				switch (res) {
				case validate_result::assigned:
					detail::move(std::move(nx1), x);
					try_initialize(st, sys, x, t, dt);
					break;
				case validate_result::resized:
					detail::move(std::move(nx1), x);
					try_reset(st);
					break;
				}
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			double abs_error;
			unsigned int max_binary_trial;
			state_type x0;
			state_type nx1;
			state_type nx2;
		};
		template<typename base_dense_output_stepper_>
		struct step_validate_dense_output_stepper {
			using base_stepper = base_dense_output_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = semidense_output_stepper_tag;
		public:
			step_validate_dense_output_stepper(base_stepper st_, double abs_error_, unsigned int max_binary_trial_) :Result(validate_result::none), st(std::move(st_)), abs_error(abs_error_), max_binary_trial(max_binary_trial_) {}
			void initialize(const state_type& x, time_type t, time_type dt) {
				st.initialize(x, t, dt);
				Result = validate_result::none;
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				switch (Result) {
				case validate_result::assigned:
					st.initialize(nx, nt, st.current_time_step());
					break;
				case validate_result::resized:
					try_reset(st);
					st.initialize(nx, nt, st.current_time_step());
					break;
				}
				auto step_range = st.do_step(sys);

				if (sys.is_invalid_step(st.current_state(), st.current_time())) {
					time_type nt1 = step_range.first;
					detail::resize(st.current_state(), nx1);
					st.calc_state(nt1, nx1);

					//initial state is invalid
					if (sys.is_invalid_step(nx1, nt1)) {
						Result = sys.validate(nx1, nx1, nt1, nx);
						if (Result == validate_result::none) {
							detail::move(std::move(nx1), nx);
							Result = validate_result::assigned;
						}
						nt = nt1;
						return std::make_pair(nt,nt);
					}

					//binary search
					time_type nt2 = step_range.second;
					detail::resize_and_copy(st.current_state(), nx2);

					detail::resize(st.current_state(), nx);

					for (unsigned int i = 0; i < max_binary_trial; ++i) {
						nt = (nt1 + nt2) / 2;
						st.calc_state(nt, nx);

						if (sys.is_invalid_step(nx, nt)) {
							detail::swap(nx2,nx);
							nt2 = nt;
						}
						else {
							detail::swap(nx1, nx);
							nt1 = nt;
						}

						//error value check
						if (detail::maximum_absolute_error<state_type, algebra_type, operations_type>(nx1, nx2) < abs_error || nt1 == nt2)break;
					}

					//update arguments by new validated state
					nt = nt1;
					Result = sys.validate(nx1, nx2, nt, nx);
					if (Result == validate_result::none) {
						detail::move(std::move(nx2), nx);
						Result = validate_result::assigned;
					}
					step_range.second = nt;

					return step_range;
				}

				Result = sys.validate(current_state(), nt, nx);
				return step_range;
			}
			void calc_state(time_type t, state_type& x)const {
				st.calc_state(t, x);
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type& x) {
				st.calc_state(t, x);
				auto TmpResult = sys.validate(x, t, nx);
				if (TmpResult != validate_result::none) {
					detail::move(std::move(nx),x);
				}
			}
			const state_type& current_state()const {
				//validated
				if (Result != validate_result::none)return nx;
				return st.current_state();
			}
			time_type current_time()const{
				//validated
				if (Result != validate_result::none)return nt;
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
			double abs_error;
			unsigned int max_binary_trial;
			time_type nt;
			state_type nx;
			state_type nx1;
			state_type nx2;
		};

		namespace detail {
			template<typename stepper_>
			auto make_step_validate_impl(double abs_error_, unsigned int max_binary_trial_, const stepper_& Stepper_, stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_stepper<stepper>(Stepper_, abs_error_, max_binary_trial_);
			}
			template<typename stepper_>
			auto make_step_validate_impl(double abs_error_, unsigned int max_binary_trial_, const stepper_& Stepper_, controlled_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_controlled_stepper<stepper>(Stepper_, abs_error_, max_binary_trial_);
			}
			template<typename stepper_>
			auto make_step_validate_impl(double abs_error_, unsigned int max_binary_trial_, const stepper_& Stepper_, dense_output_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_dense_output_stepper<stepper>(Stepper_, abs_error_, max_binary_trial_);
			}
			template<typename stepper_>
			auto make_step_validate_impl(stepper_&& Stepper_, ...) {
				static_assert(false, "given stepper type is not supported in make_step_validate.");
			}
		}
		template<typename stepper_>
		auto make_step_validate(double abs_error_, unsigned int max_binary_trial_, const stepper_& Stepper_ = stepper_()) {
			using stepper = typename boost::numeric::odeint::unwrap_reference<stepper_>::type;
			using stepper_category = typename stepper::stepper_category;
			return detail::make_step_validate_impl(abs_error_, max_binary_trial_, Stepper_, stepper_category());
		}
	}
}
#
#endif
