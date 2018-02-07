#ifndef HMLIB_GATES_EXCEPTIONS_INC
#define HMLIB_GATES_EXCEPTIONS_INC 100
#
#include "../exceptions.hpp"
namespace hmLib {
	namespace gates {
		struct exception_identifier {};
		typedef exceptions::exception_pattern<exception_identifier> exception;
		typedef exceptions::io::not_opened<exception> not_opened_exception;
		typedef exceptions::io::opened<exception> opened_exception;
	}
}
#
#endif
