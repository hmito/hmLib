#include<utility>
#include<deque>
#include<initializer_list>
#include<functional>
#include<type_traits>
#include<iostream>
#include<boost/optional.hpp>
#include<cmath>

//monad
namespace hmLib {
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
		template < typename T >
		struct result_of {
			using type = decltype(detail::result_of_impl::Func(std::declval<std::remove_pointer_t<T>*>()));
		};

		namespace monad_categories {
			struct immutable_monad_tag {};
			struct omittable_monad_tag {};
			struct flattenable_monad_tag{};
		}
		template<typename T>
		struct is_monad {
		private:
			template<typename U, typename V = typename U::monad_category>
			static auto check(U)->std::true_type;
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<T>()));
			static constexpr const bool value = type::value;
		};

		namespace detail {
			template<typename T, bool IsMonad = is_monad<T>::value>
			struct monad_base {
				using type = T;
			};
			template<typename T>
			struct monad_base<T, true> {
				using type = typename monad_base<typename T::value_type>::type;
			};
		}
		template<typename monad, bool IsNonMonad = !is_monad<monad>::value>
		struct monad_traits {
			using monad_category = typename monad::monad_category;
			using value_type = typename monad::value_type;
			using base_type = typename detail::monad_base<value_type>::type;
			template<typename other>
			using rebind_t = typename monad::template rebind<other>::type;
		};
		template<typename monad>
		struct monad_traits<monad, true> {
			using monad_category = void;
			using value_type = void;
			using base_t = monad;
		};
		template<typename T, typename U>
		struct is_same_monad{
		private:
			template<typename V, typename W>
			static auto check(V,W)->std::bool_constant<is_monad<V>::value && std::is_same<W, typename monad_traits<V>::template rebind_t<typename monad_traits<W>::value_type>>::value>;
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<T>(), std::declval<U>()));
			static constexpr const bool value = type::value;
		};
		template<typename monad>
		struct is_nested_monad : is_same_monad<monad, typename monad_traits<monad>::value_type>{};
		template<typename monad>
		struct is_omittable : std::is_same<typename monad_traits<monad>::monad_category, monad_categories::omittable_monad_tag> {};
		template<typename monad>
		struct is_flattenable : std::bool_constant<std::is_same<typename monad_traits<monad>::monad_category, monad_categories::flattenable_monad_tag>::value || is_omittable<monad>::value> {};

		namespace detail {
			template<typename T, bool IsMonad = is_monad<T>::value>
			struct apply_impl {
				template<typename fn>
				auto operator()(fn&& Fn, const T& val) { return Fn(val); }
				template<typename fn>
				auto operator()(fn&& Fn, T&& val) { return Fn(std::move(val)); }
			};
			template<typename T>
			struct apply_impl<T, true> {
				template<typename fn>
				auto operator()(fn&& Fn, const T& val) { return val.apply(Fn); }
				template<typename fn>
				auto operator()(fn&& Fn, T&& val) { return val.apply(Fn); }
			};
		}
		template<typename fn, typename T>
		auto apply(fn&& Func, T&& val) {
			return detail::apply_impl<typename std::decay<T>::type>()(std::forward<fn>(Func), std::forward<T>(val));
		}

		namespace detail{
			template<typename monad, typename value_type, bool IsOmittable = is_same_monad<monad, value_type>::value && is_omittable<monad>::value>
			struct wrap_impl {
				auto operator()(value_type&& v) { return typename monad_traits<monad>::template rebind_t<value_type>(std::move(v)); }
				auto operator()(const value_type& v) { return typename monad_traits<monad>::template rebind_t<value_type>(v); }
			};
			template<typename monad, typename value_type>
			struct wrap_impl<monad, value_type, true> {
				auto operator()(value_type&& v) { return std::move(v); }
				auto operator()(const value_type& v) { return v;}
			};
		}
		template<typename monad, typename value_type>
		auto wrap(value_type&& val) {
			return detail::wrap_impl<monad, value_type>()(std::forward<value_type>(val));
		}

		namespace detail {
			template<typename monad, bool IsOmittable = is_nested_monad<monad>::value && is_omittable<monad>::value>
			struct omit_impl {
				decltype(auto) operator()(monad&& m) { return std::move(m); }
				decltype(auto) operator()(const monad& m) { return m; }
			};
			template<typename monad>
			struct omit_impl<monad, true> {
				using upper_monad = typename monad_traits<monad>::value_type;
				auto operator()(monad&& m) { return omit_impl<upper_monad>()(upper_monad(std::move(m))); }
				auto operator()(const monad& m) { return omit_impl<upper_monad>()(upper_monad(m)); }
			};
		}
		template<typename monad>
		decltype(auto) omit(monad&& m) {
			return omit_impl<monad>()(std::forward<monad>(m));
		}

		namespace detail{
			template<typename monad, bool IsFlattable = is_nested_monad<monad>::value && is_flattenable<monad>::value>
			struct flatten_impl {
				auto operator()(monad&& m) { return std::move(m); }
				auto operator()(const monad& m) { return m; }
			};
			template<typename monad>
			struct flatten_impl<monad, true> {
				using upper_monad = typename monad_traits<monad>::value_type;
				auto operator()(monad&& m) { return flatten_impl<upper_monad>()(upper_monad(std::move(m))); }
				auto operator()(const monad& m) { return flatten_impl<upper_monad>()(upper_monad(m)); }
			};
		}
		template<typename monad>
		auto flatten(monad&& m) {
			return flatten_impl<monad>(std::forward<monad>(m));
		}

		template<typename fn>
		struct as_is_wrapper{
		public:
			fn Fn;
		public:
			as_is_wrapper(const fn& Fn_) :Fn(Fn_) {}
			as_is_wrapper(fn&& Fn_) :Fn(std::move(Fn_)){}
		};
		template<typename fn>
		as_is_wrapper<fn> as_is(fn&& Fn){
			return as_is_wrapper<fn>(std::forward<fn>(Fn));
		}

		struct evaluate_t {}static const eval;
		template<typename monad, typename fn>
		struct lazy_evaluator {
		private:
			using this_type = lazy_evaluator<monad, fn>;
			using arg_type = typename monad_traits<monad>::base_type;
		private:
			monad M;
			fn Fn;
		public:
			lazy_evaluator(monad M_, fn Fn_):M(std::move(M_)),Fn(std::move(Fn_)){}
			auto evaluate()const { return std::move(M.apply(Fn)); }
			friend auto operator >> (const this_type& This, evaluate_t) {
				return std::move(This.evaluate());
			}
			friend auto operator >> (this_type&& This, evaluate_t) {
				return std::move(This.evaluate());
			}
			template<typename nfn>
			friend auto operator >> (const this_type& This, const nfn& NFn) {
				return make_lazy_evaluator(This.M, [Fn = This.Fn, NFn](arg_type v)->decltype(auto) {return apply(NFn, Fn(v)); });
			}
			template<typename nfn>
			friend auto operator >> (this_type&& This, const nfn& NFn) {
				return make_lazy_evaluator(std::move(This.M), [Fn=This.Fn, NFn](arg_type v)->decltype(auto) {return apply(NFn, Fn(v)); });
			}
		};
		template<typename monad, typename fn>
		struct lazy_evaluator<monad, as_is_wrapper<fn> >{
		private:
			using this_type = lazy_evaluator<monad, fn>;
			using arg_type = typename monad_traits<monad>::base_type;
		private:
			monad M;
			fn Fn;
		public:
			lazy_evaluator(monad M_, as_is_wrapper<fn> AsIsFn_) :M(std::move(M_)), Fn(std::move(AsIsFn.Fn)) {}
			auto evaluate()const { return std::move(M.apply(Fn)); }
			friend auto operator >> (const this_type& This, evaluate_t) {
				return std::move(This.evaluate());
			}
			friend auto operator >> (this_type&& This, evaluate_t) {
				return std::move(This.evaluate());
			}
			template<typename nfn>
			friend auto operator >> (const this_type& This, const nfn& NFn) {
				return make_lazy_evaluator(This.M, [Fn = This.Fn, NFn](arg_type v)->decltype(auto) {return NFn(evaluate(Fn(v))); });
			}
			template<typename nfn>
			friend auto operator >> (this_type&& This, const nfn& NFn) {
				return make_lazy_evaluator(std::move(This.M), [Fn = This.Fn, NFn](arg_type v)->decltype(auto) {return NFn(evaluate(Fn(std::move(v)))); });
			}
		};
		template<typename monad,typename fn>
		auto make_lazy_evaluator(monad&& M, fn&& Fn) { return lazy_evaluator<typename remove_cvref<monad>::type, typename remove_cvref<fn>::type>(std::forward<monad>(M),std::forward<fn>(Fn)); }
		namespace detail{
			template<typename T>
			struct evaluate_impl{
				T&& operator()(T&& val){ return val; }
				const T& operator()(const T& val){ return val; }
			};
			template<typename monad, typename fn>
			struct evaluate_impl<lazy_evaluator<monad, fn>>{
				using T = lazy_evaluator<monad, fn>;
				auto operator()(T&& val){ return val.evaluate(); }
				auto operator()(const T& val){ return val.evaluate();}
			};
		}
		template<typename T>
		decltype(auto) evaluate(T&& val){ return detail::evaluate_impl<T>()(std::forward<T>(val)); }

		template<typename T>
		struct identity {
			using monad_category = monad_categories::omittable_monad_tag;
			using value_type = T;
		private:
			using this_type = identity<T>;
		public:
			template<typename U>
			struct rebind {
				using type = identity<U>;
			};
		private:
			T val;
		public:
			/*			identity() = default;
			identity(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			identity(this_type&&)= default;
			this_type& operator=(this_type&&) = default;*/
			identity() :val() {
				std::cout << "construct" << std::endl;
			}
			identity(const this_type& other) :val(other.val) {
				std::cout << "construct with copy\t" <<val<< std::endl;
			}
			this_type& operator=(const this_type& other) {
				std::cout << "copy\t" << other.val << std::endl;
				if (this != &other) {
					val = other.val;
				}
				return *this;
			}
			identity(this_type&& other) noexcept:val(std::move(other.val)) {
				std::cout << "construct with move\t" << val << std::endl;
			}
			this_type& operator=(this_type&& other)noexcept {
				std::cout << "move\t" << other.val<<std::endl;
				if (this != &other) {
					val = std::move(other.val);
				}
				return *this;
			}
		public:
			//for monad
			identity(T val_) :val(std::move(val_)) {
				std::cout << "construct with value\t" << val<<std::endl;
			}
			//for omitable_monad (explicit is better for flattenable monad)
			identity(identity<identity<T>> m_) :val(std::move(m_.val.val)) {
				std::cout << "construct with omit\t" << val<<std::endl;
			}
		public:
			//for monad
			template<typename fn>
			auto apply(fn&& Func)const {
				std::cout << "\tapply" << std::endl;
				return wrap<this_type>(hmLib::functional::apply(Func, val));
			}
		public:
			//for monad
			template<typename fn>
			friend auto operator >> (this_type&& This, fn&& Func) {
				std::cout << "\tmove operator>>\n";
				return make_lazy_evaluator(std::move(This),std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator >> (const this_type& This, fn&& Func) {
				std::cout << "\tcopy operator>>\n";
				return make_lazy_evaluator(This, std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator|(this_type&& This, fn&& Func){
				std::cout << "\tmove operator|\n";
				return make_lazy_evaluator(std::move(This), as_is(std::forward<fn>(Func)));
			}
			//for monad
			template<typename fn>
			friend auto operator|(const this_type& This, fn&& Func){
				std::cout << "\tcopy operator|\n";
				return make_lazy_evaluator(This, as_is(std::forward<fn>(Func)));
			}
		public:
			T& get() { return val; }
			const T& get()const { return val; }
			void set(T val_) { val = std::move(val_); }
		};
	}
}

int ftoi(double f) { return static_cast<int>(f * 10); }
hmLib::functional::identity<double> itod(int i) { return hmLib::functional::identity<double>(1.0 / i); }
double mysin(double v) { return std::sin(v); }
auto ans = itod(ftoi(5.5f));

//auto Ans = identity<float>(5.5) >> ftoi >> itod >> eval;

struct hoge{
	hoge() = default;
	hoge(const hoge&) = delete;
	hoge& operator=(const hoge&) = delete;
	hoge(hoge&&) = default;
	hoge& operator=(hoge&&) = default;
	friend std::ostream& operator << (std::ostream& out, const hoge&h) { return out; }
};
int main() {
	using namespace hmLib::functional;

	auto a = identity<double>(5.6);
	std::cout << typeid(a).name() << std::endl;
	std::cout << is_monad<identity<double>>::value << std::endl;
	std::cout << is_omittable < decltype(a)>::value << std::endl;
	std::cout << is_flattenable < decltype(a)>::value << std::endl;
	std::cout << is_same_monad < decltype(a), identity<double>>::value << std::endl;
	std::cout << is_same_monad <identity<double>, identity<int>>::value << std::endl;
	std::cout << std::is_same <identity<double>, identity<int>>::value << std::endl;

	std::cout << "=== for b ===" << std::endl;
	auto bl = identity<double>(3.4) 
		>> [](double v)->double {return v * 2; } 
		>> [](double v)->int {return static_cast<int>(v * 10); }
		>> itod
		>> [](double v)->double {return std::sqrt(v); }
		>> mysin
		>> ftoi;
	std::cout << "--- eval bl ---" << std::endl;
	auto b = bl >> eval;

	std::cout << typeid(b).name() << std::endl;
	std::cout << b.get()<<std::endl;

	std::cout << "=== for c ===" << std::endl;
	auto c = identity<hoge>() >> [](hoge v)->hoge{return hoge(); };
	std::cout << typeid(c).name() << std::endl;

	system("pause");

	return 0;
}
