#ifndef HMLIB_ODEINT_VALIDATE_DEFAULTSTEPVALIDATOR_INC
#define HMLIB_ODEINT_VALIDATE_DEFAULTSTEPVALIDATOR_INC 100
#
#include<cmath>
#include"../utility.hpp"
namespace hmLib {
	namespace odeint{
		template<typename state_type_, typename time_type_, typename algebra_type_, typename operations_type_>
		struct default_step_validator {
			using state_type = state_type_;
			using time_type = time_type_;
			using algebra_type = algebra_type_;
			using operations_type = operations_type_;
		public:
			default_step_validator(double seach_distance_, double error_distance_, time_type error_dt_, unsigned int max_try_num_)
				: seach_distance(seach_distance_)
				, error_distance(error_distance_)
				, error_dt(error_dt_)
				, max_try_num(max_try_num_)
				, try_cnt(0)
				, cur_dt(error_dt * 1000) {
			}
			void setup_dt(const state_type& x1, time_type t1, const state_type& x2, time_type t2) {
				cur_dt = 0.9 * (t2 - t1) / (seach_distance / 10 + detail::maximum_absolute_error<state_type, algebra_type, operations_type>(x1, x2)) * seach_distance;
				cur_dt = std::max(cur_dt, (t2 - t1) / 10000);
			}
			time_type dt()const { return cur_dt; }
			bool operator()(const state_type & x1, time_type t1, const state_type & x2, time_type t2) {
				return (++try_cnt > max_try_num || t1 == t2 ||
					(detail::maximum_absolute_error<state_type, algebra_type, operations_type>(x1, x2) < error_distance && std::abs(t1 - t2) < error_dt)
				);
			}
			void reset() { try_cnt = 0; }
		private:
			double seach_distance;
			double error_distance;
			time_type error_dt;
			unsigned int max_try_num;
			unsigned int try_cnt;
			time_type cur_dt;
		};
	}
}

#
#endif
