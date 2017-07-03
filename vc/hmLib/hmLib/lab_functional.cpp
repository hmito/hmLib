#include<iostream>
#include<functional>

namespace hmLib {
	namespace functional {
		namespace detail {
			template<typename...>
			struct is_unary_function : std::false_type {};

			template<typename T, typename R>
			struct is_unary_function<R(*)(T)> : std::true_type {};


			template<typename T,typename Arg_t>
			class is_unary_functor {
			private:
				template<typename U>
				static auto check(U v) -> decltype(v(std::declval<Arg_t>()), std::true_type());
				static auto check(...) -> decltype(std::false_type());
			public:
				typedef decltype(check(std::declval<T>())) type;
				static bool const value = type::value;
			};
		}

		template<typename F>
		auto curry_fn(F f) {
			return [f](auto a) {
				return [f, a](auto... args) {return f(a, args...); };
			};
		}


		template<typename F >
		struct wrap_function {
			F f;
		public:
			wrap_function(F f_) :f(std::move(f_)) {}
			template<typename T, bool IsUnary = detail::is_unary_functor<F, Arg_t>::value>
			auto operator()(T&& a) {
				return [f, a](auto... args) {return f(a, args...); };
			}
		};

		//bool IsUnary = detail::is_unary_functor<Fn, Arg>::value
		template<typename Fn>
		struct recursive_curry_impl {
			Fn f;
		public:
			recursive_curry_impl(Fn f_):f(f_){}
			template<typename T>
			auto operator()(T&& a) {
				if constexpr(detail::is_unary_functor<Fn, Arg>::value) {
					return f(a);
				}
				else constexpr{
					return recursive
				}
			}
		};
		template<typename Fn, typename Arg>
		struct recursive_curry<Fn, Arg, true> {
			Fn f;
		public:
			recursive_curry(Fn f_) :f(f_) {}
			auto operator()(Arg&& a) {
				return f(a);
			}
		};

		template<typename F>
		auto curry(F f) {
			return [f](auto a) {
				return [f, a](auto... args) {return f(a, args...); };
			};
		}
	}
}
int i1func(int a) { return a; }
int i2func(int a, int b) { return a + b; }
int i3func(int a, int b, int c) { return a + b + c; }
struct i1fn {int operator()(int a) { return a; }};
struct i2fn { int operator()(int a,int b) { return a+b; } };
struct i3fn { int operator()(int a, int b, int c) { return a+b+c; } };
int main() {
//	auto F = hmLib::functional::curry<decltype(i3func)>(i3func);
//	auto G = F(3);
	std::cout << hmLib::functional::is_unary_function<decltype(i1func)>::value << std::endl;
	std::cout << hmLib::functional::is_unary_function<i1fn>::value << std::endl;

	system("pause");
	return 0;
}

