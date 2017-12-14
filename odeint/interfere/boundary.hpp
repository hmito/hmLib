#ifndef HMLIB_ODEINT_INTERFERE_BOUNDARY_INC
#define HMLIB_ODEINT_INTERFERE_BOUNDARY_INC 100
#
#include<utility>
#include<vector>
#include<type_traits>
#include<tuple>
#include"../../tuple.hpp"
namespace hmLib {
	namespace odeint {
		template<typename... boundaries>
		struct boundary_composite {
		private:
			using tuple = std::tuple<boundaries...>;
		private:
			tuple Tuple;
		public:
			boundary_composite(boundaries... Boundaries)
				: Tuple(std::forward<boundaries>(Boundaries)...) {
			}
		public:
			template<typename state_type>
			void ready(const state_type& x) {
				tuple_for_each([&x](auto& b) { b.ready(x); }, Tuple);
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				bool ans = true;
				tuple_for_each([&ans,&x](auto& b) { ans &= b.valid_step(x); }, Tuple);
				return ans;
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx)const {
				bool ans = false;
				tuple_for_each([&ans,&x,&vx](auto& b) { ans |= b.validate(x,vx); }, Tuple);
				return ans;
			}
		public:
			template<std::size_t n>
			auto& get() { return std::get<n>(Tuple); }
			template<std::size_t n>
			const auto& get()const { return std::get<n>(Tuple); }
		};
		template<typename... boundaries>
		auto make_boundary_composite(boundaries... bs) {
			return boundary_composite<typename std::decay<boundaries>::type...>(std::forward<boundaries>(bs)...);
		}

		template<typename boundary_>
		struct for_each_boundary{
			using boundary_type = boundary_;
			using state_type = typename boundary_type::state_type;
			using container = std::vector<boundary_type>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			boundary_type Original;
			container Container;
		public:
			for_each_boundary(boundary_type Original_):Original(std::move(Original_)){}
		public://for interfere system
			template<typename state_type>
			void ready(const state_type& x) { 
				if(Container.size() != x.size()) {
					Container.assign(x.size(), Original);
				}

				auto itr = x.begin();
				
				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					bitr->ready(*itr);
				}
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				auto itr = x.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr) {
					if(!bitr->valid_step(*itr))return false;
				}
				return true;
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx) const {
				bool IsValidated = false;
				auto itr = x.begin();
				auto vitr = vx.begin();

				for(auto bitr = begin(); bitr != end(); ++bitr, ++itr, ++vitr) {
					IsValidated |= bitr->validate(*itr,*vitr);
				}
				return IsValidated;
			}
		public:
			iterator begin() { return Container.begin(); }
			iterator end() { return Container.end(); }
			const_iterator begin()const { return Container.begin(); }
			const_iterator end()const { return Container.end(); }
			const_iterator cbegin()const { return Container.begin(); }
			const_iterator cend()const { return Container.end(); }
			std::size_t size()const { return Container.size(); }
			bool empty()const { return Container.empty(); }
			boundary_type& at(std::size_t n) { return Container.at(n); }
			const boundary_type& at(std::size_t n)const{ return Container.at(n); }
		};
		template<typename boundary>
		auto make_for_each_boundary(boundary b) {
			return for_each_boundary<typename std::decay<boundary>::type>(std::forward<boundary>(b));
		}

		template<typename boundary1_, typename boundary2_>
		struct pair_boundary {
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
				) {
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
			boundary1& first() { return std::get<0>(Pair); }
			const boundary1& first()const { return std::get<0>(Pair); }
			boundary2& second() { return std::get<1>(Pair); }
			const boundary2& second()const { return std::get<1>(Pair); }
			template<std::size_t n>
			auto& get() { return std::get<n>(Pair); }
			template<std::size_t n>
			const auto& get()const { return std::get<n>(Pair); }
		};
		template<typename boundary1, typename boundary2>
		auto make_pair_boundary(boundary1 b1, boundary2 b2) {
			return pair_boundary<typename std::decay<boundary1>::type, typename std::decay<boundary2>::type>(std::forward<boundary1>(b1), std::forward<boundary2>(b2));
		}

		template<typename... boundaries>
		struct tuple_boundary {
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
				tuple_for_each([](auto& b, const auto& ex) { b.ready(ex); }, Tuple, x);
			}
			template<typename state_type>
			bool valid_step(const state_type& x)const {
				bool ans = true;
				tuple_for_each([&ans](auto& b, const auto& ex) { ans &= b.valid_step(ex); }, Tuple, x);
				return ans;
			}
			template<typename state_type>
			bool validate(const state_type& x, state_type& vx)const {
				bool ans = false;
				tuple_for_each([&ans](auto& b, const auto& ex, auto& evx) { ans |= b.validate(ex, evx); }, Tuple, x);
				return ans;
			}
		public:
			template<std::size_t n>
			auto& get() { return std::get<n>(Tuple); }
			template<std::size_t n>
			const auto& get()const { return std::get<n>(Tuple); }
		};
		template<typename... boundaries>
		auto make_tuple_boundary(boundaries... bs) {
			return tuple_boundary<typename std::decay<boundaries>::type...>(std::forward<boundaries>(bs)...);
		}
	}
}
#
#endif
