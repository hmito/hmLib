/*
bufgate_v1_00.hpp
*/
#ifndef VER_HMLIB_BUFGATE
#	define VER_HMLIB_BUFGATE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_BUFGATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_BUFGATE 100
#	else
#		define USE_HMLIB_BUFGATE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_BUFGATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_BUFGATE 100
#	else
#		define CHG_HMLIB_BUFGATE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_BUFGATE
#	define INC_USE_HMLIB_BUFGATE
#
#	if USE_HMLIB_BUFGATE>100
#		error hmLib : # error : [bufgate.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_BUFGATE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [bufgate.hpp] v1_00")
#		endif
#		include "hmLib_v2/bufgate/bufgate_v1_00.hpp"
#	else
#		error hmLib : # error : [bufgate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_BUFGATE && USE_HMLIB_BUFGATE<101
#//		pragma message("hmLib : # error : [bufgate.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_BUFGATE
#	define INC_CHG_HMLIB_BUFGATE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_BUFGATE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_BUFGATE
#			pragma message("chg_hmLib : # error : [bufgate.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BUFGATE<100 && 100<=USE_HMLIB_BUFGATE
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_BUFGATE<100 && 100<=CHG_HMLIB_BUFGATE
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BUFGATE<100
#			pragma message("chg_hmLib : # error : [bufgate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* gateとの互換性について*/
#	if 100<=CHG_HMLIB_BUFGATE
#		ifndef INC_CHG_HMLIB_GATE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [bufgate.hpp] gate.hppに依存しています。CHG_HMLIB用にgate.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/gate.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/gate.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_BUFGATE && CHG_HMLIB_GATE<100
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] gate.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_BUFGATE<100 && 100<=CHG_HMLIB_GATE
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] gate.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_GATE<100 || CHG_HMLIB_GATE==999)
#			pragma message("chg_hmLib : # error : [bufgate.hpp] 使用可能なgate.hppが存在しません。")
#		endif
#	endif
#
#	/* circularとの互換性について*/
#	if 100<=CHG_HMLIB_BUFGATE
#		ifndef INC_CHG_HMLIB_CIRCULAR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [bufgate.hpp] circular.hppに依存しています。CHG_HMLIB用にcircular.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/circular.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/circular.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_BUFGATE && CHG_HMLIB_CIRCULAR<100
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] circular.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_BUFGATE<100 && 100<=CHG_HMLIB_CIRCULAR
#//			pragma message("chg_hmLib : # error : [bufgate.hpp] circular.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_CIRCULAR<100 || CHG_HMLIB_CIRCULAR==999)
#			pragma message("chg_hmLib : # error : [bufgate.hpp] 使用可能なcircular.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
