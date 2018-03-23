#ifndef HMLIB_MATH_ROOT_INC
#define HMLIB_MATH_ROOT_INC 100
#
#include<utility>
#include<iterator>
#include<boost/math/tools/roots.hpp>
namespace hmLib {
	namespace math {
		template<typename T>
		struct toms748_root_stepper {
			using value_type = T;
		private:
			value_type step;
			value_type error;
		public:
			toms748_root_stepper(value_type step_, value_type error_):step(step_),error(error_){}
			template<typename fn, typename ans_type>
			std::pair<value_type, value_typeT> operator()(fn Fn, value_type& start, ans_type& startFnVal, value_type end)const {
				value_type a = start;
				ans_type fa = startFnVal;

				while(a < end) {
					auto b = std::min(a + step, end);
					auto fb = Fn(b);

					if(fb == 0) {
						start = b;
						startFnVal = fb;
						return std::make_pair(b, b);
					} else if(fa * fb < 0.0) {
						boost::uintmax_t max_iter = 128;
						auto ans = boost::math::tools::toms748_solve(Fn, a, b, fa, fb, [error](ans_type v1, ans_type v2) {return v2 - v1 < error; }, max_iter);
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
			void reset(value_type step_, value_type error_) {
				step = step_;
				error = error_;
			}
		};
		template<typename T>
		struct bisect_root_stepper {
			using value_type = T;
		private:
			value_type step;
			value_type error;
		public:
			bisect_root_stepper(value_type step_, value_type error_):step(step_), error(error_) {}
			template<typename fn, typename ans_type>
			std::pair<value_type, value_typeT> operator()(fn Fn, value_type& start, ans_type& startFnVal, value_type end)const {
				value_type a = start;
				ans_type fa = startFnVal;

				while(a < end) {
					auto b = std::min(a + step, end);
					auto fb = Fn(b);

					if(fb == 0) {
						start = b;
						startFnVal = fb;
						return std::make_pair(b, b);
					} else if(fa * fb < 0.0) {
						boost::uintmax_t max_iter = 128;
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
			void reset(value_type step_, value_type error_) {
				step = step_;
				error = error_;
			}
		};
		template<typename root_stepper, typename fn, typename T, typename output_iterator>
		output_iterator root(root_stepper Stepper, fn Fn, T minval, T maxval, output_iterator out) {
			auto FnVal = Fn(minval);
			while (minval<maxval) {
				auto Ans = Stepper(Fn, minval, FnVal, maxval);

				if (Ans.first < maxval) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			return out;
		}
		template<typename root_stepper, typename fn, typename T, typename output_iterator>
		output_iterator root_with_stability(root_stepper Stepper, fn Fn, T minval, T maxval, output_iterator out) {
			auto FnVal = Fn(minval);
			while(minval<maxval) {
				auto Ans = Stepper(Fn, minval, FnVal, maxval);

				if(Ans.first < maxval) {
					*(out++) = std::make_pair((Ans.first + Ans.second) / 2.0, FnVal<0);
				}
			}

			return out;
		}
		template<typename root_stepper, typename fn, typename T, typename output_iterator>
		output_iterator convergent_root(root_stepper Stepper, fn Fn, T minval, T maxval, output_iterator out) {
			auto FnVal = Fn(minval);
			while(minval<maxval) {
				auto Ans = Stepper(Fn, minval, FnVal);

				if(Ans.first < maxval && FnVal<0) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			return out;
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator root_toms748(fn Fn, T minval, T maxval, T step, T error, output_iterator out) {
			toms748_root_stepper<T> Stepper(step, error);
			return root(Stepper, Fn, minval, maxval, out);
		}
		template<typename root_stepper, typename fn, typename T, typename output_iterator>
		output_iterator root_with_stability_toms748(fn Fn, T minval, T maxval, T step, T error, output_iterator out) {
			toms748_root_stepper<T> Stepper(step, error);
			return root_with_stability(Stepper, Fn, minval, maxval, out);
		}
		template<typename root_stepper, typename fn, typename T, typename output_iterator>
		output_iterator convergent_root(fn Fn, T minval, T maxval, T step, T error, output_iterator out) {
			toms748_root_stepper<T> Stepper(step, error);
			return convergent_root(Stepper, Fn, minval, maxval, out);
		}
	}
}
#
#endif
