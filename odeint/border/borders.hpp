#ifndef HMLIB_ODEINT_BORDER_BORDERS_INC
#define HMLIB_ODEINT_BORDER_BORDERS_INC 100
#
#include<utility>
#include<type_traits>
#include<tuple>
#include"../../tuple.hpp"
namespace hmLib {
	namespace odeint {
		template<typename... borders_>
		struct borders {
		private:
			using border_pack = std::tuple<borders_...>;
			static constexpr size_t border_num = sizeof...(borders_);
		public:
			using appendix = decltype(hmLib::tuple_array<bool, border_num>(false));
			borders() = default;
			template<typename... arg_borders_>
			borders(arg_borders_... Args) :Borders(Args...) {}
			template<size_t n>
			auto& get() { return std::get<n>(Borders); }
			template<size_t n>
			const auto& get()const { return std::get<n>(Borders); }
		public:
			template<typename state_type, typename time_type>
			void operator()(const state_type& x, state_type& dx, time_type t, const appendix& ap)const {
				auto Res = hmLib::tuple_transform([&x, &dx, t](auto Border, bool Ap) {return Border(x, dx, t, Ap); }, Borders, ap);

				//if more than two border detected the overflow reduction
				if (hmLib::tuple_reduce([](unsigned int v1, bool v2) {return (v2 ? v1 + 1 : v1); }, Res, 0) >= 2) {
					//such multi-hit boundaries 
					hmLib::tuple_transform([&dx](auto Border, bool Ap) {Border.force_zero_deriv(dx, Ap); return false; }, Borders, Res);
				}
			}
			template<typename state_type, typename time_type>
			bool is_invalid_step(const state_type& x, const state_type& dx, time_type t, const appendix& ap)const {
				return hmLib::tuple_reduce(
					[](bool v1, bool v2) {return v1 | v2; },
					hmLib::tuple_transform([&x, &dx, t](auto Border, bool Ap) {return Border.is_invalid_step(x, dx, t, Ap); }, Borders, ap),
					false
				);
			}
			template<typename state_type, typename time_type>
			bool will_validate(const state_type& x, time_type t, appendix ap)const {
				return hmLib::tuple_reduce(
					[](bool v1, bool v2) {return v1 | v2; },
					hmLib::tuple_transform([&x, t](auto Border, bool Ap) {return Border.will_validate(x, t, Ap); }, Borders, ap),
					false
				);
			}
			template<typename state_type, typename time_type>
			bool validate(state_type& x, const state_type& dx, time_type t, appendix& ap)const {
				return hmLib::tuple_reduce(
					[](bool v1, bool v2) {return v1 | v2; },
					hmLib::tuple_transform([&x, &dx, t](auto Border, bool& Ap) {return Border.validate(x, dx, t, Ap); }, Borders, ap),
					false
				);
			}
		private:
			border_pack Borders;
		};
		template<typename... arg_borders_>
		auto make_borders(arg_borders_... args) {
			return borders<std::decay<arg_borders_>...>(args...);
		}

	}
}
#
#endif
