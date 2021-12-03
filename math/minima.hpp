#ifndef HMLIB_MATH_MINIMA_INC
#define HMLIB_MATH_MINIMA_INC 100
#
#include<array>
#include"../circular.hpp"
#include<boost/math/tools/minima.hpp>
namespace hmLib {
	namespace math {
		template<typename value_type_ =double>
		struct tolerance_breaker{
			using value_type = value_type_;
		public:
			precision_breaker() = delete;
			explict precision_breaker(value_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			precision_breaker(value_type relative_error_, value_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			T precision(T best)const{
				return relerr * std::abs(best) + abserr / 4;
			}
			bool operator()(T lower, T upper)const{
				if((upper - lower) / 2 <= precision((upper + lower) / 2) * 2 )return true;
			}
			bool operator()(T lower, T upper, T best)const{
				if(std::abs(best - (upper + lower) / 2) + (upper - lower) / 2 <= precision(best) * 2 )return true;
			}
		private:
			value_type relerr;
			value_type abserr;
		};
		template<typename T>
		struct brent_minima_stepper{
			struct state{
				friend struct brent_minima_stepper<T>;
			private:
				//min and max value of the searching range.
				T lower;
				T upper;
				T val1;
				T val2;
				T val3;
				T fval1;
				T fval2;
				T fval3;
				T delta;  // The distance moved in the last step
				T pdelta; // The distance moved in the step before last
			public:
				state() = default;
				template<typename fn>
				state(fn f, T lower_, T upper_)
					: lower(lower_)
					, upper(upper_)
					, val1(upper)
					, val2(upper)
					, val3(upper)
					, fval1(f(upper))
					, fval2(f(upper))
					, fval3(f(upper))
					, delta(0)
					, pdelta(0){
				}
				const T& value()const{return val1;}
				const T& fvalue()const{return fval1;}
				const T& lower()const{return lower;}
				const T& upper()const{return upper;}
			};
			brent_minima_stepper():tolerance(1e-6){}
			explicit brent_minima_stepper(T tolerance_):tolerance(tolerance_){}
			template<typename fn>
			state make_state(fn f, T lower, T upper)const{
				return state(f, lower, upper)
			}
			template<typename fn>
			bool operator()(fn f, state& x, T precision){
				static const T golden_ratio = 0.3819660113;

				// midpoint
				T midval = (x.upper + x.lower) / 2;

				//whether we should try parabolic fit or not?
				if(std::abs(x.pdelta) > precision){
					T r = (x.val1 - x.val2) * (x.fval1 - x.fval2);
					T q = (x.val1 - x.val3) * (x.fval1 - x.fval1);
					T p = (x.val1 - x.val3) * q - (x.val1 - x.val2) * r;
					q = 2 * (q - r);

					if(q > 0) p = -p;
					else q = -q;

					// check the accuracy of parabolic fit
					if((std::abs(p) >= std::abs(q * x.pdelta / 2)) || (p <= q * (x.lower - x.val1)) || (p >= q * (x.upper - x.val1))){
						// bad; golden section instead
						x.pdelta = (x.val1 >= midval) ? x.midval - x.val1 : x.upper - x.val1;
						x.delta = golden_ratio * x.pdelta;
					}else{
						// good; paraboratic fit
						x.pdelta = x.delta;
						x.delta = p / q;

						T newval = x.val1 + x.delta;
						if((newval - x.minv) < 2*precision || (x.maxv- newval) < 2*precision){
							x.delta = (midval - x.val1) < 0 ? static_cast<T>(-std::abs(precision)) : static_cast<T>(std::abs(precision));
						}
					}
				}else{
					// golden section
					x.pdelta = (x.val1 >= midval) ? x.lower - x.val1: x.upper - x.val1;
					x.delta = golden_ratio * x.pdelta;
				}

				// okey, let's try another positition.
				T tryval = x.val1;
				if(std::abs(x.delta) >= precision){
					 tryval += x.delta;
				}else if(x.delta > 0){
					tryval += precision;
				}else{
					tryval += -precision;
				}
				T ftryval = f(tryval);

				if(ftryval <= x.fval1){
					//if tried point is the best

					//update min/max by the previous best
					if(tryval >= x.val1) x.lower = x.val1;
					else x.upper = x.val1;

					// update holding points
					x.val3 = x.val2;
					x.val2 = x.val1;
					x.val1 = tryval;
					x.fval3 = x.fval2;
					x.fval2 = x.fval1;
					x.fval1 = ftryval;
				}else{
					//if the tried point is not the best (but should be better than one of vals)
					
					//update min/max by the tryval
					if(tryval < x.val1) x.lower = tryval;
					else x.upper = tryval;

					if((ftryval <= x.fval2) || (x.val2 == x.val1)){
						// tried point is the second best
						x.val3 = x.val2;
						x.val2 = tryval;
						x.fval3 = x.fval2;
						x.fval2 = ftryval;
					} else if((ftryval <= x.fval3) || (x.val3 == x.val1) || (x.val3  == x.val2)){
						// tried point is the third best
						x.val3 = tryval;
						x.fval3 = ftryval;
					}
				}
				return false;
			}
		private:
			T tolerance;
		};
		template<typename T>
		struct golden_section_minima_stepper{
			struct state{
				friend struct brent_minima_stepper<T>;
			private:
				//min and max value of the searching range.
				T lower;
				T upper;
				T val1;
				T val2;
				T val3;
				T fval1;
				T fval2;
				T fval3;
			public:
				state() = default;
				template<typename fn>
				state(fn f, T lower_, T upper_)
					: lower(lower_)
					, upper(upper_)
					, val1(upper)
					, val2(upper)
					, val3(upper)
					, fval1(f(upper))
					, fval2(f(upper))
					, fval3(f(upper)){
				}
				const T& value()const{return val1;}
				const T& fvalue()const{return fval1;}
				const T& lower()const{return lower;}
				const T& upper()const{return upper;}
			};
			golden_section_minima_stepper():tolerance(1e-6){}
			explicit golden_section_minima_stepper(T tolerance_):tolerance(tolerance_){}
			template<typename fn>
			state make_state(fn f, T lower, T upper)const{
				return state(f, lower, upper)
			}
			template<typename fn>
			bool operator()(fn f, state& x, T precision){
				static const T golden_ratio = 0.3819660113;

				// midpoint
				T midval = (x.upper + x.lower) / 2;

				// golden section
				T delta = golden_ratio * ((x.val1 >= midval) ? x.lower - x.val1: x.upper - x.val1);

				// okey, let's try another point.
				T tryval = x.val1;
				if(std::abs(delta) >= precision){
					 tryval += delta;
				}else if(delta > 0){
					tryval += precision;
				}else{
					tryval += -precision;
				}
				T ftryval = f(tryval);

				if(ftryval <= x.fval1){
					//if tried point is the best

					//update min/max by the previous best
					if(tryval >= x.val1) x.lower = x.val1;
					else x.upper = x.val1;

					// update holding points
					x.val3 = x.val2;
					x.val2 = x.val1;
					x.val1 = tryval;
					x.fval3 = x.fval2;
					x.fval2 = x.fval1;
					x.fval1 = ftryval;
				}else{
					//if the tried point is not the best (but should be better than one of vals)
					
					//update min/max by the tryval
					if(tryval < x.val1) x.lower = tryval;
					else x.upper = tryval;

					if((ftryval <= x.fval2) || (x.val2 == x.val1)){
						// tried point is the second best
						x.val3 = x.val2;
						x.val2 = tryval;
						x.fval3 = x.fval2;
						x.fval2 = ftryval;
					} else if((ftryval <= x.fval3) || (x.val3 == x.val1) || (x.val3  == x.val2)){
						// tried point is the third best
						x.val3 = tryval;
						x.fval3 = ftryval;
					}
				}
				return false;
			}
		private:
			T tolerance;
		};

		
		template <class F, class T>
		inline std::pair<T, T> brent_find_minima(F f, T min, T max, int digits){
			boost::uintmax_t m = (std::numeric_limits<boost::uintmax_t>::max)();
			return brent_find_minima(f, min, max, digits, m);
		}


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
					val2 = (start + end) / 2;
					val3 = end;
				}
				circular<ans_type, 3> fval{ Fn(val1),Fn(val2) ,Fn(val3) };

				do {
					if(fval1 >= fval2 && fval2 <= fval3) {
						auto ans = boost::math::tools::brent_find_minima(Fn, val1, val2, bits);
						start = val3;
						startFnVal = fval3;
						return ans;
					}

					if(val3 >= end) {
						break;
					} else if(val2 + step > end) {
						val.rotete_back(end);
						fval.rotate_back(Fn(end));
					} else {
						val.rotete_back(val2 + step);
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
		output_iterator minima(minima_stepper Stepper, fn Fn, T lower, T upper, output_iterator out) {
			auto FnVal = Fn(lower);
			while(lower<upper) {
				auto Ans = Stepper(Fn, lower, FnVal, upper);

				if(Ans.first < upper) {
					*(out++) = (Ans.first + Ans.second) / 2.0;
				}
			}

			return out;
		}
		template<typename fn, typename T, typename output_iterator>
		output_iterator minima_brent(fn Fn, T lower, T upper, T step, int bits, output_iterator out) {
			brent_minima_stepper<T> Stepper(step, bits);
			return minima(Stepper, Fn, lower, upper, out);
		}
	}
}
#
#endif
