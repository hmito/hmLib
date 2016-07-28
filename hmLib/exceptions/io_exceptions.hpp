#ifndef HMLIB_EXCEPTIONS_IOEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_IOEXCEPTIONS_INC 200
#
/*=== io excception ===
ioに関わる例外

v2_00/140214 hmIto
	hmLib_v3_06の例外規格に対応
*/
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif
namespace hmLib{
	namespace exceptions{
		namespace io {
			//バッファがoverflowした場合
			template<typename base_type_=hmLib::exceptions::exception>
			class io_exception :public base_type_ {
				typedef base_type_ base_type;
			public:
				explicit io_exception(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit io_exception(const char* Message_) :base_type(Message_) {}
			};
			//streamが開かれていない場合
			template<typename base_type_=exception>
			class not_opened :public io_exception<base_type_> {
				typedef io_exception<base_type_> base_type;
			public:
				explicit not_opened(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit not_opened(const char* Message_) :base_type(Message_) {}
			};
			//streamが開かれている場合
			template<typename base_type_=exception>
			class opened :public io_exception<base_type_> {
				typedef io_exception<base_type_> base_type;
			public:
				explicit opened(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit opened(const char* Message_) :base_type(Message_) {}
			};
			//バッファがoverflowした場合
			template<typename base_type_=exception>
			class overflow :public io_exception<base_type_> {
				typedef io_exception<base_type_> base_type;
			public:
				explicit overflow(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit overflow(const char* Message_) :base_type(Message_) {}
			};
			//バッファがeofに達した場合
			template<typename base_type_=exception>
			class end_of_file :public io_exception<base_type_> {
				typedef io_exception<base_type_> base_type;
			public:
				explicit end_of_file(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit end_of_file(const char* Message_) :base_type(Message_) {}
			};
		}

	}
	namespace io_exceptions {
		typedef exceptions::io::io_exception<> io_exception;
		typedef exceptions::io::not_opened<> not_opened;
		typedef exceptions::io::opened<> opened;
		typedef exceptions::io::overflow<> overflow;
		typedef exceptions::io::end_of_file<> end_of_file;
	}}
#
#endif
