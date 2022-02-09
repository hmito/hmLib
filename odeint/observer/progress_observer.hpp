#ifndef HMLIB_ODEINT_OBSERVER_PROGRESSOBSERVER_INC
#define HMLIB_ODEINT_OBSERVER_PROGRESSOBSERVER_INC 100
#
#include<iostream>
#include"../../io_utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename time_type>
		class progress_observer {
		private:
			progress_bar<time_type> Bar;
			std::ostream& out;
		public:
			progress_observer(time_type strt_t_, time_type end_t_, std::ostream& out_=std::cout) :Bar(strt_t_, end_t_), out(out_) {}
			void reset() { Bar.reset(Bar.beg_time(), Bar.end_time()); }
			template<typename state_type>
			void operator()(const state_type&, time_type t) {
				Bar(out, t);
			}
		};
		template<typename time_type>
		class estimate_progress_observer {
		private:
			estimate_progress_bar<time_type> Bar;
			std::ostream& out;
		public:
			estimate_progress_observer(time_type strt_t_, time_type end_t_, std::ostream& out_ = std::cout) :Bar(strt_t_, end_t_), out(out_) {}
			void reset() { Bar.reset(Bar.beg_time(), Bar.end_time()); }
			template<typename state_type>
			void operator()(const state_type&, time_type t) {
				Bar(out, t);
			}
		};
	}
}
#
#endif
