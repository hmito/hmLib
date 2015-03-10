#ifndef HMLIB_TYPEMANAGE_LISTUPPARAMETER_INC
#define HMLIB_TYPEMANAGE_LISTUPPARAMETER_INC 100
#
#include"type_manage.hpp"
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_listup_parameter_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_listup_parameter_manage<my_type>>
		struct listup_parameter{
		private:
			type_manage Type;
		public:
			template<typename output_iterator>
			output_iterator operator()(output_iterator itr,const my_type& my)const{
				Type.parameter_list(itr,my);
				itr+=size();
				return itr;
			}
			template<typename output_iterator>
			output_iterator operator()(output_iterator itr,const std::string& pre,const std::string& name)const{
				Type.parameter_label(itr,pre+name+"::");
				itr+=size();
				return itr;
			}
			unsigned int size()const{
				return Type.parameter_list_size();
			}
		};
	}
}
#
#endif
