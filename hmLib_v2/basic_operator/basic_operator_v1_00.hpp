#ifndef INC_HMLIB_BASICOPERATOR
#define INC_HMLIB_BASICOPERATOR 100
namespace hmstd{
	template<class my_type>
	class operator_increment{
	public:
		friend my_type operator++(my_type& _t,int){
			my_type t(_t);
			++_t;
			return t;
		}
	};
	template<class my_type>
	class operator_decrement{
	public:
		friend my_type operator--(my_type& _t,int){
			my_type t(_t);
			--_t;
			return t;
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_add{
	public:
		friend my_type operator+(const my_type& _t,const sub_type& _s){
			my_type ans(_t);
			ans+=_s;
			return ans;
		}
	};
	template<class my_type,class sub_type>
	class operator_radd:public operator_add<my_type,sub_type>{
	public:
		friend my_type operator+(const sub_type& _s,const my_type& _t){
			my_type ans(_t);
			ans+=_s;
			return ans;
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_sub{
	public:
		friend my_type operator-(const my_type& _t,const sub_type& _s){
			my_type ans(_t);
			ans-=_s;
			return ans;
		}
	};
	template<class my_type,class sub_type>
	class operator_rsub:public operator_sub<my_type,sub_type>{
	public:
		friend my_type operator-(const sub_type& _s,const my_type& _t){
			my_type ans(_t);
			ans-=_s;
			return ans;
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_addsub:public operator_add<my_type,sub_type>,public operator_sub<my_type,sub_type>{
	public:
		friend my_type& operator-=(my_type& _t,const sub_type& _s){
			_t+=static_cast<sub_type>((-_s));
			return _t;
		}
	};
	template<class my_type,class sub_type>
	class operator_raddsub:public operator_addsub<my_type,sub_type>,public operator_radd<my_type,sub_type>{
	public:
		friend my_type operator-(const sub_type& _s,const my_type& _t){
			my_type ans(_t);
			ans-=_s;
			return ans;
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_equal{
	public:
		friend bool operator!=(const my_type& _t,const sub_type& _s){
			return !(_t==_s);
		}
	};
	template<class my_type,class sub_type>
	class operator_requal:public operator_equal<my_type,sub_type>{
	public:
		friend bool operator==(const sub_type& _s,const my_type& _t){
			return (_t==_s);
		}
		friend bool operator!=(const sub_type& _s,const my_type& _t){
			return !(_t==_s);
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_compare{
	public:
		friend bool operator>(const my_type& _t,const sub_type& _s){
			return !(_t<_s);
		}
	};
	template<class my_type,class sub_type>
	class operator_rcompare:public operator_compare<my_type,sub_type>{
	public:
		friend bool operator<(const sub_type& _s,const my_type& _t){
			return !(_t<_s);
		}
		friend bool operator>(const sub_type& _s,const my_type& _t){
			return (_t<_s);
		}
	};
	template<class my_type,class sub_type=my_type>
	class operator_equalcompare:public operator_equal<my_type,sub_type>,public operator_compare<my_type,sub_type>{
	public:
		friend bool operator<=(const my_type& _t,const sub_type& _s){
			return (_t<_s)||(_t==_s);
		}
		friend bool operator>=(const my_type& _t,const sub_type& _s){
			return (_t>_s)||(_t==_s);
		}
	};
	template<class my_type,class sub_type>
	class operator_requalcompare:public operator_requal<my_type,sub_type>,public operator_rcompare<my_type,sub_type>{
	public:
		friend bool operator<=(const my_type& _t,const sub_type& _s){
			return (_t<_s)||(_t==_s);
		}
		friend bool operator>=(const my_type& _t,const sub_type& _s){
			return (_t>_s)||(_t==_s);
		}
		friend bool operator<=(const sub_type& _s,const my_type& _t){
			return (_s<_t)||(_t==_s);
		}
		friend bool operator>=(const sub_type& _s,const my_type& _t){
			return (_s>_t)||(_t==_s);
		}
	};
}
#endif
