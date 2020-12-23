#ifndef HMLIB_UBLAS_INC
#define HMLIB_UBLAS_INC 100

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace hmLib {
	namespace ublas {
		template<typename T>
		bool invert(const boost::numeric::ublas::matrix<T>& a, boost::numeric::ublas::matrix<T>& b) {
			namespace ub = boost::numeric::ublas;
			ub::matrix<T> tmp(a);
			ub::permutation_matrix<> pm(tmp.size1());

			//fail to factorize
			if(ub::lu_factorize(tmp, pm) != 0) return true;

			b = ub::identity_matrix<T>(tmp.size1());

			ub::lu_substitute(tmp, pm, b);

			return false;
		}
	}
}
#endif
