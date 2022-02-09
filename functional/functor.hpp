#ifndef HMLIB_FUNCTIONAL_FUNCTOR_INC
#define HMLIB_FUNCTIONAL_FUNCTOR_INC 100
#
namespace hmLib{
	struct identity {
		template<typename T>
		constexpr T&& operator()(T&& t) const noexcept{return std::forward<T>(t);}
	};
	struct noop{
		template<typename... args> 
		void operator()(args...) const noexcept{}
	};
	template<typename T>
	struct dynamic_constant{
	private:
		using this_type = dynamic_constant<T>;
		T v;
	public:
		dynamic_constant()=delete;
		explicit dynamic_constant(T&& v_)noexcept:v(std::move(v_)){}
		explicit dynamic_constant(const T& v_):v(v_){}
		const T& oprator()const noexcept{return v;}
	};
}
#
#endif
