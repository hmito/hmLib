#ifndef HMLIB_MATH_DIFFERENTIATION_INC
#define HMLIB_MATH_DIFFERENTIATION_INC 100
#
namespace hmLib {
	namespace math {
		//both first and second order derivatives
		template<typename func_type, typename real_type>
		void derivative(func_type fn, real_type x, real_type abserr, real_type relerr,
			real_type& dx, real_type& dx2) {
			auto hx = x * relerr + abserr;

			auto v0 = fn(x);
			auto vP = fn(x + hx);
			auto vN = fn(x - hx);

			dx = (vP - vN) / 2 / hx;
			dx2 = (vP + vN - 2 * v0) / hx / hx;
		}
		//both first and second order derivatives
		template<typename func_type, typename real_type>
		void derivative(func_type fn, real_type x, real_type abserr, real_type relerr,
			real_type& dx) {
			auto hx = x * relerr + abserr;

			auto vP = fn(x + hx);
			auto vN = fn(x - hx);

			dx = (vP - vN) / 2 / hx;
		}
		//both first and second order derivatives
		template<typename plane_func_type, typename real_type>
		void plane_derivative(plane_func_type fn, real_type x, real_type y, real_type abserr, real_type relerr,
			real_type& dx, real_type& dy, real_type& dx2, real_type& dxdy, real_type& dy2) {
			auto hx = x * relerr + abserr;
			auto hy = y * relerr + abserr;

			auto v00 = fn(x, y);
			auto vP0 = fn(x + hx, y);
			auto vN0 = fn(x - hx, y);
			auto v0P = fn(x, y + hy);
			auto vPP = fn(x + hx, y + hy);
			auto vNP = fn(x - hx, y + hy);
			auto v0N = fn(x, y - hy);
			auto vPN = fn(x + hx, y - hy);
			auto vNN = fn(x - hx, y - hy);

			dx = (vP0 - vN0) / 2 / hx;
			dy = (v0P - v0N) / 2 / hy;

			dx2 = (vP0 + vN0 - 2 * v00) / hx / hx;
			dy2 = (v0P + v0N - 2 * v00) / hy / hy;
			dxdy = (vPP + vNN - vPN - vNP) / 4 / hx / hy;
		}
		//only first order derivatives
		template<typename plane_func_type, typename real_type>
		void plane_derivative(plane_func_type fn, real_type x, real_type y, real_type abserr, real_type relerr,
			real_type& dx, real_type& dy) {
			auto hx = x * relerr + abserr;
			auto hy = y * relerr + abserr;

			auto vP0 = fn(x + hx, y);
			auto vN0 = fn(x - hx, y);
			auto v0P = fn(x, y + hy);
			auto v0N = fn(x, y - hy);

			dx = (vP0 - vN0) / 2 / hx;
			dy = (v0P - v0N) / 2 / hy;
		}

	}
}
#
#endif
