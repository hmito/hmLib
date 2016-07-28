#ifndef HMLIB_TYPEMANAGE_TYPENAME_INC
#define HMLIB_TYPEMANAGE_TYPENAME_INC 100
#
#include<string>
#include<type_traits>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_type_name{
			std::string operator()(void){
				static_assert(false,"fail to detect type_name class.");
			}
		};
		template<>
		struct default_type_name<bool>{
			std::string operator()(void){return "bool";}
		};
		template<>
		struct default_type_name<int>{
			std::string operator()(void){return "int";}
		};
		template<>
		struct default_type_name<unsigned int>{
			std::string operator()(void){return "unsigned int";}
		};
		template<>
		struct default_type_name<double>{
			std::string operator()(void){return "double";}
		};
		template<typename T>
		std::true_type _have_type_type_name(T* ptr=nullptr,typename T::type_name* t=nullptr);
		template<typename T>
		std::false_type _have_type_type_name(...);
		template<typename T,typename U=decltype(_have_type_type_name<T>())>
		struct have_type_type_name{};
		template<typename T>
		struct have_type_type_name<T,std::true_type>{typedef std::true_type type;};
		template<typename T>
		struct have_type_type_name<T,std::false_type>{typedef std::false_type type;};
		template<typename T,typename U=typename have_type_type_name<T>::type>
		struct autotype_type_name{};
		template<typename T>
		struct autotype_type_name<T,std::true_type>{typedef typename T::type_name type;};
		template<typename T>
		struct autotype_type_name<T,std::false_type>{typedef typename default_type_name<T> type;};
		template<typename T,typename type_name_=autotype_type_name<T>::type>
		struct type_name{
		private:
			type_name_  Type;
		public:
			std::string operator()(void){return Type();}
		};
	}
}
#
#endif
