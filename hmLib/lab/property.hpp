#ifndef HMLIB_PROPERTY_INC
#define HMLIB_PROPERTY_INC 101
#
/*=== property ===
要望
・operator +=等に対応してほしい
・defaultでは、普通に設定できるようにしてほしい
・というか、デフォルトで使えないならコンストラクタで設定させろよ
=>コンストラクタを利用すると、コピーコンストラクタが使用できなくなってしまう仕様っぽい
ってか、これかなり致命的なバグを生む。
等価代入演算子はいいとしても、コピーコンストラクタは一切の値のコピーを行っていない模様。
あ、でもpropety以外の本ちゃんの値があればいいのかな？

一つの方法は、値を保持するタイプ。
もう一つの方法は、値の参照を保持するタイプ。
とりあえず、参照型にしておきませんか？
*/
//#include <memory>
#include<functional>
namespace hmLib {
	/*
	template<typename property_type_>
	struct property_interface {
	protected:
		virtual const property_type get()const=0;
		virtual const property_type set(const property_type& val)=0;
	public:
		const property_type operator()(void) const { return get(); }
		const property_type operator()(const property_type& Value_) {
			return set(Value_);
		}
		operator const property_type() const { return  get(); }
		const property_type operator=(const property_type& Value_) { return set(Value_); }
	};
	template<typename property_type_,typename terget_type_=property_type_>
	struct property:public property_interface<property_type_>{
		typedef property_type_ property_type;
		typedef terget_type_ terget_type;
	protected:
		terget_type& ref;
	public:
		property()=delete;
		property(const property& my_)=delete;
		explicit property(terget_type& ref_): ref(ref_) {}
	protected:
		virtual const property_type get()const {
			return static_cast<const property_type>(ref);
		}
		virtual const property_type set(const property_type& val) {
			ref=val;
			return static_cast<const property_type>(ref);
		}
	};


#define arranged_propery(type,name,func) struct name##_property :public property<type>{\
	protected:\
		const type get()const override { func; }\
	public:\
		name##_property(type& ref_) :property(ref_) {}\
		}name;*/

	/*
	template<typename T>
	struct accesser {
		struct default_accesser {
		private:
			T& Ref;
		public:
			default_accesser(T& Ref_) :Ref(Ref_) {}
			const T get() const { return Ref; }
			const T set(const T& Val_) { return Ref=Val_; }
		};
		struct accesser_adaptor_interface {
			virtual const T get()const=0;
			virtual const T set(const T& Val_)=0;
		};
		template<typename accesser_>
		struct accesser_adaptor :public accesser_adaptor_interface {
		private:
			accesser_ Accesser;
		public:
			accesser_adaptor(accesser_ Accesser_) :Accesser(Accesser_) {}
			virtual const T get()const {
				return Accesser.get();
			}
			virtual const T set(const T& Val_) {
				return Accesser.set(Val_);
			}
		};
	private:
		std::unique_ptr<accesser_adaptor_interface> pAccesserAdaptor;
	public:
		accesser(T& Ref_)
			: pAccesserAdaptor(new accesser_adaptor<default_accesser>(Ref_)){
		}
		template<typename accesser_>
		accesser(accesser_ Accesser_)
			: pAccesserAdaptor(new accesser_adaptor<accesser_>(Accesser_)) {
		}
	public:
		const T get()const { return pAccesserAdaptor->get(); }
		const T set(const T& Val_) { return pAccesserAdaptor->set(Val_); }
	};
	template<typename T, typename accesser_=accesser<T>>
	class property {
	private:
		accesser_ Accesser;
	public:
		property()=delete;
		property(const property& my_)=delete;
		explicit property(accesser_ Accesser_): Accesser(Accesser_){}
		explicit property(T& Ref_): Accesser(Ref_) {}
		property& operator=(const property& other) {
			Accesser.set(other.Accesser.get());
			return *this;
		}
		const T operator()(void) const { return Accesser.get(); }
		const T operator()(const T & Value_) {
			return Accesser.set(Value_);
		}
		operator const T() const { return  Accesser.get(); }
		const T operator=(const T & Value_) { return Accesser.set(Value_); }
	};
	*/


	namespace properties {
		struct no_initialization {};
	}
	template<typename property_type_>
	class property {
	public:
		typedef property_type_ property_type;
		typedef std::function<property_type()> getter_type;
		typedef std::function<property_type(property_type)> setter_type;
	private:
		getter_type getter;
		setter_type setter;
	public:
		property()=delete;
		property(const property& my_)=delete;
		property& operator=(const property& other) {
			setter( other.getter());
			return *this;
		}
		explicit property(properties::no_initialization) {}
		explicit property(property_type_& Ref_)
			: getter([&]()->property_type {return Ref_; })
			, setter([&](property_type Value_)->property_type {Ref_=Value_; return Ref_; }) {
			std::cout<<"normal version"<<std::endl;
		}
		property(getter_type getter_, setter_type setter_)
			: getter(getter_)
			, setter(setter_) {
		}
		void initialize(getter_type getter_, setter_type setter_) {
			getter=getter_;
			setter=setter_;
		}
		const property_type operator()(void) const { return getter(); }
		const property_type operator()(const property_type & Value_) { return setter(Value_); }
		operator const property_type() const { return getter(); }
		const property_type operator=(const property_type & Value_) { return setter(Value_); }
	};
	template<typename property_type_>
	class property <const property_type_>{
	public:
		typedef property_type_ property_type;
		typedef std::function<property_type()> getter_type;
	private:
		getter_type getter;
	public:
		property()=delete;
		property(const property& my_)=delete;
		explicit property(properties::no_initialization) {}
		explicit property(const property_type_& Ref_)
			: getter([&]()->property_type {return Ref_; }){
		}
		explicit property(getter_type getter_)
			: getter(getter_){}
		void initialize(getter_type getter_) {
			getter=getter_;
		}
		const property_type operator()(void) const { return getter(); }
		operator const property_type() const { return getter(); }
	};
	template<typename property_type_>
	class property<property_type_&> {
	public:
		typedef property_type_ property_type;
		typedef std::function<const property_type&()> getter_type;
		typedef std::function<const property_type&(const property_type&)> setter_type;
	private:
		getter_type getter;
		setter_type setter;
	public:
		property()=delete;
		property(const property& my_)=delete;
		property& operator=(const property& other) {
			setter(other.getter());
			return *this;
		}
		explicit property(properties::no_initialization) {}
		explicit property(property_type_& Ref_)
			: getter([&]()->const property_type& {return Ref_; })
			, setter([&](const property_type & Value_)->const property_type&{Ref_=Value_; return Ref_;}) {
		}
		property(getter_type getter_, setter_type setter_)
			: getter(getter_)
			, setter(setter_) {}
		void initialize(getter_type getter_, setter_type setter_) {
			getter=getter_;
			setter=setter_;
		}
		const property_type& operator()(void) const { return getter(); }
		const property_type& operator()(const property_type& Value_) { return setter(Value_); }
		operator const property_type&() const { return getter(); }
		const property_type& operator=(const property_type& Value_) { return setter(Value_); }
	};
	template<typename property_type_>
	class property <const property_type_&>{
	public:
		typedef property_type_ property_type;
		typedef std::function<const property_type& ()> getter_type;
	private:
		getter_type getter;
	public:
		property()=delete;
		property(const property& my_)=delete;
		explicit property(properties::no_initialization) {}
		explicit property(const property_type_& Ref_)
			: getter([&]()->const property_type&{return Ref_;}) {
		}
		explicit property(getter_type getter_)
			: getter(getter_) {}
		void initialize(getter_type getter_) {
			getter=getter_;
		}
		const property_type& operator()(void) const { return getter(); }
		operator const property_type&() const { return getter(); }
	};

	
	/*
	namespace properties {
		struct no_initialize;
	}
	template<typename property_type_, typename terget_type_=property_type_>
	class property {
	public:
		typedef property_type_ property_type;
		typedef terget_type_ terget_type;
		typedef std::function<property_type(const terget_type&)> getter_type;
		typedef std::function<property_type(terget_type&, const property_type&)> setter_type;
	private:
		terget_type& Ref;
		getter_type getter;
		setter_type setter;
	public:
		property()=delete;
		property(const property& my_)=delete;
		property(properties::no_initialize NoInitialize) {}
		property(terget_type_& Ref_)
			:Ref(Ref_)
			, getter([](const terget_type& Ref_)->property_type {return Ref_; })
			, setter([](terget_type& Ref_, const property_type & Value_)->property_type {Ref_=Value_; return Ref_; }) {
		}
		property(terget_type_& Ref_, getter_type getter_,setter_type setter_)
			:Ref(Ref_) 
			,getter(getter_)
			,setter(setter_){
		}
		const property_type operator()(void) const { return getter(Ref); }
		const property_type operator()(const property_type & Value_) { return setter(Ref, Value_); }
		operator const property_type() const { return getter(Ref); }
		const property_type operator=(const property_type & Value_) { return setter(Ref, Value_); }
		property& operator=(const property& other) {
			setter(Ref, other.getter(other.Ref));
			return *this;
		}
	};
	*/
}
#
#endif
