#ifndef HMLIB_ITERATORINTERFACE_INC
#define HMLIB_ITERATORINTERFACE_INC 1
#
/*===iterator_interface===
iteratorの基本インターフェースを提供する

iterator_interface:v0_01/130308 hmIto
	iterator_patternから継承
	operator_interfaceに対応
	まだ高速化できる
		equal_addをデフォルトで。
		operatorではcopyできないことを前提にしている。
*/
#include<iterator>
#ifndef HMLIB_OPERATORINTERFACE_INC
#	include<hmLib/operator_interface.hpp>
#endif
namespace hmLib{
	namespace interfaces{
		struct input_iterator_interface_category{};
		struct output_iterator_interface_category{};
		struct const_forward_iterator_interface_category:public input_iterator_interface_category{};
		struct forward_iterator_interface_category:public input_iterator_interface_category,public output_iterator_interface_category{};
		struct bidirectional_iterator_interface_category:public forward_iterator_interface_category{};
		struct const_bidirectional_iterator_interface_category:public const_forward_iterator_interface_category{};
		struct random_access_iterator_interface_category:public bidirectional_iterator_interface_category{};
		struct const_random_access_iterator_interface_category:public const_bidirectional_iterator_interface_category{};
		template<typename iterator_>
		struct iterator_interface:public iterator_{};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::input_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct input_iterator_interface
			:public iterator_interface<iterator_>
			,public operator_increment_interface<my_type>
			,public operator_const_reference_interface<value_type_>
			,public operator_const_access_interface<value_type_>
			,public operator_equal_interface<my_type>{
		public:
			typedef input_iterator_interface_category pattern_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
			virtual const value_type_* itf_operator_access()const override{return &itf_operator_reference();}
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::output_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct output_iterator_interface
			:public iterator_interface<iterator_>
			,public operator_increment_interface<my_type>
			,public operator_reference_interface<value_type_>
			,public operator_access_interface<value_type_>{
		public:
			typedef output_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual const value_type_& itf_operator_reference()override=0;
			virtual const value_type_* itf_operator_access()const override{return &itf_operator_reference();}
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::forward_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct forward_iterator_interface
			:public iterator_interface<iterator_>
			,public operator_increment_interface<my_type>
			,public operator_reference_interface<value_type_>
			,public operator_access_interface<value_type_>
			,public operator_equal_interface<my_type>{
		public:
			typedef forward_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual const value_type_* itf_operator_access()const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::forward_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct const_forward_iterator_interface
			:public iterator_interface<iterator_>
			,public operator_increment_interface<my_type>
			,public operator_const_reference_interface<value_type_>
			,public operator_const_access_interface<value_type_>
			,public operator_equal_interface<my_type>{
		public:
			typedef const_forward_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual const value_type_* itf_operator_access()const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::bidirectional_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct bidirectional_iterator_interface
			:public forward_iterator_interface<my_type,value_type_,difference_type_,iterator_>
			,public operator_decrement_interface<my_type>{
		public:
			typedef bidirectional_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual void itf_operator_decrement()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual const value_type_* itf_operator_access()const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::bidirectional_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct const_bidirectional_iterator_interface
			:public const_forward_iterator_interface<my_type,value_type_,difference_type_,iterator_>
			,public operator_decrement_interface<my_type>{

		public:
			typedef const_bidirectional_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual void itf_operator_decrement()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual const value_type_* itf_operator_access()const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::random_access_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct random_access_iterator_interface
			:public bidirectional_iterator_interface<my_type,value_type_,difference_type_,iterator_>
			,public operator_bidirectional_addable_interface<my_type,difference_type_>
			,public operator_index_interface<difference_type_,value_type_>
			,public operator_forward_subable_interface<my_type,difference_type_>
			,public operator_compare_interface<my_type>
			,public operator_equalcompare_interface<my_type>
			,public operator_sub_interface<my_type,difference_type_>{
		public:
			typedef random_access_iterator_interface<my_type,value_type_,difference_type_,iterator_> iterator_interface_type;
			typedef random_access_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual void itf_operator_decrement()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
//			virtual void itf_operator_equal_add(const difference_type_& s_)const override=0;
			virtual my_type itf_operator_forward_add(const difference_type_& s_)const override=0;
			virtual difference_type_ itf_operator_sub(const my_type& s_)const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
			virtual const value_type_* itf_operator_access()const override{return &itf_operator_reference();}
			virtual const value_type_& itf_operator_index(const difference_type_& s_)const override{return static_cast<iterator_interface_type&>(itf_operator_forward_add(s_)).itf_operator_reference();}
			virtual bool itf_operator_less(const my_type& val_)const override{return itf_operator_sub(val_)<0;}
			virtual my_type itf_operator_backward_add(const difference_type_& s_)const override{return itf_operator_forward_add(s_);}
			virtual my_type itf_operator_forward_sub(const difference_type_& s_)const override{return itf_operator_forward_add(-s_);}
			virtual bool itf_operator_equalless(const my_type& val_)const override{return itf_operator_equal(val_) || itf_operator_less(val_);}
		};
		template<typename my_type,typename value_type_,typename difference_type_=int,typename iterator_=std::iterator<std::random_access_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>>
		struct const_random_access_iterator_interface
			:public const_bidirectional_iterator_interface<my_type,value_type_,difference_type_,iterator_>
			,public operator_bidirectional_addable_interface<my_type,difference_type_>
			,public operator_forward_subable_interface<my_type,difference_type_>
			,public operator_const_index_interface<difference_type_,value_type_>
			,public operator_compare_interface<my_type>
			,public operator_equalcompare_interface<my_type>
			,public operator_sub_interface<my_type,difference_type_>{
		public:
			typedef const_random_access_iterator_interface_category interface_category;
		protected:
			virtual void itf_operator_increment()override=0;
			virtual void itf_operator_decrement()override=0;
			virtual const value_type_& itf_operator_reference()const override=0;
			virtual my_type itf_operator_forward_add(const difference_type_& s_)const override=0;
			virtual difference_type_ itf_operator_sub(const my_type& s_)const override=0;
			virtual bool itf_operator_equal(const my_type& val_)const override=0;
			virtual const value_type_* itf_operator_access()const override{return &itf_operator_reference();}
			virtual const value_type_& itf_operator_index(const difference_type_& s_)const override{return itf_operator_reference(itf_operator_forward_add(s_));}
			virtual bool itf_operator_less(const my_type& val_)const override{return itf_operator_sub(val_)<0:}
			virtual my_type itf_operator_backward_add(const difference_type_& s_)const override{return itf_operator_forward_add(s_);}
			virtual my_type itf_operator_forward_sub(const difference_type_& s_)const override{return itf_operator_forward_add(-s_);}
			virtual bool itf_operator_equalless(const my_type& val_)const override{return itf_operator_equal(val_) || itf_operator_less(val_);}
		};
	}
}
#
#endif
