#ifndef HMLIB_ODEINT_INTERFERE_FLOATBOUNDARY_INC
#define HMLIB_ODEINT_INTERFERE_FLOATBOUNDARY_INC 100
#
#include<functional>
namespace hmLib {
	namespace odeint {
		template<typename value_type_ = double, typename time_type_ = double>
		struct lower_condition {
			using value_type = value_type_;
			using time_type = time_type_;
		public:
			template<typename fn>
			void operator()(const value_type& x, value_type& dx, time_type t, const fn& dxfn) {
				dx = dxfn()
			}
		};
		template<typename value_type_ = double, typename time_type_ = double>
		struct lower_condition {
			using value_type = value_type_;
			using time_type = time_type_;
		public:
			template<typename fn>
			void operator()(const value_type& x, value_type& dx, time_type t, const fn& dxfn) {
				dx = dxfn()
			}
			void operator()(const state_type& f, state_type& df, time_type t)const {
				auto dff = Sys.df(x, f[0], f[1]);
				df[0] = dff[1];
				df[1] = dff[2];

				if(xzero) {
					df[0] = std::max(df[0], 0.0);
				}
				if(yzero) {
					df[1] = std::max(df[1], 0.0);
				}
				if(xyone) {
					double d = std::sqrt(2.0)/2*df[0] - std::sqrt(2.0)/2*df[1];
					df[0] = d*std::sqrt(2.0)/2;
					df[1] = -d*std::sqrt(2.0)/2;
				}

			}
			interfere_request interfere(const state_type& f, time_type& t, time_type& dt, state_type& nf) {
				if(f[0] < -0.1) {
					return interfere_request::restep;
				}
				if(f[1] < -0.1) {
					return interfere_request::restep;
				}
				if(f[0] + f[1] > 1.1) {
					return interfere_request::restep;
				}

				bool nxzero = false;
				bool nyzero = false;
				bool nxyone = false;
				bool Initialize = false;
				nf = f;
				if(nf[0] <= 0.0) {
					nf[0] = 0.0;

					nxzero = true;
					Initialize = true;
				}
				if(nf[1] <= 0.0) {
					nf[1] = 0.0;

					nyzero = true;
					Initialize = true;
				}
				if(nf[0] + nf[1] >= 1.0) {
					double sum = nf[0] + nf[1];
					nf[0] = nf[0]/sum;
					nf[1] = nf[1]/sum;

					nxyone = true;
					Initialize = true;
				}

				//zero-zero
				if(xzero&nxzero&yzero&nyzero) {
					return interfere_request::terminate;
				}

				//zero-one
				if(xyone&nxyone&yzero&nyzero) {
					return interfere_request::terminate;
				}

				//one-zero
				if(xzero&nxzero&xyone&nxyone) {
					return interfere_request::terminate;
				}
				xzero = nxzero;
				yzero = nyzero;
				xyone = nxyone;

				return Initialize ? interfere_request::initialize : interfere_request::none;
			}

		private:
			double error;
		};

		template<typename condition_, typename state_type_ = double>
		struct float_boundary {
			using state_type = state_type_;
			using condition = condition_;
		private:
			state_type val;
			bool prev;
		public:
			float_boundary(const state_type& val_) :val(val_) {}
			float_boundary(state_type&& val_) :val(val_) {}
		public://for interfere system
			void ready(const state_type& x) { prev = is_violated(x); }
			bool valid_step(const state_type& x)const { return prev || !is_violated(x); }
			bool validate(const state_type& x, state_type& vx) const {
				if (is_violated(x)) {
					vx = val;
					return true;
				}
				return false;
			}
		public:
			bool on_boundary()const { return prev; }
			const state_type& boundary()const { return val; }
		protected:
			//upper:1.0, lower:-1.0
			static constexpr double sign = (condition()(0.0, 1.0) ? 1.0 : -1.0);
			bool is_violated(const state_type& x) const { return condition()(x, val); }
		};
//		using float_upper_boundary = float_boundary< std::greater_equal<double>, double> ;
//		using float_lower_boundary = float_boundary< std::less_equal<double>, double>;

		//This boundary requires that the differential equation dx can return mathematically valid value for both inside and outside of range.
		//	i.e., the function return any value of [numeric_limits<state_type>::lowest(), numeric_limits<state_type>::max()] or inf, -inf.
		//	unnecessary side is not limited.
		template<typename condition_, typename state_type_ = double>
		struct limited_diff_boundary : public float_boundary<condition_, state_type_> {
			using base_type = float_boundary<condition_, state_type_>;
			using state_type = state_type_;
			using condition = typename base_type::condition;
		private:
			state_type approximate_range;
			state_type strength;
		public:
			limited_diff_boundary(state_type val, state_type approximate_range_, state_type strength_)
				: base_type(val), approximate_range(approximate_range_), strength(strength_) {
			}
			template<typename functor>
			state_type operator()(state_type x, functor&& dx) {
				state_type d = dx(x);
				
				//Out of approximation range
				if(!base_type::is_violated(x + base_type::sign*approximate_range))return d;

				//Fix Upper/Lower limit value of diffenrential
				state_type tmp = strength*(approximate_range / (base_type::sign*(x - base_type::boundary()) + approximate_range) - 1);
				if(base_type::sign * d > tmp) {
					d = base_type::sign * tmp;
				}

				return d;
			}
		};
		using limited_diff_upper_boundary = limited_diff_boundary< std::greater_equal<double>, double>;
		using limited_diff_lower_boundary = limited_diff_boundary< std::less_equal<double>, double>;

		//This boundary requires that the differential equation dx can return mathematically valid value for inside of the range.
		template<typename condition_, typename state_type_ = double>
		struct strict_limited_diff_boundary : public float_boundary<condition_, state_type_> {
			using base_type = float_boundary<condition_, state_type_>;
			using state_type = state_type_;
			using condition = typename base_type::condition;
		private:
			static constexpr double sign = (condition()(0.0, 1.0) ? 1.0 : -1.0);
		private:
			state_type approximate_range;
			state_type strength;
		public:
			strict_limited_diff_boundary(state_type val, state_type approximate_range_, state_type strength_)
				: base_type(val), approximate_range(approximate_range_), strength(strength_) {
			}
		private:
			state_type diff_validate(state_type x, state_type d) {
				//Fix Upper/Lower limit value of diffenrential with zero
				state_type tmp1 = strength*(approximate_range / (base_type::sign*(x - base_type::boundary()) + approximate_range) - 1);
				if(base_type::sign * d > tmp1) {
					d = base_type::sign * tmp1;
				}
				//Fix Lower/Upper limit value of diffenrential
				state_type tmp2 = strength*(-approximate_range / (base_type::sign*(x - base_type::boundary()) + approximate_range) - 1);
				if(base_type::sign * d < tmp2) {
					d = base_type::sign * tmp2;
				}

				return d;
			}
		public://for composite system
			template<typename functor>
			state_type operator()(state_type x, functor&& dx) {
				//Outside of approximation range (valid side)
				if(!base_type::is_violated(x + base_type::sign*approximate_range)) {
					return dx(x);
				}

				//Outside of approximation range (invlaid side)
				if(base_type::is_violated(x)) {
					auto d0 = base_type::sign*diff_validate(base_type::boundary(), dx(base_type::boundary()));
					return base_type::sign*strength * (approximate_range * (d0/strength +1) / (base_type::sign*(x - base_type::boundary()) + approximate_range) - 1);
				}

				//Inside of approximation range
				return diff_validate(x, dx(x));
			}
		};
		using strict_limited_diff_upper_boundary = strict_limited_diff_boundary< std::greater_equal<double>, double>;
		using strict_limited_diff_lower_boundary = strict_limited_diff_boundary< std::less_equal<double>, double>;

	}
}
#
#endif
