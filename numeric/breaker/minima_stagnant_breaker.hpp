#ifndef HMLIB_NUMERIC_BREAKER_MINIMASTAGNANTBREAKER_INC
#define HMLIB_NUMERIC_BREAKER_MINIMASTAGNANTBREAKER_INC 100
#
#include"../evalue.hpp"
#include"../esimplex.hpp"
namespace hmLib {
	namespace numeric{
		template<typename eval_type>
		struct minima_stagnant_breaker {
		private:
			eval_type last;
			unsigned int count;
			unsigned int limit;
		public:
			minima_stagnant_breaker(eval_type inieval_, unsigned int limit_ = 1) 
				: last(inieval_), count(0), limit(limit_) {}
			template<typename value_type,typename time_type>
			bool operator()(const evalue<value_type,eval_type>& x, time_type t) {
				if (x.e >= last) {
					return (++count >= limit);
				}
				last = x.e;
				count = 0;
				return false;
			}
			template<typename value_type,typename time_type>
			bool operator()(const erange<value_type,eval_type>& x, time_type t) {
				if(x.lower.e < x.upper.e){
					return operator()(x.lower,t);
				}else{
					return operator()(x.upper,t);
				}
			}
			template<typename value_type,typename time_type>
			bool operator()(const etrirange<value_type,eval_type>& x, time_type t) {
				return operator()(x.guess,t);
			}
			template<typename value_type,typename time_type>
			bool operator()(const esimplex<value_type,eval_type>& x, time_type t) {
				return operator()(*(x.minima()),t);
			}
			void reset(eval_type inieval_) { last = inieval_; count = 0; }
		};
		template<typename eval_type>
		auto make_minima_stagnant_breaker(eval_type eval, unsigned int limit){
			return minima_stagnant_breaker<std::decay_t<eval_type>>(eval,limit);
		}
	}
}
#
#endif
