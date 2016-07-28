#ifndef HMLIB_TYPEMANAGE_LISTUPSTRINGBUILD_INC
#define HMLIB_TYPEMANAGE_LISTUPSTRINGBUILD_INC 100
#
#include"type_manage.hpp"
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_listup_string_build_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_listup_string_build_manage<my_type>>
		struct listup_string_build{
		private:
			type_manage  Type;
		public:
			template<typename output_iterator>
			output_iterator operator()(output_iterator itr,const std::string& pre,const std::string& name)const{
				Type.string_build_list(itr,pre+name+"::");
				itr+=size();
				return itr;
			}
			unsigned int size()const{
				return Type.string_build_list_size();
			}
		};
	}
}
#
#endif
