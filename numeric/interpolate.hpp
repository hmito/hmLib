#ifndef HMLIB_NUMERIC_INTERPOLATE_INC
#define HMLIB_NUMERIC_INTERPOLATE_INC 100
#
#include "evalpair.hpp"
namespace hmLib{
	namespace numeric{
		namespace detail{
			template <typename value_type>
			inline value_type bisect_interpolate(const evalpair<value_type>& a, const evalpair<value_type>& b){
				return a.v + (b.v - a.v) / 2;
			}
			template <typename value_type>
			inline value_type secant_interpolate(const evalpair<value_type>& a, const evalpair<value_type>& b){
				using std::abs;

				//secant calculation
				auto x = a.v - (a.f / (b.f - a.f)) * (b.v - a.v);

				auto tol = std::numeric_limits<value_type>::epsilon() * 5;
				if((a.v + abs(a.v) * tol < x) && (x < b.v - abs(b.v) * tol)) return x;
				
				//secant faild; return bisect
				return bisect_interpolate(a,b);
			}
			template<typename value_type>
			inline value_type double_secant_interpolate(const evalpair<value_type>& a, const evalpair<value_type>& b){
				using std::abs;
				if(abs(a.f) < abs(b.f)){
					auto z = a.v - 2 * (a.f / (b.f - a.f)) * (b.v - a.v);
					if(abs(z - a.v) < (b.v - a.v) / 2)return z;
				}else{
					auto z = b.v - 2 * (b.f / (b.f - a.f)) * (b.v - a.v);
					if(abs(z - b.v) < (b.v - a.v) / 2)return z;
				}

				//double secant faild; return bisect
				return bisect_interpolate(a,b);
			}
			template <typename value_type>
			inline value_type quadratic_interpolate(const evalpair<value_type>& a, const evalpair<value_type>& b, const evalpair<value_type>& c, unsigned int count){
				//coeffcients of quadratic
				auto B = div_or<value_type>(b.f - a.f, b.v - a.v, std::numeric_limits<value_type>::max());
				auto C = div_or<value_type>(c.f - b.f, c.v - b.v, std::numeric_limits<value_type>::max());
				auto A = div_or<value_type>(B - C, c.v - a.v, value_type(0));

				if(A == 0){
					//cannot use quadratic; try secant
					return secant_interpolate(a, b);
				}

				value_type x;
				if(hmLib::math::sign(A) * hmLib::math::sign(a.f) == hmLib::math::sign::positive){
					x = a.v;
				}else{
					x = b.v;
				}

				// Newton steps
				for(unsigned i = 0; i < count; ++i){
					x -= div_or<value_type>(a.f+(B+A*(x-b.v))*(x-a.v), B + A * (2 * x - a.v - b.v), 1 + x - a.v);
				}

				if(a.v < x && x < b.v) return x;

				//quadratic faild; try secant
				return secant_interpolate(a, b);
			}
			template <typename value_type>
			inline value_type cubic_interpolate(const evalpair<value_type>& a,const evalpair<value_type>& b,const evalpair<value_type>& c, const evalpair<value_type>& d, unsigned int count){
				auto min_diff = std::numeric_limits<value_type>::min() * 32;
				bool nocubic = (std::abs(a.f - b.f) < min_diff) || (std::abs(a.f - c.f) < min_diff) || (std::abs(a.f - d.f) < min_diff) || (std::abs(b.f - c.f) < min_diff) || (std::abs(b.f - d.f) < min_diff) || (std::abs(c.f - d.f) < min_diff);

				if(nocubic){
					//cannot use cubic; try quadratic
					return quadratic_interpolate(a, b, c, count);
				}

				//coeffcients of cubic
				value_type q11 = (c.v - d.v) * c.f / (d.f - c.f);
				value_type q21 = (b.v - c.v) * b.f / (c.f - b.f);
				value_type q31 = (a.v - b.v) * a.f / (b.f - a.f);
				value_type d21 = (b.v - c.v) * c.f / (c.f - b.f);
				value_type d31 = (a.v - b.v) * b.f / (b.f - a.f);

				value_type q22 = (d21 - q11) * b.f / (d.f - b.f);
				value_type q32 = (d31 - q21) * a.f / (c.f - a.f);
				value_type d32 = (d31 - q21) * c.f / (c.f - a.f);
				value_type q33 = (d32 - q22) * a.f / (d.f - a.f);

				value_type x = q31 + q32 + q33 + a.v;

				if(a.v < x && x < b.v) return x;

				//cubic failed; try quadratic
				return quadratic_interpolate(a, b, c, count);
			}
		} 
	}
}
#
#endif
