/*
dxGraphics_v1_00.hpp
*/
#ifndef VER_HMLIB_DXGRAPHICS
#	define VER_HMLIB_DXGRAPHICS 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXGRAPHICS
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXGRAPHICS 100
#	else
#		define USE_HMLIB_DXGRAPHICS 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXGRAPHICS
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXGRAPHICS 100
#	else
#		define CHG_HMLIB_DXGRAPHICS 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXGRAPHICS
#	define INC_USE_HMLIB_DXGRAPHICS
#
#	if USE_HMLIB_DXGRAPHICS>100
#		error hmLib : # error : [dxGraphics.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXGRAPHICS==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxGraphics.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxGraphics/dxGraphics_v1_00.h"
#	else
#		error hmLib : # error : [dxGraphics.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXGRAPHICS && USE_HMLIB_DXGRAPHICS<101
#//		pragma message("hmLib : # error : [dxGraphics.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXGRAPHICS
#	define INC_CHG_HMLIB_DXGRAPHICS
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXGRAPHICS
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXGRAPHICS
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<200 && 200<=USE_HMLIB_DXGRAPHICS
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXGRAPHICS<200 && 200<=CHG_HMLIB_DXGRAPHICS
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<100
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmColorとの互換性について*/
#	if 100<=CHG_HMLIB_DXGRAPHICS
#		ifndef CHG_HMLIB_HMCOLOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxGraphics.hpp] hmColor.hppに依存しています。CHG_HMLIB用にhmColor.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmColor.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmColor.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 101<=CHG_HMLIB_DXGRAPHICS && CHG_HMLIB_HMCOLOR<100
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] hmColor.hpp v1_01(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<100 && 100<=CHG_HMLIB_HMCOLOR
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] hmColor.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMCOLOR<100
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 使用可能なhmColor.hppが存在しません。")
#//		endif
#	endif
#	/* hmRectとの互換性について*/
#	if 100<=CHG_HMLIB_DXGRAPHICS
#		ifndef CHG_HMLIB_HMRECT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxGraphics.hpp] hmRect.hppに依存しています。CHG_HMLIB用にhmRect.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmRect.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmRect.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXGRAPHICS && CHG_HMLIB_HMRECT<102
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] hmRect.hpp v1_02(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<100 && 100<=CHG_HMLIB_HMRECT
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] hmRect.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_YYY<100
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 使用可能なyyy.hppが存在しません。")
#//		endif
#	endif
#	/* dxImageとの互換性について*/
#	if 100<=CHG_HMLIB_DXGRAPHICS
#		ifndef CHG_HMLIB_DXIMAGE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxGraphics.hpp] dxImage.hppに依存しています。CHG_HMLIB用にdxImage.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxImage.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxImage.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXGRAPHICS && CHG_HMLIB_DXIMAGE<100
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] dxImage.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<100 && 100<=CHG_HMLIB_DXIMAGE
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] dxImage.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXIMAGE<100
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 使用可能なdxImage.hppが存在しません。")
#//		endif
#	endif
#	/* dxDrawCtrlとの互換性について*/
#	if 100<=CHG_HMLIB_DXGRAPHICS
#		ifndef CHG_HMLIB_DXDRAWCTRL
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxGraphics.hpp] dxDrawCtrl.hppに依存しています。CHG_HMLIB用にdxDrawCtrl.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxDrawCtrl.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxDrawCtrl.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXGRAPHICS && CHG_HMLIB_DXDRAWCTRL<100
#			pragma message("chg_hmLib : # error : [dxGraphics.hpp] dxDrawCtrl.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXGRAPHICS<100 && 100<=CHG_HMLIB_DXDRAWCTRL
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] dxDrawCtrl.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWCTRL<100
#//			pragma message("chg_hmLib : # error : [dxGraphics.hpp] 使用可能なdxDrawCtrl.hppが存在しません。")
#//		endif
#	endif
#
#	endif
#endif
