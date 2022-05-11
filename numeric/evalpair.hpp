#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<utility>
namespace hmLib{
	namespace numeric{
		template<typename value_type_, typename evalue_type_>
		struct evalpair{
			using value_type = value_type_;
			using evalue_type = evalue_type_;
			using this_type = evalpair<value_type,evalue_type>;
			value_type v;
			evalue_type e;
		public:
			evalpair()noexcept:v(0),e(0){}
			explicit evalpair(value_type v_)noexcept:v(std::move(v_)),e(){}
			evalpair(value_type v_, evalue_type e_)noexcept:v(std::move(v_)),e(std::move(e_)){}
			template<typename fn>
			evalpair(fn Fn, value_type v_):v(std::move(v_)),e(Fn(v_)){}
			void set(value_type v_, evalue_type e_)noexcept{v=std::move(v_); e=std::move(e_);}
			template<typename fn>
			void set(fn Fn, value_type v_){v=std::move(v_); e=Fn(v);}
			template<typename fn>
			void eval(fn Fn){e=Fn(v);}
		public:
			const value_type& guess_v()const{return v;}
			const evalue_type& guess_e()const{return e;}
			friend bool operator==(const this_type& v1, const this_type& v2) { return v1.e == v2.e; }
			friend bool operator!=(const this_type& v1, const this_type& v2) { return v1.e != v2.e; }
			friend bool operator<(const this_type& v1, const this_type& v2) { return v1.e < v2.e; }
			friend bool operator<=(const this_type& v1, const this_type& v2) { return v1.e <= v2.e; }
			friend bool operator>(const this_type& v1, const this_type& v2) { return v1.e > v2.e; }
			friend bool operator>=(const this_type& v1, const this_type& v2) { return v1.e >= v2.e; }
		};
		template<typename fn, typename value_type>
		auto make_evalpair(fn Fn, value_type v){
			return evalpair<value_type, decltype(Fn(v))>(v,Fn(v));
		}
		template<typename value_type_, typename evalue_type_>
		struct evalrange{
			using value_type = value_type_;
			using evalue_type = evalue_type_;
			using this_evalpair = evalpair<value_type,evalue_type>;
			using this_type = evalrange<value_type,evalue_type>;
			this_evalpair lower;
			this_evalpair upper;
		public:
			evalrange()=default;
			evalrange(const this_evalpair& lower_, const this_evalpair& upper_)
				: lower(lower_)
				, upper(upper_){
			}
			evalrange(this_evalpair&& lower_, this_evalpair&& upper_)
				: lower(std::move(lower_))
				, upper(std::move(upper_)){
			}
			template<typename fn>
			evalrange(fn Fn, value_type lowerval, value_type upperval)
				: lower(Fn,lowerval)
				, upper(Fn,upperval){
			}
			void set(this_evalpair lower_, this_evalpair upper_)noexcept{
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			template<typename fn>
			void set(fn Fn, const value_type& lowerval, const value_type& upperval){
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
			}
			template<typename fn>
			void eval(fn Fn){
				lower.eval(Fn);
				upper.eval(Fn);
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			void order(){if(!is_ordered())std::swap(lower,upper);}
			value_type lower_v()const{return lower.v;}
			value_type upper_v()const{return upper.v;}
			value_type lower_e()const{return lower.e;}
			value_type upper_e()const{return upper.e;}
		};
		template<typename value_type_, typename evalue_type_>
		struct guess_evalrange{
			using value_type = value_type_;
			using evalue_type = evalue_type_;
			using this_evalpair = evalpair<value_type,evalue_type>;
			using this_type = guess_evalrange<value_type,evalue_type>;
			this_evalpair guess;
			this_evalpair lower;
			this_evalpair upper;
		public:
			guess_evalrange()=default;
			guess_evalrange(const this_evalpair& guess_, const this_evalpair& lower_, const this_evalpair& upper_)
				: guess(guess_)
				, lower(lower_)
				, upper(upper_){
			}
			guess_evalrange(this_evalpair&& guess_, this_evalpair&& lower_, this_evalpair&& upper_)
				: guess(std::move(guess_))
				, lower(std::move(lower_))
				, upper(std::move(upper_)){
			}
			template<typename fn>
			guess_evalrange(fn Fn, const value_type& guessval,  const value_type& lowerval,  const value_type& upperval)
				: guess(Fn,guessval)
				, lower(Fn,lowerval)
				, upper(Fn,upperval){
			}
			void set(const evalpair<value_type>& guess_, const evalpair<value_type>& lower_, const evalpair<value_type>& upper_)noexcept{
				guess = guess_;
				lower = lower_;
				upper = upper_;
			}
			template<typename fn>
			void set(fn Fn, value_type guessval, value_type lowerval, value_type upperval){
				guess.eval(Fn,guessval);
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
			}
			template<typename fn>
			void eval(fn Fn){
				guess.eval(Fn);
				lower.eval(Fn);
				upper.eval(Fn);
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			void order(){if(!is_ordered())std::swap(lower,upper);}
			value_type guess_v()const{return guess.v;}
			value_type guess_f()const{return guess.e;}
			value_type lower_v()const{return lower.v;}
			value_type upper_v()const{return upper.v;}
			value_type lower_f()const{return lower.e;}
			value_type upper_f()const{return upper.e;}
		};
	}
}
#
#endif
