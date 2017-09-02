#ifndef HMLIB_MONAD_FUNCTION_INC
#define HMLIB_MONAD_FUNCTION_INC 100
#
#include<utility>
#include"fmap.hpp"
namespace hmLib {
	namespace monad {
		namespace detail {
			template<typename arg_type, typename fn1st, typename fn2nd>
			struct is_continuous_fmap;

			namespace impl {
				template<typename arg_type, typename fn1st, typename fn2nd,
					bool can_fmap_fn1st = fmap_target<fn1st>::check<arg_type>()
				>struct is_continuous_fmap_impl {
				private:
					using mid_type = typename std::decay<decltype(std::declval<fn1st>()(std::declval<arg_type>()))>::type;
				public:
					using type = typename fmap_target<fn2nd>::template is_target<mid_type>;
					static constexpr bool value = type::value;
				};
				template<typename arg_type, typename fn1st, typename fn2nd>
				struct is_continuous_fmap_impl<arg_type, fn1st, fn2nd, false> {
					using type = std::bool_constant<
						(!fmap_target<fn2nd>::template is_target<arg_type>::value)
						&& is_continuous_fmap<typename monad_traits<arg_type>::value_type, fn1st, fn2nd>::value
					>;
					static constexpr bool value = type::value;
				};
			}

			template<typename arg_type, typename fn1st, typename fn2nd>
			struct is_continuous_fmap : public impl::is_continuous_fmap_impl<arg_type, fn1st, fn2nd>::type {};
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
			auto operator()(arg_type&& Val)const { return fmap(PrevFn(std::move(Val)), Fn); }
			auto operator()(const arg_type& Val)const { return fmap(PrevFn(Val), Fn); }
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
			auto operator()(arg_type&& Val)const { return fmap(std::move(Val), Fn); }
			auto operator()(const arg_type& Val)const { return fmap(Val, Fn); }
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
					using farg_type = typename fmap_target<efn1st>::template target_type<arg_type>;
					template<typename fn1st, typename fn2nd>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2) {
						return make_function<arg_type>(
							for_context<typename fmap_target<efn1st>::is_target>(
								[Fn1 = std::forward<fn1st>(Fn1), Fn2 = std::forward<fn2nd>(Fn2)](const farg_type& v) {return Fn2(Fn1(v)); }
						)
							);
					}
					template<typename fn1st, typename fn2nd, typename base_fn>
					auto operator()(fn1st&& Fn1, fn2nd&& Fn2, base_fn&& BaseFn) {
						return make_function(
							for_context<typename fmap_target<efn1st>::is_target>(
								[Fn1 = std::forward<fn1st>(Fn1), Fn2 = std::forward<fn2nd>(Fn2)](const farg_type& v) {return Fn2(Fn1(v)); }
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
#
#endif
