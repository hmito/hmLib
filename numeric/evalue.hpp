#ifndef HMLIB_NUMERIC_EVALUE_INC
#define HMLIB_NUMERIC_EVALUE_INC 100
#
#include<utility>
#include<type_traits>
namespace hmLib{
	namespace numeric{
		template<typename value_type_, typename eval_type_>
		struct evalue{
			using value_type = value_type_;
			using eval_type = eval_type_;
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
			using evalue = evalue<value_type,eval_type>;
			using this_type = erange<value_type,eval_type>;
			evalue lower;
			evalue upper;
		public:
			erange()=default;
			erange(const evalue& lower_, const evalue& upper_)
				: lower(lower_)
				, upper(upper_){
			}
			erange(evalue&& lower_, evalue&& upper_)
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
			void set(const evalue& lower_, const evalue& upper_)noexcept{
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			void set(evalue&& lower_, evalue&& upper_)noexcept{
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			template<typename fn>
			void set(fn Fn, const value_type& lowerval, const value_type& upperval){
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
			}
			template<typename fn>
			void set(fn Fn, value_type&& lowerval, value_type&& upperval){
				lower.eval(Fn,std::move(lowerval));
				upper.eval(Fn,std::move(upperval));
			}
			template<typename fn>
			this_type& eval(fn Fn){
				lower.eval(Fn);
				upper.eval(Fn);
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
		struct etrio{
			using value_type = value_type_;
			using eval_type = eval_type_;
			using evalue = evalue<value_type,eval_type>;
			using this_type = etrio<value_type,eval_type>;
			evalue guess;
			evalue lower;
			evalue upper;
		public:
			etrio()=default;
			etrio(const evalue& guess_, const evalue& lower_, const evalue& upper_)
				: guess(guess_)
				, lower(lower_)
				, upper(upper_){
			}
			etrio(evalue&& guess_, evalue&& lower_, evalue&& upper_)
				: guess(std::move(guess_))
				, lower(std::move(lower_))
				, upper(std::move(upper_)){
			}
			template<typename fn>
			etrio(fn Fn, const value_type& guessval, const value_type& lowerval, const value_type& upperval)
				: guess(Fn,guessval)
				, lower(Fn,lowerval)
				, upper(Fn,upperval){
			}
			template<typename fn>
			etrio(fn Fn, value_type&& guessval, value_type&& lowerval, value_type&& upperval)
				: guess(Fn,std::move(guessval))
				, lower(Fn,std::move(lowerval))
				, upper(Fn,std::move(upperval)){
			}
			void set(const evalue<value_type,eval_type>& guess_, const evalue<value_type,eval_type>& lower_, const evalue<value_type,eval_type>& upper_)noexcept{
				guess = guess_;
				lower = lower_;
				upper = upper_;
			}
			void set(evalue&& guess_, evalue&& lower_, evalue&& upper_)noexcept{
				guess = std::move(guess_);
				lower = std::move(lower_);
				upper = std::move(upper_);
			}
			template<typename fn>
			void set(fn Fn, const value_type& guessval, const value_type& lowerval, const value_type& upperval){
				guess.eval(Fn,guessval);
				lower.eval(Fn,lowerval);
				upper.eval(Fn,upperval);
			}
			template<typename fn>
			void set(fn Fn, value_type&& guessval, value_type&& lowerval, value_type&& upperval){
				guess.eval(Fn,std::move(guessval));
				lower.eval(Fn,std::move(lowerval));
				upper.eval(Fn,std::move(upperval));
			}
			template<typename fn>
			this_type& eval(fn Fn){
				guess.eval(Fn);
				lower.eval(Fn);
				upper.eval(Fn);
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
		auto make_etrio(fn Fn, value_type&& guess, value_type&& lower,value_type&& upper){
			return make_etrio<std::decay_t<value_type>, decltype(Fn(std::declval<value_type>()))>(Fn,std::forward<value_type>(guess),std::forward<value_type>(lower),std::forward<value_type>(upper));
		}
		template<typename error_type_>
		struct erange_precision_breaker{
			//following functions should be callable;
			// state_type::lower() : return lower value of the range
			// state_type::upper() : return upper value of the range
			// state_type::value() : return optimal value of the range
			using error_type = error_type_;
		public:
			erange_precision_breaker() = delete;
			explicit erange_precision_breaker(error_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			erange_precision_breaker(error_type relative_error_, error_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename value_type, typename eval_type>
			auto precision(const erange<value_type,eval_type>& x)const{
				return relerr * std::abs((x.upper.v + x.lower.v) / 2) + abserr / 4;
			}
			template<typename value_type, typename eval_type>
			auto precision(const etrio<value_type,eval_type>& x)const{
				return relerr * std::abs(x.guess.v) + abserr / 4;
			}
			template<typename value_type, typename eval_type, typename step_type>
			bool operator()(const erange<value_type,eval_type>& x, step_type)const{
				return (x.upper.v - x.lower.v) / 2 <= precision(x) * 2 ;
			}
			template<typename value_type, typename eval_type, typename step_type>
			bool operator()(const etrio<value_type,eval_type>& x, step_type)const{
				return std::abs(x.guess.v - (x.upper.v + x.lower.v) / 2) + (x.upper.v - x.lower.v) / 2 <= precision(x) * 2 ;
			}
		private:
			error_type relerr;
			error_type abserr;
		};
	}
}
#
#endif
