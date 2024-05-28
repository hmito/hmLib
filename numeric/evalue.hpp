#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<utility>
#include<type_traits>
#include"../math/sign.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type_, typename eval_type_>
		struct evalue{
			using value_type = value_type_;
			using eval_type = eval_type_;
			using evalue_type = evalue<value_type,eval_type>;
			using this_type = evalue<value_type,eval_type>;
			value_type v;
			eval_type e;
		public:
			evalue()noexcept:v(0),e(0){}
			explicit evalue(const value_type& v_)noexcept:v(v_),e(){}
			explicit evalue(value_type&& v_)noexcept:v(std::move(v_)),e(){}
			evalue(const value_type& v_, eval_type e_)noexcept:v(v_),e(std::move(e_)){}
			evalue(value_type&& v_, eval_type e_)noexcept:v(std::move(v_)),e(std::move(e_)){}
			void set(const value_type& v_, eval_type e_)noexcept{v=v_; e=std::move(e_);}
			void set(value_type&& v_, eval_type e_)noexcept{v=std::move(v_); e=std::move(e_);}
			template<typename fn>
			this_type& eval(fn Fn){e=Fn(v);return *this;}
			template<typename fn>
			this_type& eval(fn Fn,const value_type& v_){v=v_; eval(Fn); }
			template<typename fn>
			this_type& eval(fn Fn,value_type&& v_){v=std::move(v_); eval(Fn); }
		public:
			friend bool operator==(const this_type& v1, const this_type& v2) { return v1.e == v2.e; }
			friend bool operator!=(const this_type& v1, const this_type& v2) { return v1.e != v2.e; }
			friend bool operator<(const this_type& v1, const this_type& v2) { return v1.e < v2.e; }
			friend bool operator<=(const this_type& v1, const this_type& v2) { return v1.e <= v2.e; }
			friend bool operator>(const this_type& v1, const this_type& v2) { return v1.e > v2.e; }
			friend bool operator>=(const this_type& v1, const this_type& v2) { return v1.e >= v2.e; }
		};
		template<typename fn, typename value_type>
		auto make_evalue(fn Fn, value_type&& v){
			return evalue<std::decay_t<value_type>, decltype(Fn(v))>(Fn,std::forward<value_type>(v));
		}
		template<typename value_type_, typename eval_type_>
		struct erange{
			using value_type = value_type_;
			using eval_type = eval_type_;
			using evalue_type = evalue<value_type,eval_type>;
			using this_type = erange<value_type,eval_type>;
			evalue_type lower;
			evalue_type upper;
		public:
			erange()=default;
			erange(const evalue_type& lower_, const evalue_type& upper_)
				: lower(lower_)
				, upper(upper_){
			}
			erange(evalue_type&& lower_, evalue_type&& upper_)
				: lower(std::move(lower_))
				, upper(std::move(upper_)){
			}
			template<typename fn>
			erange(fn Fn, value_type lowerval, value_type upperval)
				: lower(std::move(lowerval))
				, upper(std::move(upperval)){
					lower.eval(Fn);
					upper.eval(Fn);
			}
			void set(const evalue_type& lower_, const evalue_type& upper_)noexcept{
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			void set(evalue_type&& lower_, evalue_type&& upper_)noexcept{
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			template<typename fn>
			this_type& eval(fn Fn){
				lower.eval(Fn);
				upper.eval(Fn);
				return *this;
			}
			template<typename fn>
			this_type& eval(fn Fn, const value_type& lowerval, const value_type& upperval){
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
				return *this;
			}
			template<typename fn>
			this_type& eval(fn Fn, value_type&& lowerval, value_type&& upperval){
				lower.eval(Fn,std::move(lowerval));
				upper.eval(Fn,std::move(upperval));
				return *this;
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			this_type& order(){
				if(!is_ordered())std::swap(lower,upper);
				return *this;
			}
		};
		template<typename fn, typename value_type>
		auto make_erange(fn Fn, value_type&& lower,value_type&& upper){
			return make_erange<std::decay_t<value_type>, decltype(Fn(std::declval<value_type>()))>(Fn,std::forward<value_type>(lower),std::forward<value_type>(upper));
		}
		template<typename value_type_, typename eval_type_>
		struct etrirange{
			using value_type = value_type_;
			using eval_type = eval_type_;
			using evalue_type = evalue<value_type,eval_type>;
			using this_type = etrirange<value_type,eval_type>;
			evalue_type guess;
			evalue_type lower;
			evalue_type upper;
		public:
			etrirange()=default;
			etrirange(const evalue_type& guess_, const evalue_type& lower_, const evalue_type& upper_)
				: guess(guess_)
				, lower(lower_)
				, upper(upper_){
			}
			etrirange(evalue_type&& guess_, evalue_type&& lower_, evalue_type&& upper_)
				: guess(std::move(guess_))
				, lower(std::move(lower_))
				, upper(std::move(upper_)){
			}
			template<typename fn>
			etrirange(fn Fn, const value_type& guessval, const value_type& lowerval, const value_type& upperval)
				: guess(guessval)
				, lower(lowerval)
				, upper(upperval){
				guess.eval(Fn);
				lower.eval(Fn);
				upper.eval(Fn);
			}
			template<typename fn>
			etrirange(fn Fn, value_type&& guessval, value_type&& lowerval, value_type&& upperval)
				: guess(std::move(guessval))
				, lower(std::move(lowerval))
				, upper(std::move(upperval)){
				guess.eval(Fn);
				lower.eval(Fn);
				upper.eval(Fn);
			}
			void set(const evalue_type& guess_, const evalue_type& lower_, const evalue_type& upper_)noexcept{
				guess = guess_;
				lower = lower_;
				upper = upper_;
			}
			void set(evalue_type&& guess_, evalue_type&& lower_, evalue_type&& upper_)noexcept{
				guess = std::move(guess_);
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			template<typename fn>
			this_type& eval(fn Fn){
				guess.eval(Fn);
				lower.eval(Fn);
				upper.eval(Fn);
				return *this;
			}
			template<typename fn>
			this_type&  eval(fn Fn, const value_type& guessval, const value_type& lowerval, const value_type& upperval){
				guess.eval(Fn,guessval);
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
				return *this;
			}
			template<typename fn>
			this_type&  eval(fn Fn, value_type&& guessval, value_type&& lowerval, value_type&& upperval){
				guess.eval(Fn,std::move(guessval));
				lower.eval(Fn,std::move(lowerval));
				upper.eval(Fn,std::move(upperval));
				return *this;
			}
			bool is_ordered()const{return lower.v <= upper.v;}
			this_type& order(){
				if(!is_ordered())std::swap(lower,upper);
				return *this;
			}
		public:
			friend bool operator==(const this_type& v1, const this_type& v2) { return v1.e == v2.e; }
			friend bool operator!=(const this_type& v1, const this_type& v2) { return v1.e != v2.e; }
			friend bool operator<(const this_type& v1, const this_type& v2) { return v1.e < v2.e; }
			friend bool operator<=(const this_type& v1, const this_type& v2) { return v1.e <= v2.e; }
			friend bool operator>(const this_type& v1, const this_type& v2) { return v1.e > v2.e; }
			friend bool operator>=(const this_type& v1, const this_type& v2) { return v1.e >= v2.e; }
		};
		template<typename fn, typename value_type>
		auto make_etrirange(fn Fn, value_type&& guess, value_type&& lower,value_type&& upper){
			return make_etrirange<std::decay_t<value_type>, decltype(Fn(std::declval<value_type>()))>(Fn,std::forward<value_type>(guess),std::forward<value_type>(lower),std::forward<value_type>(upper));
		}
	}
}
#
#endif
