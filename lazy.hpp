#ifndef HMLIB_LAZY_INC
#define HMLIB_LAZY_INC 100
#
#include<utility>
#include<tuple>
#include<functional>
#include<boost/variant.hpp>
#include"tuple.hpp"
namespace hmLib {
	template<typename eval_func_, typename... arg_type_>
	struct lazy {
	private:
		using this_type = lazy<eval_func_, arg_type_...>;
		using return_type = decltype(std::declval<eval_func_>()(std::declval<arg_type_>()...));
		using eval_func = eval_func_;
		using arg_tuple = std::tuple<arg_type_...>;
		using eval_type = std::pair<eval_func, arg_tuple>;
		using variant = boost::variant<return_type, eval_type>;
	private:
		mutable variant Var;
	public:
		lazy() = default;
		explicit lazy(return_type val):Var(val) {}
		this_type& operator=(return_type val) {
			Var = val;
		}
		lazy(eval_func_ Eval_, arg_type_... Args_):Var(eval_type(std::move(Eval_), arg_tuple(Args_...))) {}
		void reset(return_type val) { Var = val; }
		void reset(eval_func_ Eval_, arg_type_... Args_) { Var = eval_type(std::move(Eval_), arg_tuple(Args_...)); }
	public:
		operator return_type&()& noexcept {
			return get();
		}
		operator const return_type&()const& noexcept {
			return get();
		}
		operator return_type&&()&& noexcept {
			return static_cast<this_type&&>(*this).get();
		}
		return_type& get()& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = tuple_apply(p->first, p->second);
			}
			return *boost::get<return_type>(&Var);
		}
		const return_type& get()const& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = tuple_apply(p->first, p->second);
			}
			return *boost::get<const return_type>(&Var);
		}
		return_type&& get()&& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = tuple_apply(p->first, p->second);
			}
			return std::move(*boost::get<return_type>(&Var));
		}
		bool is_evaluated()const {
			if(boost::get<const eval_type>(&Var)) {
				return false;
			}
			return true;
		}
	};
	template<typename eval_func_>
	struct lazy<eval_func_> {
	private:
		using this_type = lazy<eval_func_>;
		using return_type = decltype(std::declval<eval_func_>()());
		using eval_func = eval_func_;
		using eval_type = eval_func;
		using variant = boost::variant<return_type, eval_type>;
	private:
		mutable variant Var;
	public:
		lazy() = default;
		explicit lazy(return_type val):Var(val) {}
		this_type& operator=(return_type val) {
			Var = val;
		}
		explicit lazy(eval_func_ Eval_):Var(eval_type(std::move(Eval_))) {}
		void reset(return_type val) { Var = val; }
		void reset(eval_func_ Eval_) { Var = eval_type(std::move(Eval_)); }
	public:
		operator return_type&()&noexcept {
			return get();
		}
		operator const return_type&()const& noexcept {
			return get();
		}
		operator return_type&&()&& noexcept {
			return static_cast<this_type&&>(*this).get();
		}
		return_type& get()& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = (*p)();
			}
			return *boost::get<return_type>(&Var);
		}
		const return_type& get()const& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = (*p)();
			}
			return *boost::get<const return_type>(&Var);
		}
		return_type&& get()&& noexcept {
			if(eval_type* p = boost::get<eval_type>(&Var)) {
				Var = (*p)();
			}
			return std::move(*boost::get<return_type>(&Var));
		}
		bool is_evaluated()const {
			if(boost::get<const eval_type>(&Var)) {
				return false;
			}
			return true;
		}
	};

	template<typename eval_func, typename... arg_type>
	auto later(eval_func&& Eval, arg_type... Args) {
		return lazy<eval_func, typename std::decay<arg_type>::type...>(std::forward<eval_func>(Eval),std::forward<arg_type>(Args)...);
	}

	template<typename eval_func_>
	struct lazy_function_wrapper {
	private:
		using eval_func = eval_func_;
	private:
		eval_func Fn;
	public:
		lazy_function_wrapper() = delete;
		lazy_function_wrapper(const eval_func& Fn_):Fn(Fn_) {}
		lazy_function_wrapper(eval_func&& Fn_):Fn(std::move(Fn_)) {}
		template<typename... arg_type>
		auto operator()(arg_type... Args)noexcept {
			return later(Fn, Args...)
		}
	};
	template<typename fn>
	auto as_lazy_function(fn&& Fn) {
		return detail::lazy_wrapper<typename std::decay<fn>::type>(std::forward<fn>(Fn));
	}
}
#
#endif
