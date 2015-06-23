#ifndef HMLIB_TYPEMANAGE_STRINGBUILD_INC
#define HMLIB_TYPEMANAGE_STRINGBUILD_INC 100
#
#include<type_traits>>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_string_build:public T::string_build{};
		template<typename T,typename string_build_=default_string_build<T>>
		struct string_build{
		private:
			string_build_  Type;
		public:
			template<typename input_iterator>
			T operator()(input_iterator begin_,input_iterator end_){
				return Type(begin_,end_);
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

