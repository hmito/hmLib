#ifndef HMLIB_OPERATORINTERFACE_INC
#define HMLIB_OPERATORINTERFACE_INC 1
#
/*===operator_interface===
operator関数群用のinterfaceを提供する

friend宣言を使うのは、引数に自分自身ではなくmy_typeを使う事で、参照を戻す際にmy_typeへのキャストを避けるため
my_type& operator++(){
	increment();
	return *this;	//Error cast from this_type to my_type
}

operator_interface:v0_01/130308 hmIto
	operator_patternから継承
	関数名をinterfaceタイプに統一
*/
namespace hmLib{
	namespace interfaces{
		// ++
		template<typename my_type>
		struct operator_increment_interface{
		private:
			typedef operator_increment_interface<my_type> this_type;
		protected:
			virtual void itf_operator_increment()=0;
		public:
			friend my_type& operator++(my_type& My_){
				static_cast<this_type&>(My_).itf_operator_increment();
				return My_;
			}
			friend my_type operator++(my_type& My_,int){
				my_type Ans(My_);
				static_cast<this_type&>(My_).itf_operator_increment();
				return Ans;
			}
		};
		// --
		template<typename my_type>
		struct operator_decrement_interface{
		private:
			typedef operator_decrement_interface<my_type> this_type;
		protected:
			virtual void itf_operator_decrement()=0;
		public:
			friend my_type& operator--(my_type& My_){
				static_cast<this_type&>(My_).itf_operator_decrement();
				return My_;
			}
			friend my_type operator--(my_type& My_,int){
				my_type Ans(My_);
				static_cast<this_type&>(My_).itf_operator_decrement();
				return Ans;
			}
		};
		// const &
		template<typename value_type>
		struct operator_const_pointer_interface{
		protected:
			virtual const value_type* itf_operator_pointer()const=0;
		public:
			const value_type* operator&()const{return itf_operator_pointer();}
		};
		// &
		template<typename value_type>
		struct operator_pointer_interface:public operator_const_pointer_interface<value_type>{
		public:
			value_type* operator&(){return const_cast<value_type*>(itf_operator_pointer());}
		};
		// const ->
		template<typename value_type>
		struct operator_const_access_interface{
		protected:
			virtual const value_type* itf_operator_access()const=0;
		public:
			const value_type* operator->()const{return itf_operator_access();}
		};
		// ->
		template<typename value_type>
		struct operator_access_interface:public operator_const_access_interface<value_type>{
		public:
			value_type* operator->(){return const_cast<value_type*>(itf_operator_access());}
		};
		// const *
		template<typename value_type>
		struct operator_const_reference_interface{
		protected:
			virtual const value_type& itf_operator_reference()const=0;
		public:
			const value_type& operator*()const{return itf_operator_reference();}
		};
		// *
		template<typename value_type>
		struct operator_reference_interface:public operator_const_reference_interface<value_type>{
		public:
			value_type& operator*(){return const_cast<value_type&>(itf_operator_reference());}
		};
		// const []
		template<typename index_type,typename value_type>
		struct operator_const_index_interface{
		protected:
			virtual const value_type& itf_operator_index(const index_type& Index_)const=0;
		public:
			const value_type& operator[](const index_type& Index_)const{return itf_operator_index(Index_);}
		};
		// []
		template<typename index_type,typename value_type>
		struct operator_index_interface:public operator_const_index_interface<index_type,value_type>{
		public:
			value_type& operator[](const index_type& Index_){return const_cast<value_type&>(itf_operator_index(Index_));}
		};
		// add +
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_add_interface{
			private:
				typedef operator_forward_add_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_add(const sub_type& s_)const=0;
			public:
				friend ans_type operator+(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_add(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_add_interface{
			private:
				typedef operator_backward_add_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_backward_add(const sub_type& s_)const=0;
			public:
				friend ans_type operator+(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_add(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_add_interface{
			private:
				typedef operator_equal_add_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_add(const sub_type& s_)const=0;
			public:
				friend my_type operator+=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_add(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_add_interface
			:public operator_forward_add_interface<my_type,sub_type,ans_type>
			,public operator_backward_add_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_add_interface
			:public operator_forward_add_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_add(const my_type& m_)const override{return itf_operator_add(m_);}
				virtual ans_type itf_operator_add(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_addable_interface
			:public operator_forward_add_interface<my_type,sub_type,my_type>
			,public operator_equal_add_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_add(const sub_type& s_)const{itf_operator_forward_add(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_addable_interface
			:public operator_bidirectional_add_interface<my_type,sub_type,my_type>
			,public operator_equal_add_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_add(const sub_type& s_)const{itf_operator_forward_add(s_);}	
		};
		template<typename my_type>
		struct operator_addable_interface
			:public operator_add_interface<my_type,my_type>
			,public operator_equal_add_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_add(const my_type& m_)const{itf_operator_add(m_);}	
		};		// sub -
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_sub_interface{
			private:
				typedef operator_forward_sub_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_sub(const sub_type& s_)const=0;
			public:
				friend ans_type operator-(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_sub(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_sub_interface{
			private:
				typedef operator_backward_sub_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_sub(const sub_type& s_)const=0;
			public:
				friend ans_type operator-(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_sub(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_sub_interface{
			private:
				typedef operator_equal_sub_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_sub(const sub_type& s_)const=0;
			public:
				friend my_type operator-=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_sub(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_sub_interface
			:public operator_forward_sub_interface<my_type,sub_type,ans_type>
			,public operator_backward_sub_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_sub_interface
			:public operator_forward_sub_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_sub(const my_type& m_)const override{return itf_operator_sub(m_);}
				virtual ans_type itf_operator_sub(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_subable_interface
			:public operator_forward_sub_interface<my_type,sub_type,my_type>
			,public operator_equal_sub_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_sub(const sub_type& s_)const{itf_operator_forward_sub(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_subable_interface
			:public operator_bidirectional_sub_interface<my_type,sub_type,my_type>
			,public operator_equal_sub_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_sub(const sub_type& s_)const{itf_operator_forward_sub(s_);}	
		};
		template<typename my_type>
		struct operator_subable_interface
			:public operator_sub_interface<my_type,my_type>
			,public operator_equal_sub_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_sub(const my_type& m_)const{itf_operator_sub(m_);}	
		};		// multiplicate *
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_multiplicate_interface{
			private:
				typedef operator_forward_multiplicate_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_multiplicate(const sub_type& s_)const=0;
			public:
				friend ans_type operator*(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_multiplicate(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_multiplicate_interface{
			private:
				typedef operator_backward_multiplicate_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_multiplicate(const sub_type& s_)const=0;
			public:
				friend ans_type operator*(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_multiplicate(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_multiplicate_interface{
			private:
				typedef operator_equal_multiplicate_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_multiplicate(const sub_type& s_)const=0;
			public:
				friend my_type operator*=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_multiplicate(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_multiplicate_interface
			:public operator_forward_multiplicate_interface<my_type,sub_type,ans_type>
			,public operator_backward_multiplicate_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_multiplicate_interface
			:public operator_forward_multiplicate_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_multiplicate(const my_type& m_)const override{return itf_operator_multiplicate(m_);}
				virtual ans_type itf_operator_multiplicate(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_multiplicatable_interface
			:public operator_forward_multiplicate_interface<my_type,sub_type,my_type>
			,public operator_equal_multiplicate_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_multiplicate(const sub_type& s_)const{itf_operator_forward_multiplicate(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_multiplicatable_interface
			:public operator_bidirectional_multiplicate_interface<my_type,sub_type,my_type>
			,public operator_equal_multiplicate_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_multiplicate(const sub_type& s_)const{itf_operator_forward_multiplicate(s_);}	
		};
		template<typename my_type>
		struct operator_multiplicatable_interface
			:public operator_multiplicate_interface<my_type,my_type>
			,public operator_equal_multiplicate_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_multiplicate(const my_type& m_)const{itf_operator_multiplicate(m_);}	
		};		// divide /
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_divide_interface{
			private:
				typedef operator_forward_divide_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_divide(const sub_type& s_)const=0;
			public:
				friend ans_type operator/(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_divide(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_divide_interface{
			private:
				typedef operator_backward_divide_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_divide(const sub_type& s_)const=0;
			public:
				friend ans_type operator/(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_divide(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_divide_interface{
			private:
				typedef operator_equal_divide_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_divide(const sub_type& s_)const=0;
			public:
				friend my_type operator/=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_divide(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_divide_interface
			:public operator_forward_divide_interface<my_type,sub_type,ans_type>
			,public operator_backward_divide_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_divide_interface
			:public operator_forward_divide_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_divide(const my_type& m_)const override{return itf_operator_divide(m_);}
				virtual ans_type itf_operator_divide(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_dividable_interface
			:public operator_forward_divide_interface<my_type,sub_type,my_type>
			,public operator_equal_divide_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_divide(const sub_type& s_)const{itf_operator_forward_divide(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_dividable_interface
			:public operator_bidirectional_divide_interface<my_type,sub_type,my_type>
			,public operator_equal_divide_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_divide(const sub_type& s_)const{itf_operator_forward_divide(s_);}	
		};
		template<typename my_type>
		struct operator_dividable_interface
			:public operator_divide_interface<my_type,my_type>
			,public operator_equal_divide_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_divide(const my_type& m_)const{itf_operator_divide(m_);}	
		};		// surplus %
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_surplus_interface{
			private:
				typedef operator_forward_surplus_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_surplus(const sub_type& s_)const=0;
			public:
				friend ans_type operator%(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_surplus(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_surplus_interface{
			private:
				typedef operator_backward_surplus_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_surplus(const sub_type& s_)const=0;
			public:
				friend ans_type operator%(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_surplus(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_surplus_interface{
			private:
				typedef operator_equal_surplus_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_surplus(const sub_type& s_)const=0;
			public:
				friend my_type operator%=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_surplus(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_surplus_interface
			:public operator_forward_surplus_interface<my_type,sub_type,ans_type>
			,public operator_backward_surplus_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_surplus_interface
			:public operator_forward_surplus_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_surplus(const my_type& m_)const override{return itf_operator_surplus(m_);}
				virtual ans_type itf_operator_surplus(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_surplusable_interface
			:public operator_forward_surplus_interface<my_type,sub_type,my_type>
			,public operator_equal_surplus_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_surplus(const sub_type& s_)const{itf_operator_forward_surplus(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_surplusable_interface
			:public operator_bidirectional_surplus_interface<my_type,sub_type,my_type>
			,public operator_equal_surplus_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_surplus(const sub_type& s_)const{itf_operator_forward_surplus(s_);}	
		};
		template<typename my_type>
		struct operator_surplusable_interface
			:public operator_surplus_interface<my_type,my_type>
			,public operator_equal_surplus_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_surplus(const my_type& m_)const{itf_operator_surplus(m_);}	
		};		// leftshift <<
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_leftshift_interface{
			private:
				typedef operator_forward_leftshift_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_leftshift(const sub_type& s_)const=0;
			public:
				friend ans_type operator<<(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_leftshift(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_leftshift_interface{
			private:
				typedef operator_backward_leftshift_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_leftshift(const sub_type& s_)const=0;
			public:
				friend ans_type operator<<(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_leftshift(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_leftshift_interface{
			private:
				typedef operator_equal_leftshift_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_leftshift(const sub_type& s_)const=0;
			public:
				friend my_type operator<<=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_leftshift(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_leftshift_interface
			:public operator_forward_leftshift_interface<my_type,sub_type,ans_type>
			,public operator_backward_leftshift_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_leftshift_interface
			:public operator_forward_leftshift_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_leftshift(const my_type& m_)const override{return itf_operator_leftshift(m_);}
				virtual ans_type itf_operator_leftshift(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_leftshiftable_interface
			:public operator_forward_leftshift_interface<my_type,sub_type,my_type>
			,public operator_equal_leftshift_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_leftshift(const sub_type& s_)const{itf_operator_forward_leftshift(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_leftshiftable_interface
			:public operator_bidirectional_leftshift_interface<my_type,sub_type,my_type>
			,public operator_equal_leftshift_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_leftshift(const sub_type& s_)const{itf_operator_forward_leftshift(s_);}	
		};
		template<typename my_type>
		struct operator_leftshiftable_interface
			:public operator_leftshift_interface<my_type,my_type>
			,public operator_equal_leftshift_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_leftshift(const my_type& m_)const{itf_operator_leftshift(m_);}	
		};		// rightshift >>
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_rightshift_interface{
			private:
				typedef operator_forward_rightshift_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_rightshift(const sub_type& s_)const=0;
			public:
				friend ans_type operator>>(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_rightshift(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_rightshift_interface{
			private:
				typedef operator_backward_rightshift_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_rightshift(const sub_type& s_)const=0;
			public:
				friend ans_type operator>>(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_rightshift(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_rightshift_interface{
			private:
				typedef operator_equal_rightshift_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_rightshift(const sub_type& s_)const=0;
			public:
				friend my_type operator>>=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_rightshift(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_rightshift_interface
			:public operator_forward_rightshift_interface<my_type,sub_type,ans_type>
			,public operator_backward_rightshift_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_rightshift_interface
			:public operator_forward_rightshift_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_rightshift(const my_type& m_)const override{return itf_operator_rightshift(m_);}
				virtual ans_type itf_operator_rightshift(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_rightshiftable_interface
			:public operator_forward_rightshift_interface<my_type,sub_type,my_type>
			,public operator_equal_rightshift_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_rightshift(const sub_type& s_)const{itf_operator_forward_rightshift(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_rightshiftable_interface
			:public operator_bidirectional_rightshift_interface<my_type,sub_type,my_type>
			,public operator_equal_rightshift_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_rightshift(const sub_type& s_)const{itf_operator_forward_rightshift(s_);}	
		};
		template<typename my_type>
		struct operator_rightshiftable_interface
			:public operator_rightshift_interface<my_type,my_type>
			,public operator_equal_rightshift_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_rightshift(const my_type& m_)const{itf_operator_rightshift(m_);}	
		};		// and &
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_and_interface{
			private:
				typedef operator_forward_and_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_and(const sub_type& s_)const=0;
			public:
				friend ans_type operator&(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_and(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_and_interface{
			private:
				typedef operator_backward_and_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_and(const sub_type& s_)const=0;
			public:
				friend ans_type operator&(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_and(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_and_interface{
			private:
				typedef operator_equal_and_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_and(const sub_type& s_)const=0;
			public:
				friend my_type operator&=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_and(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_and_interface
			:public operator_forward_and_interface<my_type,sub_type,ans_type>
			,public operator_backward_and_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_and_interface
			:public operator_forward_and_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_and(const my_type& m_)const override{return itf_operator_and(m_);}
				virtual ans_type itf_operator_and(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_andable_interface
			:public operator_forward_and_interface<my_type,sub_type,my_type>
			,public operator_equal_and_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_and(const sub_type& s_)const{itf_operator_forward_and(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_andable_interface
			:public operator_bidirectional_and_interface<my_type,sub_type,my_type>
			,public operator_equal_and_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_and(const sub_type& s_)const{itf_operator_forward_and(s_);}	
		};
		template<typename my_type>
		struct operator_andable_interface
			:public operator_and_interface<my_type,my_type>
			,public operator_equal_and_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_and(const my_type& m_)const{itf_operator_and(m_);}	
		};		// or |
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_or_interface{
			private:
				typedef operator_forward_or_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_or(const sub_type& s_)const=0;
			public:
				friend ans_type operator|(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_or(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_or_interface{
			private:
				typedef operator_backward_or_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_or(const sub_type& s_)const=0;
			public:
				friend ans_type operator|(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_or(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_or_interface{
			private:
				typedef operator_equal_or_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_or(const sub_type& s_)const=0;
			public:
				friend my_type operator|=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_or(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_or_interface
			:public operator_forward_or_interface<my_type,sub_type,ans_type>
			,public operator_backward_or_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_or_interface
			:public operator_forward_or_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_or(const my_type& m_)const override{return itf_operator_or(m_);}
				virtual ans_type itf_operator_or(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_orable_interface
			:public operator_forward_or_interface<my_type,sub_type,my_type>
			,public operator_equal_or_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_or(const sub_type& s_)const{itf_operator_forward_or(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_orable_interface
			:public operator_bidirectional_or_interface<my_type,sub_type,my_type>
			,public operator_equal_or_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_or(const sub_type& s_)const{itf_operator_forward_or(s_);}	
		};
		template<typename my_type>
		struct operator_orable_interface
			:public operator_or_interface<my_type,my_type>
			,public operator_equal_or_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_or(const my_type& m_)const{itf_operator_or(m_);}	
		};		// xor ^
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_xor_interface{
			private:
				typedef operator_forward_xor_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_xor(const sub_type& s_)const=0;
			public:
				friend ans_type operator^(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_xor(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_xor_interface{
			private:
				typedef operator_backward_xor_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_equal_xor(const sub_type& s_)const=0;
			public:
				friend ans_type operator^(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_xor(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_xor_interface{
			private:
				typedef operator_equal_xor_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_xor(const sub_type& s_)const=0;
			public:
				friend my_type operator^=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_xor(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_xor_interface
			:public operator_forward_xor_interface<my_type,sub_type,ans_type>
			,public operator_backward_xor_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_xor_interface
			:public operator_forward_xor_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_xor(const my_type& m_)const override{return itf_operator_xor(m_);}
				virtual ans_type itf_operator_xor(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_xorable_interface
			:public operator_forward_xor_interface<my_type,sub_type,my_type>
			,public operator_equal_xor_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_xor(const sub_type& s_)const{itf_operator_forward_xor(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_xorable_interface
			:public operator_bidirectional_xor_interface<my_type,sub_type,my_type>
			,public operator_equal_xor_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_xor(const sub_type& s_)const{itf_operator_forward_xor(s_);}	
		};
		template<typename my_type>
		struct operator_xorable_interface
			:public operator_xor_interface<my_type,my_type>
			,public operator_equal_xor_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_xor(const my_type& m_)const{itf_operator_xor(m_);}	
		};
		// ==
		template<typename my_type,typename sub_type>
		struct operator_forward_equal_interface{
		private:
			typedef operator_forward_equal_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_forward_equal(const sub_type& sub_)const=0;
		public:
			friend bool operator==(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_forward_equal(Sub_);
			}
			friend bool operator!=(const my_type& My_,const sub_type& Sub_){
				return !(static_cast<const this_type&>(My_).itf_operator_forward_equal(Sub_));
			}
		};
		template<typename my_type,typename sub_type>
		struct operator_bidirectional_equal_interface:public operator_forward_equal_interface<my_type,sub_type>{
		private:
			typedef operator_bidirectional_equal_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_backward_equal(const sub_type& sub_)const{return itf_operator_equal(sub_);}
		public:
			friend bool operator==(const sub_type& Sub_,const my_type& My_){
				return static_cast<const this_type&>(My_).itf_operator_backward_equal(Sub_);
			}
			friend bool operator!=(const sub_type& Sub_,const my_type& My_){
				return !(static_cast<const this_type&>(My_).itf_operator_backward_equal(Sub_));
			}
		};
		template<typename my_type>
		struct operator_equal_interface{
		private:
			typedef operator_equal_interface<my_type> this_type;
		protected:
			virtual bool itf_operator_equal(const my_type& m_)const=0;
		public:
			friend bool operator==(const my_type& My_,const my_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_equal(Sub_);
			}
			friend bool operator!=(const my_type& My_,const my_type& Sub_){
				return !(static_cast<const this_type&>(My_).itf_operator_equal(Sub_));
			}
		};
		// <,>
		template<typename my_type,typename sub_type>
		struct operator_forward_compare_interface{
		private:
			typedef operator_forward_compare_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_forward_less(const sub_type& Sub_)const=0;
			virtual bool itf_operator_forward_more(const sub_type& Sub_)const=0;
		public:
			friend bool operator<(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_forward_less(Sub_);
			}
			friend bool operator>(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_forward_more(Sub_);
			}
		};
		template<typename my_type,typename sub_type>
		struct operator_bidirectional_compare_interface:public operator_forward_compare_interface<my_type,sub_type>{
		private:
			typedef operator_bidirectional_compare_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_backward_less(const sub_type& Sub_)const{return itf_operator_more(Sub_);}
			virtual bool itf_operator_backward_more(const sub_type& Sub_)const{return itf_operator_less(Sub_);}
		public:
			friend bool operator>(const sub_type& Sub_,const my_type& My_){
				return static_cast<const this_type&>(My_).itf_operator_backward_less(Sub_);
			}
			friend bool operator<(const sub_type& Sub_,const my_type& My_){
				return !(static_cast<const this_type&>(My_).itf_operator_backward_more(Sub_));
			}
		};
		template<typename my_type>
		struct operator_compare_interface{
		private:
			typedef operator_compare_interface<my_type> this_type;
		protected:
			virtual bool itf_operator_less(const my_type& m_)const=0;
		public:
			friend bool operator<(const my_type& My_,const my_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_less(Sub_);
			}
			friend bool operator>(const my_type& My_,const my_type& Sub_){
				return !(static_cast<const this_type&>(Sub_).itf_operator_less(My_));
			}
		};
		// <=,>=
		template<typename my_type,typename sub_type>
		struct operator_forward_equalcompare_interface{
		private:
			typedef operator_forward_equalcompare_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_forward_equalless(const sub_type& Sub_)const=0;
			virtual bool itf_operator_forward_equalmore(const sub_type& Sub_)const=0;
		public:
			friend bool operator<=(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_forward_equalless(Sub_);
			}
			friend bool operator>=(const my_type& My_,const sub_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_forward_equalmore(Sub_);
			}
		};
		template<typename my_type,typename sub_type>
		struct operator_bidirectional_equalcompare_interface:public operator_forward_equalcompare_interface<my_type,sub_type>{
		private:
			typedef operator_bidirectional_equalcompare_interface<my_type,sub_type> this_type;
		protected:
			virtual bool itf_operator_backward_equalless(const sub_type& Sub_)const{return itf_operator_equalmore(Sub_);}
			virtual bool itf_operator_backward_equalmore(const sub_type& Sub_)const{return itf_operator_equalless(Sub_);}
		public:
			friend bool operator>=(const sub_type& Sub_,const my_type& My_){
				return static_cast<const this_type&>(My_).itf_operator_backward_equalless(Sub_);
			}
			friend bool operator<=(const sub_type& Sub_,const my_type& My_){
				return !(static_cast<const this_type&>(My_).itf_operator_backward_equalmore(Sub_));
			}
		};
		template<typename my_type>
		struct operator_equalcompare_interface{
		private:
			typedef operator_equalcompare_interface<my_type> this_type;
		protected:
			virtual bool itf_operator_equalless(const my_type& m_)const=0;
		public:
			friend bool operator<=(const my_type& My_,const my_type& Sub_){
				return static_cast<const this_type&>(My_).itf_operator_equalless(Sub_);
			}
			friend bool operator>=(const my_type& My_,const my_type& Sub_){
				return !(static_cast<const this_type&>(Sub_).itf_operator_equalless(My_));
			}
		};
		// <=,>=
		template<typename my_type,typename sub_type=my_type>
		struct operator_forward_comparable_interface
			:public operator_forward_equal_interface<my_type,sub_type>
			,public operator_forward_compare_interface<my_type,sub_type>
			,public operator_forward_equalcompare_interface<my_type,sub_type>{
		protected:
			virtual bool itf_operator_forward_equalless(const sub_type& s_)const override{return itf_operator_forward_equal(s_) || itf_operator_forward_less(s_);}
			virtual bool itf_operator_forward_equalmore(const sub_type& s_)const override{return itf_operator_forward_equal(s_) || itf_operator_forward_more(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_bidirectional_comparable_interface
			:public operator_bidirectional_equal_interface<my_type,sub_type>
			,public operator_bidirectional_compare_interface<my_type,sub_type>
			,public operator_bidirectional_equalcompare_interface<my_type,sub_type>{
		protected:
			virtual bool itf_operator_forward_equalless(const sub_type& s_)const override{return itf_operator_forward_equal(s_) || itf_operator_forward_less(s_);}
			virtual bool itf_operator_forward_equalmore(const sub_type& s_)const override{return itf_operator_forward_equal(s_) || itf_operator_forward_more(s_);}
			virtual bool itf_operator_backward_equalless(const sub_type& s_)const override{return itf_operator_backward_equal(s_) || itf_operator_backward_less(s_);}
			virtual bool itf_operator_backward_equalmore(const sub_type& s_)const override{return itf_operator_backward_equal(s_) || itf_operator_backward_more(s_);}
		};
		template<typename my_type>
		struct operator_comparable_interface
			:public operator_equal_interface<my_type>
			,public operator_compare_interface<my_type>
			,public operator_equalcompare_interface<my_type>{
		protected:
			virtual bool itf_operator_equalless(const my_type& s_)const override{return itf_operator_equal(s_) || itf_operator_less(s_);}
			virtual bool itf_operator_equalmore(const my_type& s_)const override{return itf_operator_equal(s_) || itf_operator_more(s_);}
		};
	}
}
#
#endif
