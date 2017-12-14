#ifndef HMLIB_ODEINT_BOUNDARYTUPLE_INC
#define HMLIB_ODEINT_BOUNDARYTUPLE_INC 100
#
#include<utility>
#include<tuple>
#include"../../tuple.hpp"
namespace hmLib {
	namespace odeint {
		template<typename boundary1_, typename boundary2_>
		struct pair_boundary{
		private:
			using boundary1 = boundary1_;
			using boundary2 = boundary2_;
			using pair = std::pair<boundary1, boundary2>;
		private:
			pair Pair;
		public:
			pair_boundary(boundary1 Boundary1_, boundary2 Boundary2_)
				: Pair(
					std::forward<boundary1>(Boundary1_), 
					std::forward<boundary2>(Boundary2_)
				){
			}
		public:
			template<typename state_type>
			void ready(const state_type& x) {
				std::get<0>(Pair).ready(std::get<0>(x));
				std::get<1>(Pair).ready(std::get<1>(x));
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				if(!std::get<0>(Pair).valid_step(std::get<0>(x)))return false;
				return std::get<1>(Pair).valid_step(std::get<1>(x));
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx)const {
				bool IsValidated = false;
				IsValidated |= std::get<0>(Pair).validate(std::get<0>(x), std::get<0>(vx));
				IsValidated |= std::get<1>(Pair).validate(std::get<1>(x), std::get<1>(vx));

				return IsValidated;
			}
		public:
			boundary1& first(){ return std::get<0>(Pair); }
			const boundary1& first()const { return std::get<0>(Pair); }
			boundary2& second(){ return std::get<1>(Pair); }
			const boundary2& second()const { return std::get<1>(Pair); }
			template<std::size_t n>
			auto& get() { return std::get<n>(Pair); }
			template<std::size_t n>
			const auto& get()const { return std::get<n>(Pair); }
		};
		template<typename... boundaries>
		struct tuple_boundary{
		private:
			using tuple = std::tuple<boundaries...>;
		private:
			tuple Tuple;
		public:
			tuple_boundary(boundaries... Boundaries) 
				: Tuple(std::forward<boundaries>(Boundaries)...) {
			}
		public:
			template<typename state_type>
			void ready(const state_type& x) {
				tuple_for_each(Tuple, x, [](auto& b, const auto& x) { b.ready(x); });
				std::get<0>(Pair).ready(std::get<0>(x));
				std::get<1>(Pair).ready(std::get<1>(x));
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				if(!std::get<0>(Pair).valid_step(std::get<0>(x)))return false;
				return std::get<1>(Pair).valid_step(std::get<1>(x));
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx)const {
				bool IsValidated = false;
				IsValidated |= std::get<0>(Pair).validate(std::get<0>(x), std::get<0>(vx));
				IsValidated |= std::get<1>(Pair).validate(std::get<1>(x), std::get<1>(vx));

				return IsValidated;
			}
		public:
			boundary1& first() { return std::get<0>(Pair); }
			const boundary1& first()const { return std::get<0>(Pair); }
			boundary2& second() { return std::get<1>(Pair); }
			const boundary2& second()const { return std::get<1>(Pair); }
			template<std::size_t n>
			auto& get() { return std::get<n>(Pair); }
			template<std::size_t n>
			const auto& get()const { return std::get<n>(Pair); }
		};
	}
}
#
#endif
