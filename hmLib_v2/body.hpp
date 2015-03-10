/*
body_v1_00.hpp
	図形を制御するクラス
*/
#ifndef VER_HMLIB_BODY
#	define VER_HMLIB_BODY 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_BODY
#	if USE_HMLIB>=30000
#		define USE_HMLIB_BODY 100
#	else
#		define USE_HMLIB_BODY 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_BODY
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_BODY 100
#	else
#		define CHG_HMLIB_BODY 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_BODY
#	define INC_USE_HMLIB_BODY
#
#	if USE_HMLIB_BODY>100
#		error hmLib # error : [body.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_BODY==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [body.hpp] v1_00")
#		endif
#		include "hmLib_v2/body/body_v1_00.hpp"
#	else
#		error hmLib # error : [body.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_BODY && USE_HMLIB_BODY<101
#//		pragma message("hmLib # error : [body.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_BODY
#	define INC_CHG_HMLIB_BODY
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_BODY
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_BODY
#			pragma message("chg_hmLib # error : [body.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BODY<200 && 200<=USE_HMLIB_BODY
#//			pragma message("chg_hmLib # error : [body.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_BODY<200 && 200<=CHG_HMLIB_BODY
#//			pragma message("chg_hmLib # error : [body.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BODY<100
#			pragma message("chg_hmLib # error : [body.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* rectangleとの互換性について*/
#	if 100<=CHG_HMLIB_BODY
#		ifndef CHG_HMLIB_RECTANGLE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib # info  : [body.hpp] rectangle.hppに依存しています。CHG_HMLIB用にrectangle.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/rectangle.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/rectangle.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_BODY && CHG_HMLIB_RECTANGLE<102
#			pragma message("chg_hmLib # error : [body.hpp] rectangle.hpp v1_02(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_BODY<100 && 100<=CHG_HMLIB_RECTANGLE
#//			pragma message("chg_hmLib # error : [body.hpp] rectangle.hpp v1_02(v3_00_00)以降とは、互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_RECTANGLE<100
#			pragma message("chg_hmLib # error : [body.hpp] 使用可能なrectangle.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
