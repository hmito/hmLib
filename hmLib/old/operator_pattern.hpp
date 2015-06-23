#ifndef HMLIB_OPERATORPATTERN_INC
#define HMLIB_OPERATORPATTERN_INC 100
#
/*
friend宣言を使うのは、引数に自分自身ではなくmy_typeを使う事で、参照を戻す際にmy_typeへのキャストを避けるため
my_type& operator++(){
	increment();
	return *this;	//Error cast from this_type to my_type
}
*/
namespace hmLib{
	namespace pattern{
		// ++
		template<typename my_type>
		struct increment_pattern{
		private:
			typedef increment_pattern<my_type> this_type;
		protected:
			virtual void increment()=0;
		public:
			friend my_type& operator++(my_type& My_){
				static_cast<this_type&>(My_).increment();
				return My_;
			}
			friend my_type operator++(my_type& My_,int){
				my_type Ans(My_);
				static_cast<this_type&>(My_).increment();
				return Ans;
			}
		};
		// --
		template<typename my_type>
		struct decrement_pattern{
		private:
			typedef decrement_pattern<my_type> this_type;
		protected:
			virtual void decrement()=0;
		public:
			friend my_type& operator--(my_type& My_){
				static_cast<this_type&>(My_).decrement();
				return My_;
			}
			friend my_type operator--(my_type& My_,int){
				my_type Ans(My_);
				static_cast<this_type&>(My_).decrement();
				return Ans;
			}
		};
		// &
		template<typename my_type,typename value_type_=my_type>
		struct pointer_pattern{
		protected:
			virtual const value_type_* pointer()const=0;
		public:
			value_type_* operator&(){return const_cast<value_type_*>(pointer());}
			const value_type_* operator&()const{return pointer();}
		};
		// const &
		template<typename my_type,typename value_type_=my_type>
		struct const_pointer_pattern{
		protected:
			virtual const value_type_* const_pointer()const=0;
		public:
			const value_type_* operator&()const{return const_pointer();}
		};
		// ->
		template<typename my_type,typename value_type_=my_type>
		struct access_pattern{
		private:
			typedef access_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_* access()const=0;
		public:
			value_type_* operator->(){return const_cast<value_type_*>(access());}
			const value_type_* operator->()const{return access();}
		};
		// const ->
		template<typename my_type,typename value_type_=my_type>
		struct const_access_pattern{
		private:
			typedef const_access_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_* const_access()const=0;
		public:
			const value_type_* operator->()const{return const_access();}
		};
		// *
		template<typename my_type,typename value_type_=my_type>
		struct reference_pattern{
		private:
			typedef reference_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_& refer()const=0;
		public:
			value_type_& operator*(){return const_cast<value_type_&>(refer());}
			const value_type_& operator*()const{return refer();}
		};
		// const *
		template<typename my_type,typename value_type_=my_type>
		struct const_reference_pattern{
		private:
			typedef const_reference_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_& const_refer()const=0;
		public:
			const value_type_& operator*()const{return const_refer();}
		};
		// []
		template<typename my_type,typename index_type,typename value_type_=my_type>
		struct index_pattern{
		private:
			typedef index_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_& index(const index_type& Index_)const=0;
		public:
			value_type_& operator[](const index_type& Index_){return const_cast<value_type_&>(index(Index_));}
			const value_type_& operator[](const index_type& Index_)const{return const_index(Index_);}
		};
		// const []
		template<typename my_type,typename index_type,typename value_type_=my_type>
		struct const_index_pattern{
		private:
			typedef const_index_pattern<my_type,value_type_> this_type;
		protected:
			virtual const value_type_& const_index(const index_type& Index_)const=0;
		public:
			const value_type_& operator[](const index_type& Index_)const{return const_index(Index_);}
		};
		// +
		template<typename my_type,typename sub_type>
		struct forward_add_pattern{
		private:
			typedef forward_add_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_add(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator+=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_add(Sub_);
				return My_;
			}
			friend my_type operator+(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_add(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_add_pattern:public forward_add_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_add_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_add(const sub_type& Sub_){return forward_add(Sub_);}
		public:
			friend my_type operator+(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_add(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct add_pattern:public forward_add_pattern<my_type,my_type>{};
		// -
		template<typename my_type,typename sub_type>
		struct forward_sub_pattern{
		private:
			typedef forward_sub_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_sub(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator-=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_sub(Sub_);
				return My_;
			}
			friend my_type operator-(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_sub(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_sub_pattern:public forward_sub_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_sub_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_sub(const sub_type& Sub_)=0;
		public:
			friend my_type operator-(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_sub(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct sub_pattern:public forward_sub_pattern<my_type,my_type>{};
		// *
		template<typename my_type,typename sub_type>
		struct forward_multi_pattern{
		private:
			typedef forward_multi_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_multi(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator*=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_multi(Sub_);
				return My_;
			}
			friend my_type operator*(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_multi(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_multi_pattern:public forward_multi_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_multi_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_multi(const sub_type& Sub_){return forward_multi(Sub_);}
		public:
			friend my_type operator*(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_multi(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct multi_pattern:public forward_multi_pattern<my_type,my_type>{};
		// /
		template<typename my_type,typename sub_type>
		struct forward_div_pattern{
		private:
			typedef forward_div_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_div(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator/=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_div(Sub_);
				return My_;
			}
			friend my_type operator/(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_div(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_div_pattern:public forward_div_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_div_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_div(const sub_type& Sub_)=0;
		public:
			friend my_type operator/(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_div(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct div_pattern:public forward_div_pattern<my_type,my_type>{};
		// %
		template<typename my_type,typename sub_type>
		struct forward_surplus_pattern{
		private:
			typedef forward_surplus_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_surplus(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator%=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_surplus(Sub_);
				return My_;
			}
			friend my_type operator%(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_surplus(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_surplus_pattern:public forward_surplus_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_surplus_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_surplus(const sub_type& Sub_)=0;
		public:
			friend my_type operator%(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_surplus(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct surplus_pattern:public forward_surplus_pattern<my_type,my_type>{};
		// <<
		template<typename my_type,typename sub_type>
		struct forward_lshift_pattern{
		private:
			typedef forward_lshift_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_lshift(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator<<=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_lshift(Sub_);
				return My_;
			}
			friend my_type operator<<(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_lshift(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_lshift_pattern:public forward_lshift_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_lshift_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_lshift(const sub_type& Sub_)=0;
		public:
			friend my_type operator<<(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_lshift(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct lshift_pattern:public forward_lshift_pattern<my_type,my_type>{};
		// >>
		template<typename my_type,typename sub_type>
		struct forward_rshift_pattern{
		private:
			typedef forward_rshift_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_rshift(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator>>=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_rshift(Sub_);
				return My_;
			}
			friend my_type operator>>(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_rshift(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_rshift_pattern:public forward_rshift_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_rshift_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_rshift(const sub_type& Sub_)=0;
		public:
			friend my_type operator>>(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_rshift(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct rshift_pattern:public forward_rshift_pattern<my_type,my_type>{};
		// &
		template<typename my_type,typename sub_type>
		struct forward_and_pattern{
		private:
			typedef forward_and_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_and(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator&=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_and(Sub_);
				return My_;
			}
			friend my_type operator&(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_and(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_and_pattern:public forward_and_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_and_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_and(const sub_type& Sub_){return forward_and(Sub_);}
		public:
			friend my_type operator&(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_and(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct and_pattern:public forward_and_pattern<my_type,my_type>{};
		// |
		template<typename my_type,typename sub_type>
		struct forward_or_pattern{
		private:
			typedef forward_or_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_or(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator|=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_or(Sub_);
				return My_;
			}
			friend my_type operator|(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_or(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_or_pattern:public forward_or_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_or_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_or(const sub_type& Sub_){return forward_or(Sub_);}
		public:
			friend my_type operator|(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_or(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct or_pattern:public forward_or_pattern<my_type,my_type>{};
		// ^
		template<typename my_type,typename sub_type>
		struct forward_xor_pattern{
		private:
			typedef forward_xor_pattern<my_type,sub_type> this_type;
		protected:
			virtual void forward_xor(const sub_type& Sub_)=0;
		public:
			friend const my_type& operator^=(my_type& My_,const sub_type& Sub_){
				static_cast<this_type&>(My_).forward_xor(Sub_);
				return My_;
			}
			friend my_type operator^(const my_type& My_,const sub_type& Sub_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).forward_xor(Sub_);
				return Ans;
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_xor_pattern:public forward_xor_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_xor_pattern<my_type,sub_type> this_type;
		protected:
			virtual void backward_xor(const sub_type& Sub_){return forward_xor(Sub_);}
		public:
			friend my_type operator^(const sub_type& Sub_,const my_type& My_){
				my_type Ans(My_);
				static_cast<this_type&>(Ans).backward_xor(Sub_);
				return Ans;
			}
		};
		template<typename my_type>
		struct xor_pattern:public forward_xor_pattern<my_type,my_type>{};
		// ==
		template<typename my_type,typename sub_type>
		struct forward_equal_pattern{
		private:
			typedef forward_equal_pattern<my_type,sub_type> this_type;
		protected:
			virtual bool forward_equal(const sub_type& sub_)const=0;
		public:
			friend bool operator==(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).forward_equal(Sub_);
			}
			friend bool operator!=(const my_type& My_,const sub_type& Sub_){
				return !(static_cast<const this_type&>(My_).forward_equal(Sub_));
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_equal_pattern:public forward_equal_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_equal_pattern<my_type,sub_type> this_type;
		protected:
			virtual bool backward_equal(const sub_type& sub_)const{return forward_equal(sub_);}
		public:
			friend bool operator==(const sub_type& Sub_,const my_type& My_){
				return static_cast<const this_type&>(My_).backward_equal(Sub_);
			}
			friend bool operator!=(const sub_type& Sub_,const my_type& My_){
				return !(static_cast<const this_type&>(My_).backward_equal(Sub_));
			}
		};
		template<typename my_type>
		struct equal_pattern:public forward_equal_pattern<my_type,my_type>{};
		// <,>
		template<typename my_type,typename sub_type>
		struct forward_compare_pattern{
		private:
			typedef forward_compare_pattern<my_type,sub_type> this_type;
		protected:
			virtual bool forward_less(const sub_type& Sub_)const=0;
			virtual bool forward_more(const sub_type& Sub_)const=0;
		public:
			friend bool operator<(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).forward_less(Sub_);
			}
			friend bool operator>(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).forward_more(Sub_);
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_compare_pattern:public forward_compare_pattern<my_type,sub_type>{
		private:
			typedef bidirectional_compare_pattern<my_type,sub_type> this_type;
		protected:
			virtual bool backward_less(const sub_type& Sub_)const{return forward_more(Sub_);}
			virtual bool backward_more(const sub_type& Sub_)const{return forward_less(Sub_);}
		public:
			friend bool operator>(const sub_type& Sub_,const my_type& My_){
				return static_cast<const this_type&>(My_).backward_less(Sub_);
			}
			friend bool operator<(const sub_type& Sub_,const my_type& My_){
				return !(static_cast<const this_type&>(My_).backward_more(Sub_));
			}
		};
		template<typename my_type>
		struct compare_pattern:public forward_compare_pattern<my_type,my_type>{};
		// <=,>=
		template<typename my_type,typename sub_type=my_type>
		struct forward_equalcompare_pattern:public forward_equal_pattern<my_type,sub_type>,public forward_compare_pattern<my_type,sub_type>{
		public:
			friend bool operator<=(const my_type& My_,const sub_type& Sub_){
				return (My_<Sub_)||(My_==Sub_);
			}
			friend bool operator>=(const my_type& My_,const sub_type& Sub_){
				return (My_>Sub_)||(My_==Sub_);
			}
		};
		template<typename my_type,typename sub_type>
		struct bidirectional_equalcompare_pattern:public bidirectional_equal_pattern<my_type,sub_type>,public bidirectional_compare_pattern<my_type,sub_type>{
		public:
			friend bool operator<=(const my_type& My_,const sub_type& Sub_){
				return (My_<Sub_)||(My_==Sub_);
			}
			friend bool operator>=(const my_type& My_,const sub_type& Sub_){
				return (My_>Sub_)||(My_==Sub_);
			}
			friend bool operator<=(const sub_type& Sub_,const my_type& My_){
				return (Sub_<My_)||(My_==Sub_);
			}
			friend bool operator>=(const sub_type& Sub_,const my_type& My_){
				return (Sub_>My_)||(My_==Sub_);
			}
		};
		template<typename my_type>
		struct equalcompare_pattern:public forward_equalcompare_pattern<my_type,my_type>{};
		// +
		template<typename my_type,typename sub_type,typename ans_type>
		struct forward_binary_add_pattern{
		private:
			typedef forward_binary_add_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type forward_binary_add(const sub_type& s_)const=0;
		public:
			friend ans_type operator+(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).forward_binary_add(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct bidirectional_binary_add_pattern:public forward_binary_add_pattern<my_type,sub_type,ans_type>{
		private:
			typedef bidirectional_binary_add_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type backward_binary_add(const sub_type& s_)const{return forward_binary_add(s_);}
		public:
			friend ans_type operator+(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).backward_binary_add(s_);}
		};
		template<typename my_type,typename ans_type>
		struct binary_add_pattern:public forward_binary_add_pattern<my_type,my_type,ans_type>{};
		// -
		template<typename my_type,typename sub_type,typename ans_type>
		struct forward_binary_sub_pattern{
		private:
			typedef forward_binary_sub_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type forward_binary_sub(const sub_type& s_)const=0;
		public:
			friend ans_type operator-(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).forward_binary_sub(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct bidirectional_binary_sub_pattern:public forward_binary_sub_pattern<my_type,sub_type,ans_type>{
		private:
			typedef bidirectional_binary_sub_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type backward_binary_sub(const sub_type& s_)const{return forward_binary_sub(s_);}
		public:
			friend ans_type operator-(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).backward_binary_sub(s_);}
		};
		template<typename my_type,typename ans_type>
		struct binary_sub_pattern:public forward_binary_sub_pattern<my_type,my_type,ans_type>{};
		// *
		template<typename my_type,typename sub_type,typename ans_type>
		struct forward_binary_multi_pattern{
		private:
			typedef forward_binary_multi_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type forward_binary_multi(const sub_type& s_)const=0;
		public:
			friend ans_type operator*(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).forward_binary_multi(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct bidirectional_binary_multi_pattern:public forward_binary_multi_pattern<my_type,sub_type,ans_type>{
		private:
			typedef bidirectional_binary_multi_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type backward_binary_multi(const sub_type& s_)const{return forward_binary_multi(s_);}
		public:
			friend ans_type operator*(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).backward_binary_multi(s_);}
		};
		template<typename my_type,typename ans_type>
		struct binary_multi_pattern:public forward_binary_multi_pattern<my_type,my_type,ans_type>{};
		// /
		template<typename my_type,typename sub_type,typename ans_type>
		struct forward_binary_div_pattern{
		private:
			typedef forward_binary_div_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type forward_binary_div(const sub_type& s_)const=0;
		public:
			friend ans_type operator/(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).forward_binary_div(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct bidirectional_binary_div_pattern:public forward_binary_div_pattern<my_type,sub_type,ans_type>{
		private:
			typedef bidirectional_binary_div_pattern<my_type,sub_type,ans_type> this_type;
		protected:
			virtual ans_type backward_binary_div(const sub_type& s_)const{return forward_binary_div(s_);}
		public:
			friend ans_type operator/(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).backward_binary_div(s_);}
		};
		template<typename my_type,typename ans_type>
		struct binary_div_pattern:public forward_binary_div_pattern<my_type,my_type,ans_type>{};
//*/
	}
}
#
#endif
