#ifndef HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC 200
#
/*=== numeric excception ===
numericに関わる例外

v2_00/140214 hmIto
	hmLib_v3_06の例外規格に対応
*/
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		namespace numeric {
			//数値演算エラー
			template<typename base_type_=exception>
			class numeric_exception :public base_type_ {
				typedef base_type_ base_type;
			public:
				explicit numeric_exception(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit numeric_exception(const char* Message_) :base_type(Message_) {}
			};
			//最初から既定の値の範囲外に値がある場合
			template<typename base_type_=exception>
			class invalid_initialvalue :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit invalid_initialvalue(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit invalid_initialvalue(const char* Message_) :base_type(Message_) {}
			};
			//計算の結果、規定の値の範囲外になった場合
			template<typename base_type_=exception>
			class out_of_valuerange :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit out_of_valuerange(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit out_of_valuerange(const char* Message_) :base_type(Message_) {}
			};
			//不当な算術演算を要求した場合 0除算等
			template<typename base_type_=exception>
			class incorrect_arithmetic_request :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit incorrect_arithmetic_request(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit incorrect_arithmetic_request(const char* Message_) :base_type(Message_) {}
			};
			//不当な算術演算が行われた場合 0除算等
			template<typename base_type_=exception>
			class incorrect_arithmetic_occurrence :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit incorrect_arithmetic_occurrence(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit incorrect_arithmetic_occurrence(const char* Message_) :base_type(Message_) {}
			};
		}

	}
	namespace numeric_exceptions {
		typedef exceptions::numeric::numeric_exception<> numeric_exception;
		typedef exceptions::numeric::invalid_initialvalue<> invalid_initialvalue;
		typedef exceptions::numeric::out_of_valuerange<> out_of_valuerange;
		typedef exceptions::numeric::incorrect_arithmetic_request<> incorrect_arithmetic_request;
		typedef exceptions::numeric::incorrect_arithmetic_occurrence<> incorrect_arithmetic_occurrence;
	}
}
#
#endif
