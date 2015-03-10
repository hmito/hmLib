/*
dxInputDevice_v1_00.hpp
*/
#ifndef VER_HMLIB_DXINPUTDEVICE
#	define VER_HMLIB_DXINPUTDEVICE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXINPUTDEVICE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXINPUTDEVICE 100
#	else
#		define USE_HMLIB_DXINPUTDEVICE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXINPUTDEVICE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXINPUTDEVICE 100
#	else
#		define CHG_HMLIB_DXINPUTDEVICE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXINPUTDEVICE
#	define INC_USE_HMLIB_DXINPUTDEVICE
#
#	if USE_HMLIB_DXINPUTDEVICE>100
#		error hmLib : # error : [dxInputDevice.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXINPUTDEVICE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxInputDevice.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxInputDevice/dxInputDevice_v1_00.h"
#	else
#		error hmLib : # error : [dxInputDevice.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXINPUT
#		pragma message("hmLib : # info  : [dxInputDevice.hpp] DxLib依存ファイルです。")
#	endif
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXINPUTDEVICE && USE_HMLIB_DXINPUTDEVICE<101
#//		pragma message("hmLib : # error : [dxInputDevice.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXINPUTDEVICE
#	define INC_CHG_HMLIB_DXINPUTDEVICE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXINPUTDEVICE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXINPUTDEVICE
#			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUTDEVICE<200 && 200<=USE_HMLIB_DXINPUTDEVICE
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXINPUTDEVICE<200 && 200<=CHG_HMLIB_DXINPUTDEVICE
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUTDEVICE<100
#			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmTaskとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUTDEVICE
#		ifndef CHG_HMLIB_HMTASK
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInputDevice.hpp] hmTask.hppに依存しています。CHG_HMLIB用にhmTask.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmTask.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmTask.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUTDEVICE && CHG_HMLIB_HMTASK<100
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] hmTask.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUTDEVICE<100 && 100<=CHG_HMLIB_HMTASK
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] hmTask.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMTASK<100
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 使用可能なhmTask.hppが存在しません。")
#//		endif
#	endif
#	/* hmPosとの互換性について*/
#	if 100<=CHG_HMLIB_DXINPUTDEVICE
#		ifndef CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxInputDevice.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmPos.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmPos.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXINPUTDEVICE && CHG_HMLIB_HMPOS<100
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] hmPos.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXINPUTDEVICE<100 && 100<=CHG_HMLIB_HMPOS
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] hmPos.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMPOS<100
#//			pragma message("chg_hmLib : # error : [dxInputDevice.hpp] 使用可能なhmPos.hppが存在しません。")
#//		endif
#	endif
#
#	endif
#endif
