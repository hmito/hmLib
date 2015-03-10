/*
dxImage_v1_00.hpp
	
*/
#ifndef VER_HMLIB_DXIMAGE
#	define VER_HMLIB_DXIMAGE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXIMAGE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXIMAGE 100
#	else
#		define USE_HMLIB_DXIMAGE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXIMAGE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXIMAGE 100
#	else
#		define CHG_HMLIB_DXIMAGE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXIMAGE
#	define INC_USE_HMLIB_DXIMAGE
#
#	if USE_HMLIB_DXIMAGE>100
#		error hmLib : # error : [dxImage.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_DXIMAGE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxImage.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxImage/dxImage_v1_00.h"
#	else
#		error hmLib : # error : [dxImage.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXIMAGE
#		pragma message("hmLib : # info  : [dxImage.hpp] DxLib依存ファイルです。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXIMAGE
#	define INC_CHG_HMLIB_DXIMAGE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXIMAGE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_DXIMAGE
#			pragma message("chg_hmLib : # error : [dxImage.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXIMAGE<100
#			pragma message("chg_hmLib : # error : [dxImage.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmPosとの互換性について*/
#	if 100<=CHG_HMLIB_DXIMAGE
#		ifndef CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxImage.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
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
#	/* hmColorとの互換性について*/
#	if 100<=CHG_HMLIB_DXIMAGE
#		ifndef CHG_HMLIB_HMCOLOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxImage.hpp] hmColor.hppに依存しています。CHG_HMLIB用にhmColor.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmColor.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmColor.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMCOLOR<101 && 100<=CHG_HMLIB_DXIMAGE
#			pragma message("chg_hmLib : # error : [dxImage.hpp] hmColor.hpp v1_01(v3_00_00)より前とは、互換性がありません。")
#		endif
#	endif
#	/* basic_iteratorとの互換性について*/
#	if 100<=CHG_HMLIB_DXIMAGE
#		ifndef CHG_HMLIB_BASICITERATOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxImage.hpp] basic_iterator.hppに依存しています。CHG_HMLIB用にbasic_iterator.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/basic_iterator.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/basic_iterator.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BASICITERATOR<101 && 100<=CHG_HMLIB_DXIMAGE
#			pragma message("chg_hmLib : # error : [dxImage.hpp] basic_iterator.hpp v1_01(v3_00_00)より前とは、互換性がありません。")
#		endif
#	endif
#	/* dxDrawCtrlとの互換性について*/
#	if 100<=CHG_HMLIB_DXIMAGE
#		ifndef CHG_HMLIB_DXDRAWCTRL
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxImage.hpp] dxDrawCtrl.hppに依存しています。CHG_HMLIB用にdxDrawCtrl.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxDrawCtrl.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxDrawCtrl.hpp"
#			endif
#		endif
#	endif
#	/* hmTypeとの互換性について*/
#	if 100<=CHG_HMLIB_DXIMAGE
#		ifndef CHG_HMLIB_HMTYPE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxImage.hpp] hmType.hppに依存しています。CHG_HMLIB用にhmType.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmType.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmType.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
