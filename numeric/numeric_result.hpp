#ifndef HMLIB_NUMERIC_NUMERICRESULT_INC
#define HMLIB_NUMERIC_NUMERICRESULT_INC 100
#
#include<type_traits>
#include<optional>
namespace hmLib{
	namespace numeric{
		template<typename state_type, typename evalue_type = typename state_type::evalue_type>
		struct step_result{
		public:
			unsigned int count;
			state_type state;
			std::optional<evalue_type> guess;
		public:
			step_result() = default;
			step_result(unsigned int count_,state_type&& state_):count(count_),state(std::move(state_)),guess(){}
			step_result(unsigned int count_,const state_type& state_):count(count_),state(state_),guess(){}
			step_result(unsigned int count_,state_type&& state_,evalue_type guess_):count(count_),state(std::move(state_)),guess(std::move(guess_)){}
			step_result(unsigned int count_,const state_type& state_,evalue_type guess_):count(count_),state(state_),guess(std::move(guess_)){}
			operator bool()const{return guess.has_value();}
			bool success()const{return guess.has_value();}
			const evalue_type& operator*()const&{return guess.operator*();}
			evalue_type operator*()&&{return std::move(guess.operator*());}
			const evalue_type* operator->()const{return guess.operator->();}
			evalue_type* operator->(){return guess.operator->();}
		};
		template<typename state_type,typename evalue_type>
		auto make_step_result(unsigned int count, state_type&& st,evalue_type&& guess){
			return step_result<std::decay_t<state_type>,typename std::decay_t<state_type>::evalue_type>(
				count,std::forward<state_type>(st),std::forward<evalue_type>(guess)
			);
		}
		template<typename state_type>
		auto make_step_result(unsigned int count, state_type&& st){
			return step_result<std::decay_t<state_type>,typename std::decay_t<state_type>::evalue_type>(
				count,std::forward<state_type>(st)
			);
		}
	}
}
#
#endif
