/*
hmAlign 廃止
	後継は、align.hpp
hmAlign_v1_01.hpp
	方向を示すクラス、hm::AlignQuad
	hm::aliq内の変数を使って、位置指定可能
hmAlign_v1_00.hpp
*/
#ifndef VER_HMLIB_HMALIGN
#	define VER_HMLIB_HMALIGN 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMALIGN
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMALIGN 999
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMALIGN 101
#	else
#		define USE_HMLIB_HMALIGN 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMALIGN
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMALIGN 999
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMALIGN 101
#	else
#		define CHG_HMLIB_HMALIGN 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMALIGN
#	define INC_USE_HMLIB_HMALIGN
#
#	if USE_HMLIB_HMALIGN>101
#		error hmLib : # error : [hmAlign.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継はalign.hppです。
#	elif USE_HMLIB_HMALIGN==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmAlign.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmAlign/hmAlign_v1_01.h"
#	else
#		error hmLib : # error : [hmAlign.hpp] 非対応バージョンです。v1_01(v2_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMALIGN
#	define INC_CHG_HMLIB_HMALIGN
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMALIGN
#		if 1<=OUT_CHG_HMLIB && 102<CHG_HMLIB_HMALIGN
#			pragma message("chg_hmLib : # error : [hmAlign.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継はalign.hppです。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMALIGN<100
#			pragma message("chg_hmLib : # error : [hmAlign.hpp] 非対応バージョンです。v1_01(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmPosとの互換性について*/
#	if 100<=CHG_HMLIB_HMALIGN
#		ifndef CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmAlign.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmPos.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmPos.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMALIGN<102 && 302<=CHG_HMLIB_HMPOS
#			pragma message("chg_hmLib : # error : [hmAlign.hpp] hmPos.hpp v3_02(v3_00_00)以降とは、互換性がありません。")
#		endif
#	endif
#
#	endif
#endif
