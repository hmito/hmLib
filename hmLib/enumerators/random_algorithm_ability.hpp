#ifndef HMLIB_RANDOMALGORITHMTRAITS_INC
#define HMLIB_RANDOMALGORITHMTRAITS_INC 100
#
#include<algorithm>
#include<functional>
#include<type_traits>
#include<hmLib/random.hpp>
#include<hmLib/type_traits.hpp>
#include"enumbase.hpp"
namespace hmLib{
	namespace enumerators{
		struct shuffle_ability{
			template<typename enumerator_traits, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_interface{
				static_assert(!std::is_void<enumerator_category>::value, "shuffle_ability cannot be used in this enumerator_category.");
			};
			template<typename enumerator_traits, typename base, typename enumerator_category = typename near_base_of<typename enumerator_traits::enumerator_tag, sentinel_enumerator_tag, range_enumerator_tag>::type >
			struct ability_impl : public base{};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, sentinel_enumerator_tag>{
			public:
				virtual void shuffle() = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, sentinel_enumerator_tag> : public base{
			public:
				void shuffle()override{ std::shuffle(base::Cur, base::End, hmLib::random::default_engine()); }
			};
			template<typename enumerator_traits>
			struct ability_interface<enumerator_traits, range_enumerator_tag>{
			public:
				virtual void shuffle_sofar() = 0;
				virtual void shuffle_range() = 0;
				virtual void shuffle() = 0;
			};
			template<typename enumerator_traits, typename base>
			struct ability_impl<enumerator_traits, base, range_enumerator_tag> : public base{
			public:
				void shuffle_sofar()override{ std::shuffle(base::Beg, base::Cur, hmLib::random::default_engine()); }
				void shuffle_range()override{ std::shuffle(base::Beg, base::End, hmLib::random::default_engine()); }
				void shuffle()override{ std::shuffle(base::Cur, base::End, hmLib::random::default_engine()); }
			};
		};
	}
}
#
#endif
