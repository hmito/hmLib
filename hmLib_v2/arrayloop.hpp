/*
arrayloop 廃止
	後継はcircular.hppのcircular。
arrayloop_v1_00追加
	STL準拠のlist型固定長配列クラス
	毎回メモリ確保を行わず、固定配列上にデータを置く
	任意の位置の要素の挿入/削除が高速
*/
#ifndef VER_HMLIB_ARRAYLOOP
#	define VER_HMLIB_ARRAYLOOP 999
#endif
#ifndef INC_HMLIB
#	include <hmLib.h>
#endif
#ifndef USE_HMLIB_ARRAYLOOP
#	if USE_HMLIB>=30000
#		define USE_HMLIB_ARRAYLOOP 999
#	elif USE_HMLIB>=20900
#		define USE_HMLIB_ARRAYLOOP 100
#	else
#		define USE_HMLIB_ARRAYLOOP 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_ARRAYLOOP
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_ARRAYLOOP 999
#	elif CHG_HMLIB>=20900
#		define CHG_HMLIB_ARRAYLOOP 100
#	else
#		define CHG_HMLIB_ARRAYLOOP 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_HMLIB_ARRAYLOOP
#	define INC_HMLIB_ARRAYLOOP
#
#	if USE_HMLIB_ARRAYLOOP >100
#		error hmLib : [arrayloop.hpp] 廃止バージョンです。v1_00より後(v3_00_00以降)は、使用できません。circular.hppを使用してください。
#	elif USE_HMLIB_ARRAYLOOP==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arrayloop.hpp] v1_00");
#		endif
#		include "hmLib_v2/arrayloop/arrayloop_v1_00.hpp"
#	else
#		error hmLib : [arrayloop.hpp] 未対応バージョンです。v1_00(v2_09_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB
#		pragma message("hmLib : # info  : [arrayloop.hpp] v1_00より後(v3_00_00以降)、廃止されます。後継にcircular.hppが存在します。");
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ARRAYLOOP
#	define INC_CHG_HMLIB_ARRAYLOOP
#
#	if 100<=USE_HMLIB_ARRAYLOOP
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_ARRAYLOOP
#			pragma message("chg_hmLib : # error : [arraydeque.hpp] 廃止バージョンです。v1_00以前(v3_00_00より前)か、circular.hppを使用してください。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYLOOP<100
#			pragma message("chg_hmLib : # error : [arraydeque.hpp] 未対応バージョンです。v1_00(v2_09_00)以上でしか使用できません。")
#		endif
#	endif
#
#	if 100<=CHG_HMLIB_ARRAYLOOP
#		ifndef INC_CHG_HMLIB_ARRAYDEQUE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [arrayloop.hpp] arraydeque.hppに依存しています。CHG_HMLIB用にarraydeque.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/arraydeque.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/arraydeque.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
