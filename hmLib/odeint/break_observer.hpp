#ifndef HMLIB_ODEINT_BREAKABLEOBSERVER_INC
#define HMLIB_ODEINT_BREAKABLEOBSERVER_INC 100 
#
#include<utility>
#include<boost/numeric/odeint/integrate/null_observer.hpp>
namespace hmLib{
	namespace odeint{
		template<typename observer, typename breaker>
		struct break_observer : public observer{
		private:
			breaker Breaker;
		public:
			template<typename... others>
			break_observer(breaker Breaker_, others... Others)
				: observer(Others...), Breaker(Breaker_){}
			template<typename state, typename time>
			bool operator()(const state& State, time Time){
				bool IsBrake = Breaker(State, Time);
				observer::operator()(State, Time);
				return IsBrake;
			}
			breaker& get_breaker(){ return Breaker; }
		};
		template<typename observer, typename observer_based_breaker>
		struct observer_based_break_observer : public observer{
		private:
			observer_based_breaker Breaker;
		public:
			template<typename... others>
			observer_based_break_observer(observer_based_breaker Breaker_, others... Others)
				: observer(Others...), Breaker(Breaker_){}
			template<typename state, typename time>
			bool operator()(const state& State, time Time){
				bool IsBrake = Breaker(State, Time, static_cast<observer&>(*this));
				observer::operator()(State, Time);
				return IsBrake;
			}
			observer_based_breaker& get_breaker(){ return Breaker; }
		};
		template<typename observer, typename breaker>
		break_observer<observer, breaker> make_break_observer(breaker Breakerition, observer Observer){
			return break_observer<observer, breaker>(std::forward<breaker>(Breakerition), std::forward<observer>(Observer));
		}
		template<typename breaker>
		break_observer<boost::numeric::odeint::null_observer, breaker> make_break_observer(breaker Breakerition){
			return break_observer<boost::numeric::odeint::null_observer, breaker>(std::forward<breaker>(Breakerition), boost::numeric::odeint::null_observer());
		}
	}
}
#
#endif
