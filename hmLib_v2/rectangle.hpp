/*
rectangle_v1_00.hpp
*/
#ifndef VER_HMLIB_RECTANGLE
#	define VER_HMLIB_RECTANGLE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_RECTANGLE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_RECTANGLE 100
#	else
#		define USE_HMLIB_RECTANGLE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_RECTANGLE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_RECTANGLE 100
#	else
#		define CHG_HMLIB_RECTANGLE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_RECTANGLE
#	define INC_USE_HMLIB_RECTANGLE
#
#	if USE_HMLIB_RECTANGLE>100
#		error hmLib : # error : [rectangle.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_RECTANGLE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [rectangle.hpp] v1_00")
#		endif
#		include "hmLib_v2/rectangle/rectangle_v1_00.hpp"
#	else
#		error hmLib : # error : [rectangle.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_RECTANGLE && USE_HMLIB_RECTANGLE<101
#//		pragma message("hmLib : # error : [rectangle.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_RECTANGLE
#	define INC_CHG_HMLIB_RECTANGLE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_RECTANGLE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_RECTANGLE
#			pragma message("chg_hmLib : # error : [rectangle.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_RECTANGLE<100 && 100<=USE_HMLIB_RECTANGLE
#//			pragma message("chg_hmLib : # error : [rectangle.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_RECTANGLE<100 && 100<=CHG_HMLIB_RECTANGLE
#//			pragma message("chg_hmLib : # error : [rectangle.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_RECTANGLE<100
#			pragma message("chg_hmLib : # error : [rectangle.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* positionとの互換性について*/
#	if 100<=CHG_HMLIB_RECTANGLE
#		ifndef INC_CHG_HMLIB_POSITION
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [rectangle.hpp] position.hppに依存しています。CHG_HMLIB用にposition.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/position.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/position.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_RECTANGLE && CHG_HMLIB_POSITION<100
#//			pragma message("chg_hmLib : # error : [rectangle.hpp] position.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_RECTANGLE<100 && 100<=CHG_HMLIB_POSITION
#//			pragma message("chg_hmLib : # error : [rectangle.hpp] position.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_POSITION<100 || CHG_HMLIB_POSITION==999)
#			pragma message("chg_hmLib : # error : [rectangle.hpp] 使用可能なposition.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
