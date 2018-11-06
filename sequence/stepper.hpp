#ifndef HMLIB_SEQUENCE_STEPPER_INC
#define HMLIB_SEQUENCE_STEPPER_INC 100
#
namespace hmLib{
	namespace sequence{
		struct const_stepper{
		private:
			int n;
		public:
			const_stepper():const_stepper(1){}
			const_stepper(int n_):n(n_){}
			int get_step()const{return n;}
			void set_step(int n_){n=n_;}
			template<typename sys_t, typename state_t>
			void operator()(sys_t& Sys, state_t& State, int Step){
				for(int i=0; i<n; ++i){
					Sys(State,Step++);
				}
			}
		};
	}
}
#
#endif
