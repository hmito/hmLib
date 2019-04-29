#ifndef HMLIB_ODEINT_VALIDATE_STATEVALIDATESTEPPER_INC
#define HMLIB_ODEINT_VALIDATE_STATEVALIDATESTEPPER_INC 100
#
#include<boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include"../utility.hpp"
#include"../stepper_categories.hpp"
#include"validate_result.hpp"
namespace hmLib {
	namespace odeint{ 
		template<typename base_stepper_, typename base_stepper_category_>
		struct state_validate_stepper {};
		template<typename base_stepper_>
		struct state_validate_stepper<base_stepper_, stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using stepper_category = stepper_tag;
		public:// stepper functions
			state_validate_stepper(base_stepper Stepper_) :IsInitialized(false), CanBreak(false), Stepper(std::move(Stepper_)) {}
			order_type order()const { return Stepper.order(); }
			template<typename sys_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt) {
				Stepper.do_step(sys, x, t, dt);
				validate_result res = sys.validate(x, t, nx);
				switch (res) {
				case validate_result::assigned:
					x = std::move(nx);
					try_initialize(Stepper, sys, x, t, dt);
					break;
				case validate_result::resized:
					x = std::move(nx);
					try_reset(Stepper);
					break;
				}
			}
		public:// interfere functions
			base_stepper& base() { return Stepper; }
		private:
			base_stepper Stepper;
			state_type nx;
		};
		template<typename base_stepper_>
		struct state_validate_stepper<base_stepper_, controlled_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using stepper_category = controlled_stepper_tag;
		public:
			state_validate_stepper(base_stepper Stepper_) :CanBreak(false), Stepper(std::move(Stepper_)) {}
			template<typename sys_type>
			boost::numeric::odeint::controlled_step_result try_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				auto result = Stepper.try_step(sys, x, t, dt);
				//if not success, don't try interfere because this result will be rejected.
				if (result != boost::numeric::odeint::controlled_step_result::success)return result;

				validate_result res = sys.validate(x, t, nx);
				switch (res) {
				case validate_result::assigned:
					x = std::move(nx);
					try_initialize(Stepper, sys, x, t, dt);
					break;
				case validate_result::resized:
					x = std::move(nx);
					try_reset(Stepper);
					break;
				}
				return result;
			}
		private:
			base_stepper Stepper;
			state_type nx;
		};
		template<typename base_stepper_>
		struct state_validate_stepper<base_stepper_, dense_output_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using stepper_category = semidense_output_stepper_tag;
		public:
			state_validate_stepper(base_stepper Stepper_) : CanBreak(false), ReserveInitialize(false), Stepper(std::move(Stepper_)) {}
			void initialize(const state_type& x, time_type t, time_type dt) {
				Stepper.initialize(x, t, dt);
				Result = validate_result::none;
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				switch (Result) {
				case validate_result::assigned:
					Stepper.initialize(nx, Stepper.current_time(), Stepper.current_time_step());
					break;
				case validate_result::resized:
					try_reset(Stepper);
					Stepper.initialize(nx, Stepper.current_time(), Stepper.current_time_step());
					break;
				}
				auto step_range = Stepper.do_step(sys);

				Result = sys.validate(current_state(), nt, nx);
				return step_range;
			}
			template<typename sys_type>
			void calc_state(sys_type sys, time_type t, state_type& x)const {
				Stepper.calc_state(t, x);
				auto Result = sys.validate(x, t, nx);
				if (Result != validate_result::none) {
					x = std::move(nx);
				}
			}
			const state_type& current_state()const {
				//validated
				if (Result!=validate_result::none)return nx;
				return Stepper.current_state();
			}
			const time_type& current_time()const {
				return Stepper.current_time();
			}
			const time_type& current_time_step()const {
				return Stepper.current_time_step();
			}
		private:
			validate_result Result;
			base_stepper Stepper;
			state_type nx;
		};
	}
}
#
#endif
