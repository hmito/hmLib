/*
dxArea廃止
dxArea_v2_04
	dxAreaが描画範囲を制限していなかった問題を修正
dxArea_v2_03
	dxArObjectを継承する形に修正
		dxObjectの更新に準拠
dxArea_v2_02
	dxObjectとhmoBoxを継承する形に修正
		dxBasicObject直接継承では、dxSubObjectとdxObjectにしか対応しないdxo.drawを使用できないため
	basic_dxMoveAreaType::AtoF,FtoA,setViewPosにバグがあったのを修正
dxArea_v2_01
	関数オブジェクトに対応
*/
#ifndef VER_HMLIB_DXAREA
#	define VER_HMLIB_DXAREA 204
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXAREA
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXAREA 999
#	elif USE_HMLIB>=20804
#		define USE_HMLIB_DXAREA 204
#	elif USE_HMLIB>=20803
#		define USE_HMLIB_DXAREA 203
#	elif USE_HMLIB>=20802
#		define USE_HMLIB_DXAREA 202
#	elif USE_HMLIB>=20800
#		define USE_HMLIB_DXAREA 201
#	elif USE_HMLIB>=20100
#		define USE_HMLIB_DXAREA 200
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_DXAREA 100
#	else
#		define USE_HMLIB_DXAREA 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXAREA
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXAREA 999
#	elif CHG_HMLIB>=20804
#		define CHG_HMLIB_DXAREA 204
#	elif CHG_HMLIB>=20803
#		define CHG_HMLIB_DXAREA 203
#	elif CHG_HMLIB>=20802
#		define CHG_HMLIB_DXAREA 202
#	elif CHG_HMLIB>=20800
#		define CHG_HMLIB_DXAREA 201
#	elif CHG_HMLIB>=20100
#		define CHG_HMLIB_DXAREA 200
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_DXAREA 100
#	else
#		define CHG_HMLIB_DXAREA 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXAREA
#	define INC_USE_HMLIB_DXAREA
#
#	if USE_HMLIB_DXAREA>204
#		error hmLib : # error : [dxArea.hpp] 非対応バージョンです。v2_04より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_DXAREA==204
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v2_04")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v2_04.hpp"
#	elif USE_HMLIB_DXAREA==203
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v2_03")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v2_03.hpp"
#	elif USE_HMLIB_DXAREA==202
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v2_02")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v2_02.hpp"
#	elif USE_HMLIB_DXAREA==201
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v2_01")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v2_01.hpp"
#	elif USE_HMLIB_DXAREA==200
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v2_00")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v2_00.hpp"
#	elif USE_HMLIB_DXAREA==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxArea.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxArea/dxArea_v1_00.hpp"
#	else
#		error hmLib : # error : [dxArea.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXAREA && USE_HMLIB_DXAREA<202
#		pragma message("hmLib : # error : [dxArea.hpp] basic_dxMoveAreaTypeの関数にバグが存在します。v2_02(v2_08_02)以上への対応を推奨します。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXAREA
#	define INC_CHG_HMLIB_DXAREA
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXAREA
#		if 1<=OUT_CHG_HMLIB && 204<CHG_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 非対応バージョンです。v2_04より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<203 && 203<=USE_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 前方互換性は保証されていません。v2_03(v2_08_03)より前は、dxFnの定義が異なります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXAREA<203 && 203<=CHG_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 後方互換性は保証されていません。v2_03(v2_08_03)以降は、dxFnの定義が異なります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<202 && 202<=USE_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 前方互換性は保証されていません。v2_02(v2_08_02)より前は、hmoBoxを継承していません。一部に致命的なバグが存在します。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXAREA<202 && 202<=CHG_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 後方互換性は保証されていません。v2_02(v2_08_02)以降は、hmoBoxを継承する形となります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<201 && 201<=USE_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 前方互換性は保証されていません。v2_01(v2_08_00)より前は、dxAreaの描画用メンバ変数が関数ポインタしか使用できません。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXAREA<201 && 201<=CHG_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 後方互換性は保証されていません。v2_01(v2_08_00)以降は、dxAreaの描画用メンバ変数が関数ポインタから関数オブジェクトに置き換わります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<200 && 200<=USE_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 前方互換性は保証されていません。v2_00(v2_01_00)より前は、関数の引数の変更があります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXAREA<200 && 200<=CHG_HMLIB_DXAREA
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 後方互換性は保証されていません。v2_00(v2_01_00)以降は、関数の引数の変更があります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<100
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* dxObjectとの互換性について*/
#	if 100<=CHG_HMLIB_DXAREA
#		ifndef CHG_HMLIB_DXOBJECT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxArea.hpp] dxObject.hppに依存しています。CHG_HMLIB用にdxObject.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxObject.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxObject.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXAREA && CHG_HMLIB_DXOBJECT<100
#//			pragma message("chg_hmLib : # error : [dxArea.hpp] dxObject.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXAREA<100 && 100<=CHG_HMLIB_DXOBJECT
#//			pragma message("chg_hmLib : # error : [dxArea.hpp] dxObject.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_DXOBJECT<100 || CHG_HMLIB_DXOBJECT==999)
#			pragma message("chg_hmLib : # error : [dxArea.hpp] 使用可能なdxObject.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
