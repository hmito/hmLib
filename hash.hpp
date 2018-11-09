#ifndef HMLIB_HASH_INC
#define HMLIB_HASH_INC 100
#
#include<utility>
#include<cstdint>
#include<cstring>
#include"utility.hpp"
#include"type_traits.hpp"
namespace hmLib {
	namespace detail {
		template<typename result_type_, result_type_ fnv_hash_prime, result_type_ fnv_hash_offset_basis>
		struct basic_fnv1a_algorithm {
			using result_type = result_type_;
			template<typename input_iterator, hmLib_static_restrict(is_iterator<input_iterator>::value) >
			result_type get(input_iterator beg, input_iterator end) {
				return merge(fnv_hash_offset_basis, beg, end);
			}
			template<typename input_iterator, hmLib_static_restrict(is_iterator<input_iterator>::value) >
			result_type merge(result_type seed, input_iterator beg, input_iterator end) {
				static_assert(sizeof(result_type)>=sizeof(decltype(*(std::declval<input_iterator>()))), "value_type size is larger than seed_type size in fnv_hash.");
				for(; beg!=end; ++beg) {
					seed = merge(seed, *beg);
				}
				return seed;
			}
			template<typename value_type, hmLib_static_restrict(!is_iterator<value_type>::value)  >
			result_type get(value_type val) {
				return merge(fnv_hash_offset_basis, val);
			}
			template<typename value1_type, typename value2_type, hmLib_static_restrict(!is_iterator<value1_type>::value)  >
			result_type get(value1_type val1, value2_type val2) {
				return merge(fnv_hash_offset_basis, val1, val2);
			}
			template<typename value1_type, typename value2_type, typename value3_type, typename... others >
			result_type get(value1_type val1, value2_type val2, value3_type val3, others... Others) {
				return merge(fnv_hash_offset_basis, val1, val2, val3, Others...);
			}
			template<typename value_type, hmLib_static_restrict(!is_iterator<value_type>::value) >
			result_type merge(result_type seed, value_type val) {
				static_assert(sizeof(result_type)>=sizeof(value_type), "value_type size is larger than seed_type size in fnv_hash.");
				result_type v = 0;
				std::memcpy(&v, &val, sizeof(decltype(val)));
				//v = *reinterpret_cast<result_type*>(&val);
				return (seed ^ v) * fnv_hash_prime;
			}
			template<typename value1_type, typename value2_type, hmLib_static_restrict(!is_iterator<value1_type>::value)  >
			result_type merge(result_type seed, value1_type val1, value2_type val2) {
				return merge(merge(seed, val1), val2);
			}
			template<typename value1_type, typename value2_type, typename value3_type, typename... others >
			result_type merge(result_type seed, value1_type val1, value2_type val2, value3_type val3, others... Others) {
				return merge(merge(seed, val1), val2, val3, Others...);
			}
		};
	}
	using fnv1a_32_algorithm = detail::basic_fnv1a_algorithm<std::uint32_t, UINT32_C(16777619), UINT32_C(2166136261)>;
	using fnv1a_64_algorithm = detail::basic_fnv1a_algorithm<std::uint64_t, UINT64_C(1099511628211), UINT64_C(14695981039346656037)>;
	namespace detail {
		template<unsigned int Bits>
		struct fnv1a_algorithm_selector { using type = void; };
		template<>
		struct fnv1a_algorithm_selector<32> { using type = fnv1a_32_algorithm; };
		template<>
		struct fnv1a_algorithm_selector<64> { using type = fnv1a_64_algorithm; };
	}
	using fnv1a_algorithm = typename detail::fnv1a_algorithm_selector<8*sizeof(std::size_t)>::type;
	template<typename T>
	struct fnv1a_range_hash {
		using result_type = std::size_t;
		using argument_type = T;
		result_type operator()(const argument_type& key)const { return fnv1a_algorithm().get(std::begin(key), std::end(key)); }
	};

	std::size_t merge_hash(std::size_t seed1, std::size_t seed2) {
		return seed1 ^ (seed2 + 0x9e3779b9 + (seed1 << 6) + (seed1 >> 2));
	};
	template<typename... others>
	std::size_t merge_hash(std::size_t seed1, std::size_t seed2, std::size_t seed3, others... other_seeds) {
		return merge_hash(merge_hash(seed1, seed2), seed3, other_seeds...);
	}

	template<typename T>
	struct hash {
		using result_type = std::size_t;
		using argument_type = typename std::hash<T>::argument_type;
		result_type operator()(const argument_type& key)const { return std::hash<T>(key); }
	};
	template<typename T1, typename T2>
	struct hash< std::pair<T1, T2> > {
		using result_type = std::size_t;
		using argument_type = std::pair<T1, T2>;
		result_type operator()(const argument_type& key)const { return merge_hash(hash<T1>(key.first), hash<T2>(key.second)); }
	};
}
#
#endif
