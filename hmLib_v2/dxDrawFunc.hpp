/*
dxDrawFunc_v1_00.hpp
*/
#ifndef VER_HMLIB_DXDRAWFUNC
#	define VER_HMLIB_DXDRAWFUNC 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXDRAWFUNC
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXDRAWFUNC 100
#	else
#		define USE_HMLIB_DXDRAWFUNC 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXDRAWFUNC
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXDRAWFUNC 100
#	else
#		define CHG_HMLIB_DXDRAWFUNC 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXDRAWFUNC
#	define INC_USE_HMLIB_DXDRAWFUNC
#
#	if USE_HMLIB_DXDRAWFUNC>100
#		error hmLib : # error : [dxDrawFunc.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXDRAWFUNC==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxDrawFunc.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxDrawFunc/dxDrawFunc_v1_00.hpp"
#	else
#		error hmLib : # error : [dxDrawFunc.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXDRAWFUNC
#		pragma message("hmLib : # info  : [dxDrawFunc.hpp] DxLib依存ファイルです。")
#	endif
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXDRAWFUNC && USE_HMLIB_DXDRAWFUNC<101
#//		pragma message("hmLib : # error : [dxDrawFunc.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXDRAWFUNC
#	define INC_CHG_HMLIB_DXDRAWFUNC
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXDRAWFUNC
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXDRAWFUNC
#			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<200 && 200<=USE_HMLIB_DXDRAWFUNC
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXDRAWFUNC<200 && 200<=CHG_HMLIB_DXDRAWFUNC
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<100
#			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* dxImageとの互換性について*/
#	if 100<=CHG_HMLIB_DXDRAWFUNC
#		ifndef CHG_HMLIB_DXIMAGE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDrawFunc.hpp] dxImage.hppに依存しています。CHG_HMLIB用にdxImage.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxImage.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxImage.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXDRAWFUNC && CHG_HMLIB_DXIMAGE<100
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] dxImage.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<100 && 100<=CHG_HMLIB_DXIMAGE
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] dxImage.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXIMAGE<100
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 使用可能なdxImage.hppが存在しません。")
#//		endif
#	endif
#	/* dxFontとの互換性について*/
#	if 100<=CHG_HMLIB_DXDRAWFUNC
#		ifndef CHG_HMLIB_DXFONT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDrawFunc.hpp] dxFont.hppに依存しています。CHG_HMLIB用にdxFont.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxFont.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxFont.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXDRAWFUNC && CHG_HMLIB_DXFONT<100
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] dxFont.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<100 && 100<=CHG_HMLIB_DXFONT
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] dxFont.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXFONT<100
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] 使用可能なdxFont.hppが存在しません。")
#//		endif
#	endif
#	/* hmAlignとの互換性について*/
#	if 100<=CHG_HMLIB_DXDRAWFUNC
#		ifndef CHG_HMLIB_HMALIGN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDrawFunc.hpp] hmAlign.hppに依存しています。CHG_HMLIB用にhmAlign.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmAlign.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmAlign.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXDRAWFUNC && CHG_HMLIB_HMALIGN<102
#			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] hmAlign.hpp v1_02(v3_00_00)より前とは、互換性がありません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXDRAWFUNC && CHG_HMLIB_HMALIGN<100
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] hmAlign.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<100 && 100<=CHG_HMLIB_HMALIGN
#//			pragma message("chg_hmLib : # error : [dxDrawFunc.hpp] hmAlign.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#	endif
#
#	endif
#endif
