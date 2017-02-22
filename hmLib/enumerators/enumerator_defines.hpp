#ifndef HMLIB_ENUMERATORS_ENUMERATORDEFINES_INC
#define HMLIB_ENUMERATORS_ENUMERATORDEFINES_INC 100
#
#include"enumerator.hpp"
#include"traversal_ability.hpp"
namespace hmLib{
	template<typename value_type, typename... abilities>
	using output_enumerator = enumerator<value_type, referable_enumerator_tag, enumerators::forward_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using input_enumerator = enumerator<value_type, sentinel_enumerator_tag, enumerators::forward_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using forward_enumerator = enumerator<value_type, range_enumerator_tag,enumerators::forward_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using bidirectional_enumerator = enumerator<value_type, range_enumerator_tag, enumerators::bidirectional_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using random_access_enumerator = enumerator<value_type, range_enumerator_tag, enumerators::random_access_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using forward_mutable_enumerator = enumerator<value_type, mutable_range_enumerator_tag, enumerators::forward_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using bidirectional_mutable_enumerator = enumerator<value_type, mutable_range_enumerator_tag, enumerators::bidirectional_traversal_ability, abilities...>;
	template<typename value_type, typename... abilities>
	using random_access_mutable_enumerator = enumerator<value_type, mutable_range_enumerator_tag, enumerators::random_access_traversal_ability, abilities...>;
}
#
#endif
