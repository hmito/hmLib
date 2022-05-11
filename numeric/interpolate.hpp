#ifndef HMLIB_NUMERIC_INTERPOLATE_INC
#define HMLIB_NUMERIC_INTERPOLATE_INC 100
#
#include "evalpair.hpp"
namespace hmLib{
	namespace numeric{
		namespace detail{
			template <typename value_type,typename evalue_type>
			inline value_type bisect_interpolate(const evalpair<value_type,evalue_type>& a, const evalpair<value_type,evalue_type>& b){
				return a.v + (b.v - a.v) / 2;
			}
			template <typename value_type,typename evalue_type>
			inline value_type secant_interpolate(const evalpair<value_type,evalue_type>& a, const evalpair<value_type,evalue_type>& b){
				using std::abs;

				//secant calculation
				auto x = a.v - (a.e / (b.e - a.e)) * (b.v - a.v);

				auto tol = std::numeric_limits<value_type>::epsilon() * 5;
				if((a.v + abs(a.v) * tol.v < x) && (x < b.v - abs(b.v) * tol)) return x;
				
				//secant faild; return bisect
				return bisect_interpolate(a,b);
			}
			template <typename value_type,typename evalue_type>
			inline value_type double_secant_interpolate(const evalpair<value_type,evalue_type>& a, const evalpair<value_type,evalue_type>& b){
				using std::abs;
				if(abs(a.e) < abs(b.e)){
					auto z = a.v - 2 * (a.e / (b.e - a.e)) * (b.v - a.v);
					if(abs(z - a.v) < (b.v - a.v) / 2)return z;
				}else{
					auto z = b.v - 2 * (b.e / (b.e - a.e)) * (b.v - a.v);
					if(abs(z - b.v) < (b.v - a.v) / 2)return z;
				}

				//double secant faild; return bisect
				return bisect_interpolate(a,b);
			}
			template <typename value_type,typename evalue_type>
			inline value_type quadratic_interpolate(const evalpair<value_type,evalue_type>& a, const evalpair<value_type,evalue_type>& b, const evalpair<value_type,evalue_type>& c, unsigned int count){
				//coeffcients of quadratic
				auto B = div_or<value_type>(b.e - a.e, b.v - a.v, std::numeric_limits<value_type>::max());
				auto C = div_or<value_type>(c.e - b.e, c.v - b.v, std::numeric_limits<value_type>::max());
				auto A = div_or<value_type>(B - C, c.v - a.v, value_type(0));

				if(A == 0){
					//cannot use quadratic; try secant
					return secant_interpolate(a, b);
				}

				value_type x;
				if(hmLib::math::sign(A) * hmLib::math::sign(a.e) == hmLib::math::sign::positive){
					x = a.v;
				}else{
					x = b.v;
				}

				// Newton steps
				for(unsigned i = 0; i < count; ++i){
					x -= div_or<value_type>(a.e+(B+A*(x-b.v))*(x-a.v), B + A * (2 * x - a.v - b.v), 1 + x - a.v);
				}

				if(a.v < x && x < b.v) return x;

				//quadratic faild; try secant
				return secant_interpolate(a, b);
			}
			template <typename value_type,typename evalue_type>
			inline value_type cubic_interpolate(const evalpair<value_type,evalue_type>& a,const evalpair<value_type,evalue_type>& b,const evalpair<value_type,evalue_type>& c, const evalpair<value_type,evalue_type>& d, unsigned int count){
				auto min_diff = std::numeric_limits<value_type>::min() * 32;
				bool nocubic = (std::abs(a.e - b.e) < min_diff) || (std::abs(a.e - c.e) < min_diff) || (std::abs(a.e - d.e) < min_diff) || (std::abs(b.e - c.e) < min_diff) || (std::abs(b.e - d.e) < min_diff) || (std::abs(c.e - d.e) < min_diff);

				if(nocubic){
					//cannot use cubic; try quadratic
					return quadratic_interpolate(a, b, c, count);
				}

				//coeffcients of cubic
				value_type q11 = (c.v - d.v) * c.e / (d.e - c.e);
				value_type q21 = (b.v - c.v) * b.e / (c.e - b.e);
				value_type q31 = (a.v - b.v) * a.e / (b.e - a.e);
				value_type d21 = (b.v - c.v) * c.e / (c.e - b.e);
				value_type d31 = (a.v - b.v) * b.e / (b.e - a.e);

				value_type q22 = (d21 - q11) * b.e / (d.e - b.e);
				value_type q32 = (d31 - q21) * a.e / (c.e - a.e);
				value_type d32 = (d31 - q21) * c.e / (c.e - a.e);
				value_type q33 = (d32 - q22) * a.e / (d.e - a.e);

				value_type x = q31 + q32 + q33 + a;

				if(a.v < x && x < b.v) return x;

				//cubic failed; try quadratic
				return quadratic_interpolate(a, b, c, count);
			}
		} 
	}
}
#
#endif
