#ifndef HMLIB_TYPEMANAGE_STRINGBUILD_INC
#define HMLIB_TYPEMANAGE_STRINGBUILD_INC 100
#
#include<type_traits>
#include<boost/lexical_cast.hpp>
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace typemanage{
		template<typename T>
		struct default_string_build{
			template<typename input_iterator>
			T operator()(input_iterator begin_,input_iterator end_){
				input_iterator itr(begin_);
				hmLib_assert(itr!=end_,hmLib::exceptions::out_of_range_access,"invalid number of given parameters.");
				T ans=boost::lexical_cast<T>(*(itr++));
				hmLib_assert((itr-begin_)==size(),hmLib::exceptions::out_of_range_access,"invalid used number of parameters");
				return std::move(ans);
			}
			unsigned int size()const{return 1;}
		};
		//メンバクラスにstring_buildizer型があるかを判定するメタクラス
		template<typename T>
		std::true_type _have_type_string_build(T* ptr=nullptr,typename T::string_build* t=nullptr);
		template<typename T>
		std::false_type _have_type_string_build(...);
		template<typename T,typename U=decltype(_have_type_string_build<T>())>
		struct have_type_string_build{};
		template<typename T>
		struct have_type_string_build<T,std::true_type>{typedef std::true_type type;};
		template<typename T>
		struct have_type_string_build<T,std::false_type>{typedef std::false_type type;};
		template<typename T,typename U=typename have_type_string_build<T>::type>
		struct autotype_string_build{};
		template<typename T>
		struct autotype_string_build<T,std::true_type>{typedef typename T::string_build type;};
		template<typename T>
		struct autotype_string_build<T,std::false_type>{typedef typename default_string_build<T> type;};
		template<typename T,typename string_build_=autotype_string_build<T>::type>
		struct string_build{
		private:
			string_build_  Type;
		public:
			template<typename input_iterator>
			T operator()(input_iterator begin_,input_iterator end_){return Type(begin_,end_);}
			unsigned int size()const;
		};
	}
}
#
#endif
