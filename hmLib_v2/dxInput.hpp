/*
dxInput_v1_00.hpp
*/
#ifndef VER_HMLIB_DXINPUT
#	define VER_HMLIB_DXINPUT 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXINPUT
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXINPUT 100
#	else
#		define USE_HMLIB_DXINPUT 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXINPUT
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXINPUT 100
#	else
#		define CHG_HMLIB_DXINPUT 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXINPUT
#	define INC_USE_HMLIB_DXINPUT
#
#	if USE_HMLIB_DXINPUT>100
#		error hmLib : # error : [dxInput.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXINPUT==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxInput.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxInput/dxInput_v1_00.h"
#	else
#		error hmLib : # error : [dxInput.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXINPUT
#		pragma message("hmLib : # info  : [dxInput.hpp] DxLib依存ファイルです。")
#	endif
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXINPUT && USE_HMLIB_DXINPUT<101
#//		pragma message("hmLib : # error : [dxInput.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXINPUT
#	define INC_CHG_HMLIB_DXINPUT
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXINPUT
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXINPUT
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<200 && 200<=USE_HMLIB_DXINPUT
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXINPUT<200 && 200<=CHG_HMLIB_DXINPUT
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmTaskとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUT
#		ifndef CHG_HMLIB_HMTASK
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInput.hpp] hmTask.hppに依存しています。CHG_HMLIB用にhmTask.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmTask.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmTask.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUT && CHG_HMLIB_HMTASK<100
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] hmTask.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100 && 100<=CHG_HMLIB_HMTASK
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] hmTask.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMTASK<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 使用可能なhmTask.hppが存在しません。")
#		endif
#	endif
#	/* hmStrとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUT
#		ifndef CHG_HMLIB_HMSTR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInput.hpp] hmStr.hppに依存しています。CHG_HMLIB用にhmStr.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmStr.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmStr.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUT && CHG_HMLIB_HMSTR<100
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] hmStr.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100 && 100<=CHG_HMLIB_HMSTR
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] hmStr.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMSTR<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 使用可能なhmStr.hppが存在しません。")
#		endif
#	endif
#	/* dxFontとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUT
#		ifndef CHG_HMLIB_DXFONT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInput.hpp] dxFont.hppに依存しています。CHG_HMLIB用にdxFont.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxFont.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxFont.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUT && CHG_HMLIB_DXFONT<100
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxFont.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100 && 100<=CHG_HMLIB_DXFONT
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxFont.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXFONT<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 使用可能なdxFont.hppが存在しません。")
#		endif
#	endif
#	/* dxDrawFuncとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUT
#		ifndef CHG_HMLIB_DXDRAWFUNC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInput.hpp] dxDrawFunc.hppに依存しています。CHG_HMLIB用にdxDrawFunc.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxDrawFunc.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxDrawFunc.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUT && CHG_HMLIB_DXDRAWFUNC<100
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxDrawFunc.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100 && 100<=CHG_HMLIB_DXDRAWFUNC
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxDrawFunc.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWFUNC<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 使用可能なdxDrawFunc.hppが存在しません。")
#		endif
#	endif
#	/* dxDrawCtrlとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUT
#		ifndef CHG_HMLIB_DXDRAWCTRL
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInput.hpp] dxDrawCtrl.hppに依存しています。CHG_HMLIB用にdxDrawCtrl.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxDrawCtrl.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxDrawCtrl.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUT && CHG_HMLIB_DXDRAWCTRL<100
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxDrawCtrl.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUT<100 && 100<=CHG_HMLIB_DXDRAWCTRL
#//			pragma message("chg_hmLib : # error : [dxInput.hpp] dxDrawCtrl.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDRAWCTRL<100
#			pragma message("chg_hmLib : # error : [dxInput.hpp] 使用可能なdxDrawCtrl.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
