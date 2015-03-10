/*
dxColorSet廃止
	後継は、hmColorSet.hpp
dxColorSet_v1_01
	色相再編
*/
#ifndef VER_HMLIB_DXCOLORSET
#	define VER_HMLIB_DXCOLORSET 101
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXCOLORSET
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXCOLORSET 999
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_DXCOLORSET 101
#	else
#		define USE_HMLIB_DXCOLORSET 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXCOLORSET
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXCOLORSET 999
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_DXCOLORSET 101
#	else
#		define CHG_HMLIB_DXCOLORSET 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXCOLORSET
#	define INC_USE_HMLIB_DXCOLORSET
#
#	if USE_HMLIB_DXCOLORSET>101
#		error hmLib : # error : [dxColorSet.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_DXCOLORSET==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxColorSet.hpp] v1_01")
#		endif
#		include "hmLib_v2/dxColorSet/dxColorSet_v1_01.h"
#	else
#		error hmLib : # error : [dxColorSet.hpp] 非対応バージョンです。v1_01(v2_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXCOLORSET && USE_HMLIB_DXCOLORSET<101
#//		pragma message("hmLib : # error : [dxColorSet.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXCOLORSET
#	define INC_CHG_HMLIB_DXCOLORSET
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXCOLORSET
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_DXCOLORSET
#			pragma message("chg_hmLib : # error : [dxColorSet.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXCOLORSET<100
#			pragma message("chg_hmLib : # error : [dxColorSet.hpp] 非対応バージョンです。v1_01(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* dxColorとの互換性について*/
#	if 100<=CHG_HMLIB_DXCOLORSET
#		ifndef CHG_HMLIB_DXCOLOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxColorSet.hpp] dxColor.hppに依存しています。CHG_HMLIB用にdxColor.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxColor.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxColor.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXCOLORSET && CHG_HMLIB_DXCOLOR<100
#//			pragma message("chg_hmLib : # error : [dxColorSet.hpp] dxColor.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXCOLORSET<100 && 100<=CHG_HMLIB_DXCOLOR
#//			pragma message("chg_hmLib : # error : [dxColorSet.hpp] dxColor.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_DXCOLOR<100 || CHG_HMLIB_DXCOLOR==999)
#			pragma message("chg_hmLib : # error : [dxColorSet.hpp] 使用可能なdxColor.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
