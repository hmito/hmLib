#ifndef HMLIB_NUMERIC_NUMERICRESULT_INC
#define HMLIB_NUMERIC_NUMERICRESULT_INC 100
#
#include<type_traits>
#include<optional>
namespace hmLib{
	namespace numeric{
		template<typename state_type, typename value_type = typename state_type::value_type>
		struct step_result{
		public:
			unsigned int count;
			state_type state;
			std::optional<value_type> guess;
		public:
			step_result() = default;
			step_result(unsigned int count_,state_type&& state_):count(count_),state(std::move(state_)),guess(){}
			step_result(unsigned int count_,const state_type& state_):count(count_),state(state_),guess(){}
			step_result(unsigned int count_,state_type&& state_,value_type guess_):count(count_),state(std::move(state_)),guess(std::move(guess_)){}
			step_result(unsigned int count_,const state_type& state_,value_type guess_):count(count_),state(state_),guess(std::move(guess_)){}
			operator bool()const{return guess.has_value();}
			bool success()const{return guess.has_value();}
			const value_type& operator*()const&{return guess.operator*();}
			value_type operator*()&&{return std::move(guess.operator*());}
			const value_type* operator->()const{return guess.operator->();}
			value_type* operator->(){return guess.operator->();}
		};
		template<typename state_type,typename value_type>
		auto make_step_result(unsigned int count, state_type&& st,value_type&& guess){
			return step_result<std::decay_t<state_type>,typename std::decay_t<state_type>::value_type>(
				count,std::forward<state_type>(st),std::forward<value_type>(guess)
			);
		}
		template<typename state_type>
		auto make_step_result(unsigned int count, state_type&& st){
			return step_result<std::decay_t<state_type>,typename std::decay_t<state_type>::value_type>(
				count,std::forward<state_type>(st)
			);
		}
	}
}
#
#endif
