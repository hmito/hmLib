#ifndef HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC 200
#
/*=== numeric excception ===
numeric‚ÉŠÖ‚í‚é—áŠO

v2_00/140214 hmIto
	hmLib_v3_06‚Ì—áŠO‹KŠi‚É‘Î‰
*/
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		namespace numeric {
			//”’l‰‰ZƒGƒ‰[
			template<typename base_type_=exception>
			class numeric_exception :public base_type_ {
				typedef base_type_ base_type;
			public:
				explicit numeric_exception(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit numeric_exception(const char* Message_) :base_type(Message_) {}
			};
			//Å‰‚©‚çŠù’è‚Ì’l‚Ì”ÍˆÍŠO‚É’l‚ª‚ ‚éê‡
			template<typename base_type_=exception>
			class invalid_initialvalue :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit invalid_initialvalue(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit invalid_initialvalue(const char* Message_) :base_type(Message_) {}
			};
			//ŒvZ‚ÌŒ‹‰ÊA‹K’è‚Ì’l‚Ì”ÍˆÍŠO‚É‚È‚Á‚½ê‡
			template<typename base_type_=exception>
			class out_of_valuerange :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit out_of_valuerange(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit out_of_valuerange(const char* Message_) :base_type(Message_) {}
			};
			//•s“–‚ÈZp‰‰Z‚ğ—v‹‚µ‚½ê‡ 0œZ“™
			template<typename base_type_=exception>
			class incorrect_arithmetic_request :public numeric_exception<base_type_> {
				typedef numeric_exception<base_type_> base_type;
			public:
				explicit incorrect_arithmetic_request(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit incorrect_arithmetic_request(const char* Message_) :base_type(Message_) {}
			};
			//•s“–‚ÈZp‰‰Z‚ªs‚í‚ê‚½ê‡ 0œZ“™
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
