#ifndef HMLIB_TYPEMANAGE_LISTUPNAME_INC
#define HMLIB_TYPEMANAGE_LISTUPNAME_INC 100
#
/*===typemanage::listup_name===
自身と自身の保持するクラスの名前情報をリストアップするクラス

listup_name:v1_00/130711 hmIto
	リリース
*/
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_listup_name_manage:public default_type_manage<my_type>{};
		template<typename my_type,typename type_manage=default_listup_name_manage<my_type>>
		struct listup_name{
		private:
			type_manage  Type;
		public:
			template<typename output_iterator>
			output_iterator operator()(output_iterator itr,const std::string& pre,const std::string& name){
				(*itr++)=pre+name;
				Type.name_label(itr,pre+name+"::");
				itr+=Type.name_list_size();
				return itr;
			}
			template<typename output_iterator>
			output_iterator operator()(output_iterator itr){
				(*itr++)=Type.name();
				Type.name_list(itr);
				itr+=Type.name_list_size();
				return itr;
			}
			unsigned int size()const{
				return 1+Type.name_list_size();
			}
		};
	}
}
#
#endif
