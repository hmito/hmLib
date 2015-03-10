#ifndef HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_NUMERICEXCEPTIONS_INC 100
#
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		//”’l‰‰ZƒGƒ‰[
		class numeric_exception:public virtual exception{
			typedef exception excp_type;
		public:
			explicit numeric_exception(const std::string& Message_):excp_type(Message_.c_str()){}
			explicit numeric_exception(const char* Message_):excp_type(Message_){}
		};
		//Å‰‚©‚çŠù’è‚Ì’l‚Ì”ÍˆÍŠO‚É’l‚ª‚ ‚éê‡
		class invalid_initialvalue:public numeric_exception{
			typedef exception excp_type;
			typedef numeric_exception base_type;
		public:
			explicit invalid_initialvalue(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit invalid_initialvalue(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//ŒvZ‚ÌŒ‹‰ÊA‹K’è‚Ì’l‚Ì”ÍˆÍŠO‚É‚È‚Á‚½ê‡
		class out_of_valuerange:public numeric_exception{
			typedef exception excp_type;
			typedef numeric_exception base_type;
		public:
			explicit out_of_valuerange(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit out_of_valuerange(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//•s“–‚ÈZp‰‰Z‚ğ—v‹‚µ‚½ê‡ 0œZ“™
		class incorrect_arithmetic_request:public numeric_exception{
			typedef exception excp_type;
			typedef numeric_exception base_type;
		public:
			explicit incorrect_arithmetic_request(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit incorrect_arithmetic_request(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
		//•s“–‚ÈZp‰‰Z‚ªs‚í‚ê‚½ê‡ 0œZ“™
		class incorrect_arithmetic_occurrence:public numeric_exception{
			typedef exception excp_type;
			typedef numeric_exception base_type;
		public:
			explicit incorrect_arithmetic_occurrence(const std::string& Message_):excp_type(Message_.c_str()),base_type(Message_.c_str()){}
			explicit incorrect_arithmetic_occurrence(const char* Message_):excp_type(Message_),base_type(Message_){}
		};
	}
}
#
#endif
