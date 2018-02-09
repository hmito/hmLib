#ifndef HMLIB_MATH_ROOT_INC
#define HMLIB_MATH_ROOT_INC 100
#
#include<utility>
#include<iterator>
#include<boost/math/tools/roots.hpp>
namespace hmLib {
	namespace root {
		template<typename fn, typename T, typename output_iterator>
		output_iterator small_step_toms748(fn Fn, T minval, T maxval, std::size_t n, T error, output_iterator out) {
			using ans_t = decltype(Fn(minval));
			T val = minval;
			auto fval = Fn(val);
			if(fval == 0) {
				*(out++) = val;
			}

			for(std::size_t i = 1; i <= n; ++i) {
				auto nval = minval + i*(maxval - minval) / n;
				auto nfval = Fn(nval);

				if(fval*nfval <= 0) {
					if(nfval == 0) {
						*(out++) = nval;
					} else if(fval != 0) {
						auto valpair = boost::math::tools::toms748_solve(Fn, val, nval,fval, nfval, [=](ans_t v1, ans_t v2) {return v2 - v1 < error; });
						*(out++) = (valpair.first + valpair.second) / 2;
					}
				}
				val = std::move(nval);
				fval = std::move(nfval);
			}

			return out;
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator small_step_bisect(fn Fn, T minval, T maxval, std::size_t n, T error, output_iterator out) {
			using ans_t = decltype(Fn(minval));
			T val = minval;
			auto fval = Fn(val);
			if(fval == 0) {
				*(out++) = val;
			}

			for(std::size_t i = 1; i <= n; ++i) {
				auto nval = minval + i*(maxval - minval) / n;
				auto nfval = Fn(nval);

				if(fval*nfval <= 0) {
					if(nfval == 0) {
						*(out++) = nval;
					} else if(fval != 0) {
						auto valpair = boost::math::tools::bisect(Fn, val, nval,[=](ans_t v1, ans_t v2) {return v2 - v1 < error; });
						*(out++) = (valpair.first + valpair.second) / 2;
					}
				}
				val = std::move(nval);
				fval = std::move(nfval);
			}

			return out;
		}
	}
}
#
#endif