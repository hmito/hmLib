#ifndef HMLIB_EXCEPTIONS_MEMORYEXCEPTIONS_INC
#define HMLIB_EXCEPTIONS_MEMORYEXCEPTIONS_INC 200
#
/*=== memory excception ===
memoryに関わる例外

v2_00/140214 hmIto
	hmLib_v3_06の例外規格に対応
*/
#ifndef HMLIB_EXCEPTIONS_EXCEPTION_INC
#	include"exception.hpp"
#endif

namespace hmLib{
	namespace exceptions{
		namespace memory {
			//メモリーエラー
			template<typename base_type_=exception>
			class memory_exception :public base_type_ {
				typedef base_type_ base_type;
			public:
				explicit memory_exception(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit memory_exception(const char* Message_) :base_type(Message_) {}
			};
			//メモリ割り当てに失敗した場合
			template<typename base_type_=exception>
			class fail_allocation :public memory_exception<base_type_> {
				typedef memory_exception<base_type_> base_type;
			public:
				explicit fail_allocation(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit fail_allocation(const char* Message_) :base_type(Message_) {}
			};
			//メモリ解放に失敗した場合
			template<typename base_type_=exception>
			class fail_deallocation :public memory_exception<base_type_> {
				typedef memory_exception<base_type_> base_type;
			public:
				explicit fail_deallocation(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit fail_deallocation(const char* Message_) :base_type(Message_) {}
			};
			//動的キャストに失敗した場合
			template<typename base_type_=exception>
			class incorrect_downcast_request :public memory_exception<base_type_> {
				typedef memory_exception<base_type_> base_type;
			public:
				explicit incorrect_downcast_request(const std::string& Message_) :base_type(Message_.c_str()) {}
				explicit incorrect_downcast_request(const char* Message_) :base_type(Message_) {}
			};
		}

	}
	namespace memory_exceptions {
		typedef exceptions::memory::memory_exception<> memory_exception;
		typedef exceptions::memory::fail_allocation<> fail_allocation;
		typedef exceptions::memory::fail_deallocation<> fail_deallocation;
		typedef exceptions::memory::incorrect_downcast_request<> incorrect_downcast_request;
	}
}
#
#endif
