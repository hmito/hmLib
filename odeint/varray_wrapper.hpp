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
					for(auto itr = std::next(x.begin()); itr!=x.end(); ++itr){
						if(val <std::abs(*itr)) val = std::abs(*itr);
					}
					return val;
				}
			};
		}
	}
}
namespace hmLib {
	template< typename value_type, typename T, std::size_t N>
	value_type max(value_type ini, const hmLib::varray<T, N>& x) {
		value_type val = ini;
		for(auto itr = x.begin(); itr!=x.end(); ++itr) {
			if(val < std::abs(*itr)) val = std::abs(*itr);
		}
		return val;
	}
}
#
#endif
