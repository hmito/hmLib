#include<utility>
#include<deque>
#include<initializer_list>
#include<functional>
#include<vector>
#include<future>
#include<type_traits>
#include<iostream>
#include<boost/optional.hpp>
#include<cmath>
//todo
//	繰り返し適用（例えばomitの場合、あるメンバー以下にネストされた子メンバーへのomit適用）をどうするか
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

		template<typename Pred>
		struct negate_of: public std::is_same<typename Pred::type, std::false_type> {};
	}
}
//monad traits
namespace hmLib{
	namespace monad {
		namespace monad_categories {
			struct immutable_monad_tag {};
			struct omittable_monad_tag {};
			struct flattenable_monad_tag {};
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
		template<typename T>
		using is_not_monad = functional::negate_of<is_monad<T>>;

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
		template<typename M, bool IsNonMonad = !is_monad<M>::value>
		struct monad_traits {
			using monad_category = typename M::monad_category;
			using value_type = typename M::value_type;
			using base_type = typename detail::monad_base<value_type>::type;
			template<typename other>
			using rebind_t = typename M::template rebind<other>::type;
		};
		template<typename M>
		struct monad_traits<M, true> {
			using monad_category = nullptr_t;
			using value_type = nullptr_t;
			using base_t = M;
		};

		template<typename T, typename U>
		struct is_same_monad {
		private:
			template<typename V, typename W>
			static auto check(V, W)->std::bool_constant<is_monad<V>::value && std::is_same<W, typename monad_traits<V>::template rebind_t<typename monad_traits<W>::value_type>>::value>;
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<T>(), std::declval<U>()));
			static constexpr const bool value = type::value;
		};
		template<typename M>
		struct is_nested_monad : is_same_monad<M, typename monad_traits<M>::value_type> {};
		template<typename M>
		struct is_omittable : std::is_same<typename monad_traits<M>::monad_category, monad_categories::omittable_monad_tag> {};
		template<typename M>
		struct is_flattenable : std::bool_constant<std::is_same<typename monad_traits<M>::monad_category, monad_categories::flattenable_monad_tag>::value || is_omittable<M>::value> {};
	}
}
//fmap
namespace hmLib {
	namespace monad {
		template<typename T>
		using default_fmap_target = is_not_monad<T>;

		template<typename fn>
		struct fmap_target {
		private:
			template<typename T, typename efn, typename V = typename efn::template fmap_target<std::nullptr_t>>
			static auto check_impl(efn)->typename efn::template fmap_target<T>;
			template<typename T>
			static auto check_impl(...)->default_fmap_target<T>;
		public:
			template<typename T>
			using type = decltype(check_impl<T>(std::declval<typename std::decay<fn>::type>()));
			template<typename T>
			static constexpr bool check() { return type<T>::value; }
			template<typename T>
			constexpr bool operator()(T&&)const { return type<T>::value; }
		};

		namespace detail {
			template<template<typename> typename context_, typename fn>
			struct for_context_wrapper {
				template<typename T>
				using fmap_target = context_<T>;
				const fn& Fn;
			public:
				for_context_wrapper(const fn& Fn_) :Fn(Fn_) {}
				template<typename T>
				auto operator()(T&& Val) { return Fn(std::forward<T>(Val)); }
			};
		}
		template<template<typename> typename context, typename fn>
		auto for_context(fn&& Fn) {
			return detail::for_context_wrapper<context, typename std::decay<fn>::type>(std::forward<fn>(Fn));
		}

		namespace detail {
			template<typename T>
			using for_top_context = std::true_type;
		}
		template<typename fn>
		auto for_top(fn&& Fn) { return for_context<detail::for_top_context>(Fn); }

		namespace detail {
			template<typename eT, typename efn, bool IsTarget = fmap_target<efn>::check<eT>(), bool IsMonad = is_monad<eT>::value>
			struct fmap_impl {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return Fn(std::forward<T>(val)); }
			};
			template<typename eT, typename efn>
			struct fmap_impl<eT, efn, false, true> {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return val.apply(std::forward<fn>(Fn)); }
			};
			template<typename eT, typename efn>
			struct fmap_impl<eT, efn, false, false> {
				template<typename T, typename fn>
				auto operator()(T&& val, fn&& Fn) { return std::forward<T>(val); }
			};
		}
		template<typename T, typename fn>
		auto fmap(T&& val, fn&& Func) {
			return detail::fmap_impl<typename std::decay<T>::type, typename std::decay<fn>::type>()(std::forward<T>(val), std::forward<fn>(Func));
		}
	}
}
//function
namespace hmLib {
	namespace monad {
		namespace detail {
			template<typename arg_type, typename fn1st, typename fn2nd>
			struct is_continuous_fmap;

			namespace impl {
				template<typename arg_type, typename fn1st, typename fn2nd,
					typename mid_type = typename std::decay<decltype(std::declval<fn1st>()(std::declval<arg_type>()))>::type,
					bool can_fmap_fn1st = fmap_target<fn1st>::check<arg_type>(),
					bool can_fmap_fn2nd = fmap_target<fn2nd>::check<mid_type>()
				>struct is_continuous_fmap_impl :public std::false_type {};
				template<typename arg_type, typename mid_type, typename fn1st, typename fn2nd>
				struct is_continuous_fmap_impl<arg_type, fn1st, fn2nd, mid_type, true, true> : public std::true_type {};
				template<typename arg_type, typename mid_type, typename fn1st, typename fn2nd>
				struct is_continuous_fmap_impl<arg_type, fn1st, fn2nd, mid_type, false, false> : public is_continuous_fmap<typename monad_base<arg_type>::type, fn1st, fn2nd> {};
			}

			template<typename arg_type, typename fn1st, typename fn2nd>
			struct is_continuous_fmap : public impl::is_continuous_fmap_impl<arg_type, fn1st, fn2nd> {};
			template<typename fn1st, typename fn2nd>
			struct is_continuous_fmap<void, fn1st, fn2nd> : public std::false_type {};

		}

		template<typename arg_type_, typename fn_, typename prevfn_>
		struct function {
		private:
			using this_type = function<arg_type_, fn_, prevfn_>;
			using fn = fn_;
			using prevfn = prevfn_;
		public:
			using arg_type = arg_type_;
			using result_type = decltype(fmap(std::declval<prevfn>()(std::declval<arg_type >()), std::declval<fn>()));
		private:
			const fn Fn;
			const prevfn PrevFn;
		public:
			function() = default;
			function(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			function(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			function(fn Fn_, prevfn PrevFn_) : Fn(std::move(Fn_)), PrevFn(std::move(PrevFn_)) {}
		public:
			auto operator()(arg_type&& Val)const { fmap(PrevFn(std::move(Val)), Fn); }
			auto operator()(const arg_type& Val)const { fmap(PrevFn(Val), Fn); }
		public:
			template<typename nfn>
			friend auto operator >> (const this_type& This, nfn&& NFn) {
				return detail::make_composite_function<arg_type>(This.Fn, std::forward<nfn>(NFn), This.PrevFn);
			}
			template<typename nfn>
			friend auto operator >> (this_type&& This, nfn&& NFn) {
				return detail::make_composite_function<arg_type>(std::move(This.Fn), std::forward<nfn>(NFn), std::move(This.PrevFn));
			}
		};
		template<typename arg_type_, typename fn_>
		struct function <arg_type_, fn_, void> {
		private:
			using this_type = function<arg_type_, fn_, void>;
			using fn = fn_;
		public:
			using arg_type = arg_type_;
			using result_type = decltype(fmap(std::declval<arg_type >(), std::declval<fn>()));
		private:
			const fn Fn;
		public:
			function() = default;
			function(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			function(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit function(fn Fn_) :Fn(std::move(Fn_)) {}
		public:
			auto operator()(arg_type&& Val)const { fmap(std::move(Val), Fn); }
			auto operator()(const arg_type& Val)const { fmap(Val, Fn); }
		public:
			template<typename nfn>
			friend auto operator >> (const this_type& This, nfn&& NFn) {
				return detail::make_composite_function<arg_type>(This.Fn, std::forward<nfn>(NFn));
			}
			template<typename nfn>
			friend auto operator >> (this_type&& This, nfn&& NFn) {
				return detail::make_composite_function<arg_type>(std::move(This.Fn), std::forward<nfn>(NFn));
			}
		};

		template<typename arg_type, typename fn>
		auto make_function(fn&& Fn) {
			return function<arg_type, typename std::decay<fn>::type, void>(std::forward<fn>(Fn));
		}
		template<typename arg_type, typename fn, typename base_fn>
		auto make_function(fn&& Fn, base_fn&& BaseFn) {
			return function<arg_type, typename std::decay<fn>::type, typename std::decay<base_fn>::type>(std::forward<fn>(Fn), std::forward<base_fn>(BaseFn));
		}

		namespace detail {
			namespace impl {
				template<typename arg_type, typename efn1st, typename efn2nd, bool IsContinuous = is_continuous_fmap<arg_type, efn1st, efn2nd>::value>
				struct make_composite_functon_impl {
					template<typename fn1st, typename fn2nd>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2) {
						return make_function(
							for_context<typename fmap_target<efn1st>::type>(
								[Fn1 = std::forward<fn1st>(Fn1), Fn2 = std::forward<fn2nd>(Fn2)](const arg_type& v) {return Fn2(Fn1(v)); }
						)
						);
					}
					template<typename fn1st, typename fn2nd, typename base_fn>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2, base_fn&& BaseFn) {
						return make_function(
							for_context<typename fmap_target<efn1st>::type>(
								[Fn1 = std::forward<fn1st>(Fn1), Fn2 = std::forward<fn2nd>(Fn2)](const arg_type& v) {return Fn2(Fn1(v)); }
						),
							std::forward<base_fn>(BaseFn)
							);
					}
				};
				template<typename arg_type, typename efn1st, typename efn2nd>
				struct make_composite_functon_impl<arg_type, efn1st, efn2nd, false> {
					template<typename fn1st, typename fn2nd>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2) {
						return make_function(
							std::forward<fn2nd>(Fn2),
							[Fn1 = std::forward<fn1st>(Fn1)](const arg_type& v) {return fmap(v, Fn1); }
						)
					}
					template<typename fn1st, typename fn2nd, typename base_fn>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2, base_fn&& BaseFn) {
						return make_function(
							std::forward<fn2nd>(Fn2),
							[Fn1 = std::forward<fn1st>(Fn1), BaseFn = std::forward<base_fn>(BaseFn)](const arg_type& v) {return fmap(BaseFn(v), Fn1); }
						)
					}
				};
			}
			template<typename arg_type, typename fn1st, typename fn2nd>
			auto make_composite_function(fn1st&& Fn1, fn2nd&& Fn2) {
				return impl::make_composite_functon_impl<arg_type, typename std::decay<fn1st>::type, typename std::decay<fn2nd>::type>()(
					std::forward<fn1st>(Fn1),
					std::forward<fn2nd>(Fn2)
				);
			}
			template<typename arg_type, typename fn1st, typename fn2nd, typename base_fn>
			auto make_composite_function(fn1st&& Fn1, fn2nd&& Fn2, base_fn&& BaseFn) {
				return impl::make_composite_functon_impl<arg_type, typename std::decay<fn1st>::type, typename std::decay<fn2nd>::type>()(
					std::forward<fn1st>(Fn1),
					std::forward<fn2nd>(Fn2),
					std::forward<base_fn>(BaseFn)
				);
			}
		}
	}
}
//evaluate
namespace hmLib {
	namespace monad {
		namespace detail {
			template<typename T>
			struct has_evaluate_function {
			private:
				template<typename eT>
				auto check(eT&& v)->decltype(v.evaluate(), std::true_type);
				auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<T>()));
				static constexpr bool value = type::value;
			};
			template<typename T, bool HasEvaluateFunc = has_evaluate_function<T>::value>
			struct evaluate_impl {
				template<typename U>
				decltype(auto) operator()(U&& val) { return std::forward<U>(val); }
			};
			template<typename T>
			struct evaluate_impl<T, true> {
				template<typename U>
				decltype(auto) operator()(U&& val) { return val.evaluate(); }
			};
		}
		template<typename T>
		decltype(auto) evaluate(T&& val) { return detail::evaluate_impl<typename std::decay<T>::type>()(std::forward<T>(val)); }
		struct do_evaluate {
			template<typename T>
			auto operator()(T&& val) { return evaluate(val); }
		};
	}
}
//fmap_later
namespace hmLib {
	namespace monad {
		namespace detail {
			template<typename T, typename function_>
			struct lazy_fmap {
			private:
				using this_type = lazy_fmap<T, function_>;
				using this_function = function_;
			private:
				const T& Ref;		//Reference Capture
				this_function Fn;	//Applier
			public:
				lazy_fmap(const T& Ref_, const this_function& Fn_) :Ref(Ref_), Fn(Fn_) {}
				lazy_fmap(const T& Ref_, this_function&& Fn_): Ref(Ref_), Fn(std::move(Fn_)) {}
				auto evaluate()const { return Fn(Ref); }
				template<typename nfn>
				friend auto operator >> (const this_type& This, nfn&& NFn) {
					return lazy_fmap_binding<typename std::decay<nfn>::type>()(This.Ref, std::forward<nfn>(NFn), This.Fn);
				}
			};
			template<typename efn>
			struct lazy_fmap_binding {
				template<typename T, typename fn, typename function>
				auto operator()(const T& Ref, fn&& NFn, const function& Func) {
					return fmap_later(Ref, Func >> NFn);
				}
			};
			template<>
			struct lazy_fmap_binding<do_evaluate> {
				template<typename T, typename fn, typename function>
				auto operator()(const T& Ref, fn&& NFn, const function& Func) {
					return Func(Ref);
				}
			};
		}
		namespace detail {
			template<typename efn>
			struct fmap_later_impl {
				template<typename T, typename fn>
				auto operator()(const T& val, fn&& Fn) {
					return detail::lazy_fmap<T, typename std::decay<decltype(make_function<T>(Fn))>::type>(val, make_function<T>(Fn));
				}
			};
			template<typename arg_type_, typename fn_, typename prevfn_>
			struct fmap_later_impl<function<arg_type_,fn_,prevfn_>>{
				template<typename T, typename fn>
				auto operator()(const T& val, fn&& Fn) {
					return detail::lazy_fmap<T, fn>(val, std::forward<fn>(Fn));
				}
			};
		}
		template<typename T, typename fn>
		auto fmap_later(const T& val, fn&& Fn) {
			return detail::fmap_later_impl<typename std::decay<fn>::type>()(val, std::forward<fn>(Fn));
		}
	}
}
//monad_mixin
namespace hmLib{
	namespace monad{
		namespace detail {
			template<typename M, bool IsOmittable = is_nested_monad<M>::value && is_omittable<M>::value>
			struct omit_impl {
				template<typename T>
				decltype(auto) operator()(T&& m) { return std::forward<T>(m); }
			};
			template<typename M>
			struct omit_impl<M, true> {
				using upper_monad = typename monad_traits<M>::value_type;
				template<typename T>
				auto operator()(T&& m) { return omit_impl<upper_monad>()(upper_monad::flatten(std::forward<T>(m))); }
			};
		}
		template<typename M>
		decltype(auto) omit(M&& m) {
			return detail::omit_impl<typename std::decay<M>::type>()(std::forward<M>(m));
		}
		struct do_omit {
			template<typename T>
			using fmap_target  = is_omittable<T>;
			template<typename T>
			decltype(auto) operator()(T&& v) const { return omit(std::forward<T>(v)); }
		};

		namespace detail {
			template<typename M, bool IsFlattable = is_nested_monad<M>::value && is_flattenable<M>::value>
			struct flatten_impl {
				template<typename T>
				decltype(auto) operator()(T&& m) { return std::forward<T>(m); }
			};
			template<typename M>
			struct flatten_impl<M, true> {
				using upper_monad = typename monad_traits<M>::value_type;
				template<typename T>
				auto operator()(T&& m) { return flatten_impl<upper_monad>()(upper_monad::flatten(std::forward<T>(m))); }
			};
		}
		template<typename M>
		decltype(auto) flatten(M&& m) {
			return detail::flatten_impl<typename std::decay<M>::type>()(std::forward<monad>(m));
		}
		struct do_flatten {
			template<typename T>
			using fmap_target = is_flattenable<T>;
			template<typename T>
			decltype(auto) operator()(T&& v) const { return flatten(std::forward<T>(v)); }
		};

		namespace detail {
			template<typename M, typename value_type, bool IsOmittable = is_same_monad<M, value_type>::value && is_omittable<M>::value>
			struct omitted_wrap_impl {
				auto operator()(value_type&& v) { return typename monad_traits<M>::template rebind_t<value_type>(std::move(v)); }
				auto operator()(const value_type& v) { return typename monad_traits<M>::template rebind_t<value_type>(v); }
			};
			template<typename M, typename value_type>
			struct omitted_wrap_impl<M, value_type, true> {
				decltype(auto) operator()(value_type&& v) { return omit(std::move(v)); }
				decltype(auto) operator()(const value_type& v) { return omit(v); }
			};
		}
		template<typename M, typename value_type>
		decltype(auto) omitted_wrap(value_type&& val) {
			return detail::omitted_wrap_impl<typename std::decay<M>::type, typename std::decay<value_type>::type>()(std::forward<value_type>(val));
		}

		template<typename Type, typename value_type_, typename monad_category_>
		struct monad_mixin {
			using monad_category = monad_category_;
			using value_type = value_type_;
		public:
			//for monad
			//template<typename U>
			//struct rebind { using type = Type<U>; };
			//explicit just(T val_) :val(std::move(val_)) {}
			//template<typename fn>
			//auto apply(fn&& Func)const {
			//	return omitted_wrap<this_type>(hmLib::monad::fmap(val, Func));
			//}
		public:
			//for omitable_monad/flattenable_monad
			//static just<T> flatten(const just<just<T>>& m);
		public:
			template<typename fn>
			auto fmap(fn&& Func)const & {
				return monad::fmap(static_cast<const Type&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap(fn&& Func) && {
				return monad::fmap(static_cast<Type&&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap_later(fn&& Func)const & {
				return monad::fmap_later(static_cast<const Type&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap_later(fn&& Func) && {
				return monad::fmap_later(static_cast<Type&&>(*this), std::forward<fn>(Func));
			}
		public:
			//for monad
			template<typename fn>
			friend auto operator >> (Type&& This, fn&& Func) {
				return std::move(This).fmap_later(std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator >> (const Type& This, fn&& Func) {
				return This.fmap_later(std::forward<fn>(Func));
			}
		};
	}
}
//lazy monad
namespace hmLib{
	namespace monad{
		template<typename arg_type, typename eval_func, typename return_type = decltype(std::declval<eval_func>()(std::declval<arg_type>()))>
		struct lazy:public monad::monad_mixin<lazy<arg_type,eval_func, return_type>, arg_type, monad_categories::omittable_monad_tag> {
		private:
			using this_type = lazy<arg_type, eval_func, return_type>;
		private:
			mutable boost::optional<return_type> Opt;
			arg_type Val;
			eval_func Eval;
		public:
			//for monad
			template<typename other_return_type, typename other_arg_type, typename other_eval_func>
			struct rebind { using type = lazy<other_return_type, other_arg_type, other_eval_func>; };
			explicit lazy(arg_type Val_) :Opt(), Val(std::move(Val_)), Eval() {}
			lazy(arg_type Val_, eval_func Eval_) :Opt(), Val(std::move(Val_)), Eval(std::move(Eval_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				return omitted_wrap<this_type>(hmLib::monad::fmap(get(), Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			template<typename eval_func1, typename return_type1, typename eval_func2, typename return_type2>
			static auto flatten(const lazy<lazy<arg_type, eval_func1, return_type1>, eval_func2, return_type2>& m) {
				return make_lazy(m.Val.Val, [Fn1 = m.Val.get_eval(), Fn2 = m.get_eval()](const arg_type& v){return static_cast<return_type2>(Fn2(Fn1(v))); });
			}
		public:
			operator return_type()const noexcept{
				return get();
			}
			const return_type& get()const noexcept{
				if (!Opt) {
					Opt.set(Eval(Val));
				}
				return Opt.get();
			}
			bool is_evaluated()const { return static_cast<bool>(Opt); }
			const eval_func& get_eval()const { return Eval; }
		};
		template<typename arg_type, typename eval_func>
		auto make_lazy(const arg_type& Val, eval_func&& Eval) {
			return lazy<arg_type, eval_func>(Val, std::forward<eval_func>(Eval));
		}

		namespace detail {
			template<typename arg_type_, typename eval_func_>
			struct later_wrapper {
			private:
				using arg_type = arg_type_;
				using eval_func = eval_func_;
				using return_type = decltype(std::declval<eval_func>()(std::declval<arg_type>()));
			private:
				const eval_func& Fn;
			public:
				auto operator()(const arg_type& Arg)const noexcept{
					return lazy<arg_type, eval_func, return_type>(Arg, Fn);
				}
				auto operator()(arg_type&& Arg)const noexcept{
					return lazy<arg_type, eval_func, return_type>(std::move(Arg), Fn);
				}
			};
		}
		template<typename arg_type, typename fn>
		auto later(const fn& Fn) {
			return detail::later_wrapper<arg_type, typename std::decay<fn>::type>(Fn);
		}
	}
}

//monadic
namespace hmLib{
	namespace monad{
		template<typename T>
		struct monadic_define {
			//monadic context of this type is not defined.
			//auto operator()(T v)const noexcept;
		};
	}
	template<typename T>
	auto monadic(T&& val) { return monad::monadic_define<typename std::decay<T>::type>()(std::forward<T>(val)); }
}

//just monad
namespace hmLib {
	namespace monad {
		template<typename T>
		struct just :public monad_mixin<just<T>, T, monad_categories::omittable_monad_tag> {
		private:
			using this_type = just<T>;
		private:
			T val;
		public:
			just() = default;
			just(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			just(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			//for monad
			template<typename U>
			struct rebind { using type = just<U>; };
			explicit just(const T& val_) :val(val_) {}
			explicit just(T&& val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				return omitted_wrap<this_type>(hmLib::monad::fmap(val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			static just<T> flatten(just<just<T>> m_) {return just(m_.get().get());}
		public:
			T& get() { return val; }
			const T& get()const { return val; }
			void set(T val_) { val = std::move(val_); }
		};

		template<typename T>
		auto make_just(T&& val) { return just<typename std::decay<T>::type>(std::forward<T>(val)); }

		template<>
		struct monadic_define<bool> {
			auto operator()(bool v)const noexcept { return just<bool>(v); }
		};
		template<>
		struct monadic_define<signed char> {
			auto operator()(signed char v)const noexcept { return just<signed char >(v); }
		};
		template<>
		struct monadic_define<unsigned char> {
			auto operator()(unsigned char v)const noexcept { return just<unsigned char>(v); }
		};
		template<>
		struct monadic_define<signed short> {
			auto operator()(signed short v)const noexcept { return just<signed short>(v); }
		};
		template<>
		struct monadic_define<unsigned short> {
			auto operator()(unsigned short v)const noexcept { return just<unsigned short >(v); }
		};
		template<>
		struct monadic_define<signed int> {
			auto operator()(signed int v)const noexcept { return just<signed int>(v); }
		};
		template<>
		struct monadic_define<unsigned int> {
			auto operator()(unsigned int v)const noexcept { return just<unsigned int >(v); }
		};
		template<>
		struct monadic_define<signed long> {
			auto operator()(signed long v)const noexcept { return just<signed long>(v); }
		};
		template<>
		struct monadic_define<unsigned long> {
			auto operator()(unsigned long v)const noexcept { return just<unsigned long >(v); }
		};
		template<>
		struct monadic_define<signed long long> {
			auto operator()(signed long long v)const noexcept { return just<signed long long>(v); }
		};
		template<>
		struct monadic_define<unsigned long long> {
			auto operator()(unsigned long long v)const noexcept { return just<unsigned long long >(v); }
		};
		template<>
		struct monadic_define<float> {
			auto operator()(float v)const noexcept { return just<float>(v); }
		};
		template<>
		struct monadic_define<double> {
			auto operator()(double v)const noexcept { return just<double>(v); }
		};
		template<>
		struct monadic_define<long double> {
			auto operator()(long double v)const noexcept { return just<long double>(v); }
		};
	}
}

//optional monad
namespace hmLib {
	namespace monad {
		template<typename T>
		struct optional :public monad_mixin<optional<T>, T, monad_categories::omittable_monad_tag> {
		private:
			using this_type = optional<T>;
		private:
			boost::optional<T> val;
		public:
			optional() = default;
			optional(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			optional(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit optional(boost::optional<T> val_) :val(std::move(val_)) {}
		public:
			//for monad
			template<typename U>
			struct rebind { using type = optional<U>; };
			explicit optional(T val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				using ans_type = decltype(omitted_wrap<this_type>(hmLib::functional::fmap(std::declval<T>(), Func)));
				if (!val)return ans_type();
				return omitted_wrap<this_type>(hmLib::functional::fmap(*val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			static optional<T> flatten(const optional<optional<T>>& m_){
				if (!m_)return this_type();
				if(!(m_.get()))return this_type();
				return this_type(m_.get().get());
			}
		public:
			operator bool()const { return val; }
			T& get() { return *val; }
			const T& get()const { return *val; }
			void set(T val_) { *val = std::move(val_); }
		};
		template<typename T>
		auto make_optional(T&& val) { return optional<typename std::decay<T>::type>(std::forward<T>(val)); }

		template<typename T>
		struct monadic_define<boost::optional<T>> {
			auto operator()(boost::optional<T> v)const noexcept { return optional<T>(std::move(v)); }
		};
	}
}

//vector monad and others...
namespace hmLib{
	namespace monad{
		template<typename T>
		struct vector :public monad_mixin<vector<T>, T, monad_categories::flattenable_monad_tag> {
		private:
			using this_type = vector<T>;
			using container = std::vector<T>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			std::vector<T> val;
		public:
			vector() = default;
			vector(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			vector(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit vector(std::vector<T> val_) :val(std::move(val_)) {}
			template<typename input_iterator>
			vector(input_iterator Beg, input_iterator End) : val(Beg, End) {}
			vector(std::initializer_list<T> IL) :vector(std::begin(IL), std::end(IL)) {}
		public:
			//for monad
			template<typename U>
			struct rebind { using type = vector<U>; };
			explicit vector(T val_) :val(1, std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				using ans_type = decltype(hmLib::monad::fmap(std::declval<T>(), Func));
				std::vector<ans_type> Applied(val.size());

				auto Out = std::begin(Applied);
				for (const auto& v : val) *(Out++) = hmLib::monad::fmap(v, Func);
				return vector<ans_type>(std::move(Applied));
			}
		public:
			//for omitable_monad/flattenable_monad
			explicit vector(vector<vector<T>> m_) {
				for (auto&& v : m_.val) {
					val.insert(v.end(), std::begin(v), std::end(v));
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
		public:
			void push_back(const T& e) { val.push_back(e); }
			void emplace_back(T&& e) { val.emplace_back(e); }
		};
		template<typename T>
		auto make_vector(T&& val) { return vector<typename std::decay<T>::type>(std::forward<T>(val)); }
		template<typename input_iterator>
		auto make_vector(input_iterator Begin, input_iterator End) { return vector<typename std::decay<decltype(*Begin)>::type>(Begin, End); }

		template<typename T>
		struct future {
			std::future<T> Future;
		};

		template<typename T,typename TransformRule>
		struct transform{};
	}
}

int main(void) {
	{
		hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>> v1;

		auto v2 = hmLib::monad::fmap(v1, hmLib::monad::do_omit());

		std::cout << "omit" << std::endl;
		std::cout << "before:" << typeid(v1).name() << std::endl;
		std::cout << "after :" << typeid(v2).name() << std::endl;
	}

	{
		hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>> e1;
		hmLib::monad::vector<hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>>> v1;
		v1.push_back(e1);
		v1.push_back(e1);
		v1.push_back(e1);

		auto v2 = hmLib::monad::fmap(v1, hmLib::monad::do_omit());

		std::cout << "omit" << std::endl;
		std::cout << "before:" << typeid(v1).name() << std::endl;
		std::cout << "after :" << typeid(v2).name() << std::endl;
	}


	{
		auto Fn = [](double v) {return v; };
		auto ForMonad = hmLib::monad::for_context<hmLib::monad::is_monad>(Fn);
		auto ForOmmitable = hmLib::monad::for_context<hmLib::monad::is_omittable>(Fn);
	}

	{
		hmLib::monad::do_omit Do;

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_omit" << std::endl;
		std::cout << "double:" << Check(5.5) << std::endl;
		std::cout << "vector:" << Check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	{
		hmLib::monad::do_flatten Do;

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_flatten" << std::endl;
		std::cout << "double:" << Check(5.5) << std::endl;
		std::cout << "vector:" << Check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	{
		auto Do = [](double v) {return v * 2; };

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_normal" << std::endl;
		std::cout << "double:" << Check(5.5) << std::endl;
		std::cout << "vector:" << Check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	auto Val = hmLib::monadic(4);

	auto Ans = Val >> [](int a) {return a*0.5; } >> [](double f) {return hmLib::monad::make_optional(f); } >> hmLib::monad::do_evaluate();

	std::cout << typeid(Ans).name()<<std::endl;

	system("pause");
	return 0;
}