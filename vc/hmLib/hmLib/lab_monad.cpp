#include<utility>
#include<deque>
#include<initializer_list>
#include<functional>
#include<vector>
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
			template<typename eT, bool IsMonad = is_monad<eT>::value>
			struct apply_impl {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return Fn(std::forward<T>(val)); }
			};
			template<typename eT>
			struct apply_impl<eT, true> {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return val.apply(std::forward<fn>(Fn)); }
			};
		}
		template<typename T, typename fn>
		auto apply(T&& val, fn&& Func) {
			return detail::apply_impl<typename std::decay<T>::type>()(std::forward<T>(val), std::forward<fn>(Func));
		}

		namespace detail {
			template<typename monad, bool IsOmittable = is_nested_monad<monad>::value && is_omittable<monad>::value>
			struct omit_impl {
				template<typename T>
				decltype(auto) operator()(T&& m) { return std::forward<T>(m); }
			};
			template<typename monad>
			struct omit_impl<monad, true> {
				using upper_monad = typename monad_traits<monad>::value_type;
				template<typename T>
				auto operator()(T&& m) { return omit_impl<upper_monad>()(upper_monad(std::forward<T>(m))); }
			};
		}
		template<typename monad>
		decltype(auto) omit(monad&& m) {
			return detail::omit_impl<monad>()(std::forward<monad>(m));
		}
		struct do_omit {
			template<typename T>
			decltype(auto) operator()(T&& v) const { return omit(std::forward<T>(v)); }
		};

		namespace detail{
			template<typename monad, bool IsFlattable = is_nested_monad<monad>::value && is_flattenable<monad>::value>
			struct flatten_impl {
				template<typename T>
				decltype(auto) operator()(T&& m) { return std::forward<T>(m); }
			};
			template<typename monad>
			struct flatten_impl<monad, true> {
				using upper_monad = typename monad_traits<monad>::value_type;
				template<typename T>
				auto operator()(T&& m) { return flatten_impl<upper_monad>()(upper_monad(std::forward<T>(m))); }
			};
		}
		template<typename monad>
		decltype(auto) flatten(monad&& m) {
			return detail::flatten_impl<monad>()(std::forward<monad>(m));
		}
		struct do_flatten {
			template<typename T>
			decltype(auto) operator()(T&& v) const { return flatten(std::forward<T>(v)); }
		};

		namespace detail {
			template<typename monad, typename value_type, bool IsOmittable = is_same_monad<monad, value_type>::value && is_omittable<monad>::value>
			struct omitted_wrap_impl {
				auto operator()(value_type&& v) { return typename monad_traits<monad>::template rebind_t<value_type>(std::move(v)); }
				auto operator()(const value_type& v) { return typename monad_traits<monad>::template rebind_t<value_type>(v); }
			};
			template<typename monad, typename value_type>
			struct omitted_wrap_impl<monad, value_type, true> {
				decltype(auto) operator()(value_type&& v) { return omit(std::move(v)); }
				decltype(auto) operator()(const value_type& v) { return omit(v); }
			};
		}
		template<typename monad, typename value_type>
		decltype(auto) omitted_wrap(value_type&& val) {
			return detail::omitted_wrap_impl<monad, value_type>()(std::forward<value_type>(val));
		}

		template<typename fn>
		struct as_is_wrapper{
		public:
			fn Fn;
		public:
			as_is_wrapper(fn Fn_) :Fn(std::move(Fn_)){}
		};
		template<typename fn>
		auto as_is(fn&& Fn){
			return as_is_wrapper<typename std::decay<fn>::type>(std::forward<fn>(Fn));
		}

		template<typename T, typename fn>
		struct lazy_apply {
		private:
			using this_type = lazy_apply<T, fn>;
		private:
			T Val;
			fn Fn;
		public:
			lazy_apply(T Val_, fn Fn_):Val(std::move(Val_)),Fn(std::move(Fn_)){}
			auto evaluate()const { return Fn(Val); }
			template<typename nfn>
			friend auto operator >> (const this_type& This, nfn&& NFn) {
				return then(This.Val, std::forward<nfn>(NFn), This.Fn);
			}
			template<typename nfn>
			friend auto operator >> (this_type&& This, nfn&& NFn) {
				return then(std::move(This.Val), std::forward<nfn>(NFn), std::move(This.Fn));
			}
			template<typename nfn>
			friend auto operator | (const this_type& This, nfn&& NFn) {
				return then(This.Val, as_is(std::forward<nfn>(NFn)), This.Fn);
			}
			template<typename nfn>
			friend auto operator | (this_type&& This, nfn&& NFn) {
				return then(std::move(This.Val), as_is(std::forward<nfn>(NFn)), std::move(Fn));
			}
		};
		template<typename T, typename fn>
		auto make_lazy_apply(T&& val, fn&& Fn) {
			return lazy_apply<T, fn>(std::forward<T>(val), std::forward<fn>(Fn));
		}

		namespace detail {
			template<typename T>
			struct evaluate_impl {
				template<typename U>
				decltype(auto) operator()(U&& val) { return std::forward<U>(val); }
			};
			template<typename monad, typename fn>
			struct evaluate_impl<lazy_apply<monad, fn>> {
				template<typename U>
				auto operator()(U&& val) { return val.evaluate(); }
			};
		}
		template<typename T>
		decltype(auto) evaluate(T&& val) { return detail::evaluate_impl<typename std::decay<T>::type>()(std::forward<T>(val)); }
		struct do_evaluate {
			template<typename T>
			auto operator()(T&& val) { return evaluate(val); }
		};

		namespace detail {
			template<typename eT, typename efn>
			struct then_impl{
				template<typename T, typename fn>
				auto operator()(T&& Val, fn&& Fn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = std::forward<fn>(Fn)](const T& v)->decltype(auto) {return apply(v, Fn); }
					);
				}
				template<typename T, typename fn, typename prior>
				auto operator()(T&& Val, fn&& Fn, prior&& PriorFn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = std::forward<fn>(Fn), PriorFn=std::forward<prior>(PriorFn)](const T& v)->decltype(auto) {return apply(PriorFn(v), Fn); }
					);
				}
			};
			template<typename eT, typename efn>
			struct then_impl<eT, as_is_wrapper<efn>> {
				template<typename T>
				auto operator()(T&& Val, const as_is_wrapper<efn>& AsIsFn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = AsIsFn.Fn](const T& v)->decltype(auto){return Fn(v); }
					);
				}
				template<typename T>
				auto operator()(T&& Val, as_is_wrapper<efn>&& AsIsFn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = std::move(AsIsFn.Fn)](const T& v)->decltype(auto){return Fn(v); }
					);
				}
				template<typename T, typename prior>
				auto operator()(T&& Val, const as_is_wrapper<efn>& AsIsFn, prior&& PriorFn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = AsIsFn.Fn, PriorFn = std::forward<prior>(PriorFn)](const T& v)->decltype(auto){return Fn(PriorFn(v)); }
					);
				}
				template<typename T, typename prior>
				auto operator()(T&& Val, as_is_wrapper<efn>&& AsIsFn, prior&& PriorFn) {
					return make_lazy_apply(
						std::forward<T>(Val),
						[Fn = std::move(AsIsFn.Fn), PriorFn = std::forward<prior>(PriorFn)](const T& v)->decltype(auto){return Fn(PriorFn(v)); }
					);
				}
			};
			template<typename eT>
			struct then_impl<eT, do_evaluate> {
				template<typename T, typename fn>
				auto operator()(T&& Val, fn&& DoEval) {
					return std::forward<T>(Val);
				}
				template<typename T, typename fn, typename prior>
				auto operator()(T&& Val, fn&& DoEval, prior&& PriorFn) {
					return PriorFn(std::forward<T>(Val));
				}
			};
		}
		template<typename T,typename fn>
		auto then(T&& M, fn&& Fn) { return detail::then_impl<typename std::decay<T>::type, typename std::decay<fn>::type>()(std::forward<T>(M),std::forward<fn>(Fn)); }
		template<typename T, typename fn, typename prior>
		auto then(T&& M, fn&& Fn, prior&& PriorFn) { return detail::then_impl<typename std::decay<T>::type, typename std::decay<fn>::type>()(std::forward<T>(M), std::forward<fn>(Fn), std::forward<prior>(PriorFn)); }

		template<typename Type, typename value_type_, typename monad_category_>
		struct monad_mixin {
			using monad_category = monad_categories::omittable_monad_tag;
			using value_type = value_type_;
		public:
			template<typename fn>
			auto then(fn&& Func)& {
				return hmLib::functional::then(static_cast<const Type&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto then(fn&& Func) && {
				return hmLib::functional::then(static_cast<Type&&>(std::move(*this)), std::forward<fn>(Func));
			}
		public:
			//for monad
			template<typename fn>
			friend auto operator >> (Type&& This, fn&& Func) {
				std::cout << "\tmove operator>>\n";
				return hmLib::functional::then(std::move(This), std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator >> (const Type& This, fn&& Func) {
				std::cout << "\tcopy operator>>\n";
				return hmLib::functional::then(This, std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator|(Type&& This, fn&& Func) {
				std::cout << "\tmove operator|\n";
				return hmLib::functional::then(std::move(This), hmLib::functional::as_is(std::forward<fn>(Func)));
			}
			//for monad
			template<typename fn>
			friend auto operator|(const Type& This, fn&& Func) {
				std::cout << "\tcopy operator|\n";
				return hmLib::functional::then(This, hmLib::functional::as_is(std::forward<fn>(Func)));
			}
		};

		template<typename T>
		struct monad {
			using monad_category = monad_categories::omittable_monad_tag;
			using value_type = T;
		private:
			using this_type = monad<T>;
		public:
			template<typename U>
			struct rebind {
				using type = monad<U>;
			};
		private:
			T val;
		public:
			monad() :val() {
				std::cout << "construct" << std::endl;
			}
			monad(const this_type& other) :val(other.val) {
				std::cout << "construct with copy\t" <<val<< std::endl;
			}
			this_type& operator=(const this_type& other) {
				std::cout << "copy\t" << other.val << std::endl;
				if (this != &other) {
					val = other.val;
				}
				return *this;
			}
			monad(this_type&& other) noexcept:val(std::move(other.val)) {
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
			explicit monad(T val_) :val(std::move(val_)) {
				std::cout << "construct with value\t" << val<<std::endl;
			}
			//for omitable_monad (explicit is better for flattenable monad)
			explicit monad(monad<monad<T>> m_) :val(std::move(m_.val.val)) {
				std::cout << "construct with omit\t" << val<<std::endl;
			}
		public:
			//for monad
			template<typename fn>
			auto apply(fn&& Func)const {
				std::cout << "\tapply" << std::endl;
				return omitted_wrap<this_type>(hmLib::functional::apply(val, Func));
			}
			template<typename fn>
			auto then(fn&& Func)&{
				return hmLib::functional::then(*this, std::forward<fn>(Func));
			}
			template<typename fn>
			auto then(fn&& Func)&& {
				return hmLib::functional::then(std::move(*this), std::forward<fn>(Func));
			}
		public:
			//for monad
			template<typename fn>
			friend auto operator >> (this_type&& This, fn&& Func) {
				std::cout << "\tmove operator>>\n";
				return hmLib::functional::then(std::move(This), std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator >> (const this_type& This, fn&& Func) {
				std::cout << "\tcopy operator>>\n";
				return hmLib::functional::then(This, std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator|(this_type&& This, fn&& Func){
				std::cout << "\tmove operator|\n";
				return hmLib::functional::then(std::move(This), hmLib::functional::as_is(std::forward<fn>(Func)));
			}
			//for monad
			template<typename fn>
			friend auto operator|(const this_type& This, fn&& Func){
				std::cout << "\tcopy operator|\n";
				return hmLib::functional::then(This, hmLib::functional::as_is(std::forward<fn>(Func)));
			}
		public:
			T& get() { return val; }
			const T& get()const { return val; }
			void set(T val_) { val = std::move(val_); }
		};

		template<typename T>
		struct identity :public monad_mixin<identity<T>, T, monad_categories::omittable_monad_tag>{
		private:
			using this_type = identity<T>;
		private:
			T val;
		public:
			identity() = default;
			identity(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			identity(this_type&&)= default;
			this_type& operator=(this_type&&) = default;
		public:
			//for monad
			template<typename U>
			struct rebind { using type = identity<U>; };
			explicit identity(T val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				return omitted_wrap<this_type>(hmLib::functional::apply(val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			explicit identity(identity<identity<T>> m_) :val(std::move(m_.val.val)) {}
		public:
			T& get() { return val; }
			const T& get()const { return val; }
			void set(T val_) { val = std::move(val_); }
		};
		template<typename T>
		struct maybe :public monad_mixin<maybe<T>, T, monad_categories::omittable_monad_tag> {
		private:
			using this_type = maybe<T>;
		private:
			boost::optional<T> val;
		public:
			maybe() = default;
			maybe(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			maybe(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//for monad
			template<typename U>
			struct rebind { using type = maybe<U>; };
			explicit maybe(T val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				if (!val)return maybe();
				return omitted_wrap<this_type>(hmLib::functional::apply(*val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			explicit maybe(maybe<maybe<T>> m_) :val(std::move(m_.val.val)) {}
		public:
			operator bool()const { return val; }
			T& get() { return *val; }
			const T& get()const { return *val; }
			void set(T val_) { *val = std::move(val_); }
		};
		template<typename T>
		struct mvector :public monad_mixin<mvector<T>, T, monad_categories::flattenable_monad_tag> {
		private:
			using this_type = mvector<T>;
			using container = std::vector<T>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			std::vector<T> val;
		public:
			mvector() = default;
			mvector(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			mvector(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit mvector(std::vector<T> val_):val(std::move(val_)){}
			template<typename input_iterator>
			mvector(input_iterator Beg, input_iterator End):val(Beg,End){}
		public:
			//for monad
			template<typename U>
			struct rebind { using type = mvector<U>; };
			explicit mvector(T val_) :val(1,std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				using ans_type = decltype(hmLib::functional::apply(std::declval<T>(), Func));
				std::vector<ans_type> Applied(val.size());
				
				auto Out = std::begin(Applied);
				for (const auto& v : val)(*Out)++ = hmLib::functional::apply(v, Func);
				return mvector<ans_type>(std::move(Applied));
			}
		public:
			//for omitable_monad/flattenable_monad
			explicit mvector(mvector<mvector<T>> m_) {
				for (auto&& v : m_.val) {
					val.insert(v.end(),std::begin(v), std::end(v));
				}
			}
		public:
			std::size_t size()const { return val.size(); }
			iterator begin() { return val.begin(); }
			iterator end() { return val.end(); }
			const_iterator cbegin()const { return val.cbegin(); }
			const_iterator cend()const { return val.cend(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
		};
	}
}

int ftoi(double f) { return static_cast<int>(f * 10); }
hmLib::functional::identity<double> itod(int i) { return hmLib::functional::identity<double>(1.0 / i); }
double mysin(double v) { return std::sin(v); }
hmLib::functional::identity<double> idtoid(hmLib::functional::identity<double> v){
	v.set(v.get()*2);
	return std::move(v);
}
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
		>> as_is(idtoid)
		>> ftoi
		>> as_is(do_flatten())
		>> do_evaluate();
	std::cout << "--- eval bl ---" << std::endl;
	auto b = evaluate(bl);

	std::cout << typeid(b).name() << std::endl;
	std::cout << b.get()<<std::endl;

	std::cout << "=== for c ===" << std::endl;
	auto c = identity<hoge>() >> [](const hoge& v)->hoge{return hoge(); };
	std::cout << typeid(c).name() << std::endl;

	maybe<int> Maybe;
	mvector<double> Vec;

	system("pause");

	return 0;
}
