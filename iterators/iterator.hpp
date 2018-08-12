#ifndef HMLIB_ITERATORS_ITERATOR_INC
#define HMLIB_ITERATORS_ITERATOR_INC 100
#
#include<iterator>
#include<type_traits>
namespace hmLib {
	namespace iterators {
		struct incrementable_traversal_tag {};
		struct singlepass_traversal_tag: public incrementable_traversal_tag {};
		struct forward_traversal_tag: public singlepass_traversal_tag {};
		struct bidirectional_traversal_tag: public forward_traversal_tag {};
		struct random_access_traversal_tag: public bidirectional_traversal_tag {};

		template<typename iterator>
		struct is_writable_iterator {
			using value_type = typename std::iterator_traits<iterator>::value_type;
		private:
			template<typename iterator_>
			static auto check(iterator_ itr)->decltype(
				*(std::declval<iterator_>()) = std::declval<value_type>(),
				std::true_type()
				);
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<typename std::decay<iterator>::type>()));
			constexpr static bool value = type::value;
		};
		template<typename iterator>
		struct is_readable_iterator {
			using value_type = typename std::iterator_traits<iterator>::value_type;
		public:
			using type = typename std::is_convertible<decltype(*(std::declval<iterator>())), value_type>::type;
			constexpr static bool value = type::value;
		};
		template<typename iterator>
		struct is_arrow_accessible_iterator {
		private:
			template<typename iterator_>
			static auto check(iterator_ itr)->decltype(
				std::declval<iterator_>().operator->(),
				std::true_type()
				);
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<typename std::decay<iterator>::type>()));
			constexpr static bool value = type::value;
		};
		template<typename iterator>
		struct is_random_accessible_iterator {
			using value_type = typename std::iterator_traits<iterator>::value_type;
			using difference_type = typename std::iterator_traits<iterator>::difference_type;
		private:
			template<typename iterator_>
			static auto check(iterator_ itr) -> typename std::is_convertible<
				decltype(std::declval<iterator_>().operator[](std::declval<difference_type>())),
				value_type
			>::type;
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<typename std::decay<iterator>::type>()));
			constexpr static bool value = type::value;
		};
		template<typename iterator>
		struct is_lvalue_iterator {
			using value_type = typename std::iterator_traits<iterator>::value_type;
			using reference = typename std::iterator_traits<iterator>::reference;
		public:
			using type = typename std::is_same<value_type&, reference>::type;
			constexpr static bool value = type::value;
		};
	}
	template<typename iterator_t>
	struct iterator_traits {
	private:
		using value_type = typename std::iterator_traits<iterator_t>::value_type;
	};
}
#
#endif
