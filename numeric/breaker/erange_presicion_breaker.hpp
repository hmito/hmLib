#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<cmath>
#include"../evalue.hpp"
namespace hmLib{
	namespace numeric{
		template<typename error_type_>
		struct erange_precision_breaker{
			//following functions should be callable;
			// state_type::lower() : return lower value of the range
			// state_type::upper() : return upper value of the range
			// state_type::value() : return optimal value of the range
			using error_type = error_type_;
		public:
			erange_precision_breaker() = delete;
			explicit erange_precision_breaker(error_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			erange_precision_breaker(error_type relative_error_, error_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename value_type, typename eval_type>
			auto precision(const erange<value_type,eval_type>& x)const{
				return relerr * std::abs((x.upper.v + x.lower.v) / 2) + abserr / 4;
			}
			template<typename value_type, typename eval_type>
			auto precision(const etrio<value_type,eval_type>& x)const{
				return relerr * std::abs(x.guess.v) + abserr / 4;
			}
			template<typename value_type, typename eval_type, typename step_type>
			bool operator()(const erange<value_type,eval_type>& x, step_type)const{
				return (x.upper.v - x.lower.v) / 2 <= precision(x) * 2 ;
			}
			template<typename value_type, typename eval_type, typename step_type>
			bool operator()(const etrio<value_type,eval_type>& x, step_type)const{
				return std::abs(x.guess.v - (x.upper.v + x.lower.v) / 2) + (x.upper.v - x.lower.v) / 2 <= precision(x) * 2 ;
			}
		private:
			error_type relerr;
			error_type abserr;
		};
	}
}
#
#endif
