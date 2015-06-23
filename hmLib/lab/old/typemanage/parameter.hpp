#ifndef HMLIB_TYPEMANAGE_PARAMETER_INC
#define HMLIB_TYPEMANAGE_PARAMETER_INC 100
#
#include<type_traits>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_parameter:public T::parameter{};
		template<typename T,typename parameter_=default_parameter<T>>
		struct parameter{
		private:
			parameter_ Type;
		public:
			template<typename output_iterator>
			void operator()(output_iterator itr_,const T& t_)const{
				Type(itr_,t_);
			}
			template<typename output_iterator>
			void label(output_iterator itr_,const std::string& Pre_)const{
				Type.label(itr_,Pre_);
			}
			unsigned int size()const{
				return Type.size();
			}
		};
	}
}
#
#endif
