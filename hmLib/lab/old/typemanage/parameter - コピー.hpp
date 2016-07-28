#ifndef HMLIB_TYPEMANAGE_PARAMETER_INC
#define HMLIB_TYPEMANAGE_PARAMETER_INC 100
#
#include<type_traits>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_parameter{
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_,bool WithType,bool WithValue)const{
				static_assert(std::false_type::value,"fail to detect parameter class.");
			}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const T& t_,bool WithType,bool WithValue)const{
				static_assert(std::false_type::value,"fail to detect parameter class.");
			}
			unsigned int size(bool WithType,bool WithValue)const{
				static_assert(std::false_type::value,"fail to detect parameter class.");
			}
		};
		//メンバクラスにparameterizer型があるかを判定するメタクラス
		template<typename T>
		std::true_type _have_type_parameter(T* ptr=nullptr,typename T::parameter* t=nullptr);
		template<typename T>
		std::false_type _have_type_parameter(...);
		template<typename T,typename U=decltype(_have_type_parameter<T>())>
		struct have_type_parameter{};
		template<typename T>
		struct have_type_parameter<T,std::true_type>{typedef std::true_type type;};
		template<typename T>
		struct have_type_parameter<T,std::false_type>{typedef std::false_type type;};
		template<typename T,typename U=typename have_type_parameter<T>::type>
		struct autotype_parameter{};
		template<typename T>
		struct autotype_parameter<T,std::true_type>{typedef typename T::parameter type;};
		template<typename T>
		struct autotype_parameter<T,std::false_type>{typedef typename default_parameter<T> type;};
		template<typename T,typename parameter_=autotype_parameter<T>::type>
		struct parameter{
		private:
			parameter_ Type;
		public:
			template<typename output_iterator>
			void operator()(output_iterator itr_,const std::string& Pre_,bool WithType,bool WithValue)const{
				Type(itr_,Pre_,WithType,WithValue);
			}
			template<typename output_iterator>
			void operator()(output_iterator itr_,const T& t_,bool WithType,bool WithValue)const{
				Type(itr_,t_,WithType,WithValue);
			}
			unsigned int size(bool WithType,bool WithValue)const{
				return Type.size(WithType,WithValue);
			}
		};
	}
}
#
#endif
