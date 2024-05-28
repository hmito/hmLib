#ifndef HMLIB_NUMERIC_ESIMPLEXPRECISIONBREAKER_INC
#define HMLIB_NUMERIC_ESIMPLEXPRECISIONBREAKER_INC 100
#
#include<cmath>
#include<iterator>
#include"../esimplex.hpp"
namespace hmLib{
	namespace numeric{
		template<typename error_type_>
		struct esimplex_precision_breaker{
			//following functions should be callable;
			// state_type::begin() : return begin iterator of the node array of the simplex
			// state_type::end() : return end iterator of the node array of simplex
			//  each node also should have begin/end for access value on each axis. 
			using error_type = error_type_;
		public:
			esimplex_precision_breaker() = delete;
			explicit esimplex_precision_breaker(error_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			esimplex_precision_breaker(error_type relative_error_, error_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename value_type, typename eval_type,typename time_type>
			bool operator()(const esimplex<value_type,eval_type>& x, time_type)const{
				unsigned int size = std::distance(x.begin(), x.end()) - 1;
				auto minimaitr = x.minima();
				for (unsigned int i = 0; i < size; ++i) {
					auto nitr = x.begin(); 
					auto lower = (nitr->v)[i];
					auto upper = (nitr->v)[i];
					++nitr;
					for(;nitr != x.end(); ++nitr){
						lower = std::min(lower,(nitr->v)[i]);
						upper = std::max(upper,(nitr->v)[i]);
					}

					if(std::abs(minimaitr->v[i] - (upper + lower) / 2) + (upper - lower) / 2 > (relerr * std::abs(minimaitr->v[i]) + abserr / 4) * 2){
						return false;
					}
				}
				return true;
			}
		private:
			error_type relerr;
			error_type abserr;
		};
	}
}
#
#endif
