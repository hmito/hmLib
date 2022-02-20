#ifndef HMLIB_ODEINT_STATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_STATEBREAKOBSERVER_INC 100
#
#include <vector>
#include "../../statistics/linear_regression.hpp"
#include "../../circular.hpp"
#include "../../geometry.hpp"
#include "../../iterators/integer_iterator.hpp"
#include "../utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_range_accessor_>
		struct converging_step_breaker {
		public:
			using state_range_accessor = state_range_accessor_;
		private:
			state_range_accessor accessor;
			double distance_torelance;
			double slope_torelance;
		public:
			converging_step_breaker(state_range_accessor accessor_, double distance_torelance_, double slope_torelance_) 
				: accessor(std::move(accessor_))
				, distance_torelance(distance_torelance_)
				, slope_torelance(slope_torelance_){
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				auto Range = accessor();
				if (std::distance(Range.begin(), Range.end()) <= 1)return false;

				std::vector<double> Nrm;
				for(const auto& px: Range){
					auto dis = hmLib::odeint::distance_norm_inf(x, px);
					if (dis >= distance_torelance)return false;
					Nrm.push_back(dis);
				}

				auto xrange = hmLib::make_integer_range<std::size_t>(0,Nrm.size());
				auto prm = hmLib::statistics::linaer_regression(xrange.begin(),xrange.end(),Nrm.begin());

				return (std::abs(prm.first) < slope_torelance);
			}
		};
		template<typename state_range_accessor_>
		auto make_converging_step_breaker(state_range_accessor_ accessor_, double distance_torelance_, double slope_torelance_) {
			return converging_step_breaker<state_range_accessor_>(accessor_, distance_torelance_, slope_torelance_);
		}
		template<typename state_range_accessor_>
		struct equal_state_breaker {
		public:
			using state_range_accessor = state_range_accessor_;
		private:
			state_range_accessor accessor;
			double distance_torelance;
		public:
			equal_state_breaker(state_range_accessor accessor_, double distance_torelance_) 
				: accessor(accessor_)
				, distance_torelance(distance_torelance_){
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				auto Range = accessor();
				for (const auto& sx: Range) {
					if (hmLib::odeint::distance_norm_inf(x, sx) < distance_torelance) {
						return true;
					}
				}
				return false;
			}
		};
		template<typename state_range_accessor_>
		auto make_equal_state_breaker(state_range_accessor_ accessor_, double distance_torelance_){
			return equal_state_breaker<state_range_accessor_>(accessor_, distance_torelance_);
		}
		template<typename state_range_accessor_>
		struct cross_step_breaker {
		public:
			using state_range_accessor = state_range_accessor_;
		private:
			state_range_accessor accessor;
		public:
			explicit cross_step_breaker(state_range_accessor accessor_) :accessor(accessor_){}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				auto Range = accessor();
				auto Last = std::next(std::begin(Range), std::distance(std::begin(Range), std::end(Range)) - 1);
				auto Prev = std::begin(Range);
				auto s1 = hmLib::plane_geometry::make_segment(hmLib::plane_geometry::make_point(x), hmLib::plane_geometry::make_point(*Last));
				for (auto Itr = std::next(Prev); Prev != Last; ++Itr, ++Prev) {
					auto s2 = hmLib::plane_geometry::make_segment(hmLib::plane_geometry::make_point(*Itr), hmLib::plane_geometry::make_point(*Prev));

					if(hmLib::plane_geometry::is_cross(s1, s2))return true;
				}
				return false;
			}
		};
		template<typename state_range_accessor_>
		auto make_cross_step_breakerr(state_range_accessor_ accessor_){
			return cross_step_breaker<state_range_accessor_>(accessor_); 
		}
	}
}
#
#endif
