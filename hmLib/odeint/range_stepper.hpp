#ifndef HMLIB_ODEINT_BOUNEDSTEPPER_INC
#define HMLIB_ODEINT_BOUNEDSTEPPER_INC 100
#
#include<utility>
#include<boost/optional.hpp>
#include<boost/numeric/odeint/stepper/dense_output_runge_kutta.hpp>
#include"utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename dense_output_stepper_, typename region_type_=unsigned int>
		struct region_abridged_stepper{
		public:
			using stepper_type = dense_output_stepper_;
			using state_type = typename stepper_type::state_type;
			using value_type = typename stepper_type::value_type;
			using deriv_type = typename stepper_type::deriv_type;
			using time_type = typename stepper_type::time_type;
			using algebra_type =  typename stepper_type::algebra_type;
			using operations_type = typename stepper_type::operations_type;
			using resizer_type = typename stepper_type::resizer_type;
			using stepper_category = boost::numeric::odeint::dense_output_stepper_tag;
			using region_type = region_type_;
		private:
			double RegionError;
			stepper_type Stepper;
			boost::optional<region_type> CurrentRegion;
			state_type CurrentState;
			time_type CurrentTime;
		public:
			region_abridged_stepper(double RegionError_, const stepper_type& Stepper_)
				: RegionError(RegionError_)
				, Stepper(Stepper_){
			}
			region_abridged_stepper(double RegionError_, stepper_type&& Stepper_)noexcept
				: RegionError(RegionError_)
				, Stepper(std::move(Stepper_)){}
		public:
			void initialize(const state_type& State, time_type Time, time_type dt){
				CurrentState = State;
				CurrentTime = Time;
				CurrentRegion = boost::none;
				Stepper.initialize(CurrentState,CurrentTime,dt);
			}
			template<typename region_system_>
			std::pair<time_type, time_type> do_step(region_system_& System){
				if(CurrentTime != Stepper.current_time()){
					time_type dt = Stepper.current_time_step();
					Stepper.initialize(CurrentState, CurrentTime,dt);
					CurrentRegion = boost::none;
				}
				
				if(!CurrentRegion){
					CurrentRegion = System.region(CurrentState, CurrentTime);
				}
				
				auto TimePair = Stepper.do_step(System);
				auto NewRegion = System.region(Stepper.current_state(), Stepper.current_time());

				if(NewRegion != *CurrentRegion){
					auto NewState = Stepper.current_state();
					auto NewTime = Stepper.current_time();
					
					state_type State;
					while(detail::abs_distance(CurrentState,NewState) < RegionError){
						auto Time = CurrentTime+NewTime;
						Stepper.calc_state(Time,State);
						NewRegion = System.region(State,Time);
						if(NewRegion == *CurrentRegion){
							std::swap(State,CurrentState);
							CurrentTime = Time;
						}else{
							std::swap(State,NewState);
							NewTime = Time;
						}
					}
					if(NewRegion){
						std::swap(CurrentState,NewState);
						CurrentTime = NewTime;
						CurrentRegion = NewRegion;
					}
				}else{
					CurrentState = Stepper.current_state();
					CurrentTime = Stepper.current_time();
					CurrentRegion = NewRegion;					
				}
				
				TimePair.second = CurrentTime;
				
				return TimePair;
			}
			void calc_state(time_type t, state_type& x){
				Stepper.calc_state(t,x);
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
		};
	}
}
#
#endif
