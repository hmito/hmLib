#ifndef HMLIB_MINIMA_SIMPLEXPRECISIONBREAKER_INC
#define HMLIB_MINIMA_SIMPLEXPRECISIONBREAKER_INC 100
#
#include<cmath>
#include<iterator>
namespace hmLib{
	namespace minima{
 		template<typename value_type_>
		struct simplex_precision_breaker{
			//following functions should be callable;
			// state_type::begin() : return begin iterator of the node array of the simplex
			// state_type::end() : return end iterator of the node array of simplex
			//  each node also should have begin/end for access value on each axis. 
			using value_type = value_type_;
		public:
			simplex_precision_breaker() = delete;
			explicit simplex_precision_breaker(value_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			simplex_precision_breaker(value_type relative_error_, value_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename state_type>
			bool operator()(const state_type& x)const{
				unsigned int size = std::distance(x.begin(), x.end()) - 1;
				for (unsigned int i = 0; i < size; ++i) {
					auto nitr = x.begin(); 
					auto lower = (*nitr).value()[i];
					auto upper = (*nitr).value()[i];
					++nitr;
					for(;nitr != x.end(); ++nitr){
						lower = std::min(lower,(*nitr).value()[i]);
						upper = std::max(upper,(*nitr).value()[i]);
					}

					if(std::abs(x.value()[i] - (upper + lower) / 2) + (upper - lower) / 2 > (relerr * std::abs(x.value()[i]) + abserr / 4) * 2){
						return false;
					}
				}
				return true;
			}
		private:
			value_type relerr;
			value_type abserr;
		};
	}
}
#
#endif
