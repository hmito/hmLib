#ifndef HMLIB_LAZY_INC
#define HMLIB_LAZY_INC 100
#
#include<utility>
#include<tuple>
#include<functional>
#include<boost/variant.hpp>
#include"tuple.hpp"
namespace hmLib {
	template<typename return_type_, typename... arg_type_>
	struct lazy {
	private:
		using this_type = lazy<return_type_, arg_type_...>;
		using return_type = return_type_;
		using eval_func = std::function<return_type(arg_type_...)>;
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
		template<typename eval_func_>
		lazy(eval_func_ Eval_, arg_type_... Args_):Var(eval_type(std::move(Eval_), arg_tuple(Args_...))) {}
		void reset(return_type val) { Var = val; }
		template<typename eval_func_>
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
	template<typename return_type_>
	struct lazy<return_type_> {
	private:
		using this_type = lazy<return_type_>;
		using return_type = return_type_;
		using eval_func = std::function<return_type()>;
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
		template<typename eval_func_>
		explicit lazy(eval_func_ Eval_):Var(eval_type(std::move(Eval_))) {}
		void reset(return_type val) { Var = val; }
		template<typename eval_func_>
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

	template<typename return_type, typename eval_func, typename... arg_type>
	auto make_lazy(eval_func&& Eval, arg_type&&... Args) {
		return lazy<return_type, typename std::decay<arg_type>::type...>(std::forward<eval_func>(Eval),std::forward<arg_type>(Args)...);
	}

	template<typename eval_func_>
	struct lazy_function {
	private:
		using eval_func = eval_func_;
	private:
		eval_func Fn;
	public:
		lazy_function() = delete;
		lazy_function(const eval_func& Fn_):Fn(Fn_) {}
		lazy_function(eval_func&& Fn_):Fn(std::move(Fn_)) {}
		template<typename... arg_type>
		auto operator()(arg_type... Args) {
			return make_lazy<decltype(Fn(Args...))>(Fn, Args...);
		}
	};
	template<typename fn>
	auto make_lazy_function(fn&& Fn) {
		return lazy_function<typename std::decay<fn>::type>(std::forward<fn>(Fn));
	}
}
#
#endif
