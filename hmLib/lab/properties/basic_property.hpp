#ifndef HMLIB_PROPERTIES_BASICPROPERTY_INC
#define HMLIB_PROPERTIES_BASICPROPERTY_INC 101
#
/*
変数へのアクセス制御可能でありながら、変数のように扱える、property変数を定義
参照先の寿命管理はしていないので、原則として、同一スコープの変数のみ参照可能
別スコープの変数を利用する際は、weak_propertyを利用する

=== properties::basic_property ===
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
		class basic_property{
		public:
			typedef typename std::remove_reference<T>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T(void)> getter_func;
			typedef std::function<const T(const T)> setter_func;
		protected:
			basic_property() {}
		protected:
			getter_func getter;
			setter_func setter;
		public://accessor
			const T operator()(void) const { return getter(); }
			operator const T() const { return getter(); }
			const T operator()(const T Value_) { return setter(Value_); }
			const T operator=(const T Value_) { return setter(Value_); }
		};
		template<typename T>
		class basic_property <const T> {
		public:
			typedef typename std::remove_reference<const T>::type value_type;
			typedef typename std::add_lvalue_reference<value_type>::type reference_type;
			typedef std::function<const T(void)> getter_func;
		protected:
			basic_property() {}
		protected:
			getter_func getter;
		public://accessor
			const T operator()(void) const { return getter(); }
			operator const T() const { return getter(); }
		};
	}
}
#
#endif
