/*
dxObject廃止
dxObject_v2_06.hpp
	dxOのObjectアクセスクラス名をaccessorに変更
	dxO::accessorのアクセス演算子をiteratorに準拠
dxObject_v2_05.hpp
	dxArObjectが描画範囲を制限していた問題を修正
dxObject_v2_04.hpp
	dxFnを描画関数オブジェクトとして特化
	dxFnObjectをdxFnに特化
	dxArObjectを追加
dxObject_v2_03.hpp
	dxReObjectを追加
dxObject_v2_02.hpp
	関数ポインタ用関数オブジェクトdxFn追加
	dxFnObjectを関数オブジェクト用に改良
dxObject_v2_01.hpp
	dxBasic_v2_01に対応
dxObject_v2_00.hpp
	activeの位置をposの次に移動
dxObject_v1_01.hpp
	dxoProgress追加
	dxoBasicButがactiveでない時も押せたバグを修正
dxObject_v1_00.hpp
*/
#ifndef VER_HMLIB_DXOBJECT
#	define VER_HMLIB_DXOBJECT 206
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXOBJECT
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXOBJECT 999
#	elif USE_HMLIB>=20901
#		define USE_HMLIB_DXOBJECT 206
#	elif USE_HMLIB>=20804
#		define USE_HMLIB_DXOBJECT 205
#	elif USE_HMLIB>=20803
#		define USE_HMLIB_DXOBJECT 204
#	elif USE_HMLIB>=20802
#		define USE_HMLIB_DXOBJECT 203
#	elif USE_HMLIB>=20800
#		define USE_HMLIB_DXOBJECT 202
#	elif USE_HMLIB>=20200
#		define USE_HMLIB_DXOBJECT 201
#	elif USE_HMLIB>=20100
#		define USE_HMLIB_DXOBJECT 200
#	elif USE_HMLIB>=20001
#		define USE_HMLIB_DXOBJECT 101
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_DXOBJECT 100
#	else
#		define USE_HMLIB_DXOBJECT 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXOBJECT
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXOBJECT 999
#	elif CHG_HMLIB>=20901
#		define CHG_HMLIB_DXOBJECT 206
#	elif CHG_HMLIB>=20804
#		define CHG_HMLIB_DXOBJECT 205
#	elif CHG_HMLIB>=20803
#		define CHG_HMLIB_DXOBJECT 204
#	elif CHG_HMLIB>=20802
#		define CHG_HMLIB_DXOBJECT 203
#	elif CHG_HMLIB>=20800
#		define CHG_HMLIB_DXOBJECT 202
#	elif CHG_HMLIB>=20200
#		define CHG_HMLIB_DXOBJECT 201
#	elif CHG_HMLIB>=20100
#		define CHG_HMLIB_DXOBJECT 200
#	elif CHG_HMLIB>=20001
#		define CHG_HMLIB_DXOBJECT 101
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_DXOBJECT 100
#	else
#		define CHG_HMLIB_DXOBJECT 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXOBJECT
#	define INC_USE_HMLIB_DXOBJECT
#
#	if USE_HMLIB_DXOBJECT>206
#		error hmLib : # error : [dxObject.hpp] 非対応バージョンです。v2_06より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_DXOBJECT==206
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_06")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_06.hpp"
#	elif USE_HMLIB_DXOBJECT==205
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_05")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_05.hpp"
#	elif USE_HMLIB_DXOBJECT==204
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_04")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_04.hpp"
#	elif USE_HMLIB_DXOBJECT==203
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_03")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_03.hpp"
#	elif USE_HMLIB_DXOBJECT==202
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_02")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_02.hpp"
#	elif USE_HMLIB_DXOBJECT==201
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_01")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_01.hpp"
#	elif USE_HMLIB_DXOBJECT==200
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v2_00")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v2_00.hpp"
#	elif USE_HMLIB_DXOBJECT==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v1_01")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v1_01.hpp"
#	elif USE_HMLIB_DXOBJECT==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxObject.hpp] v1_00")
#		endif
#		include "hmLib_v2/dxObject/dxObject_v1_00.hpp"
#	else
#		error hmLib : # error : [dxObject.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_DXOBJECT && USE_HMLIB_DXOBJECT<101
#//		pragma message("hmLib : # error : [dxObject.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXOBJECT
#	define INC_CHG_HMLIB_DXOBJECT
#
#	define FILENAME __FILE__
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXOBJECT
#		if 1<=OUT_CHG_HMLIB && 206<CHG_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 非対応バージョンです。v2_06より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<206 && 206<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v2_06(v2_09_01)より前は、dxO::accessorがdxO::Acsとして設計されています。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXOBJECT<206 && 206<=CHG_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 後方互換性は保証されていません。v2_06(v2_09_01)以降は、dxO::AcsがdxO::accessorとして設計されています。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<204 && 204<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v2_04(v2_08_03)より前は、dxFnに関連するクラスの定義が大幅に変わっています。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXOBJECT<204 && 204<=CHG_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 後方互換性は保証されていません。v2_04(v2_08_03)以降は、dxFnに関連するクラスの定義が大幅に変わっています。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<203 && 203<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v2_03(v2_08_02)より前は、dxReObjectが使用できなくなります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<202 && 202<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v2_02(v2_08_00)より前は、dxFnObjectの描画用メンバ変数は、関数オブジェクトから関数ポインタに置き換わります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXOBJECT<202 && 202<=CHG_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 後方互換性は保証されていません。v2_02(v2_08_00)以降は、dxFnObjectの描画用メンバ変数は、関数ポインタから関数オブジェクトに置き換わります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<200 && 200<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v2_00(v2_01_00)より前は、draw関数の引数変更があります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_DXOBJECT<200 && 200<=CHG_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 後方互換性は保証されていません。v2_00(v2_01_00)以降は、draw関数の引数変更があります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<101 && 101<=USE_HMLIB_DXOBJECT
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 前方互換性は保証されていません。v1_01(v2_00_01)より前は、dxButのバグが存在します。dxProgressが使用不能となります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<100
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* dxBasicとの互換性について*/
#	if 100<=CHG_HMLIB_DXOBJECT
#		ifndef CHG_HMLIB_DXBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxObject.hpp] dxBasic.hppに依存しています。CHG_HMLIB用にdxBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/dxBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/dxBasic.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXOBJECT && CHG_HMLIB_DXBASIC<100
#//			pragma message("chg_hmLib : # error : [dxObject.hpp] dxBasic.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<100 && 100<=CHG_HMLIB_DXBASIC
#//			pragma message("chg_hmLib : # error : [dxObject.hpp] dxBasic.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_DXBASIC<100 || CHG_HMLIB_DXBASIC==999)
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 使用可能なdxBasic.hppが存在しません。")
#		endif
#	endif
#	/* hmObjectとの互換性について*/
#	if 100<=CHG_HMLIB_DXOBJECT
#		ifndef CHG_HMLIB_HMOBJECT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxObject.hpp] hmObject.hppに依存しています。CHG_HMLIB用にhmObject.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmObject.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmObject.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXOBJECT && CHG_HMLIB_HMOBJECT<100
#//			pragma message("chg_hmLib : # error : [dxObject.hpp] hmObject.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXOBJECT<100 && 100<=CHG_HMLIB_HMOBJECT
#//			pragma message("chg_hmLib : # error : [dxObject.hpp] hmObject.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_HMOBJECT<100 || CHG_HMLIB_HMOBJECT==999)
#			pragma message("chg_hmLib : # error : [dxObject.hpp] 使用可能なhmObject.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
