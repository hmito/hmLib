#ifndef HMLIB_FUNCTIONAL_INC
#define HMLIB_FUNCTIONAL_INC 100
#
namespace hmLib{
	template<typename T>
	class function_ptr;

	template<typename ReturnType,typename ...ArgumentTypes>
	class function_ptr<ReturnType(ArgumentTypes...)>{
		using this_type = function_ptr<ReturnType(ArgumentTypes...)>;
		using pointer = ReturnType(*)(ArgumentTypes...);
		pointer Ptr;
	public:
		function_ptr():Ptr(nullptr){}
		function_ptr(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		function_ptr(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		function_ptr(pointer Ptr_) :Ptr(Ptr_){}
		this_type& operator=(pointer Ptr_) noexcept { Ptr = Ptr_; return *this; }
		ReturnType operator()(ArgumentTypes... Args){
			return Ptr(Args...);
		}
	};

	namespace functional {
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
}
#
#endif
