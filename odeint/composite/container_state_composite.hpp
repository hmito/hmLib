#ifndef HMLIB_ODEINT_COMPOSITE_CONTAINERSTATECOMPOSITE_INC
#define HMLIB_ODEINT_COMPOSITE_CONTAINERSTATECOMPOSITE_INC 100
#
#include <vector>
#include <utility>
#include <type_traits>
#include "composite_system.hpp"
namespace hmLib {
	namespace odeint {
		namespace composite {
			template<typename state_type_, typename time_type_, typename condition_>
			struct state_at_condition {
				using state_type = state_type_;
				using time_type = time_type_;
				using condition_type = condition_;
				using element_type = typename condition_type::state_type;
			private:
				std::size_t n;
				condition_type Cond;
			public:
				state_at_condition(std::size_t n_, condition_type  Cond_) :n(n_), Cond(std::move(Cond_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Cond.update(x[n], t);
				}
				bool valid(const state_type& x, time_type t)  {
					return Cond.valid(x[n], t);
				}
				void validate(state_type& x, time_type t) {
					Cond.validate(x[n], t);
				}
				bool condition()  {
					return Cond.condition();
				}
			};
			template<typename state_type_, typename time_type_, typename require_>
			struct state_at_require {
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
				using element_type = typename require_type::state_type;
			private:
				std::size_t n;
				require_type Req;
			public:
				state_at_require(std::size_t n_, require_type  Req_) :n(n_), Req(std::move(Req_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Req.update(x[n], t);
				}
				bool valid(const state_type& x, time_type t)  {
					return Req.valid(x[n], t);
				}
				void validate(state_type& x, time_type t) {
					Req.validate(x[n], t);
				}
				void require(const state_type& x, state_type& dx, time_type t)  {
					Req.require(x[n], dx[n], t);
				}
			};
			template<typename state_type_, typename time_type_, typename reqcon_>
			struct state_at_require_condition {
				using state_type = state_type_;
				using time_type = time_type_;
				using reqcon_type = reqcon_;
				using element_type = typename reqcon_type::state_type;
			private:
				std::size_t n;
				reqcon_type Req;
			public:
				state_at_require_condition(std::size_t n_, reqcon_type  Req_) :n(n_), Req(std::move(Req_)) {}
			public:
				void update(const state_type& x, time_type t) {
					Req.update(x[n], t);
				}
				bool valid(const state_type& x, time_type t)  {
					return Req.valid(x[n], t);
				}
				void validate(state_type& x, time_type t) {
					Req.validate(x[n], t);
				}
				void require(const state_type& x, state_type& dx, time_type t)  {
					Req.require(x[n], dx[n], t);
				}
				bool condition() const {
					return Req.condition();
				}
			};
			namespace detail {
				template<typename state_type, typename time_type, typename target_type, bool IsRequire = composite::is_require<target_type>::value, bool IsCondition = composite::is_condition<target_type>::value>
				struct select_state_at {};
				template<typename state_type, typename time_type, typename target_type>
				struct select_state_at<state_type, time_type, target_type, true, false> {
					using type = state_at_require<state_type, time_type, target_type>;
				};
				template<typename state_type, typename time_type, typename target_type>
				struct select_state_at<state_type, time_type, target_type, false, true> {
					using type = state_at_condition<state_type, time_type, target_type>;
				};
				template<typename state_type, typename time_type, typename target_type>
				struct select_state_at<state_type, time_type, target_type, true, true> {
					using type = state_at_require_condition<state_type, time_type, target_type>;
				};
			}

			template<typename state_type_, typename time_type_, typename require_>
			struct state_for_each_require {
				using state_type = state_type_;
				using time_type = time_type_;
				using require_type = require_;
			private:
				require_type  ReqBase;
				std::vector<require_type> Req;
			public:
				state_for_each_require(require_type  Req_):ReqBase(Req_) {}
			public:
				void update(const state_type& x, time_type t) {
					if(x.size()!=Req.size()) {
						Req.assign(x.size(), ReqBase);
					}
					auto ritr = std::begin(Req);
					auto itr = std::begin(x);
					auto end = std::end(x);
					while(itr!=end) {
						(*ritr++).update(*itr++, t);
					}
				}
				bool valid(const state_type& x, time_type t)  {
					auto ritr = std::begin(Req);
					auto itr = std::begin(x);
					auto end = std::end(x);
					while(itr!=end) {
						if(!(*ritr++).valid(*itr++, t))return false;
					}
					return true;
				}
				void validate(state_type& x, time_type t) {
					auto ritr = std::begin(Req);
					auto itr = std::begin(x);
					auto end = std::end(x);
					while(itr!=end) {
						(*ritr++).validate(*itr++, t);
					}
				}
				void require(const state_type& x, state_type& dx, time_type t)  {
					auto ritr = std::begin(Req);
					auto itr = std::begin(x);
					auto end = std::end(x);
					auto ditr = std::begin(dx);
					while(itr!=end) {
						(*ritr++).require(*itr++, *ditr++, t);
					}
				}
			};
		}
		template<typename state_type, typename target_type, typename time_type = double>
		auto state_at(std::size_t n, target_type&& Req) {
			return typename composite::detail::select_state_at<state_type, time_type, typename std::decay<target_type>::type>::type(n, std::forward<target_type>(Req));
		}
		template<typename state_type, typename require, typename time_type = double>
		auto state_for_each(require&& Req) {
			return composite::state_for_each_require<state_type, time_type, typename std::decay<require>::type>(std::forward<require>(Req));
		}
	}
}
#
#endif
