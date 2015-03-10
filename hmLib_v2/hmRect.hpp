/*
hmRect廃止
	後継はrectangle.hpp
hmRect_v1_01.hpp
	check関数を追加
hmRect_v1_00.hpp
*/
#ifndef VER_HMLIB_HMRECT
#	define VER_HMLIB_HMRECT 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMRECT
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMRECT 999
#	elif USE_HMLIB>=20805
#		define USE_HMLIB_HMRECT 101
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMRECT 100
#	else
#		define USE_HMLIB_HMRECT 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMRECT
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMRECT 999
#	elif CHG_HMLIB>=20805
#		define CHG_HMLIB_HMRECT 101
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMRECT 100
#	else
#		define CHG_HMLIB_HMRECT 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMRECT
#	define INC_USE_HMLIB_HMRECT
#
#	if USE_HMLIB_HMRECT>101
#		error hmLib : # error : [hmRect.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継にrectangle.hppが存在します。
#	elif USE_HMLIB_HMRECT==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRect.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmRect/hmRect_v1_01.hpp"
#	elif USE_HMLIB_HMRECT==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRect.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmRect/hmRect_v1_00.hpp"
#	else
#		error hmLib : # error : [hmRect.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMRECT
#	define INC_CHG_HMLIB_HMRECT
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMRECT
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_HMRECT
#			pragma message("chg_hmLib : # error : [hmRect.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継にrectangle.hppが存在します。")
#		endif
#		if 2<=OUT_CHG_HMLIB && CHG_HMLIB_HMRECT<102 && 102<=USE_HMLIB_HMRECT
#			pragma message("chg_hmLib : #warning: [hmRect.hpp] 前方互換性は保証されていません。v1_01(v2_08_05)より前は、hmRect::check関数は使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRECT<100
#			pragma message("chg_hmLib : # error : [hmRect.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmPosとの互換性について*/
#	if 100<=CHG_HMLIB_HMRECT
#		ifndef CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmRect.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmPos.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmPos.hpp"
#			endif
#		endif
#
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRECT<102 && 302<=CHG_HMLIB_HMPOS
#			pragma message("chg_hmLib : # error : [hmRect.hpp] hmPos.hpp v3_02(v3_00_00)以降とは、互換性がありません。")
#		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMPOS<302 && 102<=CHG_HMLIB_HMRECT
#			pragma message("chg_hmLib : # error : [hmRect.hpp] hmPos.hpp v3_02(v3_00_00)より前とは、互換性がありません。")
#		endif
#	endif
#
#	endif
#endif
