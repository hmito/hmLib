#ifndef HMLIB_ODEINT_COMPOSITE_TUPLESTATECOMPOSITE_INC
#define HMLIB_ODEINT_COMPOSITE_TUPLESTATECOMPOSITE_INC 100
#
#include <tuple>
#include <utility>
#include <type_traits>
#include "composite_system.hpp"
namespace hmLib {
	namespace odeint {
		namespace composite {
			template<std::size_t N, typename state_type_, typename time_type_, typename condition_>
			struct tuple_at_condition {
				static constexpr std::size_t n = N;
				using state_type = state_type_;
				using time_type = time_type_;
				using condition_type = condition_;
				using element_type = typename condition_type::state_type;
			private:
				condition_type Cond;
			public:
				tuple_at_condition(condition_type  Cond_) : Cond(std::move(Cond_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Cond.update(std::get<n>(x), t);
				}
				bool valid(const state_type& x, time_type t) const {
					return Cond.valid(std::get<n>(x), t);
				}
				void validate(state_type& x, time_type t) {
					Cond.validate(std::get<n>(x), t);
				}
				bool condition() const {
					return Cond.condition();
				}
			};
			template<std::size_t N, typename state_type_, typename time_type_, typename require_>
			struct tuple_at_require {
				static constexpr std::size_t n = N;
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
				using element_type = typename require_type::state_type;
			private:
				require_type Req;
			public:
				tuple_at_require(require_type  Req_) : Req(std::move(Req_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Req.update(std::get<n>(x), t);
				}
				bool valid(const state_type& x, time_type t) const {
					return Req.valid(std::get<n>(x), t);
				}
				void validate(state_type& x, time_type t) {
					Req.validate(std::get<n>(x), t);
				}
				void require(const state_type& x, state_type& dx, time_type t) const {
					Req.require(std::get<n>(x), std::get<n>(dx), t);
				}
			};
			template<std::size_t N, typename state_type_, typename time_type_, typename require_>
			struct tuple_at_require_condition {
				static constexpr std::size_t n = N;
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
				using element_type = typename require_type::state_type;
			private:
				require_type Reqcon;
			public:
				tuple_at_require_condition(require_type Req_) : Reqcon(std::move(Req_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Reqcon.update(std::get<n>(x), t);
				}
				bool valid(const state_type& x, time_type t) const {
					return Reqcon.valid(std::get<n>(x), t);
				}
				void validate(state_type& x, time_type t) {
					Reqcon.validate(std::get<n>(x), t);
				}
				void require(const state_type& x, state_type& dx, time_type t) const {
					Reqcon.require(std::get<n>(x), std::get<n>(dx), t);
				}
				bool condition() const {
					return Reqcon.condition();
				}
			};
			namespace detail {
				template<std::size_t N, typename state_type, typename time_type, typename target_type, bool IsRequire = composite::is_require<target_type>::value, bool IsCondition = composite::is_condition<target_type>::value>
				struct select_tuple_at {};
				template<std::size_t N, typename state_type, typename time_type, typename target_type>
				struct select_tuple_at<N, state_type, time_type, target_type, true, false> {
					using type = tuple_at_condition<N, state_type, time_type, target_type>;
				};
				template<std::size_t N, typename state_type, typename time_type, typename target_type>
				struct select_tuple_at<N, state_type, time_type, target_type, false, true> {
					using type = tuple_at_require<N, state_type, time_type, target_type>;
				};
				template<std::size_t N, typename state_type, typename time_type, typename target_type>
				struct select_tuple_at<N, state_type, time_type, target_type, true, true> {
					using type = tuple_at_require_condition<N, state_type, time_type, target_type>;
				};
			}
		}
		template<typename state_type, std::size_t N, typename target_type, typename time_type = double>
		auto tuple_at(target_type&& Req) {
			return typename composite::detail::select_tuple_at<N, state_type, time_type, typename std::decay<target_type>::type>::type(std::forward<target_type>(Req));
		}
	}
}
#
#endif
