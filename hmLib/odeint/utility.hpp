#ifndef HMLIB_ODEINT_UTILITY_INC
#define HMLIB_ODEINT_UTILITY_INC 100
#
#include<boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include"../utility.hpp"
namespace hmLib{
	namespace odeint{
		namespace detail{
			struct has_begin_and_end_impl{
				template <class T>
				static auto check(T&& x)->decltype(x.begin(), x.end(), std::true_type{});

				template <class T>
				static auto check(...)->std::false_type;
			};
			template <class T>
			class has_begin_and_end :public decltype(has_begin_and_end_impl::check<T>(std::declval<T>())) {};

			template<typename state,hmLib_static_restrict(has_begin_and_end<state>::value)>
			double abs_distance(const state& State1, const state& State2){
				auto itr1 = State1.begin();
				auto end1 = State1.end();
				auto itr2 = State2.begin();

				double Max = 0;
				for(; itr1 != end1; ++itr1, ++itr2){
					Max = std::max(Max, std::abs(*itr1 - *itr2));
				}

				return Max;
			}
			template<typename state, hmLib_static_restrict(!has_begin_and_end<state>::value)>
			double abs_distance(const state& State1, const state& State2){
				return boost::numeric::odeint::vector_space_norm_inf<state>()(State1+( State2 * -1.0));
			}
		}
	}
}
#
#endif
