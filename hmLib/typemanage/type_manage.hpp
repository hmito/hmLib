#ifndef HMLIB_TYPEMANAGE_TYPEMANAGE_INC
#define HMLIB_TYPEMANAGE_TYPEMANAGE_INC 100
#
#include<string>
#include<boost/lexical_cast.hpp>
namespace hmLib{
	namespace typemanage{
		template<typename my_type>
		struct default_type_manage:public my_type::type_manage{};
		template<>
		struct default_type_manage<bool>{
			//クラス名を返す
			std::string name()const{return "bool";}
			template<typename output_iterator>
			void name_list(output_iterator itr,const std::string& pre)const{
				(*itr++)=pre+name();
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_manage<unsigned int>{
			//クラス名を返す
			std::string name()const{return "unsigned int";}
			template<typename output_iterator>
			void name_list(output_iterator itr,const std::string& pre)const{
				(*itr++)=pre+name();
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_manage<int>{
			//クラス名を返す
			std::string name()const{return "int";}
			template<typename output_iterator>
			void name_list(output_iterator itr,const std::string& pre)const{
				(*itr++)=pre+name();
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_manage<double>{
			//クラス名を返す
			std::string name()const{return "double";}
			template<typename output_iterator>
			void name_list(output_iterator itr,const std::string& pre)const{
				(*itr++)=pre+name();
			}
			unsigned int size()const{
				return 1;
			}
		};
/*		struct type_manage{
			//クラス名を返す
			std::string name()const;
			//クラス名を羅列する
			template<typename output_iterator>
			void name_label(output_iterator itr,const std::string& pre)const;
			//クラス名を羅列する
			template<typename output_iterator>
			void name_list(output_iterator itr)const;
			//クラス名の羅列数を返す
			unsigned int name_list_size()const;
			//文字列からコンストラクトする
			template<typename input_iterator>
			my_type string_build(input_iterator begin,input_iterator end)const;
			//作成に必要な文字列
			template<typename output_iterator>
			void string_build_list(output_iterator itr,const std::string& pre)const:
			//作成に必要な文字列の数
			unsigned int string_build_list_size()const;
			//パラメータの値を与える
			template<typename output_iterator>
			void parameter_label(output_iterator itr,const my_type& val)const;
			//パラメータの名前を与える
			template<typename output_iterator>
			void parameter_list(output_iterator itr, const std::string& pre)const;
			//パラメータの数を与える
			unsigned int parameter_list_size()const;
		};
*/
	}
}
#
#endif
