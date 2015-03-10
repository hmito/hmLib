/*
hmDebug 廃止
	後継は、debugstream.hpp
hmDebug_v1_00.hpp
	debug窓に出力するostream hm::dout
	debug時のみ出力するflow hm::debugflow
	debug時のみ出力するcout hm::d_cout
*/
#ifndef VER_HMLIB_HMDEBUG
#	define VER_HMLIB_HMDEBUG 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMDEBUG
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMDEBUG 999
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_HMDEBUG 100
#	else
#		define USE_HMLIB_HMDEBUG 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMDEBUG
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMDEBUG 999
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_HMDEBUG 100
#	else
#		define CHG_HMLIB_HMDEBUG 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMDEBUG
#	define INC_USE_HMLIB_HMDEBUG
#
#	if USE_HMLIB_HMDEBUG>100
#		error hmLib : # error : [hmDebug.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継は、debugstream.hppです。
#	elif USE_HMLIB_HMDEBUG==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmDebug.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmDebug/hmDebug_v1_00.h"
#	else
#		error hmLib : # error : [hmDebug.hpp] 非対応バージョンです。v1_00(v2_09_02)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMDEBUG
#		pragma message("hmLib : # info  : [hmDebug.hpp] windows.h依存ファイルです。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMDEBUG
#	define INC_CHG_HMLIB_HMDEBUG
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMDEBUG
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_HMDEBUG
#			pragma message("chg_hmLib : # error : [hmDebug.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継は、debugstream.hppです。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMDEBUG<100
#			pragma message("chg_hmLib : # error : [hmDebug.hpp] 非対応バージョンです。v1_00(v2_09_02)より前は、使用できません。")
#		endif
#	endif
#	/* hmStreamBufとの互換性について*/
#	if 100<=CHG_HMLIB_HMDEBUG && CHG_HMLIB_HMDEBUG<=100
#		ifndef CHG_HMLIB_HMSTREAMBUF
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmDebug.hpp] hmStreamBuf.hppに依存しています。CHG_HMLIB用にhmStreamBuf.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmStreamBuf.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmStreamBuf.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
