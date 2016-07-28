#ifndef HMLIB_TYPEMANAGE_STRINGBUILD_INC
#define HMLIB_TYPEMANAGE_STRINGBUILD_INC 100
#
#include"type_manage.hpp"
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_string_build_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_string_build_manage<my_type>>
		struct string_build{
		private:
			type_manage  Type;
		public:
			template<typename input_iterator>
			my_type operator()(input_iterator begin,input_iterator end){
				return Type.string_build(begin,end);
			}
			template<typename output_iterator>
			void label(output_iterator itr,const std::string& pre)const{
				Type.string_build_list(itr,pre);
			}
			unsigned int size()const{
				return Type.string_build_list_size();
			}
		};
	}
}
#
#endif
