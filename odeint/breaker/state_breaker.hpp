#ifndef HMLIB_ODEINT_STATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_STATEBREAKOBSERVER_INC 100
#
#include <array>
#include "../utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_range_accessor_>
		struct equal_state_breaker {
		public:
			using state_range_accessor = state_range_accessor_;
		private:
			unsigned int Cnt;
			unsigned int Interval;
			state_range_accessor Accessor;
			double Tolerance;
		public:
			equal_state_breaker(unsigned int Interval_, state_range_accessor Accessor_, double Tolerance_) :Cnt(0), Interval(Interval_), Accessor(Accessor_), Tolerance(Tolerance_){}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if (++Cnt < Interval)return false;
				Cnt = 0;

				auto Range = Accessor();
				for (const auto& sx: Range) {
					if (hmLib::odeint::distance_norm_inf(x, sx) < Tolerance) {
						return true;
					}
				}

				return false;
			}
		};
		template<typename state_range_accessor_>
		struct cross_state_breaker {
		public:
			using state_range_accessor = state_range_accessor_;
		private:
			unsigned int Cnt;
			unsigned int Interval;
			state_range_accessor Accessor;
		private:
			template<typename state>
			bool is_cross_segment(const state& a1, const state& a2, const state& b1, const state& b2) {
				if (a1[0] >= a2[0]) {
					if ((a1[0] < b1[0] && a1[0] < b2[0]) || (a2[0] > b1[0] && a2[0] > b2[0])) {
						return false;
					}
				}
				else {
					if ((a2[0] < b1[0] && a2[0] < b2[0]) || (a1[0] > b1[0] && a1[0] > b2[0])) {
						return false;
					}
				}
				if (a1[1] >= a2[1]) {
					if ((a1[1] < b1[1] && a1[1] < b2[1]) || (a2[1] > b1[1] && a2[1] > b2[1])) {
						return false;
					}
				}
				else {
					if ((a2[1] < b1[1] && a2[1] < b2[1]) || (a1[1] > b1[1] && a1[1] > b2[1])) {
						return false;
					}
				}

				if (((a1[0] - a2[0]) * (b1[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b1[0])) *
					((a1[0] - a2[0]) * (b2[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b2[0])) > 0) {
					return false;
				}
				if (((b1[0] - b2[0]) * (a1[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a1[0])) *
					((b1[0] - b2[0]) * (a2[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a2[0])) > 0) {
					return false;
				}
				return true;
			}
		public:
			cross_state_breaker(unsigned int Interval_, state_range_accessor Accessor_) :Cnt(0), Interval(Interval_), Accessor(Accessor_){}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if (++Cnt < Interval)return false;
				Cnt = 0;

				auto Range = Accessor();
				for (const auto& sx : Range) {
					if (hmLib::odeint::distance_norm_inf(x, sx) < Tolerance) {
						return true;
					}
				}
				for (; Beg != End; ++Beg) {
					if (is_cross_segment(x, y, p, Beg->second))return true;
					p = Beg->second;
				}
				return false;
			}
		};
	}
}
#
#endif
