#ifndef HMLIB_PROPERTIES_PROPERTY_INC
#define HMLIB_PROPERTIES_PROPERTY_INC 101
#
/*
変数へのアクセス制御可能でありながら、変数のように扱える、property変数を定義
参照先の寿命管理はしていないので、原則として、同一スコープの変数のみ参照可能
別スコープの変数を利用する際は、weak_propertyを利用する

=== properties::property ===
v1_01/140621 hmIto
	referenceタイプの特殊化は必要ないことを確認、削除
v1_00/140616 hmIto
	仕様策定
*/
#include <functional>
#include <type_traits>
#include "utility.hpp"
#include "exceptions.hpp"
#include "property_connection.hpp"
namespace hmLib {
	namespace properties {
		template<typename T>
		class property:public connection_host_mixin{
		public:
			typedef typename std::remove_reference<T>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T(void)> getter_func;
			typedef std::function<const T(const T)> setter_func;
		protected:
			getter_func getter;
			setter_func setter;
		public:
			//delete default constructor
			property()=delete;
			//delete copy constructor
			property(const property& my_)=delete;
			const property& operator=(const property& other)=delete;
			//delete move constructor
			property(property&& my_)=delete;
			const property& operator=(property&& other)=delete;
			//construct with accessor
			explicit property(reference_type Ref_)
				: getter([&]()->const T {return Ref_; })
				, setter([&](const T Value_)->const T{Ref_=Value_; return Ref_;}) {
			}
			property(reference_type Ref_, setter_func setter_)
				: getter([&]()->const T{return Ref_;})
				, setter(setter_) {
			}
			property(getter_func getter_, setter_func setter_)
				: getter(getter_)
				, setter(setter_) {
			}
			//no initializ
			explicit property(no_initialization) {}
			void initialize(reference_type Ref_) {
				hmLib_assert(getter==false && setter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T{return Ref_; };
				setter=[&](const T Value_)->const T{Ref_=Value_; return Ref_;};
			}
			void initialize(reference_type Ref_, setter_func setter_) {
				hmLib_assert(getter==false && setter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T{return Ref_;};
				setter=setter_;
			}
			void initialize(getter_func getter_, setter_func setter_) {
				hmLib_assert(getter==false && setter==false,exceptions::initialization_overlap,"This property have already been initialized.");
				getter=getter_;
				setter=setter_;
			}
		public://accessor
			const T operator()(void) const {return getter();}
			operator const T() const {return getter();}
			const T operator()(const T Value_) {return setter(Value_);}
			const T operator=(const T Value_) {return setter(Value_);}
		};
		template<typename T>
		class property<T&>:public connection_host_mixin{
		public:
			typedef typename std::remove_reference<T&>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T&(void)> getter_func;
			typedef std::function<const T&(const T&)> setter_func;
		protected:
			getter_func getter;
			setter_func setter;
		public:
			//delete default constructor
			property()=delete;
			//delete copy constructor
			property(const property& my_)=delete;
			const property& operator=(const property& other)=delete;
			//delete move constructor
			property(property&& my_)=delete;
			const property& operator=(property&& other)=delete;
			//construct with accessor
			explicit property(reference_type Ref_)
				: getter([&]()->const T&{return Ref_;})
				, setter([&](const T& Value_)->const T&{Ref_=Value_; return Ref_; }) {}
			property(reference_type Ref_, setter_func setter_)
				: getter([&]()->const T&{return Ref_; })
				, setter(setter_) {}
			property(getter_func getter_, setter_func setter_)
				: getter(getter_)
				, setter(setter_) {}
			//no initializ
			explicit property(no_initialization) {}
			void initialize(reference_type Ref_) {
				hmLib_assert(getter==false && setter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T&{return Ref_;};
				setter=[&](const T& Value_)->const T&{Ref_=Value_; return Ref_; };
			}
			void initialize(reference_type Ref_, setter_func setter_) {
				hmLib_assert(getter==false && setter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T&{return Ref_; };
				setter=setter_;
			}
			void initialize(getter_func getter_, setter_func setter_) {
				hmLib_assert(getter==false && setter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=getter_;
				setter=setter_;
			}
		public://accessor
			const T& operator()(void) const { return getter(); }
			operator const T&() const { return getter(); }
			const T& operator()(const T& Value_) { return setter(Value_); }
			const T& operator=(const T& Value_) { return setter(Value_); }
		};
		template<typename T>
		class property <const T>:public connection_host_mixin{
		public:
			typedef typename std::remove_reference<const T>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T(void)> getter_func;
		protected:
			getter_func getter;
		public:
			//delete default constructor
			property()=delete;
			//delete copy constructor
			property(const property& my_)=delete;
			const property& operator=(const property& other)=delete;
			//delete move constructor
			property(property&& my_)=delete;
			const property& operator=(property&& other)=delete;
			//construct with accessor
			explicit property(reference_type Ref_)
				: getter([&]()->const T{return Ref_;}){}
			explicit property(getter_func getter_)
				: getter(getter_){}
			//no initializ
			explicit property(no_initialization) {}
			void initialize(reference_type Ref_) {
				hmLib_assert(getter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T{return Ref_;};
			}
			void initialize(getter_func getter_) {
				hmLib_assert(getter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=getter_;
			}
		public://accessor
			const T operator()(void) const { return getter(); }
			operator const T() const { return getter(); }
		};
		template<typename T>
		class property <const T&>:public connection_host_mixin{
		public:
			typedef typename std::remove_reference<const T&>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T&(void)> getter_func;
		protected:
			getter_func getter;
		public:
			//delete default constructor
			property()=delete;
			//delete copy constructor
			property(const property& my_)=delete;
			const property& operator=(const property& other)=delete;
			//delete move constructor
			property(property&& my_)=delete;
			const property& operator=(property&& other)=delete;
			//construct with accessor
			explicit property(reference_type Ref_)
				: getter([&]()->const T&{return Ref_; }) {}
			explicit property(getter_func getter_)
				: getter(getter_) {}
			//no initializ
			explicit property(no_initialization) {}
			void initialize(reference_type Ref_) {
				hmLib_assert(getter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=[&]()->const T&{return Ref_; };
			}
			void initialize(getter_func getter_) {
				hmLib_assert(getter==false, exceptions::initialization_overlap, "This property have already been initialized.");
				getter=getter_;
			}
		public://accessor
			const T& operator()(void) const { return getter(); }
			operator const T&() const { return getter(); }
		};
	}
}
#
#endif
