#ifndef HMLIB_ODEINT_INTERFERE_BOUNDARY_INC
#
namespace hmLib {
	namespace odeint {
		

		template<typename state_type_>
		struct sigmoid_boundary_diff{
			template<typename functor>
			double sigmoid_zero_boundary(double x, functor dx, double boundary_error, double abs_error) {
				if (boundary_error < x)return dx(x);

				double v = dx(boundary_error);
				double dv = (dx(boundary_error + abs_error) - dx(boundary_error - abs_error)) / (2 * abs_error);

				double vk = v * 2;
				double va = dv * 4 / (vk + std::numeric_limits<double>::min());

				if (v>0) {
					//				double ans = vk / (1 + std::exp(-va*(x - boundary_error)));
					return vk / (1 + std::exp(-va*(x - boundary_error)));
				}

				double ba = std::log(1 / boundary_error + 1) / boundary_error;
				double bk = -(vk / (1 + std::exp(-va*(0.0 - boundary_error)))) / (0.5 - (1 / (1 + std::exp(ba*(boundary_error)))));
				double bv = -bk / (1 + std::exp(ba*boundary_error));

				//			double ans = vk / (1 + std::exp(-va*(x - boundary_error))) + bk / (1 + std::exp(ba*x)) + bv;

				return vk / (1 + std::exp(-va*(x - boundary_error))) + bk / (1 + std::exp(ba*x)) + bv;
			}
		};
		struct float_lower_boundary {
			using state_type = double;
		private:
			state_type val;
			bool prev;
		public:
			float_lower_boundary(const state_type& val_) :val(val_) {}
			float_lower_boundary(state_type&& val_) :val(val_) {}
		public:
			void ready(const state_type& x) { prev = check(x); }
			bool valid_step(const state_type& x)const { return prev || !check(x); }
			bool validate(const state_type& x, state_type& vx) const {
				if (check(x)) {
					vx = val;
					return true;
				}
				return false;
			}
		private:
			bool check(const state_type& x) const { return val >= x; }
		};

		template<typename boundary_type_>
		struct boundary {
			using state_type = state_type_;
			using time_type = time_type_;
		private:
			boundary_category bcat;
		public:
			void ready(const state_type& x) {
				bcat.ready(x);
			}
			bool valid_step(const state_type& x) {
				return bcat.valid_step(x, t);
			}
			bool validate(const state_type& x, state_type& vx) {

			}
			bool on_boundary() {

			}
			template<typename fn>
			void operator()(const state_type& x, state_type& dx, fn&& Fn) {

			}
		};
	}
}
#
#endif
