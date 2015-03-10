/*
hmPos廃止
	後継はposition.hpp
hmPos_v3_01
	std::pair非継承型に変更
hmPos_v3_00
	std::pairを使ったテンプレート型に変更
hmPos_v2_03
	PintX,PintY,area(平行四辺形面積)を追加
	関数名の変更
		InnerPrd=>inprd
		Min/Max=>min/max
hmPos_v2_02
	InnerPrd(内積)を追加
hmPos_v2_01
	Pdouble<->Pint互換性を実装
hmPos_v2_00
	Posクラスと改名
	Pdoubleクラス導入
hmPos_v1_03
	代入演算子、Set関数にアドレス確認機能を追加
hmPos_v1_02
	大小判定演算子を破棄
	大小判定関数を導入
hmPos_v1_01
	orthoを導入
*/
#ifndef VER_HMLIB_HMPOS
#	define VER_HMLIB_HMPOS 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMPOS
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMPOS 999
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMPOS 301
#	else
#		define USE_HMLIB_HMPOS 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMPOS
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMPOS 999
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMPOS 301
#	else
#		define CHG_HMLIB_HMPOS 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMPOS
#	define INC_USE_HMLIB_HMPOS
#
#	if USE_HMLIB_HMPOS>301
#		error hmLib : # error : [hmPos.hpp] 廃止バージョンです。v3_01より後(v3_00_00以降)は、使用できません。後継にposition.hppが存在します。
#	elif USE_HMLIB_HMPOS==301
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmPos.hpp] v3_01")
#		endif
#		include "hmLib_v2/hmPos/hmPos_v3_01.hpp"
#	else
#		error hmLib : # error : [hmPos.hpp] 非対応バージョンです。v3_01(v2_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMPOS
#	define INC_CHG_HMLIB_HMPOS
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMPOS
#		if 1<=OUT_CHG_HMLIB && 301<CHG_HMLIB_HMPOS
#			pragma message("chg_hmLib : # error : [hmPos.hpp] 廃止バージョンです。v3_01より後(v3_00_00以降)は、使用できません。後継にposition.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMPOS<301
#			pragma message("chg_hmLib : # error : [hmPos.hpp] 非対応バージョンです。v3_01(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmBasicとの互換性について*/
#	if 100<=CHG_HMLIB_HMPOS && CHG_HMLIB_HMPOS<302
#		ifndef CHG_HMLIB_HMBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmPos.hpp] hmBasic.hppに依存しています。CHG_HMLIB用にhmBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmBasic.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
