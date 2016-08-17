#ifndef HMLIB_ODEINT_BOUNEDSTEPPER_INC
#define HMLIB_ODEINT_BOUNEDSTEPPER_INC 100
#
#include<utility>
#include<boost/optional.hpp>
#include<boost/numeric/odeint.hpp>
#include"utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename stepper_, typename region_type_ = int, typename stepper_category = typename boost::numeric::odeint::unwrap_reference<stepper_>::type::stepper_category>
		struct region_abridged_stepper{
			static_assert(true,"region_abridged_stepper can use only dense_output_stepper as the basic stepper.");
		};
		template<typename stepper_, typename region_type_>
		struct region_abridged_stepper<stepper_, region_type_, boost::numeric::odeint::dense_output_stepper_tag>{
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
			using region_type = region_type_;
		private:
			template<typename region_system>
			struct region_fix_system{
				region_system& Ref;
				region_type Region;
				region_fix_system(region_system& Ref_, region_type Region_)
					: Ref(Ref_)
					, Region(Region_){}
				void operator()(const state_type& x, deriv_type& dxdt, time_type t){
					Ref(x, dxdt, t, Region);
				}
				region_type region(const state_type& x, time_type t){
					return Ref.region(x, t);
				}
				region_type current_region(){ return Region; }
				void update_current_region(region_type Region_){ Region = Region_; }
			};
		private:
			double RegionError;
			stepper_type Stepper;
			boost::optional<region_type> CurrentRegion;
			state_type CurrentState;
			time_type CurrentTime;
		public:
			region_abridged_stepper(double RegionError_, const stepper_type& Stepper_)
				: RegionError(RegionError_)
				, Stepper(Stepper_){}
			region_abridged_stepper(double RegionError_, stepper_type&& Stepper_)noexcept
				: RegionError(RegionError_)
				, Stepper(std::move(Stepper_)){}
		public:
			void initialize(const state_type& State, time_type Time, time_type dt){
				CurrentState = State;
				CurrentTime = Time;
				CurrentRegion = boost::none;
				Stepper.initialize(CurrentState, CurrentTime, dt);
			}
			template<typename region_system_>
			std::pair<time_type, time_type> do_step(region_system_& System_){
				if(CurrentTime != Stepper.current_time()){
					time_type dt = Stepper.current_time_step();
					Stepper.initialize(CurrentState, CurrentTime, dt);
					CurrentRegion = boost::none;
				}

				if(!CurrentRegion){
					CurrentRegion = System_.region(CurrentState, CurrentTime);
				}

				region_fix_system<region_system_> System(System_, *CurrentRegion);

				auto TimePair = Stepper.do_step(System);
				auto NewRegion = System.region(Stepper.current_state(), Stepper.current_time());

				if(NewRegion != *CurrentRegion){
					auto NewState = Stepper.current_state();
					auto NewTime = Stepper.current_time();
					auto Time = CurrentTime;

					state_type State;
					while(detail::abs_distance(CurrentState, NewState) > RegionError){
						auto HalfTime = (CurrentTime + NewTime) / 2.;

						if(Time == HalfTime){
							//If time cannot have enough difference, next step is direct linear position fitting
							while(detail::abs_distance(CurrentState, NewState) > RegionError){
								algebra_type Algebra;
								Algebra.for_each3(State, CurrentState, NewState, [=](double& out, double in1, double in2){out = in1 / 2 + in2 / 2; });

								NewRegion = System.region(State, Time);
								if(NewRegion == *CurrentRegion){
									CurrentState = State;
								} else{
									NewState = State;
								}
							}
							break;
						}

						Time = HalfTime;

						Stepper.calc_state(Time, State);
						NewRegion = System.region(State, Time);
						if(NewRegion == *CurrentRegion){
							CurrentState = State;
							CurrentTime = Time;
						} else{
							NewState = State;
							NewTime = Time;
						}
					}
					if(NewRegion >= 0){
						CurrentState = NewState;
						CurrentTime = NewTime;
						CurrentRegion = NewRegion;
					}
				} else{
					CurrentState = Stepper.current_state();
					CurrentTime = Stepper.current_time();
					CurrentRegion = NewRegion;
				}

				TimePair.second = CurrentTime;

				return TimePair;
			}
			void calc_state(time_type t, state_type& x){
				Stepper.calc_state(t, x);
			}
			const time_type& current_time()const{
				return CurrentTime;
			}
			const state_type& current_state()const{
				return CurrentState;
			}
			time_type current_time_step()const{
				return Stepper.current_time_step();
			}
			region_type current_region()const{
				if(!CurrentRegion)return 0;
				return *CurrentRegion;
			}
			stepper_type& base(){ return Stepper; }
		};
	}
}
#
#endif
