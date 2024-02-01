#ifndef HMLIB_NUMERIC_NUMERICRESULT_INC
#define HMLIB_NUMERIC_NUMERICRESULT_INC 100
#
#include<optional>
namespace hmLib{
	namespace numeric{
		template<typename guess_type, typename state_type>
		struct step_result{
		private:
			unsigned int countval;
			state_type stateval;
			std::optional<guess_type> guessval;
		public:
			step_result() = default;
			step_result(unsigned int count_,state_type&& state_):countval(count_),stateval(std::move(state_)),guessval(){}
			step_result(unsigned int count_,const state_type& state_):countval(count_),stateval(state_),guessval(){}
			step_result(unsigned int count_,state_type&& state_,guess_type guess_):countval(count_),stateval(std::move(state_)),guessval(std::move(guess_)){}
			step_result(unsigned int count_,const state_type& state_,guess_type guess_):countval(count_),stateval(state_),guessval(std::move(guess_)){}
			operator bool()const{return guessval;}
			const guess_type& operator*()const&{return *guessval;}
			guess_type operator*()&&{return std::move(*guessval);}
			const state_type& state()const&{return stateval;}
			state_type state()&&{return std::move(stateval);}
			unsigned int count()const{return countval;}
		};
	}
}
#
#endif
