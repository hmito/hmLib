#ifndef HMLIB_ODEINT_INTERFERE_INTERFERESTEPPER_INC
#define HMLIB_ODEINT_INTERFERE_INTERFERESTEPPER_INC 100
#
#include<boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include<boost/numeric/odeint/util/unwrap_reference.hpp>
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
#include"interfere.hpp"
namespace hmLib {
	namespace odeint {
		template<typename base_stepper_, typename stepper_category_>
		struct interfere_stepper {};
		template<typename base_stepper_>
		struct interfere_stepper<base_stepper_, boost::numeric::odeint::stepper_tag>{
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using stepper_category = boost::numeric::odeint::stepper_tag;
		public:// stepper functions
			interfere_stepper(base_stepper Stepper_) :IsInitialized(false), CanBreak(false), Stepper(std::move(Stepper_)) {}
			order_type order()const { return Stepper.order(); }
			template<typename sys_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt) {
				if (!IsInitialized) {
					initialize(sys, x, t, dt);
				}

				Stepper.do_step(sys, x, t, dt);
				interfere(sys, x, t, dt);
			}
		public:// interfere functions
			base_stepper& base() { return Stepper; }
			bool can_break()const { return CanBreak; }
			bool is_initialized()const { return IsInitialized; }
			template<typename sys_type>
			void initialize(sys_type sys, state_type& x, time_type t, time_type dt) {
				IsInitialized = true;
				CanBreak = false;

				interfere(sys, x, t, dt);
				if(can_break())return;

				try_initialize(Stepper, sys, x, t, dt);
			}
			void reset() {
				IsInitialized = false;
				try_reset(Stepper);
			}
		private:
			template<typename sys_type>
			void interfere(sys_type sys, state_type& x, time_type t, time_type dt) {
				interfere_request req = sys.interfere(x, t, dt, nx);
				switch (req) {
				case interfere_request::breakable:
					CanBreak = true;
					return;
				case interfere_request::reset:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					return;
				case interfere_request::initialize:
					try_initialize(Stepper, sys, x,t,dt);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					return;
				default:
					CanBreak = false;
					return;
				}
			}
		private:
			bool IsInitialized;
			bool CanBreak;
			base_stepper Stepper;
			state_type nx;
		};
		template<typename base_stepper_>
		struct interfere_stepper<base_stepper_, boost::numeric::odeint::error_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using order_type = typename base_stepper::order_type;
			using stepper_category = boost::numeric::odeint::error_stepper_tag;
		public:
			interfere_stepper(base_stepper Stepper_) :CanBreak(false), Stepper(std::move(Stepper_)) {}
			order_type order()const { return Stepper.order(); }
			order_type stepper_order()const { return Stepper.stepper_order(); }
			order_type error_order()const { return Stepper.error_order(); }
			template<typename sys_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt) {
				Stepper.do_step(sys, x, t, dt);

				interfere_request req = sys.interfere(x, t, dt, nx);
				switch (req) {
				case interfere_request::breakable:
					CanBreak = true;
					break;
				case interfere_request::reset:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				case interfere_request::initialize:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				default:
					CanBreak = false;
					break;
				}
			}
			template<typename sys_type, typename error_type>
			void do_step(sys_type sys, state_type& x, time_type t, time_type dt, error_type& xerr) {
				Stepper.do_step(sys, x, t, dt, xerr);

				interfere_request req = sys.interfere(x, t, dt, nx);
				switch (req) {
				case interfere_request::breakable:
					CanBreak = true;
					break;
				case interfere_request::reset:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				case interfere_request::initialize:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				default:
					CanBreak = false;
					break;
				}
			}
			bool can_break()const { return CanBreak; }
			void reset() {
				CanBreak = false;
				try_reset(Stepper);
			}
		private:
			bool CanBreak;
			base_stepper Stepper;
			state_type nx;
		};
		template<typename base_stepper_>
		struct interfere_stepper<base_stepper_, boost::numeric::odeint::controlled_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using stepper_category = boost::numeric::odeint::controlled_stepper_tag;
		public:
			interfere_stepper(base_stepper Stepper_) :CanBreak(false), Stepper(std::move(Stepper_)) {}
			template<typename sys_type>
			boost::numeric::odeint::controlled_step_result try_step(sys_type sys, state_type& x, time_type& t, time_type& dt) {
				auto result = Stepper.try_step(sys, x, t, dt);
				//if not success, don't try interfere because this result will be rejected.
				if (result != boost::numeric::odeint::controlled_step_result::success)return result;

				interfere_request req = sys.interfere(x, t, dt, nx);
				switch (req) {
				case interfere_request::breakable:
					CanBreak = true;
					break;
				case interfere_request::reset:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				case interfere_request::initialize:
					try_reset(Stepper);
					boost::numeric::odeint::copy(nx, x);
					CanBreak = false;
					break;
				default:
					CanBreak = false;
					break;
				}
				return result;
			}
			bool can_break()const { return CanBreak; }
			void reset() {
				CanBreak = false;
				try_reset(Stepper);
			}
		private:
			bool CanBreak;
			base_stepper Stepper;
			state_type nx;
		};
		template<typename base_stepper_>
		struct interfere_stepper<base_stepper_, boost::numeric::odeint::dense_output_stepper_tag> {
			using base_stepper = base_stepper_;
			using state_type = typename base_stepper::state_type;
			using deriv_type = typename base_stepper::deriv_type;
			using time_type = typename base_stepper::time_type;
			using stepper_category = boost::numeric::odeint::dense_output_stepper_tag;
		public:
			interfere_stepper(base_stepper Stepper_) : CanBreak(false), ReserveInitialize(false), Stepper(std::move(Stepper_)) {}
			void initialize(const state_type& x, time_type t, time_type dt) {
				CanBreak = false;
				Stepper.initialize(x, t, dt);
			}
			template<typename sys_type>
			std::pair<time_type, time_type> do_step(sys_type sys) {
				if (ReserveInitialize) {
					try_reset(Stepper);
					initialize(nx, nt, ndt);
					ReserveInitialize = false;
				}
				auto result = Stepper.do_step(sys);

				nt = current_time();
				ndt = current_time_step();
				interfere_request req = sys.interfere(current_state(), nt, ndt, nx);
				switch (req) {
				case interfere_request::breakable:
					CanBreak = true;
					break;
				case interfere_request::reset:
					ReserveInitialize = true;
					result.second = t;
					CanBreak = false;
					break;
				case interfere_request::initialize:
					ReserveInitialize = true;
					result.second = t;
					CanBreak = false;
					break;
				default:
					CanBreak = false;
					break;
				}
				return result;
			}
			void calc_state(time_type t, state_type& x)const {
				Stepper.calc_state(t, x);
			}
			const state_type& current_state()const {
				if (ReserveInitialize)return nx;
				return Stepper.current_state();
			}
			const time_type& current_time()const {
				if (ReserveInitialize)return nt;
				return Stepper.current_time();
			}
			const time_type& current_time_step()const {
				if (ReserveInitialize)return ndt;
				return Stepper.current_time_step();
			}
			bool can_break()const { return CanBreak; }
			void reset() {
				CanBreak = false;
				try_reset(Stepper);
			}
		private:
			bool CanBreak;
			bool ReserveInitialize;
			base_stepper Stepper;
			state_type nx;
			time_type nt;
			time_type ndt;
		};

		template<typename stepper_, typename stepper_category = typename boost::numeric::odeint::unwrap_reference<stepper_>::type::stepper_category>
		auto make_interfere_stepper(stepper_&& Stepper_) {
			return interfere_stepper<
				typename boost::numeric::odeint::unwrap_reference<stepper_>::type,
				stepper_category
			>(std::forward<stepper_>(Stepper_));
		}
	}
}
#
#endif
