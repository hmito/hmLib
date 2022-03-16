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
		
		struct toms748_stepper{
			template<typename value_type>
			struct state{
			private:
				vfpair<value_type> a;
				vfpair<value_type> b;
				vfpair<value_type> p;
				vfpair<value_type> q;
			public:
				template<typename T>
				state(F fn, T lower, T upper){
					hmLib_assert(lower < upper, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748::state require lower < upper.");

					a = make_vfpair(lower, fn);
					b = make_vfpair(upper, fn);

					hmLib_assert(hmLib::math::sign(a.f) * hmLib::math::sign(b.f) == hmLib::math::sign::negative, hmLib::numeric_exceptions::incorrect_arithmetic_request, "toms748::state require fn(lower) * fn(upper) < 0.");

					if(a.f == 0 || b.f == 0){
						if(a.f == 0)b = a;
						else if(b.f == 0)a = b;
						return;
					}

					// dummy value for d.f, e and e.f:
					e.f = e.v = d.f = 1e5F;

					bracket(secant_interpolate(), fn, false);
					if(count && (a.f != 0) && !tol(a, b)){
						bracket(quadratic_interpolate(2), fn, true);
					}
				}
				template<typename state_value_type, typename value_type, typename fn>
				void bracket(value_type x, fn F, bool cube_update){
					if(cube_update) q = p
						
					auto tol = std::numeric_limits<T>::epsilon() * 2;

					if((b.v - a.v) < 2 * tol * a.v){
						x = a.v + (s.b.v - a.v) / 2;
					}else if(x <= s.a + std::abs(a.v) * tol){
						x = a.v + std::abs(a.v) * tol;
					}else if(x >= s.b - std::abs(b.v) * tol){
						x = b.v - std::abs(b.v) * tol;
					}

					auto fx = F(x);

					if(fx == 0){
						a = typename state<state_value_type>::mypair(x,fx);
						p = typename state<state_value_type>::mypair(0,0);
					}else if(hmLib::math::sign(a.f)*hmLib::math::sign(fx) == hmLib::math::sign::negative){
						p = b;
						b = typename state<state_value_type>::mypair(x,fx);
					}else{
						p = a;
						a = typename state<state_value_type>::mypair(x,fx);
					}
				}
			};
		public:
			template<typename F, typename T>
			auto make_state(F fn, T lower, T upper){
				return state<decltype(lower * fn(lower)>(fn,lower,upper);
			}
			template<typename value_type>
			void operator()(F fn, state<value_type>& x){
				// save our brackets:
				auto dist = x.b - x.a;
				
				// re-bracket, and check for termination:
				x.bracket(x.interpolate(2),fn, true);
				if((a.f == 0) || tol(a, b))break;

				// Bracket again, and check termination condition, update e:
				x.bracket(x.interpolate(3), fn, false);
				if((a.f == 0) || tol(a, b))break;

				// Now we take a double-length secant step:
				x.bracket(x.double_secant_interpolate(),fn,true);

				// If convergence of above three processes are worse than bisect method, just try bisect
				if((b.v - a.v) >= 0.5*dist){
					x.bracket(x.bisect_interpolate(),fn,true);
				}
			}
		};

		// namespace detail
		template <class F, class T, class Tol, class Policy>
		std::pair<T, T> toms748_solve(F f, const T& ax, const T& bx, const T& fax, const T& fbx, Tol tol, boost::uintmax_t& max_iter, const Policy& pol){
			//
			// Sanity check - are we allowed to iterate at all?
			//
			if (max_iter == 0)return std::make_pair(ax, bx);

			boost::uintmax_t count = max_iter;
			T a, b, a.f, b.f, c, u, fu, a0, b0, d, d.f, e, e.f;
			static const T mu = 0.5f;

			// initialise a, b and a.f, b.f:
			a = ax;
			b = bx;
			if(a >= b)return policies::raise_domain_error("Parameters a and b out of order: a=%1%", a, pol));
			a.f = fax;
			b.f = fbx;

			if(tol(a, b) || (a.f == 0) || (b.f == 0)){
				max_iter = 0;
				if(a.f == 0)b = a;
				else if(b.f == 0)a = b;
				return std::make_pair(a, b);
			}

			if(boost::math::sign(a.f) * boost::math::sign(b.f) > 0)return policies::raise_domain_error("Parameters a and b do not bracket the root: a=%1%", a, pol));
			
			// dummy value for d.f, e and e.f:
			e.f = e = d.f = 1e5F;

			if(a.f != 0){
				//
				// On the first step we take a secant step:
				//
				c = detail::secant_interpolate(a, b, a.f, b.f);
				detail::bracket(f, a, b, c, a.f, b.f, d, d.f);
				--count;

				if(count && (a.f != 0) && !tol(a, b)){
					//
					// On the second step we take a quadratic interpolation:
					//
					c = detail::quadratic_interpolate(a, b, d, a.f, b.f, d.f, 2);
					e = d;
					e.f = d.f;
					detail::bracket(f, a, b, c, a.f, b.f, d, d.f);
					--count;
				}
			}

			while(count && (a.f != 0) && !tol(a, b)){
				// save our brackets:
				a0 = a;
				b0 = b;
				
				//
				// Starting with the third step taken
				// we can use either quadratic or cubic interpolation.
				// Cubic interpolation requires that all four function values
				// a.f, b.f, d.f, and e.f are distinct, should that not be the case
				// then variable prof will get set to true, and we'll end up
				// taking a quadratic step instead.
				//
				T min_diff = tools::min_value<T>() * 32;
				bool prof = (fabs(a.f - b.f) < min_diff) || (fabs(a.f - d.f) < min_diff) || (fabs(a.f - e.f) < min_diff) || (fabs(b.f - d.f) < min_diff) || (fabs(b.f - e.f) < min_diff) || (fabs(d.f - e.f) < min_diff);
				if(prof){
					c = detail::quadratic_interpolate(a, b, d, a.f, b.f, d.f, 2);
					//("Can't take cubic step!!!!");
				}else{
					c = detail::cubic_interpolate(a, b, d, e, a.f, b.f, d.f, e.f);
				}
				//
				// re-bracket, and check for termination:
				//
				e = d;
				e.f = d.f;
				detail::bracket(f, a, b, c, a.f, b.f, d, d.f);
				if((0 == --count) || (a.f == 0) || tol(a, b))break;

				//
				// Now another interpolated step:
				//
				prof = (fabs(a.f - b.f) < min_diff) || (fabs(a.f - d.f) < min_diff) || (fabs(a.f - e.f) < min_diff) || (fabs(b.f - d.f) < min_diff) || (fabs(b.f - e.f) < min_diff) || (fabs(d.f - e.f) < min_diff);
				if(prof){
					c = detail::quadratic_interpolate(a, b, d, a.f, b.f, d.f, 3);
					//("Can't take cubic step!!!!");
				}else{
					c = detail::cubic_interpolate(a, b, d, e, a.f, b.f, d.f, e.f);
				}

				//
				// Bracket again, and check termination condition, update e:
				//
				detail::bracket(f, a, b, c, a.f, b.f, d, d.f);
				if((0 == --count) || (a.f == 0) || tol(a, b))break;

				//
				// Now we take a double-length secant step:
				//
				if(fabs(a.f) < fabs(b.f)){
					u = a;
					fu = a.f;
				}else{
					u = b;
					fu = b.f;
				}

				c = u - 2 * (fu / (b.f - a.f)) * (b - a);
				if(fabs(c - u) > (b - a) / 2){
					c = a + (b - a) / 2;
				}

				//
				// Bracket again, and check termination condition:
				//
				e = d;
				e.f = d.f;

				detail::bracket(f, a, b, c, a.f, b.f, d, d.f);

				if((0 == --count) || (a.f == 0) || tol(a, b))break;
				//
				// And finally... check to see if an additional bisection step is 
				// to be taken, we do this if we're not converging fast enough:
				//
				if((b - a) < mu * (b0 - a0))continue;
				//
				// bracket again on a bisection:
				//
				e = d;
				e.f = d.f;
				detail::bracket(f, a, b, T(a + (b - a) / 2), a.f, b.f, d, d.f);
				--count;
			} // while loop

			max_iter -= count;
			if(a.f == 0){
				b = a;
			}else if(b.f == 0){
				a = b;
			}

			return std::make_pair(a, b);
		}
	}
}
#
#endif
