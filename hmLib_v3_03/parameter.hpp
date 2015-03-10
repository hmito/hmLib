#ifndef HMLIB_PARAMETER_INC
#define HMLIB_PARAMETER_INC 101
#
#include<iostream>
#include<string>
#include<sstream>
#include<type_traits>
#include<typeinfo>
namespace hmLib{
	//parameter処理用のクラス
	template<typename parameterizer_tag_,typename value_type_>
	struct parameterizer{
		typedef parameterizer_tag_ parameterizer_tag;
		typedef value_type_ value_type;
	};
	//parameterizerの機能識別用タグ
	struct basic_parameterizer_tag{};
	struct type_name_parameterizer_tag:basic_parameterizer_tag{
/*
	public://type_name_parameterizer_tag
		static std::string type_name(){
			return std::typeinfo(T).name();
		}
*/
	};
	struct iterator_build_parameterizer_tag:basic_parameterizer_tag{
/*
	public://iterator_build_parameterizer_tag
		template<class InputIterator>
		static value_type build(InputIterator Begin,InputIterator End){
			double Sigma=boost::lexical_cast<double>(*Begin);
			++Begin;
			hmLib_assert_normal(Begin==End,"Parameter num Error");
			return value_type(Sigma);
		}
*/
	};
	struct out_prm_parameterizer:basic_parameterizer_tag{
/*
	public://out_prm_parameterizer
		static unsigned int num_prm(bool WithType,bool WithValue){
			unsigned int ans=0;
			if(WithValue)ans+=1;
			return ans;
		}
		template<typename prmoutput_>
		static void out_prm_name(prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<"Sigma";
			return out;
		}
	public://out_prm_parameterizer
		template<typename prmoutput_>
		void out_prm_val(value_type& Value,prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<Value.Sigma;
			return out;
		}
*/
/*
	class output{
	public:
		output operator+(conststd::string& Name_);
		template<class T>
		output& operator<<(T t);
		void endl();
	};
*/
	};
	struct normal_parameterizer_tag:type_name_parameterizer_tag,iterator_build_parameterizer_tag,out_prm_parameterizer{
/*
	public://type_name_parameterizer_tag
		static std::string type_name(){
			return std::typeinfo(T).name();
		}
	public://iterator_build_parameterizer_tag
		template<class InputIterator>
		static value_type build(InputIterator Begin,InputIterator End){
			double Sigma=boost::lexical_cast<double>(*Begin);
			++Begin;
			hmLib_assert_normal(Begin==End,"Parameter num Error");
			return value_type(Sigma);
		}
	public://out_prm_parameterizer
		static unsigned int num_prm(bool WithType,bool WithValue){
			unsigned int ans=0;
			if(WithValue)ans+=1;
			return ans;
		}
		template<typename prmoutput_>
		static void out_prm_name(prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<"Sigma";
		}
	public://out_prm_parameterizer
		template<typename prmoutput_>
		static void out_prm_val(value_type& Value,prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<Value.Sigma;
		}
*/
	};
	//基本変数用のparameterizer
	template<typename T>
	struct default_parameterizer:public parameterizer<normal_parameterizer_tag,T>{
	public://type_name_parameterizer_tag
		static std::string type_name(){
			return std::typeinfo(T).name();
		}
	public://out_prm_parameterizer
		template<class InputIterator>
		static value_type build(InputIterator Begin,InputIterator End){
			value_type Val=boost::lexical_cast<value_type>(*Begin);
			++Begin;
			hmLib_assert_normal(Begin==End,"Parameter num Error");
			return Val;
		}
		static unsigned int num_prm(bool WithType,bool WithValue){
			unsigned int ans=0;
			if(WithValue)ans+=1;
			return ans;
		}
		template<typename prmoutput_>
		static void out_prm_name(prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<"Value";
			return out;
		}
	public:
		template<typename prmoutput_>
		void out_prm_val(value_type& Value,prmoutput_ out,bool WithType,bool WithValue){
			if(WithValue)out<<Value;
			return out;
		}
	};
	//メンバクラスにparameterizer型があるかを判定するメタクラス
	template<typename T>
	std::true_type _have_type_parameterizer(T* ptr=nullptr,typename T::parameterizer* t=nullptr);
	template<typename T>
	std::false_type _have_type_parameterizer(...);
	template<typename T,typename U=decltype(_have_type_parameterizer<T>())>
	struct have_type_parameterizer{};
	template<typename T>
	struct have_type_parameterizer<T,std::true_type>{typedef std::true_type type;};
	template<typename T>
	struct have_type_parameterizer<T,std::false_type>{typedef std::false_type type;};
	template<typename T,typename U=typename have_type_parameterizer<T>::type>
	struct auto_type_parameterizer{};
	template<typename T>
	struct auto_type_parameterizer<T,std::true_type>{typedef typename T::parameterizer type;};
	template<typename T>
	struct auto_type_parameterizer<T,std::false_type>{typedef default_parameterizer<T> type;};
	//parameterクラス
	template<typename T>
	struct basic_parameter{
		typedef T value_type;
		typedef basic_parameter<T> my_type;
	protected:
		value_type& Value;
	protected:
		basic_parameter(value_type& Value_):Value(Value_){}
		basic_parameter(my_type& My_):Value(My_.Value){}
	public:
		value_type& parameter_ref(){return Value;}
		const value_type& parameter_cref()const{return Value;}
	};
	//parameterクラス
//	template<typename T,typename P=typename T::parameterizer,typename PT=typename P::parameterizer_tag>
	template<typename T,typename P=typename auto_type_parameterizer<T>::type,typename PT=typename auto_type_parameterizer<T>::type::parameterizer_tag>
	struct parameter:basic_parameter<T>{
		typedef P parameterizer_type;
	private:
		typedef basic_parameter<T> base_type;
		typedef parameter<T,P,PT> my_type;
	public:
		parameter(value_type& Value_):base_type(Value_){}
		parameter(my_type& My_):base_type(My_){}
	};
	template<typename T,typename P>
	struct parameter<T,P,type_name_parameterizer_tag>:basic_parameter<T>{
		typedef P parameterizer_type;
	private:
		typedef basic_parameter<T> base_type;
		typedef parameter<T,P,type_name_parameterizer_tag> my_type;
	public:
		parameter(value_type& Value_):base_type(Name_,Value_){}
		parameter(my_type& My_):base_type(My_){}
	public:
		static std::string type_name(){return parameterizer_type::type_name();}
	};
	template<typename T,typename P>
	struct parameter<T,P,normal_parameterizer_tag>:basic_parameter<T>{
		typedef P parameterizer_type;
	private:
		typedef basic_parameter<T> base_type;
		typedef parameter<T,P,normal_parameterizer_tag> my_type;
	public:
		parameter(value_type& Value_):base_type(Value_){}
		parameter(my_type& My_):base_type(My_){}
	public:
		static std::string type_name(){return parameterizer_type::type_name();}
	public://iterator_build_parameterizer_tag
		template<class InputIterator>
		static value_type build(InputIterator Begin,InputIterator End){return parameterizer_type::build(Begin,End);}
	public://out_prm_parameterizer
		static unsigned int num_prm(bool WithType,bool WithValue){return parameterizer_type::num_prm(WithType,WithValue);}
		template<typename prmoutput_>
		static void out_prm_name(prmoutput_ out,bool WithType,bool WithValue){parameterizer_type::out_prm_name(out,WithType,WithValue);}
	public://out_prm_parameterizer
		template<typename prmoutput_>
		void out_prm_val(prmoutput_ out,bool WithType,bool WithValue){parameterizer_type::out_prm_val(Value,out,WithType,WithValue);}
	public:
		static unsigned int num_build(){return num_prm(false,true);}
	};
}
#
#endif
