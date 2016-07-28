#ifndef HMLIB_PROPERTIES_EXCEPTIONS_INC
#define HMLIB_PROPERTIES_EXCEPTIONS_INC 100
#
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace properties{
		namespace exceptions {
			namespace {
				class exception_identifier {};
				class initialization_overap_identifier {};
			}
			//property系列の基本例外
			typedef hmLib::exceptions::exception_pattern<exception_identifier> exception;
			//requested initialization to an initialized property.
			typedef hmLib::exceptions::exception_pattern<initialization_overap_identifier, exception> initialization_overlap;
			//not open
			typedef hmLib::exceptions::io::not_opened<exception> not_connected;
			//sopen
			typedef hmLib::exceptions::io::opened<exception> connected;
		}
	}
}
#
#endif
