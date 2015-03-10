#ifndef INC_HMLIB_HMRAND_CPP
#define INC_HMLIB_HMRAND_CPP 100
#ifndef INC_HMLIB_HMRAND
#	include "hmLib_v2/hmRand.hpp"
#endif
#include <ctime>
hmRand hm::rand((uint)std::time(NULL));
#endif
