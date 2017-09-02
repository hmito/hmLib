#ifndef HMLIB_MONAD_FMAPLATER_INC
#define HMLIB_MONAD_FMAPLATER_INC 100
#
#include<type_traits>
#include<utility>
//evaluate
namespace hmLib {
	namespace monad {
		namespace detail {
			template<typename T>
			struct has_evaluate_function {
			private:
				template<typename eT>
				static auto check(eT&& v)->decltype(v.evaluate(), std::true_type);
				static auto check(...)->std::false_type;
			public:
				using type = decltype(check(std::declval<T>()));
				static constexpr bool value = type::value;
			};
			template<typename T, bool HasEvaluateFunc = has_evaluate_function<T>::value>
			struct evaluate_impl {
				template<typename U>
				auto operator()(U&& val) { return std::forward<U>(val); }
			};
			template<typename T>
			struct evaluate_impl<T, true> {
				template<typename U>
				auto operator()(U&& val) { return val.evaluate(); }
			};
		}
		template<typename T>
		auto evaluate(T&& val) { return detail::evaluate_impl<typename std::decay<T>::type>()(std::forward<T>(val)); }
		struct do_evaluate {
			template<typename T>
			auto operator()(T&& val) { return evaluate(val); }
		};

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
				lazy_fmap(const T& Ref_, this_function&& Fn_) : Ref(Ref_), Fn(std::move(Fn_)) {}
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
			struct fmap_later_impl<function<arg_type_, fn_, prevfn_>> {
				template<typename T, typename fn>
				auto operator()(const T& val, fn&& Fn) {
					return detail::lazy_fmap<T, fn>(val, std::forward<fn>(Fn));
				}
			};
			template<>
			struct fmap_later_impl<do_evaluate> {
				template<typename T, typename fn>
				auto operator()(const T& val, fn&& Fn) {
					return evaluate(val);
				}
			};
		}
		template<typename T, typename fn>
		auto fmap_later(const T& val, fn&& Fn) {
			return detail::fmap_later_impl<typename std::decay<fn>::type>()(val, std::forward<fn>(Fn));
		}
	}
}
#
#endif
