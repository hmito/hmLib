#ifndef HMLIB_TYPEMANAGE_PARAMETER_INC
#define HMLIB_TYPEMANAGE_PARAMETER_INC 100
#
#include"type_manage.hpp"
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_parameter_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_parameter_manage<my_type>>
		struct parameter{
		private:
			type_manage Type;
		public:
			template<typename output_iterator>
			void operator()(output_iterator itr,const my_type& my)const{
				Type.parameter_list(itr,my);
			}
			template<typename output_iterator>
			void label(output_iterator itr,const std::string& pre)const{
				Type.parameter_label(itr,pre);
			}
			unsigned int size()const{
				return Type.parameter_list_size();
			}
		};
	}
}
#
#endif
