#ifndef INC_HMLIB_HMDEBUG_CPP
#define INC_HMLIB_HMDEBUG_CPP 100
#ifndef INC_HMLIB_HMDEBUG
#	include "hmLib_v2/hmDebug.hpp"
#endif
namespace hmstd{
	debugstream dout;
}
namespace hm{
	hmstd::debugflow d_cout(std::cout);
	hmstd::debugflow d_dout(hmstd::dout);
}
#endif
