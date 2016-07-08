#ifndef HMLIB_ODEINT_BREAKABLEOBSERVER_INC
#define HMLIB_ODEINT_BREAKABLEOBSERVER_INC 100 
#
#include<utility>
#include<boost/numeric/odeint/integrate/null_observer.hpp>
namespace hmLib{
	namespace odeint{
		template<typename observer, typename break_condition>
		struct break_observer : public observer{
		private:
			break_condition BreakCond;
		public:
			template<typename... others>
			break_observer(break_condition BreakCond_, others... Others)
				: observer(Others...), BreakCond(BreakCond_){}
			template<typename state, typename time>
			bool operator()(const state& State, time Time){
				observer::operator()(State, Time);
				return BreakCond(State);
			}
		};
		template<typename observer, typename break_condition>
		break_observer<observer, break_condition> make_break_observer(break_condition BreakCondition, observer Observer){
			return break_observer<observer, break_condition>(std::forward<break_condition>(BreakCondition), std::forward<observer>(Observer));
		}
		template<typename break_condition>
		break_observer<boost::numeric::odeint::null_observer, break_condition> make_break_observer(break_condition BreakCondition){
			return break_observer<boost::numeric::odeint::null_observer, break_condition>(std::forward<break_condition>(BreakCondition), boost::numeric::odeint::null_observer());
		}
	}
}
#
#endif
