/*
dxPad hmLib_v3未対応
dxPad_v1_00.hpp
*/
#ifndef VER_HMLIB_DXPAD
#	define VER_HMLIB_DXPAD 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXPAD
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXPAD 999
#	elif USE_HMLIB>=20805
#		define USE_HMLIB_DXPAD 100
#	else
#		define USE_HMLIB_DXPAD 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXPAD
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXPAD 999
#	elif CHG_HMLIB>=20805
#		define CHG_HMLIB_DXPAD 100
#	else
#		define CHG_HMLIB_DXPAD 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXPAD
#	define INC_USE_HMLIB_DXPAD
#
#	if USE_HMLIB_DXPAD>100
#		error hmLib : # error : [dxPad.hpp] 非対応バージョンです。v1_00より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_DXPAD==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxPad.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxPad/dxPad_v1_00.hpp"
#	else
#		error hmLib : # error : [dxPad.hpp] 非対応バージョンです。v1_00(v2_08_05)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXPAD && USE_HMLIB_DXPAD<101
#//		pragma message("hmLib : # error : [dxPad.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXPAD
#	define INC_CHG_HMLIB_DXPAD
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXPAD
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXPAD
#			pragma message("chg_hmLib : # error : [dxPad.hpp] 非対応バージョンです。v1_00より後(v3_00_00以降)は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXPAD<200 && 200<=USE_HMLIB_DXPAD
#//			pragma message("chg_hmLib : # error : [dxPad.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXPAD<200 && 200<=CHG_HMLIB_DXPAD
#//			pragma message("chg_hmLib : # error : [dxPad.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXPAD<100
#			pragma message("chg_hmLib : # error : [dxPad.hpp] 非対応バージョンです。v1_00(v2_08_05)より前は、使用できません。")
#		endif
#	endif
#	/* dxBasicとの互換性について*/
#	if 100<=CHG_HMLIB_DXPAD
#		ifndef CHG_HMLIB_DXBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxPad.hpp] dxBasic.hppに依存しています。CHG_HMLIB用にdxBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxBasic.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXPAD && CHG_HMLIB_DXBASIC<100
#//			pragma message("chg_hmLib : # error : [dxPad.hpp] dxBasic.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXPAD<100 && 100<=CHG_HMLIB_DXBASIC
#//			pragma message("chg_hmLib : # error : [dxPad.hpp] dxBasic.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_DXBASIC<100 || CHG_HMLIB_DXBASIC==999)
#			pragma message("chg_hmLib : # error : [dxPad.hpp] 使用可能なdxBasic.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
