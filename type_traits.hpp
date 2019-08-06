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

	template <typename T>
	struct has_begin_and_end {
	private:
		template <class U>
		static auto check(U&& x)->decltype(x.begin(), x.end(), std::true_type{});
		static auto check(...)->std::false_type;
	public:
		using type = decltype(check(std::declval<typename std::decay<T>::type>()));
		constexpr static bool value = type::value;
	};

	template<typename T>
	struct is_iterator{
	private:
		template<typename U>
		static auto check(U&&)->decltype(typename std::iterator_traits<U>::iterator_category{}, std::true_type{});
		static auto check(...)->std::false_type;
	public:
		using type = decltype(check(std::declval<typename std::decay<T>::type>()));
		constexpr static bool value = type::value;
	};

	template<typename T, bool is_iterator_ = is_iterator<T>::value>
	struct is_const_iterator: public std::false_type {};

	template<typename T>
	struct is_const_iterator<T, true> {
	public:
		using type = std::integral_constant<bool, std::is_assignable<decltype(*std::declval<T>()), typename std::iterator_traits<T>::value_type>::value>;
		constexpr static bool value = type::value;
	};

	namespace detail {
		struct result_of_impl {
			template < class Result, class ... Args >
			static Result Func(Result(*)(Args ...));
			template < class T, class Result, class ... Args >
			static Result Func(Result(T::*)(Args ...));
			template < class T, class Result, class ... Args >
			static Result Func(Result(T::*)(Args ...) const);
			template < class T, class FuncType = decltype(&T::operator()) >
			static decltype(Func(std::declval<FuncType>())) Func(T*);
		};
	}
	template < typename Fn >
	struct result_of {
		using type = decltype(detail::result_of_impl::Func(std::declval<std::remove_pointer_t<Fn>*>()));
	};

	template<typename Fn, typename T>
	struct is_applicable {
	private:
		template<typename eFn, typename eT, typename ans_type = decltype(std::declval<eFn>()(std::declval<eT>()))>
		static auto check(eFn&&, eT&&)->std::true_type;
		static auto check(...)->std::false_type;
	public:
		using type = decltype(check(std::declval<Fn>(), std::declval<T>()));
		static constexpr const bool value = type::value;
	};
}
#
#endif
