/*
dxDisplay_v1_00.hpp
*/
#ifndef VER_HMLIB_DXDISPLAY
#	define VER_HMLIB_DXDISPLAY 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXDISPLAY
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXDISPLAY 100
#	else
#		define USE_HMLIB_DXDISPLAY 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXDISPLAY
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXDISPLAY 100
#	else
#		define CHG_HMLIB_DXDISPLAY 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXDISPLAY
#	define INC_USE_HMLIB_DXDISPLAY
#
#	if USE_HMLIB_DXDISPLAY>100
#		error hmLib : # error : [dxDisplay.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXDISPLAY==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxDisplay.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxDisplay/dxDisplay_v1_00.h"
#	else
#		error hmLib : # error : [dxDisplay.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXDISPLAY
#		pragma message("hmLib : # info  : [dxDisplay.hpp] DxLib依存ファイルです。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXDISPLAY
#	define INC_CHG_HMLIB_DXDISPLAY
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXDISPLAY
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXDISPLAY
#			pragma message("chg_hmLib : # error : [dxDisplay.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXDISPLAY<100
#			pragma message("chg_hmLib : # error : [dxDisplay.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmPos.hppとの互換性について*/
#	if 100<=CHG_HMLIB_DXDISPLAY
#		ifndef CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDisplay.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmPos.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmPos.hpp"
#			endif
#		endif
#	endif
#	/* hmTaskとの互換性について*/
#	if 100<=CHG_HMLIB_DXDISPLAY
#		ifndef CHG_HMLIB_HMTASK
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDisplay.hpp] hmTask.hppに依存しています。CHG_HMLIB用にhmTask.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmTask.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmTask.hpp"
#			endif
#		endif
#	endif
#	/* dxImageとの互換性について*/
#	if 100<=CHG_HMLIB_DXDISPLAY
#		ifndef CHG_HMLIB_DXIMAGE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxDisplay.hpp] dxImage.hppに依存しています。CHG_HMLIB用にdxImage.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxImage.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxImage.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
