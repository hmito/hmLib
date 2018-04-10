#ifndef HMLIB_TYPETRAITS_INC
#define HMLIB_TYPETRAITS_INC 100
#
#include<type_traits>
#include<iterator>
namespace hmLib{
	template<typename type1, typename type2>
	struct select_derived{
		template<typename type, bool Type1IsBase = std::is_base_of<type1,type2>::value, bool Type2IsBase = std::is_base_of<type2, type1>::value>
		struct check{
			using ans_type = void;
		};
		template<typename type>
		struct check<type, true, false>{
			using ans_type = type2;
		};
		template<typename type>
		struct check<type, false, true>{
			using ans_type = type1;
		};
		using type = typename check<void>::ans_type;
	};
	template<typename terget, typename... others>
	struct near_base_of{
		template<typename terget_, typename candidate_>
		struct check{
			using ans_type = candidate_;
		};
		using type = typename check<terget, void>::ans_type;
	};
	template<typename terget, typename try_type, typename... others>
	struct near_base_of<terget, try_type, others...>{
		template<typename terget_, typename candidate_, bool IsBase = std::is_base_of<try_type, terget>::value>
		struct check{
			using ans_type = typename near_base_of<terget, others...>::template check<terget_, candidate_>::ans_type;
		};
		template<typename terget_, typename candidate_>
		struct check<terget_, candidate_, true>{
			using new_candidate = typename select_derived<candidate_, try_type>::type;
			using ans_type = typename near_base_of<terget, others...>::template check<terget_, new_candidate>::ans_type;
		};
		template<typename terget_>
		struct check<terget_, void, true>{
			using ans_type = typename near_base_of<terget, others...>::template check<terget_, try_type>::ans_type;
		};
		using type = typename check<terget, void>::ans_type;
	};

	namespace detail {
		struct has_begin_and_end_impl {
			template <class T>
			static auto check(T&& x)->decltype(x.begin(), x.end(), std::true_type{});
			template <class T>
			static auto check(...)->std::false_type;
		};
	}
	template <class T>
	class has_begin_and_end :public decltype(detail::has_begin_and_end_impl::check<T>(std::declval<T>())) {};

	template<typename iterator>
	using is_const_iterator = typename std::is_const<
		std::remove_reference_t<
			typename std::iterator_traits<iterator>::reference
		>
	>::type;
}
#
#endif
