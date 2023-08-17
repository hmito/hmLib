#ifndef HMLIB_RECUR_BREAKER_EVALSTAGnANTBREAKER_INC
#define HMLIB_RECUR_BREAKER_EVALSTAGnANTBREAKER_INC 100
#
namespace hmLib {
	namespace recur{
		template<typename evaluator, typename eval_type>
		struct eval_stagnant_breaker {
		private:
			evaluator eval;
			eval_type last;
			unsigned int count;
			unsigned int limit;
		public:
			template<typename evaluator_,typename eval_type_>
			eval_stagnant_breaker(evaluator_&& eval_, eval_type_ inieval_, unsigned int limit_ = 1) 
				: eval(std::forward<evaluator_>(eval)), last(inieval_), count(0), limit(limit_) {}
			template<typename state_type>
			bool operator()(const state_type& x, time_type t) {
				auto thiseval = eval(x);
				if (thiseval == last) {
					return (++count >= limit);
				}
				last = thiseval;
				count = 0;
				return false;
			}
			void reset(eval_type_ inieval_) { last = inieval_; count = 0; }
		};
	}
}
#
#endif
