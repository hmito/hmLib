#ifndef HMLIB_ODEINT_COMPOSITESTEPPER_INC
#define HMLIB_ODEINT_COMPOSITESTEPPER_INC 100
#
#include<utility>
#include<boost/numeric/odeint.hpp>
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
#include"utility.hpp"
namespace hmLib {
	namespace odeint {
		//composite_stepper
		//bool is_exceed(const state_type& State, time_type Time, const state& NewState, time_type NewTime);
		//void exceed(state& State, time_type& Time, const state& Lower, time_type LowerTime, const state& Upper, time_type UpperTime);
		/*!
		@brief stepper for region_system
		*/
		template<typename stepper_, typename stepper_category = typename boost::numeric::odeint::unwrap_reference<stepper_>::type::stepper_category>
		class composite_stepper {};

		template<typename stepper_>
		struct composite_stepper<stepper_, boost::numeric::odeint::dense_output_stepper_tag> {
		public:
			using stepper_type = stepper_;
			using state_type = typename stepper_type::state_type;
			using value_type = typename stepper_type::value_type;
			using deriv_type = typename stepper_type::deriv_type;
			using time_type = typename stepper_type::time_type;
			using algebra_type = typename stepper_type::algebra_type;
			using operations_type = typename stepper_type::operations_type;
			using resizer_type = typename stepper_type::resizer_type;
			using stepper_category = boost::numeric::odeint::dense_output_stepper_tag;
		private:
			stepper_type Stepper;
			double ExceedError;
			state_type CurrentState;
			time_type CurrentTime;
		public:
			composite_stepper(const stepper_type& Stepper_, double ExceedError_)
				: Stepper(Stepper_)
				, ExceedError(ExceedError_){
			}
			composite_stepper(stepper_type&& Stepper_, double ExceedError_)noexcept
				: Stepper(std::move(Stepper_))
				, ExceedError(ExceedError_) {
			}
		public:
			void initialize(const state_type& State, time_type Time, time_type dt) {
				CurrentState = State;
				CurrentTime = Time;
				Stepper.initialize(CurrentState, CurrentTime, dt);
			}
			template<typename composite_system>
			std::pair<time_type, time_type> do_step(composite_system& Sys) {
				if(CurrentTime != Stepper.current_time()) {
					Stepper.initialize(CurrentState, CurrentTime, Stepper.current_time_step());
				}

				auto TimePair = Stepper.do_step(Sys);

				//check if state exceed the range where currecnt system can calculate.
				if(Sys.is_exceed(CurrentState, CurrentTime, Stepper.current_state(), Stepper.current_time())) {
					auto LowerState = CurrentState;
					auto LowerTime = CurrentTime;
					auto UpperState = Stepper.current_state();
					auto UpperTime = Stepper.current_time();

					auto State = CurrentState;
					auto Time = (LowerTime + UpperTime) / 2.;

					boost::numeric::odeint::euler
					while(boost::numeric::odeint::detail::abs_distance(LowerState, UpperState) > ExceedError && LowerTime < Time && Time < UpperTimer) {
						//calculate State at Time
						Stepper.calc_state(Time, State);

						//update upper or lower condition
						if(Sys.is_exceed(CurrentState, CurrentTime, State, Time)) {
							UpperState = std::move(State);
							UpperTime = Time;
						} else {
							LowerState = std::move(State);
							LowerTime = Time;
						}

						//update time
						Time = (LowerTime + UpperTime) / 2.;
					}

					//If time cannot have enough difference, next step is direct linear position fitting
					while(boost::numeric::odeint::detail::abs_distance(LowerState, UpperState) > ExceedError) {
						algebra_type Algebra;
						Algebra.for_each3(State, LowerState, UpperState, [=](double& out, double in1, double in2) {out = in1 / 2 + in2 / 2; });

						//update upper or lower condition
						if(Sys.is_exceed(CurrentState, CurrentTime, State, Time)) {
							UpperState = std::move(State);
							UpperTime = Time;
						} else {
							LowerState = std::move(State);
							LowerTime = Time;
						}
					}

					//updat  current state and time
					Sys.exceed(CurrentState, CurrentTime, LowerState, LowerTime, UpperState, UpperTime);
					TimePair.second = CurrentTime;
				} else {
					CurrentState = Stepper.current_state();
					CurrentTime = Stepper.current_time();
				}

				return TimePair;
			}
			void calc_state(time_type t, state_type& x) {
				if(t==CurrentTime) {
					x = CurrentState;
				} else {
					Stepper.calc_state(t, x);
				}
			}
			const time_type& current_time()const {
				return CurrentTime;
			}
			const state_type& current_state()const {
				return CurrentState;
			}
			time_type current_time_step()const {
				return Stepper.current_time_step();
			}
			stepper_type& base_stepper() { return Stepper; }
		};

		template<typename stepper_>
		auto make_composite_dense_output(double AbsError, double RelError, double ExceedError, stepper_ BaseStepper) {
			auto DenseOutput = boost::numeric::odeint::make_dense_output(AbsError, RelError, BaseStepper);
			return composite_stepper<decltype(DenseOutput)>(DenseOutput, ExceedError);
		}
	}
}
#
#endif
