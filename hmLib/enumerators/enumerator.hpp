#ifndef HMLIB_ENUMERATORS_ENUMERATOR_INC
#define HMLIB_ENUMERATORS_ENUMERATOR_INC 100
#
#include"enumerator_traits.hpp"
#include"enumbase.hpp"
namespace hmLib{
	//ability pack
	namespace enumerators{
		template<typename enumerator_traits, typename... others>
		struct enumerator_pack{};
		template<typename enumerator_traits, typename ability, typename... others>
		struct enumerator_pack<enumerator_traits, ability, others...>
			: public ability::template ability_interface<enumerator_traits>
			, public enumerator_pack<enumerator_traits, others...>{
		};
		template<typename enumbase, typename base, typename... others>
		struct enumerator_impl_pack : public base{};
		template<typename enumbase, typename base, typename ability, typename... others>
		struct enumerator_impl_pack<enumbase, base, ability, others...>
			: public enumerator_impl_pack<enumbase, typename ability::template ability_impl<enumbase, base>, others...>{
		};
	}
	template<typename enumerator_traits, typename... abilities>
	struct enumerator: public enumerators::enumerator_pack<enumerator_traits, abilities...>{
	private:
		using this_type = enumerator<enumerator_traits, abilities...>;
	public:
		using enumerator_tag = typename enumerator_traits::enumerator_tag;
		using value_type = typename enumerator_traits::value_type;
		using difference_type = typename enumerator_traits::difference_type;
		using reference = typename enumerator_traits::reference;
		using pointer = typename enumerator_traits::pointer;
	public:
		template<typename iterator_>
		using enumbase = enumerators::enumbase_from_tag<enumerator_tag, iterator_, this_type>;
	public:
		struct impl: public enumerators::enumerator_impl_pack<enumbase, enumbase, abilities...>{};
	};
}
#
#endif
