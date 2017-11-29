#ifndef HMLIB_ODEINT_VARRAYWRAPPER_INC
#define HMLIB_ODEINT_VARRAYWRAPPER_INC 100
#
#include<boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include "../varray.hpp"
namespace boost {
	namespace numeric {
		namespace odeint {
			template< typename T, std::size_t N>
			struct vector_space_norm_inf< hmLib::varray<T,N> > {
				typedef T result_type;
				result_type operator()(const hmLib::varray<T, N> &x) const {
					T val = std::abs(x.front());
					for(auto itr = std::next(x.begin()); itr!=x.end(); ++x){
						if(val <std::abs(*val)) val = std::abs(*val);
					}
					return val;
				}
			};
		}
	}
}
#
#endif
