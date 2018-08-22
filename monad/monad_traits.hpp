#ifndef HMLIB_MONAD_MONADTRAITS_INC
#define HMLIB_MONAD_MONADTRAITS_INC 100
#
#include<type_traits>
#include"../type_traits.hpp"
namespace hmLib {
	namespace monad {
		template<typename fn_,typename arg_type_>
		struct later_applier{
			using fn = fn_;
			using arg_type = arg_type_;
			using return_type = decltype(std::declval<fn>()(std::declval<arg_type>()));
		private:
			fn Fn;
			arg_type Arg;
		public:
			template<typename fn_t, typename arg_t>
			later_applier(fn_t&& Fn_, arg_t&& Arg_) :Fn(std::forward<fn_t>(Fn_)), Arg(std::forward<arg_t>(Arg_)){}
			decltype(auto) operator()(void) { return Fn(Arg); }
			template<typename fn_t>
			auto operator&(fn_t&& Fn_)const & {
				return make_later_applier([Fn1 = Fn, Fn2 = std::forward<fn_t>(Fn_)](const arg_type& Arg_) {return Fn2(Fn1(Arg_)); }, Arg);
			}
			template<typename fn_t>
			auto operator&(fn_t&& Fn_)&& {
				return make_later_applier([Fn1 = std::move(Fn), Fn2 = std::forward<fn_t>(Fn_)](const arg_type& Arg_) {return Fn2(Fn1(Arg_)); }, std::move(Arg));
			}
		};
		template<typename fn_t, typename arg_t>
		auto make_later_applier(fn_t&& Fn_, arg_t&& Arg_) {
			return later_applier<typename std::decay<fn_t>::type, typename std::decay<arg_t>::type>(std::forward<fn_t>(Fn_), std::forward<arg_t>(Arg_));
		}

		namespace detail {
			template<typename fn, typename arg>
			struct apply_later_impl {
				template<typename fn_t, typename arg_t>
				auto operator()(fn_t&& Fn_, arg_t&& Arg_) {
					return make_later_applier(std::forward<fn_t>(Fn_), std::forward<arg_t>(Arg_));
				}
			};
			template<typename fn, typename later_fn, typename later_arg>
			struct apply_later_impl<fn, later_applier<later_fn, later_arg>> {
				template<typename fn_t>
				auto operator()(fn_t&& Fn_, const later_applier<later_fn, later_arg>& Arg_) {
					return Arg_&Fn_;
				}
				template<typename fn_t>
				auto operator()(fn_t&& Fn_, later_applier<later_fn, later_arg>&& Arg_) {
					return std::move(Arg_)&Fn_;
				}
			};
		}
		template<typename fn_t, typename arg_t>
		auto apply_later(fn_t&& Fn_, arg_t&& Arg_) {
			return detail::apply_later_impl<typename std::decay<fn_t>::type, typename std::decay<arg_t>::type>()(std::forward<fn_t>(Fn_), std::forward<arg_t>(Arg_));
		}

		template<typename T>
		struct is_monad {
		private:
			template<typename U, typename V = typename monad_traits::>
			static auto check(U)->std::true_type;
			static auto check(...)->std::false_type;
		public:
			using type = decltype(check(std::declval<T>()));
			static constexpr const bool value = type::value;
		};
		template<typename T>
		using is_not_monad = std::is_same<typename is_monad<T>::type, std::false_type>;

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

		template<typename M>
		struct monad_traits {
			using monad_category = typename M::monad_category;
			using value_type = typename M::value_type;
			using base_type = typename detail::monad_base<value_type>::type;
			template<typename other>
			using rebind_t = typename M::template rebind<other>::type;
		};
		template<typename M>
		struct monad_traits<M, true> {
			using monad_category = void;
			using value_type = void;
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
			using fmap_target = is_omittable<T>;
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
	}
}
#
#endif
