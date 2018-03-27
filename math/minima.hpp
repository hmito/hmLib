#ifndef HMLIB_MATH_MINIMA_INC
#define HMLIB_MATH_MINIMA_INC 100
#
#include<array>
#include"../circular.hpp"
#include<boost/math/tools/minima.hpp>
namespace hmLib {
	namespace math {
		template<typename T>
		struct brent_minima_stepper {
			using value_type = T;
		private:
			value_type step;
			int bits;
		public:
			brent_minima_stepper(value_type step_, int bits_):step(step_), bits(bits_) {}
			template<typename fn, typename ans_type>
			std::pair<value_type, value_type> operator()(fn Fn, value_type& start, ans_type& startFnVal, value_type end) {
				circular<T, 3> val{ start, start + step / 2 , start + step };
				if(start + step > end) {
					val[1] = (start + end) / 2;
					val[2] = end;
				}
				circular<ans_type, 3> fval{ Fn(val[0]),Fn(val[1]) ,Fn(val[2]) };

				do {
					if(fval[0] >= fval[1] && fval[1] <= fval[2]) {
						auto ans = boost::math::tools::brent_find_minima(Fn, val[0], val[1], bits);
						start = val[2];
						startFnVal = fval[2];
						return ans;
					}

					if(val[2] >= end) {
						break;
					} else if(val[1] + step > end) {
						val.rotete_back(end);
						fval.rotate_back(Fn(end));
					} else {
						val.rotete_back(val[1] + step);
						fval.rotate_back(Fn(val.back()));
					}
				} while(true);

				start = val.back();
				startFnVal = fval.back();

				return std::make_pair(end, end);
			}
			void reset(value_type step_, int bits_) {
				step = step_;
				bits = bits_;
			}
		};
		template<typename minima_stepper, typename fn, typename T, typename output_iterator>
		output_iterator minima(minima_stepper Stepper, fn Fn, T minval, T maxval, output_iterator out) {
			auto FnVal = Fn(minval);
			while(minval<maxval) {
				auto Ans = Stepper(Fn, minval, FnVal, maxval);

				if(Ans.first < maxval) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			return out;
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator minima_brent(fn Fn, T minval, T maxval, T step, int bits, output_iterator out) {
			brent_minima_stepper<T> Stepper(step, bits);
			return minima(Stepper, Fn, minval, maxval, out);
		}
	}
}
#
#endif
