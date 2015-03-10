#ifndef HMLIB_ITERATORPATTERN_INC
#define HMLIB_ITERATORPATTERN_INC 100
#
#include<iterator>
#ifndef HMLIB_OPERATORPATTERN_INC
#	include "operator_pattern.hpp"
#endif
namespace hmLib{
	namespace pattern{
		struct input_iterator_pattern_category{};
		struct output_iterator_pattern_category{};
		struct forward_const_iterator_pattern_category
			:public input_iterator_pattern_category{};
		struct forward_iterator_pattern_category
			:public input_iterator_pattern_category
			,public output_iterator_pattern_category{};
		struct bidirectional_iterator_pattern_category
			:public forward_iterator_pattern_category{};
		struct bidirectional_const_iterator_pattern_category
			:public forward_const_iterator_pattern_category{};
		struct random_access_iterator_pattern_category
			:public bidirectional_iterator_pattern_category{};
		struct random_access_const_iterator_pattern_category
			:public bidirectional_const_iterator_pattern_category{};
		struct iterator_pattern{};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct input_iterator_pattern
			:public virtual std::iterator<std::input_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public const_reference_pattern<my_type,value_type_>
			,public const_access_pattern<my_type,value_type_>
			,public equal_pattern<my_type>
			,public iterator_pattern{
		public:
			typedef input_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual const value_type_& const_refer()const /*C++0x_override/**/=0;
			virtual const value_type_* const_access()const /*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct output_iterator_pattern
			:public virtual std::iterator<std::output_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public reference_pattern<my_type,value_type_>
			,public access_pattern<my_type,value_type_>
			,public iterator_pattern{
		public:
			typedef output_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual value_type_& refer()/*C++0x_override/**/=0;
			virtual value_type_* access()/*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct forward_iterator_pattern
			:public virtual std::iterator<std::forward_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public reference_pattern<my_type,value_type_>
			,public access_pattern<my_type,value_type_>
			,public equal_pattern<my_type>
			,public iterator_pattern{
		public:
			typedef forward_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual value_type_& refer()/*C++0x_override/**/=0;
			virtual value_type_* access()/*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct forward_const_iterator_pattern
			:public virtual std::iterator<std::forward_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public const_reference_pattern<my_type,value_type_>
			,public const_access_pattern<my_type,value_type_>
			,public equal_pattern<my_type>
			,public iterator_pattern{
		public:
			typedef forward_const_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual const value_type_& const_refer()const /*C++0x_override/**/=0;
			virtual const value_type_* const_access()const /*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct bidirectional_iterator_pattern
			:public virtual std::iterator<std::bidirectional_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public decrement_pattern<my_type>
			,public reference_pattern<my_type,value_type_>
			,public access_pattern<my_type,value_type_>
			,public equal_pattern<my_type>
			,public iterator_pattern{
		public:
			typedef bidirectional_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual void decrement()/*C++0x_override/**/=0;
			virtual const value_type_& refer()const /*C++0x_override/**/=0;
			virtual const value_type_* access()const /*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct bidirectional_const_iterator_pattern
			:public virtual std::iterator<std::bidirectional_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public decrement_pattern<my_type>
			,public const_reference_pattern<my_type,value_type_>
			,public const_access_pattern<my_type,value_type_>
			,public equal_pattern<my_type>
			,public iterator_pattern{
		public:
			typedef bidirectional_const_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual void decrement()/*C++0x_override/**/=0;
			virtual const value_type_& const_refer()const /*C++0x_override/**/=0;
			virtual const value_type_* const_access()const /*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct random_access_iterator_pattern
			:public virtual std::iterator<std::random_access_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public decrement_pattern<my_type>
			,public bidirectional_add_pattern<my_type,difference_type_>
			,public forward_sub_pattern<my_type,difference_type_>
			,public reference_pattern<my_type,value_type_>
			,public index_pattern<my_type,difference_type_,value_type_>
			,public access_pattern<my_type,value_type_>
			,public equalcompare_pattern<my_type>
			,public binary_sub_pattern<my_type,difference_type_>
			,public iterator_pattern{
		public:
			typedef random_access_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual void decrement()/*C++0x_override/**/=0;
			virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/=0;
			virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/=0;
			virtual const value_type_& refer()const/*C++0x_override/**/=0;
			virtual const value_type_& index(const difference_type_& index_)const/*C++0x_override/**/=0;
			virtual const value_type_* access()const/*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/=0;
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/=0;
			virtual difference_type_ forward_binary_sub(const my_type& val_)const /*C++0x_override/**/=0;
		};
		template<typename my_type,typename value_type_,typename difference_type_=int>
		struct random_access_const_iterator_pattern
			:public virtual std::iterator<std::random_access_iterator_tag,value_type_,difference_type_,value_type_*,value_type_&>
			,public increment_pattern<my_type>
			,public decrement_pattern<my_type>
			,public bidirectional_add_pattern<my_type,difference_type_>
			,public forward_sub_pattern<my_type,difference_type_>
			,public const_reference_pattern<my_type,value_type_>
			,public const_index_pattern<my_type,difference_type_,value_type_>
			,public const_access_pattern<my_type,value_type_>
			,public equalcompare_pattern<my_type>
			,public binary_sub_pattern<my_type,difference_type_>
			,public iterator_pattern{
		public:
			typedef random_access_const_iterator_pattern_category pattern_category;
		protected:
			virtual void increment()/*C++0x_override/**/=0;
			virtual void decrement()/*C++0x_override/**/=0;
			virtual void forward_add(const difference_type_& sub_)/*C++0x_override/**/=0;
			virtual void forward_sub(const difference_type_& sub_)/*C++0x_override/**/=0;
			virtual const value_type_& const_refer()const /*C++0x_override/**/=0;
			virtual const value_type_& const_index(const difference_type_& index_)const /*C++0x_override/**/=0;
			virtual const value_type_* const_access()const /*C++0x_override/**/=0;
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/=0;
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/=0;
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/=0;
			virtual difference_type_ forward_binary_sub(const my_type& val_)const /*C++0x_override/**/=0;
		};
	}
}
#
#endif
