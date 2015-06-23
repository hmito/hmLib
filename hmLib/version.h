#ifndef HMLIB_VERSION_INC
#define HMLIB_VERSION_INC	40000
#define HMLIB_VERSION_STR "v4_00_00"
#ifdef HMLIB_VERSION
#	undef HMLIB_VERSION
#endif
#define HMLIB_VERSION HMLIB_VERSION_INC
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#
#if HMLIB_OUT > 0
#	pragma message("=== hmLib_"HMLIB_VERSION_STR" ===")
#endif
#ifdef HMLIB_UPDATE
#	include"hmLib_update.h"
#endif
#
#endif
