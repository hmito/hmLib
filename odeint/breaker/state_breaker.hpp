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
		template<typename state_type_, std::size_t container_size_>
		struct converging_steps_breaker {
			static_assert(container_size_>2);
			using state = state_type_;
		private:
			hmLib::circular<state, container_size_> Buf;
			unsigned int Cnt;
			unsigned int Interval;
			double distance_torelance;
			double slope_torelance;
		public:
			converging_steps_breaker(unsigned int Interval_, double Tolerance_) 
				: Cnt(0)
				, Interval(Interval_)
				, Accessor(Accessor_)
				, Tolerance(Tolerance_){
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, time_type t) {
				if(++Cnt < Interval)return false;
				Cnt = 0;

				bool EndFlag = true;
				if(Buf.size() > 1){
					std::vector<double> Nrm;
					for(const auto& px: Buf){
						auto dis = hmLib::odeint::distance_norm_inf(x, px);
						if(dis >= distance_torelance){
							EndFlag = false;
							break;
						}
						Nrm.push_back(dis);
					}

					if(EndFlag && Nrm.size()>1){
						auto xrange = hmLib::make_integer_range(0,Nrm.size());
						auto prm = hmLib::statistics::linaer_regression(xrange.begin(),xrange.end(),Nrm.begin());

						if(std::abs(prm.first) >= slope_torelance){
							EndFlag = false;
						}
					}
				}
				Buf.rotate_back(x);

				return EndFlag;
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
