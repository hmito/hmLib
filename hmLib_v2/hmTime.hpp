/*
hmTime廃止
	boost::date_timeで代替可能と判断したため
hmTime_v1_01.hpp
	hmTime::setを追加
	hmDate::setを追加
hmTime_v1_00.hpp
	winTimeを継承する形で新規追加
*/
#ifndef VER_HMLIB_HMTIME
#	define VER_HMLIB_HMTIME 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMTIME
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMTIME 999
#	elif USE_HMLIB>=20801
#		define USE_HMLIB_HMTIME 101
#	elif USE_HMLIB>=20702
#		define USE_HMLIB_HMTIME 100
#	else
#		define USE_HMLIB_HMTIME 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMTIME
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMTIME 999
#	elif CHG_HMLIB>=20801
#		define CHG_HMLIB_HMTIME 101
#	elif CHG_HMLIB>=20702
#		define CHG_HMLIB_HMTIME 100
#	else
#		define CHG_HMLIB_HMTIME 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMTIME
#	define INC_USE_HMLIB_HMTIME
#
#	if USE_HMLIB_HMTIME>101
#		error hmLib : # error : [hmTime.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。boost::date_timeの利用を推奨しています。
#	elif USE_HMLIB_HMTIME==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmTime.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmTime/hmTime_v1_01.hpp"
#	elif USE_HMLIB_HMTIME==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmTime.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmTime/hmTime_v1_00.hpp"
#	else
#		error hmLib : # error : [hmTime.hpp] 非対応バージョンです。v1_00(v2_07_02)より前は、使用できません。
#	endif
#
#	if 2<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMTIME
#		pragma message("hmLib : #warning: [hmTime.hpp] windows.h依存ライブラリです。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMTIME
#	define INC_CHG_HMLIB_HMTIME
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMTIME
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_HMTIME
#			pragma message("chg_hmLib : # error : [hmTime.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。boost::date_timeの利用を推奨しています。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMTIME<100
#			pragma message("chg_hmLib : # error : [hmTime.hpp] 非対応バージョンです。v1_00(v2_07_02)より前は、使用できません。")
#		endif
#	endif
#	/* hmBasicとの互換性について*/
#	if 100<=CHG_HMLIB_HMTIME && CHG_HMLIB_HMTIME<=101
#		ifndef CHG_HMLIB_HMBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmTime.hpp] hmBasic.hppに依存しています。CHG_HMLIB用にhmBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmBasic.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
