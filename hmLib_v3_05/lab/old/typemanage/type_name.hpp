#ifndef HMLIB_TYPEMANAGE_TYPENAME_INC
#define HMLIB_TYPEMANAGE_TYPENAME_INC 100
#
#include<string>
#include<type_traits>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_type_name:public T::type_name{};
		template<>
		struct default_type_name<bool>{
			std::string operator()(void){return "bool";}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_)const{
				(*itr++)=Pre_+"bool";
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_name<int>{
			std::string operator()(void){return "int";}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_)const{
				(*itr++)=Pre_+"int";
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_name<unsigned int>{
			std::string operator()(void){return "unsigned int";}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_)const{
				(*itr++)=Pre_+"unsigned int";
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<>
		struct default_type_name<double>{
			std::string operator()(void){return "double";}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_)const{
				(*itr++)=Pre_+"double";
			}
			unsigned int size()const{
				return 1;
			}
		};
		template<typename T,typename type_name_=default_type_name<T>>
		struct type_name{
		private:
			type_name_  Type;
		public:
			std::string operator()(void)const{
				return Type();
			}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_)const{
				Type(itr_,Pre_);
			}
			unsigned int size()const{
				return Type.size();
			}
		};
	}
}
#
#endif
