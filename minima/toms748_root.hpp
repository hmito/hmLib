#ifndef HMLIB_MINIMA_TOMS748ROOT_INC
#define HMLIB_MINIMA_TOMS748ROOT_INC 100
#
#include<utility>
#include "../exceptions.hpp"
#include "../math/math.hpp"
#include "../math/sign.hpp"
namespace hmLib{
	namespace minima{
		template<typename T>
		struct vfpair{
			using value_type = T;
			T v;
			T f;
		public:
			vfpair()=default;
			explict vfpair(T x_):x(x_),f(){}
			vfpair(T x_, T f_):x(x_),f(f_){}
			template<typename F>
			vfpair<T>& eval(F fn){f=fn(v); return *this;}
		};
		template<typename F>
		auto make_vfpair(T x, F fn){
			return vfpair<decltype(x * fn(x))>(x,fn(x));
		}
		namespace detail{
			template <typename value_type>
			inline value_type bisect_interpolate(const vfpair<value_type>& a, const vfpair<value_type>& b){
				return a.v + (b.v - a.v) / 2;
			}
			template <typename value_type>
			inline value_type secant_interpolate(const vfpair<value_type>& a, const vfpair<value_type>& b){
				using std::abs;

				//secant calculation
				auto x = a.v - (a.f / (b.f - a.f)) * (b.v - a.v);

				auto tol = std::numeric_limits<value_type>::epsilon() * 5;
				if((a.v + abs(a.v) * tol.v < x) && (x < b.v - abs(b.v) * tol)) return x;
				
				//secant faild; return bisect
				return bisect_interpolate(a,b);
			}
			template<typename value_type>
			inline value_type double_secant_interpolate(const vfpair<value_type>& a, const vfpair<value_type>& b){
				using std::abs;
				if(std::abs(a.f) < std::abs(b.f)){
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
			inline value_type quadratic_interpolate(const vfpair<value_type>& a, const vfpair<value_type>& b, const vfpair<value_type>& c, unsigned int count){
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
			inline value_type cubic_interpolate(const vfpair<value_type>& a,const vfpair<value_type>& b,const vfpair<value_type>& c, const vfpair<value_type>& d, unsigned int count){
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

				value_type x = q31 + q32 + q33 + a;

				if(a.v < x && x < b.v) return x;

				//cubic failed; try quadratic
				return quadratic_interpolate(a, b, c, count);
			}
		} 
		
		template<typename T>
		struct range_state{
			vfpair<T> lower;
			vfpair<T> upper;
			template<typename T>
			state(F fn, T lowerval, T upperval):lower(lowerval),upper(upperval){
				hmLib_assert(lower.v < upper.v, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748::state require lower < upper.");

				lower.eval(fn);
				upper.eval(fn);

				hmLib_assert(hmLib::math::sign(upper.f) * hmLib::math::sign(lower.f) == hmLib::math::sign::negative, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748::state require fn(lower) * fn(upper) < 0.");

				if(lower.f == 0 || upper.f == 0){
					if(lower.f == 0)upper = lower;
					else if(upper.f == 0)lower = upper;
				}
			}
		};
		template<typename T>
		struct toms748_stepper{
			using pair = vfpair<T>;
			using state =range_state<T>
		public:
			toms748_stepper():Stage(0){}
			template<typename F>
			void initialize(F, state&)noexcept{
				Stage = 0;
			}
			template<typename F>
			void operator()(F fn, state& x){
				switch(Stage++){
				case 1:
					bracket(x, quadratic_interpolate(x.lower,x.upper,p,2), fn, true);
					break;
				case 2:
					Dist = x.upper.v - x.lower.v;			
					bracket(x, cubic_interpolate(x.lower,x.upper,p,q,2),fn, true);
				case 3:
					bracket(x, cubic_interpolate(x.lower,x.upper,p,q,3), fn, false);
					break;
				case 4:
					bracket(x, double_secant_interpolate(x.lower,x.upper),fn,true);
					break;
				case 5:
					if(x.upper.v - x.lower.v >= 0.5*Dist){
						// If convergence of above three processes are worse than bisect method, just try bisect
						bracket(x, bisect_interpolate(x.lower,x.upper),fn,true);
						Stage = 2;
					}else{
						//otherwise just try additional interpolate
						Dist = x.upper.v - x.lower.v;			
						bracket(x, cubic_interpolate(x.lower,x.upper,2),fn, true);
						Stage = 3;
					}
					break;
				default://start from here
					bracket(x, secant_interpolate(x.lower,x.upper), fn, false);
					Stage = 1; //reset stage number
					break;
				}
			}
		private:
			template<typename F>
			void bracket(state& x, T z, F fn, bool cube_update){
				if(cube_update) q = p;
					
				auto tol = std::numeric_limits<T>::epsilon() * 2;
				if((x.upper.v - x.lower.v) < 2 * tol * x.lower.v){
					z = detail::bisect_interpolate(x.lower,x.upper);
				}else if(z <= x.lower.a + std::abs(x.lower.v) * tol){
					z = x.lower.v + std::abs(x.lower.v) * tol;
				}else if(z >= x.upper.b - std::abs(x.upper.v) * tol){
					z = x.upper.v - std::abs(x.upper.v) * tol;
				}

				auto fz = fn(z);

				if(fz == 0){
					p = pair(0,0);
					x.lower = pair(z,fz);
					x.upper = pair(z,fz);
				}else if(hmLib::math::sign(a.f)*hmLib::math::sign(fx) == hmLib::math::sign::negative){
					p = x.upper;
					x.upper = pair(z,fz);
				}else{
					p = x.lower;
					x.lower = pair(z,fz);
				}
			}
		private:
			int Stage;
			T Dist;
			vfpair<T> p;
			vfpair<T> q;
		};
	}
}
#
#endif
