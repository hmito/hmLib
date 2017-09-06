#ifndef HMLIB_MONAD_LAZY_INC
#define HMLIB_MONAD_LAZY_INC 100
#
#include<functional>
#include<boost/optional.hpp>
namespace hmLib {
	namespace monad {
		template<typename arg_type, typename eval_func, typename return_type = decltype(std::declval<eval_func>()(std::declval<arg_type>()))>
		struct lazy :public monad::monad_mixin<lazy<arg_type, eval_func, return_type>, arg_type, monad_categories::omittable_monad_tag> {
		private:
			using this_type = lazy<arg_type, eval_func, return_type>;
		private:
			mutable boost::optional<return_type> Opt;
			arg_type Val;
			eval_func Eval;
		public:
			//for monad
			template<typename other_arg_type, typename other_eval_func = eval_func, typename other_return_type = return_type>
			struct rebind { using type = lazy<other_return_type, other_eval_func, other_arg_type>; };
//			explicit lazy(arg_type Val_) :Opt(), Val(std::move(Val_)), Eval() {}
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
			operator return_type()const noexcept {
				return get();
			}
			return_type& get()noexcept {
				if (!Opt) {
					Opt=Eval(Val);
				}
				return Opt.get();
			}
			const return_type& get()const noexcept {
				if (!Opt) {
					Opt = Eval(Val);
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
				eval_func Fn;
			public:
				later_wrapper() = delete;
				later_wrapper(const eval_func& Fn_) :Fn(Fn_) {}
				later_wrapper(eval_func&& Fn_) :Fn(std::move(Fn_)) {}
				auto operator()(const arg_type& Arg)const noexcept {
					return lazy<arg_type, eval_func, return_type>(Arg, Fn);
				}
				auto operator()(arg_type&& Arg)const noexcept {
					return lazy<arg_type, eval_func, return_type>(std::move(Arg), Fn);
				}
			};
		}
		template<typename arg_type, typename fn>
		auto later(fn&& Fn) {
			return detail::later_wrapper<arg_type, typename std::decay<fn>::type>(std::forward<fn>(Fn));
		}
	}
}
#
#endif
