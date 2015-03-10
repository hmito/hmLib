/*
hmStreamBuf.hpp v1_1
	NBでstreamを使用すると、エラーになるバグを修正
*/
#ifndef VER_HMLIB_HMSTREAMBUF
#	define VER_HMLIB_HMSTREAMBUF 999
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMSTREAMBUF
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMSTREAMBUF 999
#	elif USE_HMLIB>=20701
#		define USE_HMLIB_HMSTREAMBUF 101
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMSTREAMBUF 100
#	else
#		define USE_HMLIB_HMSTREAMBUF 0
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_HMSTREAMBUF
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMSTREAMBUF 999
#	elif CHG_HMLIB>=20701
#		define CHG_HMLIB_HMSTREAMBUF 101
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMSTREAMBUF 100
#	else
#		define CHG_HMLIB_HMSTREAMBUF 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMSTREAMBUF
#	define INC_USE_HMLIB_HMSTREAMBUF
#
#	if USE_HMLIB_HMSTREAMBUF>101
#		error hmLib : [hmStreamBuf.hpp] 廃止バージョンです。v1_01より後(v3_00_00以上)では、使用できません。後継にstreambuf_pattern.hppが存在します。
#	elif USE_HMLIB_HMSTREAMBUF==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmStreamBuf.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmStreamBuf/hmStreamBuf_v1_01.hpp"
#	elif USE_HMLIB_HMSTREAMBUF==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmStreamBuf.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmStreamBuf/hmStreamBuf_v1_00.hpp"
#	else
#		error hmLib : [hmStreamBuf.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMBASIC
#		pragma message("hmLib : # info  : [hmStreamBuf.hpp] v3_00_00以上では、hmStreamBuf.hppは廃止されます。後継にstreambuf_pattern.hppが存在します。")
#	endif
#	if 2<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMSTREAMBUF && USE_HMLIB_HMSTREAMBUF<101
#		pragma message("hmLib : #warning: [hmStreamBuf.hpp] class hmNBstreambufに致命的なバグが存在します。v1_01(v2_07_01)以上の使用を推奨します。")
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMSTREAMBUF
#	define INC_CHG_HMLIB_HMSTREAMBUF
#
#	if 100<=USE_HMLIB_HMSTREAMBUF
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_HMSTREAMBUF
#			pragma message("chg_hmLib : # error : [hmStreamBuf.hpp] 非対応バージョンです。v1_01より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMSTREAMBUF<101 && 101<=USE_HMLIB_HMSTREAMBUF
#			pragma message("chg_hmLib : # error : [hmStreamBuf.hpp] class hmNBstreambufに致命的なバグが存在します。v1_01(v2_07_01)以上の使用を推奨します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMSTREAMBUF<100
#			pragma message("chg_hmLib : # error : [hmStreamBuf.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	endif
#endif
