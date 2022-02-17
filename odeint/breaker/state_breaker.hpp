#ifndef HMLIB_ODEINT_STATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_STATEBREAKOBSERVER_INC 100
#
#include <array>
#include "../utility.hpp"
#include "../../geometry.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_type_, unsigned int container_size_>
		struct converging_steps_breaker {
			using state = state_type_;
		private:
			std::array<state, container_size_>;
			unsigned int Cnt;
			unsigned int Interval;
			state x1;
			state x2;
			double dis1;
			double dis2;
			double dis1L;
			double converging_rate;
		public:
			converging_steps_breaker(unsigned int Interval_, state_range_accessor Accessor_, double Tolerance_) :Cnt(0), Interval(Interval_), Accessor(Accessor_), Tolerance(Tolerance_){}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				dis1 += hmLib::odeint::distance_norm_inf(x, x1); 
				dis2 += hmLib::odeint::distance_norm_inf(x, x2);

				if(Cnt==Interval){
					if(dis1L*converging_rate > dis2 & dis1*converging_rate > dis2)return true;
					dis1L = dis1;
					x2 = std::move(x1);
					x1 = x;
					Cnt = 0;
				}

				++Cnt;
				return false;
			}
		};
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
		public:
			cross_state_breaker(unsigned int Interval_, state_range_accessor Accessor_) :Cnt(0), Interval(Interval_), Accessor(Accessor_){}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if (++Cnt < Interval)return false;
				Cnt = 0;

				auto Range = Accessor();
				auto Last = std::next(std::begin(Range), std::distance(std::begin(), std::end()) - 1);
				auto Prev = std::begin(Range);
				auto s1 = hmLib::plane_geometry::make_semgnet(hmLib::plane_geometry::make_point(x), hmLib::plane_geometry::make_point(*Last));
				for (auto Itr = std::next(Prev); Prev != Last; ++Itr, ++Prev) {
					auto s2 = hmLib::plane_geometry::make_semgnet(hmLib::plane_geometry::make_point(*Itr), hmLib::plane_geometry::make_point(*Prev));

					if(hmLib::plane_geometry::is_cross(s1, s2))return true;
				}
				return false;
			}
		};
	}
}
#
#endif
