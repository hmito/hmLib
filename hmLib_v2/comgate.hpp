/*
comgate_v1_00.hpp
	RS232c型シリアル通信用gateクラス
*/
#ifndef VER_HMLIB_COMGATE
#	define VER_HMLIB_COMGATE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_COMGATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_COMGATE 100
#	else
#		define USE_HMLIB_COMGATE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_COMGATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_COMGATE 100
#	else
#		define CHG_HMLIB_COMGATE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_COMGATE
#	define INC_USE_HMLIB_COMGATE
#
#	if USE_HMLIB_COMGATE>100
#		error hmLib : # error : [comgate.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_COMGATE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [comgate.hpp] v1_00")
#		endif
#		include "hmLib_v2/comgate/comgate_v1_00.h"
#	else
#		error hmLib : # error : [comgate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_COMGATE && USE_HMLIB_COMGATE<101
#//		pragma message("hmLib : # error : [comgate.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_COMGATE
#	define INC_CHG_HMLIB_COMGATE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_COMGATE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_COMGATE
#			pragma message("chg_hmLib : # error : [comgate.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_COMGATE<200 && 200<=USE_HMLIB_COMGATE
#//			pragma message("chg_hmLib : # error : [comgate.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_COMGATE<200 && 200<=CHG_HMLIB_COMGATE
#//			pragma message("chg_hmLib : # error : [comgate.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_COMGATE<100
#			pragma message("chg_hmLib : # error : [comgate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* gateとの互換性について*/
#	if 100<=CHG_HMLIB_COMGATE
#		ifndef INC_CHG_HMLIB_GATE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [comgate.hpp] gate.hppに依存しています。CHG_HMLIB用にgate.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/gate.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/gate.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_COMGATE && CHG_HMLIB_GATE<100
#//			pragma message("chg_hmLib : # error : [comgate.hpp] gate.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_COMGATE<100 && 100<=CHG_HMLIB_GATE
#//			pragma message("chg_hmLib : # error : [comgate.hpp] gate.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_GATE<100
#//			pragma message("chg_hmLib : # error : [comgate.hpp] 使用可能なgate.hppが存在しません。")
#//		endif
#	endif
#
#	endif
#endif
