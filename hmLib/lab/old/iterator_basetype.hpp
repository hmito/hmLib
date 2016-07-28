#ifndef HMLIB_ITERATORINTERFACE_INC
#define HMLIB_ITERATORINTERFACE_INC 1
#
/*===iterator_interface===
iteratorの基本インターフェースを提供する

iterator_interface:v0_01/130308 hmIto
	iterator_patternから継承
	operator_interfaceに対応
*/
#include<iterator>
#ifndef HMLIB_OPERATORINTERFACE_INC
#	include<hmLib/operator_interface.hpp>
#endif
#ifndef HMLIB_OBJECTINTERFACE_INC
#	include<hmLib/object_interface.hpp>
#endif
namespace hmLib{
	namespace basetype{
		template<typename value_type_>
		struct input_iterator_basetype:public interface::input_iterator_interface<input_iterator_basetype,value_type_>{
			template<target_iterator>struct adaptor;
		};
		template<typename value_type_>
		template<iterator_>
		struct input_iterator_basetype<value_type_>::adaptor:public input_iterator_basetype<value_type_>{
		private:
			typedef input_iterator_basetype<value_type_>::adaptor adaptor;
			typedef input_iterator_basetype<value_type_> basetype;
			typedef iterator_ iterator;
		private:
			iterator titr;
		protected:
			virtual void itf_operator_increment()override{++titr;}
			virtual const value_type_& itf_operator_reference()const override{return *titr;}
			virtual const value_type_* itf_operator_access()const override{return &(*titr);}
			virtual bool itf_operator_equal(const basetype& val_)const override{
				adaptor* adp=dynamic_cast<adaptor*>(&val_);
				if(adp==nullptr)return false;
				return adp->titr==titr;
			}
			virtual basetype* itf_clone(void)const override{new }
		};
	}
}
#
#endif
