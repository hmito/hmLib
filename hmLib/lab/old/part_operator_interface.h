﻿		// %Name% %Sign%
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_forward_%Name%_interface{
			private:
				typedef operator_forward_%Name%_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_forward_%Name%(const sub_type& s_)const=0;
			public:
				friend ans_type operator%Sign%(const my_type& m_,const sub_type& s_){return static_cast<const this_type&>(m_).itf_operator_forward_%Name%(s_);}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_backward_%Name%_interface{
			private:
				typedef operator_backward_%Name%_interface<my_type,sub_type,ans_type> this_type;
			protected:
				virtual ans_type itf_operator_backward_%Name%(const sub_type& s_)const=0;
			public:
				friend ans_type operator%Sign%(const sub_type& s_,const my_type& m_){return static_cast<const this_type&>(m_).itf_operator_backward_%Name%(s_);}
		};
		template<typename my_type,typename sub_type>
		struct operator_equal_%Name%_interface{
			private:
				typedef operator_equal_%Name%_interface<my_type,sub_type> this_type;
			protected:
				virtual void itf_operator_equal_%Name%(const sub_type& s_)const=0;
			public:
				friend my_type operator%Sign%=(const my_type& m_,const sub_type& s_){
					static_cast<const this_type&>(m_).itf_operator_equal_%Name%(s_);
					return m_;
				}
		};
		template<typename my_type,typename sub_type,typename ans_type>
		struct operator_bidirectional_%Name%_interface
			:public operator_forward_%Name%_interface<my_type,sub_type,ans_type>
			,public operator_backward_%Name%_interface<my_type,sub_type,ans_type>{
		};
		template<typename my_type,typename ans_type>
		struct operator_%Name%_interface
			:public operator_forward_%Name%_interface<my_type,my_type,ans_type>{
			protected:
				virtual ans_type itf_operator_forward_%Name%(const my_type& m_)const override{return itf_operator_%Name%(m_);}
				virtual ans_type itf_operator_%Name%(const my_type& m_)const=0;	
		};
		template<typename my_type, typename sub_type>
		struct operator_forward_%AbleName%_interface
			:public operator_forward_%Name%_interface<my_type,sub_type,my_type>
			,public operator_equal_%Name%_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_%Name%(const sub_type& s_)const{itf_operator_forward_%Name%(s_);}	
		};
		template<typename my_type, typename sub_type>
		struct operator_bidirectional_%AbleName%_interface
			:public operator_bidirectional_%Name%_interface<my_type,sub_type,my_type>
			,public operator_equal_%Name%_interface<my_type,sub_type>{
			protected:
				virtual void itf_operator_equal_%Name%(const sub_type& s_)const{itf_operator_forward_%Name%(s_);}	
		};
		template<typename my_type>
		struct operator_%AbleName%_interface
			:public operator_%Name%_interface<my_type,my_type>
			,public operator_equal_%Name%_interface<my_type,my_type>{
			protected:
				virtual void itf_operator_equal_%Name%(const my_type& m_)const{itf_operator_%Name%(m_);}	
		};
		
