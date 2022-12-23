#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<utility>
namespace hmLib{
	namespace numeric{
		template<typename T>
		struct evalpair{
			using value_type = T;
			T v;
			T f;
		public:
			evalpair()noexcept:v(0),f(0){}
			explicit evalpair(T v_)noexcept:v(v_),f(){}
			evalpair(T v_, T f_)noexcept:v(v_),f(f_){}
			template<typename F>
			evalpair(F fn, T v_):v(v_),f(fn(v_)){}
			void set(T v_, T f_)noexcept{v=v_; f=f_;}
			template<typename F>
			void set(F fn, T v_){v=v_; f=fn(v);}
			template<typename F>
			void eval(F fn){f=fn(v);}
			T guess_v()const{return v;}
			T guess_f()const{return f;}
		};
		template<typename F, typename T>
		auto make_evalpair(F fn, T v){
			return evalpair<decltype(v * fn(v))>(v,fn(v));
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
			template<typename F>
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
			T lower_v()const{return lower.v;}
			T upper_v()const{return upper.v;}
			T lower_f()const{return lower.f;}
			T upper_f()const{return upper.f;}
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
			template<typename F>
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
			template<typename F>
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
			T guess_v()const{return guess.v;}
			T guess_f()const{return guess.f;}
			T lower_v()const{return lower.v;}
			T upper_v()const{return upper.v;}
			T lower_f()const{return lower.f;}
			T upper_f()const{return upper.f;}
		};
	}
}
#
#endif
