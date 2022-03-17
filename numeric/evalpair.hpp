#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<utility>
#include<type_traits>
namespace hmLib{
	namespace numeric{
		template<typename T>
		struct evalpair{
			using value_type = T;
			T v;
			T f;
		public:
			evalpair()=default;
			explicit evalpair(T x_)noexcept:x(x_),f(){}
			evalpair(T v_, T f_)noexcept:v(v_),f(f_){}
			template<typename F, std::enable_if_t<std::is_invocable_v<F,T>,std::nullptr_t> = nullptr>
			evalpair(F fn, T v_):v(v_),f(fn(v_)){}
			void set(T v_, T f_)noexcept{v= v_; f=f_;}
			template<typename F, std::enable_if_t<std::is_invocable_v<F, T>, std::nullptr_t> = nullptr>
			void set(F fn, T v_){v= v_; f=fn(v);}
			template<typename F>
			void eval(F fn){f=fn(v);}
		};
		template<typename F, typename T>
		auto make_evalpair(F fn, T x){
			return evalpair<decltype(x * fn(x))>(x,fn(x));
		}
		template<typename T>
		struct evalrange{
			using value_type = T;
			evalpair<T> lower;
			evalpair<T> upper;
		public:
			evalrange()=default;
			evalrange(const evalpair<T>& lower_, const evalpair<T>& upper_)
				: lower(lower_)
				, upper(upper_){
			}
			template<typename F, std::enable_if_t<std::is_invocable_v<F, T>, std::nullptr_t> = nullptr>
			evalrange(F fn, T lowerval, T upperval)
				: lower(fn,lowerval)
				, upper(fn,upperval){
			}
			void set(const evalpair<T>& lower_, const evalpair<T>& upper_)noexcept{
				lower = lower_;
				upper = upper_;
			}
			template<typename F>
			void set(F fn, T lowerval, T upperval){
				lower.eval(fn,lowerval);
				upper.eval(fn,upperval);
			}
			template<typename F>
			void eval(F fn){
				lower.eval(fn);
				upper.eval(fn);
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			void order(){if(!is_ordered())std::swap(lower,upper);}
		};
		template<typename T>
		struct guess_evalrange{
			using value_type = T;
			evalpair<T> guess;
			evalpair<T> lower;
			evalpair<T> upper;
		public:
			guess_evalrange()=default;
			guess_evalrange(const evalpair<T>& guess_, const evalpair<T>& lower_, const evalpair<T>& upper_)
				: guess(guess_)
				, lower(lower_)
				, upper(upper_){
			}
			template<typename F, std::enable_if_t<std::is_invocable_v<F, T>, std::nullptr_t> = nullptr>
			guess_evalrange(F fn, T guessval, T lowerval, T upperval)
				: guess(fn,guessval)
				, lower(fn,lowerval)
				, upper(fn,upperval){
			}
			void set(const evalpair<T>& guess_, const evalpair<T>& lower_, const evalpair<T>& upper_)noexcept{
				guess = guess_;
				lower = lower_;
				upper = upper_;
			}
			template<typename F, std::enable_if_t<std::is_invocable_v<F, T>, std::nullptr_t> = nullptr>
			void set(F fn, T guessval, T lowerval, T upperval){
				guess.eval(fn,guessval);
				lower.eval(fn,lowerval);
				upper.eval(fn,upperval);
			}
			template<typename F>
			void eval(F fn){
				guess.eval(fn);
				lower.eval(fn);
				upper.eval(fn);
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			void order(){if(!is_ordered())std::swap(lower,upper);}
		};
	}
}
#
#endif
