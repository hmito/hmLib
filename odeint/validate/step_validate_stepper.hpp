#ifndef HMLIB_ODEINT_VALIDATE_STEPVALIDATESTEPPER_INC
#define HMLIB_ODEINT_VALIDATE_STEPVALIDATESTEPPER_INC 100
#
#include<cmath>
#include<utility>
#include<boost/optional.hpp>
#include<boost/numeric/odeint/integrate/max_step_checker.hpp>
#include<boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include<boost/numeric/odeint/util/resizer.hpp>
#include<boost/numeric/odeint/util/unwrap_reference.hpp>
#include<boost/numeric/odeint/util/copy.hpp>
#include"../utility.hpp"
#include"../stepper_categories.hpp"
#include"validate_result.hpp"
#include"default_step_validator.hpp"
namespace hmLib {
	namespace odeint {
		template<typename base_stepper_, typename step_validator_, typename appendix_type_>
		struct step_validate_stepper{
			using base_stepper = base_stepper_;
			using step_validator = step_validator_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair<base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = adaptive_stepper_tag;
		public:// stepper functions
			step_validate_stepper(base_stepper st_, step_validator stvld_)
				: st(std::move(st_))
				, stvld(stvld_) {
			}
			template<typename sys_type>
			bool do_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				detail::resize_and_copy(x.first, nx1);

				//system initialize with appendix
				sys.initialize(x.second);

				//initial state should be valid
				if (sys.is_invalid_step(x.first, t)) {
					auto Result = sys.validate(x.first, x.first, t, nx, x.second);
					switch (Result) {
					case validate_result::assigned:
						detail::move(std::move(nx), x.first);
						try_initialize(st, sys, x.first, t, dt);
						break;
					case validate_result::resized:
						detail::move(std::move(nx), x.first);
						try_reset(st);
						break;
					}
					//after validation nx & x.second should be valid
					sys.initialize(x.second);
				}

				//backup inital state
				time_type t0 = t;
				detail::resize_and_copy(x.first, nx1);

				//do step
				st.do_step(sys, x.first, t, dt);
				t += dt;	//t should be updated manually

				//check on the way to the current time point
				bool NeedRestep = false;
				time_type nt1 = t0;
				time_type nt2 = t;

				//setup step validator
				stvld.setup_dt(nx1, t0, x.first, t);
				if (boost::numeric::odeint::detail::less_with_sign(nt1 + stvld.dt(), t, stvld.dt())) {
					//setup state
					detail::resize_and_copy(nx1, nx);
					do {
						//detail::copy(nx1, nx);
						time_type nt = nt1;
						time_type ndt = stvld.dt();
						try_initialize(st, sys, nx, nt, ndt);
						st.do_step(sys, nx, nt, ndt);
						nt = nt + ndt;

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::resize_and_swap(nx, nx2);
							NeedRestep = true;
							break;
						}else {
							nt1 = nt;
							detail::copy(nx, nx1);
						}
					} while (boost::numeric::odeint::detail::less_with_sign(nt1 + stvld.dt(), t, stvld.dt()));
				}
				if (!NeedRestep) {
					if (sys.is_invalid_step(x.first, t)) {
						NeedRestep = true;
						nt2 = t;
						detail::resize_and_swap(x.first, nx2);
					}
				}

				//if restep is required
				if (NeedRestep) {
					detail::copy(nx1, nx);
					try_initialize(st, sys, nx, nt1, (nt1 + nt2) / 2 - nt1);
					//start bisection search
					stvld.reset();
					while (!stvld(nx1, nt1, nx2, nt2)) {
						time_type nt = nt1;
						time_type ndt = (nt1 + nt2) / 2 - nt;
						//detail::copy(nx1, nx);
						st.do_step(sys, nx, nt, ndt);
						nt = nt + ndt;

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::move(std::move(nx), nx2);

							//nx&nt back to nx1
							detail::resize_and_copy(nx1, nx);
							//initialize with nx1
							try_initialize(st, sys, nx, nt1, ndt);
						}
						else {
							nt1 = nt;
							detail::copy(nx, nx1);
							//initialize is not required because stepper is now nx1&nt1
						}
					}

					//update arguments by new validated state
					t = nt1;
					auto Result = sys.validate(nx1, nx2, t, x.first, x.second);
					switch (Result) {
					case validate_result::assigned:
						try_initialize(st, sys, x.first, t, dt);
						break;
					case validate_result::resized:
						try_reset(st);
						break;
					default:
						detail::move(std::move(nx2), x.first);
						try_initialize(st, sys, x.first, t, dt);
						break;
					}

					return true;
				}

				validate_result Result = sys.validate(x.first, t, nx1,x.second);
				switch (Result) {
				case validate_result::assigned:
					detail::move(std::move(nx1), x.first);
					try_initialize(st, sys, x.first, t, dt);
					break;
				case validate_result::resized:
					detail::move(std::move(nx1), x.first);
					try_reset(st);
					break;
				}

				return false;
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			step_validator stvld;
			base_state_type nx;
			base_state_type nx1;
			base_state_type nx2;
		};
		template<typename base_controlled_stepper_, typename step_validator_, typename appendix_type_>
		struct step_validate_controlled_stepper {
			using base_stepper = base_controlled_stepper_;
			using step_validator = step_validator_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair<base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = adaptive_stepper_tag;
		public:// stepper functions
			step_validate_controlled_stepper(base_stepper st_, step_validator stvld_)
				: st(std::move(st_))
				, stvld(stvld_) {
			}
			template<typename sys_type>
			bool do_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				detail::resize_and_copy(x.first, nx1);

				//system initialize with appendix
				sys.initialize(x.second);

				//initial state should be valid
				if (sys.is_invalid_step(x.first, t)) {
					auto Result = sys.validate(x.first, x.first, t, nx, x.second);
					switch (Result) {
					case validate_result::assigned:
						detail::move(std::move(nx), x.first);
						try_initialize(st, sys, x.first, t, dt);
						break;
					case validate_result::resized:
						detail::move(std::move(nx), x.first);
						try_reset(st);
						break;
					}
					//after validation nx & ap should be valid
					sys.initialize(x.second);
				}

				//backup inital state
				time_type t0 = t;
				detail::resize_and_copy(x.first, nx1);

				boost::numeric::odeint::failed_step_checker fail_checker;
				boost::numeric::odeint::controlled_step_result tryres;

				//do step
				do {
					tryres = st.try_step(sys, x.first, t, dt);
					fail_checker();  // check number of failed steps
				} while (tryres == boost::numeric::odeint::controlled_step_result::fail);
				fail_checker.reset();

				//check on the way to the current time point
				bool NeedRestep = false;
				time_type nt1 = t0;
				time_type nt2 = t;

				//setup step validator
				stvld.setup_dt(nx1, t0, x.first, t);

				//check step is valid
				if (boost::numeric::odeint::detail::less_with_sign(nt1 + stvld.dt(), t, stvld.dt())) {
					//setup state
					detail::resize_and_copy(nx1, nx);
					do {
						//detail::copy(nx1, nx);
						time_type nt = nt1;
						time_type ndt = stvld.dt();
						try_initialize(st, sys, nx, nt, ndt);

						//do step
						do {
							tryres = st.try_step(sys, nx, nt, ndt);
							fail_checker();  // check number of failed steps
						} while (tryres == boost::numeric::odeint::controlled_step_result::fail);
						fail_checker.reset();

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::resize_and_swap(nx, nx2);
							NeedRestep = true;
							break;
						}
						else {
							nt1 = nt;
							detail::copy(nx, nx1);
						}
					} while (boost::numeric::odeint::detail::less_with_sign(nt1 + stvld.dt(), t, stvld.dt()));
				}
				if (!NeedRestep) {
					if (sys.is_invalid_step(x.first, t)) {
						NeedRestep = true;
						nt2 = t;
						detail::resize_and_swap(x.first, nx2);
					}
				}

				//if restep is required
				if (NeedRestep) {
					detail::copy(nx1, nx);
					try_initialize(st, sys, nx, nt1, (nt1 + nt2) / 2 - nt1);
					//start bisection search
					stvld.reset();
					while (!stvld(nx1, nt1, nx2, nt2)) {
						time_type nt = nt1;
						time_type ndt = (nt1 + nt2) / 2 - nt;

						//do step
						do {
							tryres = st.try_step(sys, nx, nt, ndt);
							fail_checker();  // check number of failed steps
						} while (tryres == boost::numeric::odeint::controlled_step_result::fail);
						fail_checker.reset();

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::move(std::move(nx), nx2);

							//nx&nt back to nx1
							detail::resize_and_copy(nx1, nx);
							//initialize with nx1
							try_initialize(st, sys, nx, nt1, ndt);
						}
						else {
							nt1 = nt;
							detail::copy(nx, nx1);
							//initialize is not required because stepper is now nx1&nt1
						}
					}

					//update arguments by new validated state
					t = nt1;
					auto Result = sys.validate(nx1, nx2, t, x.first, x.second);
					switch (Result) {
					case validate_result::assigned:
						try_initialize(st, sys, x.first, t, dt);
						break;
					case validate_result::resized:
						try_reset(st);
						break;
					default:
						detail::move(std::move(nx2), x.first);
						try_initialize(st, sys, x.first, t, dt);
						break;
					}

					return true;
				}

				validate_result Result = sys.validate(x.first, t, nx1, x.second);
				switch (Result) {
				case validate_result::assigned:
					detail::move(std::move(nx1), x.first);
					try_initialize(st, sys, x.first, t, dt);
					break;
				case validate_result::resized:
					detail::move(std::move(nx1), x.first);
					try_reset(st);
					break;
				}

				return false;
			}
		public:
			base_stepper& base() { return st; }
		private:
			base_stepper st;
			step_validator stvld;
			base_state_type nx;
			base_state_type nx1;
			base_state_type nx2;
		};
		template<typename base_dense_output_stepper_, typename step_validator_, typename appendix_type_>
		struct step_validate_dense_output_stepper {
			using base_stepper = base_dense_output_stepper_;
			using step_validator = step_validator_;
			using appendix_type = appendix_type_;
			using base_state_type = typename base_stepper::state_type;
			using state_type = std::pair<base_state_type, appendix_type>;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = dense_output_stepper_tag;
		public:
			step_validate_dense_output_stepper(base_stepper st_, step_validator stvld_)
				: Result(validate_result::none)
				, st(std::move(st_))
				, stvld(stvld_){
			}
			void initialize(const state_type& x, time_type t, time_type dt) {
				st.initialize(x.first, t, dt);
				ap = x.second;
				prev_ap = ap;
				Result = validate_result::none;
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				//update reserved initialization
				switch (Result) {
				case validate_result::assigned:
					st.initialize(nx, nt, st.current_time_step());
					break;
				case validate_result::resized:
					try_reset(st);
					st.initialize(nx, nt, st.current_time_step());
					break;
				}
				Result = validate_result::none;

				//system initialize with appendix
				sys.initialize(ap);

				//validate initial state if it is invalid
				if (sys.is_invalid_step(st.current_state(), st.current_time())) {
					prev_ap = ap;
					auto res = sys.validate(st.current_state(), st.current_state(), st.current_time(), nx, ap);
					switch (res) {
					case validate_result::assigned:
						st.initialize(nx, st.current_time(), st.current_time_step());
						break;
					case validate_result::resized:
						try_reset(st);
						st.initialize(nx, st.current_time(), st.current_time_step());
						break;
					}
					//after validation nx & ap should be valid
					sys.initialize(ap);
				}

				//backup previous state
				detail::resize_and_copy(st.current_state(), nx1);

				//do step
				auto step_range = st.do_step(sys);

				//check on the way to the current time point
				bool NeedRestep = false;
				time_type nt1 = step_range.first;
				time_type nt2 = step_range.second;

				//setup step validator
				stvld.setup_dt(nx1, nt1, st.current_state(), nt2);
				if (boost::numeric::odeint::detail::less_with_sign(nt1 + stvld.dt(), nt2, stvld.dt())){
					//setup state
					detail::resize(st.current_state(), nx);
					do {
						nt = nt1 + stvld.dt();
						st.calc_state(nt, nx);

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::resize_and_swap(nx, nx2);
							NeedRestep = true;
							break;
						} else {
							nt1 = nt;
							detail::swap(nx, nx1);
						}
					} while (boost::numeric::odeint::detail::less_with_sign(nt, nt2, stvld.dt()));
				}
				if (!NeedRestep) {
					if (sys.is_invalid_step(st.current_state(), st.current_time())) {
						NeedRestep = true;
						nt2 = st.current_time();
						detail::resize_and_copy(st.current_state(), nx2);
					}
				}

				//if restep is required
				if (NeedRestep) {
					//start bisection search
					stvld.reset();
					while (!stvld(nx1, nt1, nx2, nt2)) {
						nt = (nt1 + nt2) / 2;
						st.calc_state(nt, nx);

						if (sys.is_invalid_step(nx, nt)) {
							nt2 = nt;
							detail::swap(nx2, nx);
						}
						else {
							nt1 = nt;
							detail::swap(nx1, nx);
						}
					}

					//update arguments by new validated state
					nt = nt1;
					prev_ap = ap;
					Result = sys.validate(nx1, nx2, nt, nx, ap);
					if (Result == validate_result::none) {
						detail::move(std::move(nx2), nx);
						Result = validate_result::assigned;
					}
					step_range.second = nt;

					return step_range;
				}

				//if step validation is not required
				//	just doing state validation
				nt = st.current_time();
				prev_ap = ap;
				Result = sys.validate(st.current_state(), nt, nx, ap);
				return step_range;
			}
			void calc_state(time_type t, state_type& x)const {
				st.calc_state(t, x.first);
				x.second = prev_ap;
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type& x) {
				st.calc_state(t, x.first);
				x.second = prev_ap;
				sys.initialize(x.second);
				auto TmpResult = sys.validate(x.first, t, nx, x.second);
				if (TmpResult != validate_result::none) {
					detail::move(std::move(nx),x.first);
				}
			}
			std::pair<const base_state_type&, appendix_type> current_state()const {
				//validated
				if (Result != validate_result::none)return std::pair<const base_state_type&, appendix_type> (nx,ap);
				return std::pair<const base_state_type&, appendix_type>(st.current_state(),ap);
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
			step_validator stvld;
			time_type nt;
			appendix_type ap;
			appendix_type prev_ap;
			base_state_type nx;
			base_state_type nx1;
			base_state_type nx2;
		};
		/*
		template<typename base_dense_output_stepper_>
		struct step_validate_dense_output_stepper< base_dense_output_stepper_, void>{
			using base_stepper = base_dense_output_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using algebra_type = typename base_stepper::algebra_type;
			using operations_type = typename base_stepper::operations_type;
			using stepper_category = semidense_output_stepper_tag;
		public:
			step_validate_dense_output_stepper(double abs_error_, unsigned int max_binary_trial_, base_stepper st_) :Result(validate_result::none), st(std::move(st_)), abs_error(abs_error_), max_binary_trial(max_binary_trial_) {}
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
						return std::make_pair(nt, nt);
					}

					//binary search
					time_type nt2 = step_range.second;
					detail::resize_and_copy(st.current_state(), nx2);

					detail::resize(st.current_state(), nx);

					for (unsigned int i = 0; i < max_binary_trial; ++i) {
						nt = (nt1 + nt2) / 2;
						st.calc_state(nt, nx);

						if (sys.is_invalid_step(nx, nt)) {
							detail::swap(nx2, nx);
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
			void calc_state(time_type t, state_type & x)const {
				st.calc_state(t, x);
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type & x) {
				st.calc_state(t, x);
				auto TmpResult = sys.validate(x, t, nx);
				if (TmpResult != validate_result::none) {
					detail::move(std::move(nx), x);
				}
			}
			const state_type& current_state()const {
				//validated
				if (Result != validate_result::none)return nx;
				return st.current_state();
			}
			time_type current_time()const {
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
		*/
		namespace detail {
			template<typename appendix_,typename stepper_, typename step_validator_>
			auto make_step_validate_impl(const stepper_& Stepper_, const step_validator_& StepValidator_, stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_stepper<stepper, step_validator_, appendix_>(Stepper_, StepValidator_);
			}
			template<typename appendix_,typename stepper_, typename step_validator_>
			auto make_step_validate_impl(const stepper_& Stepper_, const step_validator_& StepValidator_, controlled_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_controlled_stepper<stepper, step_validator_, appendix_>(Stepper_, StepValidator_);
			}
			template<typename appendix_, typename stepper_, typename step_validator_>
			auto make_step_validate_impl(const stepper_& Stepper_, const step_validator_& StepValidator_, dense_output_stepper_tag) {
				using stepper = typename std::decay<typename boost::numeric::odeint::unwrap_reference<stepper_>::type>::type;
				return step_validate_dense_output_stepper<stepper, step_validator_, appendix_>(Stepper_, StepValidator_);
			}
			template<typename appendix_, typename stepper_, typename step_validator_>
			auto make_step_validate_impl(const stepper_& Stepper_, const step_validator_& StepValidator_, ...) {
				static_assert(false, "given stepper type is not supported in make_step_validate.");
			}
		}
		template<typename appendix_, typename stepper_>
		auto make_step_validate(double seach_distance_, double error_distance_, double error_dt_, unsigned int max_try_num_, const stepper_& Stepper_ = stepper_()) {
			using stepper = typename boost::numeric::odeint::unwrap_reference<stepper_>::type;
			using stepper_category = typename stepper::stepper_category;
			using step_validator = default_step_validator<typename stepper::state_type, typename stepper::time_type, typename stepper::algebra_type, typename stepper::operations_type>;
			return detail::make_step_validate_impl<appendix_>(Stepper_, step_validator(seach_distance_, error_distance_, error_dt_, max_try_num_), stepper_category());
		}
	}
}
#
#endif
