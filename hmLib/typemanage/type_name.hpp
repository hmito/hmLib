#ifndef HMLIB_TYPEMANAGE_TYPENAME_INC
#define HMLIB_TYPEMANAGE_TYPENAME_INC 100
#
#include"type_manage.hpp"
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_type_name_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_type_name_manage<my_type>>
		struct type_name{
		private:
			type_manage  Type;
		public:
			std::string operator()(void)const{
				return Type.name();
			}
			template<typename output_iterator>
			void operator()(output_iterator itr,const std::string& pre)const{
				Type.name_label(itr,pre);
			}
			template<typename output_iterator>
			void operator()(output_iterator itr)const{
				Type.name_list(itr);
			}
			unsigned int size()const{
				return Type.name_list_size();
			}
		};
	}
}
#
#endif
