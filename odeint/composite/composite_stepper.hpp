#ifndef HMLIB_ODEINT_COMPOSITE_COMPOSITESTEPPER_INC
#define HMLIB_ODEINT_COMPOSITE_COMPOSITESTEPPER_INC 100
#
#include<utility>
#include<cmath>
#include<functional>
#include<boost/numeric/odeint/stepper/stepper_categories.hpp>
#include "../../functional.hpp"
namespace hmLib {
	namespace odeint {
		namespace detail {
			template<typename state_type, typename argebra_type, typename operations_type>
			double maximum_absolute_error(const state_type& v1, const state_type& v2) {
				state_type err=v1;
				return maximum_absolute_error(err, v1, v2);
			}
			template<typename state_type, typename argebra_type, typename operations_type>
			double maximum_absolute_error(state_type& err, const state_type& v1, const state_type& v2) {
				argebra_type::for_each3(err, v1, v2, typename operations_type::template scale_sum2<double, double>(1.0, -1.0));
				return argebra_type::norm_inf(err);
			}
			template<typename state_type, typename time_type>
			struct validator {
				template<typename cmp_system>
				static void call_validate(void* Ptr, state_type& x, time_type t) {
					static_cast<cmp_system*>(Ptr)->validate(x, t);
				}
			private:
				void* Ptr;
				function_ptr<void(void*, state_type&, time_type)> Fn;
			public:
				validator() :Ptr(nullptr) {}
				void reset() { Ptr = nullptr; }
				template<typename cmp_system>
				void reset(cmp_system& Sys) {
					Ptr = static_cast<void*>(&Sys);
					Fn = call_validate<cmp_system>;
				}
				void operator()(state_type& x, time_type t) { if(Ptr)Fn(Ptr, x, t); }
			};

		}
		//composite_system
		//bool is_exceed(const state_type& State, time_type Time, const state_type& NewState, time_type NewTime);
		//void exceed(state_type& State, time_type& Time, state_type state& Lower, time_type LowerTime, const state_type& Upper, time_type UpperTime);
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
			using validator = detail::validator<state_type, time_type>;
		private:
			stepper_type Stepper;
			double ExceedError;
			state_type CurrentState;
			time_type CurrentTime;
			void* LastSys;
			validator Validator;
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
				//boost::numeric::odeint::euler
				Sys.update(CurrentState, CurrentTime);

				auto TimePair = Stepper.do_step(Sys);

				//check if state exceed the range where currecnt system can calculate.
				if(!Sys.valid(Stepper.current_state(), Stepper.current_time())) {
					auto LowerState = CurrentState;
					auto LowerTime = CurrentTime;
					auto UpperState = Stepper.current_state();
					auto UpperTime = Stepper.current_time();

					auto State = CurrentState;
					auto Time = (LowerTime + UpperTime) / 2.;

					while(detail::maximum_absolute_error<state_type, algebra_type, operations_type>(State,LowerState,UpperState) > ExceedError && LowerTime < Time && Time < UpperTime) {
						//calculate State at Time
						Stepper.calc_state(Time, State);

						//update upper or lower condition
						if(!Sys.valid(State, Time)) {
							UpperState = std::move(State);
							UpperTime = Time;
						} else {
							LowerState = std::move(State);
							LowerTime = Time;
						}

						//update time
						Time = (LowerTime + UpperTime) / 2.;
					}

					//If time have insufficient accuracy, try direct linear position fitting
					while(detail::maximum_absolute_error<state_type, algebra_type, operations_type>(State, LowerState, UpperState) > ExceedError) {
						algebra_type Algebra;
						Algebra.for_each3(State, LowerState, UpperState, typename operations_type::template scale_sum2<double, double>(0.5, 0.5));

						//update upper or lower condition
						if(!Sys.valid(State, Time)) {
							UpperState = std::move(State);
							UpperTime = Time;
						} else {
							LowerState = std::move(State);
							LowerTime = Time;
						}
					}

					CurrentState = UpperState;
					CurrentTime = UpperTime;
					Validator.reset(Sys);

					//updat  current state and time
					Sys.validate(CurrentState, CurrentTime);
					TimePair.second = CurrentTime;
				} else {
					CurrentState = Stepper.current_state();
					CurrentTime = Stepper.current_time();
					Validator.reset();
				}

				return TimePair;
			}
			void calc_state(time_type t, state_type& x) {
				if(t==CurrentTime) {
					x = CurrentState;
				} else {
					Stepper.calc_state(t, x);
					Validator(x,t);
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
