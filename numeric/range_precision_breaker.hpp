#ifndef HMLIB_NUMERIC_RANGEPRECISIONBREAKER_INC
#define HMLIB_NUMERIC_RANGEPRECISIONBREAKER_INC 100
#
#include<cmath>
#include"evalpair.hpp"
namespace hmLib{
	namespace numeric{
 		template<typename value_type_>
		struct range_precision_breaker{
			//following functions should be callable;
			// state_type::lower() : return lower value of the range
			// state_type::upper() : return upper value of the range
			// state_type::value() : return optimal value of the range
			using value_type = value_type_;
		public:
			range_precision_breaker() = delete;
			explicit range_precision_breaker(value_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			range_precision_breaker(value_type relative_error_, value_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename T>
			auto precision(const evalrange<T>& x)const{
				using std::abs;
				return relerr * abs((x.upper.v + x.lower.v) / 2) + abserr / 4;
			}
			template<typename T, typename step_type>
			bool operator()(const evalrange<T>& x, step_type)const{
				using std::abs;
				return (x.upper.v - x.lower.v) / 2 <= precision(x) * 2 ;
			}
			template<typename T>
			auto precision(const guess_evalrange<T>& x)const {
				using std::abs;
				return relerr * std::abs(x.guess.v) + abserr / 4;
			}
			template<typename T, typename step_type>
			bool operator()(const guess_evalrange<T>& x, step_type)const {
				using std::abs;
				return abs(x.guess.v - (x.upper.v + x.lower.v) / 2) + (x.upper.v - x.lower.v) / 2 <= precision(x) * 2;
			}
		private:
			value_type relerr;
			value_type abserr;
		};
	}
}
#
#endif
