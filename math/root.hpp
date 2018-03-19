#ifndef HMLIB_MATH_ROOT_INC
#define HMLIB_MATH_ROOT_INC 100
#
#include<utility>
#include<iterator>
#include<boost/math/tools/roots.hpp>
namespace hmLib {
	namespace root {
		template<typename fn, typename T,typename ans_type>
		std::pair<T,T> root_step_toms748(fn Fn, T& start, ans_type& startFnVal, T end, T step, T error) {
			T a = start;
			ans_type fa = startFnVal;

			while (a < end) {
				auto b = std::min(a + step, end);
				auto fb = Fn(b);

				if (fb == 0) {
					start = b;
					startFnVal = fb;
					return std::make_pair(b, b);
				} else if (fa * fb < 0.0) {
					auto ans = boost::math::tools::toms748_solve(Fn, a, b, fa, fb, [error](ans_type v1, ans_type v2) {return v2 - v1 < error; });
					start = b;
					startFnVal = fb;
					return ans;
				}
				
				a = b;
				fa = fb;
			}

			start = a;
			startFnVal = fa;

			return std::make_pair(a, a);
		}
		template<typename fn, typename T, typename ans_type>
		std::pair<T,T> root_step_bisect(fn Fn, T& start, ans_type& startFnVal, T end, T step, T error) {
			T a = start;
			ans_type fa = startFnVal;

			while (a < end) {
				auto b = std::min(a + step, end);
				auto fb = Fn(b);

				if (fb == 0) {
					start = b;
					startFnVal = fb;
					return std::make_pair(b, b);
				}
				else if (fa * fb < 0.0) {
					auto ans = boost::math::tools::bisect(Fn, a, b, [error](ans_type v1, ans_type v2) {return v2 - v1 < error; });
					start = b;
					startFnVal = fb;
					return ans;
				}

				a = b;
				fa = fb;
			}

			start = a;
			startFnVal = fa;

			return std::make_pair(a, a);
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator root_toms748(fn Fn, T minval, T maxval, std::size_t n, T error, output_iterator out) {
			auto FnVal = Fn(minval);
			T step = (maxval - minval) / n;

			if (FnVal == 0) {
				*(out++) = minval;
			}

			while (minval<maxval) {
				Ans = root_step_toms748(Fn, minval, FnVal, maxval, step, error);

				if (Ans.first < end) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			if (FnVal == 0) {
				*(out++) = maxval;
			}

			return out;
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator root_bisect(fn Fn, T minval, T maxval, std::size_t n, T error, output_iterator out) {
			auto FnVal = Fn(minval);
			T step = (maxval - minval) / n;

			if (FnVal == 0) {
				*(out++) = minval;
			}

			while (minval<maxval) {
				Ans = root_step_bisect(Fn, minval, FnVal, maxval, step, error);

				if (Ans.first < end) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			if (FnVal == 0) {
				*(out++) = maxval;
			}

			return out;
		}
	}
}
#
#endif
