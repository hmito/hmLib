#ifndef HMLIB_MINIMA_RANGEPRECISIONBREAKER_INC
#define HMLIB_MINIMA_RANGEPRECISIONBREAKER_INC 100
#
#include<cmath>
namespace hmLib{
	namespace minima{
 		template<typename value_type_>
		struct range_precision_breaker{
			//following functions should be callable;
			// state_type::lower() : return lower value of the range
			// state_type::upper() : return upper value of the range
			// state_type::value() : return optimal value of the range
			using value_type = value_type_;
		public:
			range_precision_breaker() = delete;
			explict range_precision_breaker(value_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			range_precision_breaker(value_type relative_error_, value_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename state_type>
			auto precision(const state_type& x)const{
				return relerr * std::abs(x.value()) + abserr / 4;
			}
			template<typename state_type>
			bool operator()(const state_type& x)const{
				return std::abs(x.value() - (x.upper() + x.lower()) / 2) + (x.upper() - x.lower()) / 2 <= precision(x.value()) * 2 ;
			}
		private:
			value_type relerr;
			value_type abserr;
		};
	}
}
#
#endif
