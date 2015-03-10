/*
dxBasic廃止
dxBasic_v2_05
	winTime.hppを廃止し、hmTime.hppに対応
dxBasic_v2_04
	hmBasic.h v1_3に対応
dxBasic_v2_03
	dx::resetDrawScreen実行時にNowScreenが変更されなかったバグを修正
dxBasic_v2_02
	dxFont::make(...)が正常に機能しなかったバグを修正
dxBasic_v2_01
	dxGraphが画像保存できないバグを修正
	描画可能なグラフィックハンドルを作るdxBasicScreen,dxScreenの追加
	dx::setDrawScreenの引数をdxBasicScreenに変更
dxBasic_v2_00
	DxLiv_V3に対応
	dx::ini,dx::endの追加とdxManagerの廃止
	DxLibを利用するクラスのコンストラクタ廃止
	dx namespaceに全関数を移行
	関数名の一部変更、クラス間で統一化
	dxColor_v2 に対応
	dxColorSet.hに分割
	dxAlign.hppにPOS::Positionを分割変名
	dxInputの描画できなくなるバグを修正
	自動ハンドル消去機能のないUnsafeな画像クラス、dxBasicGraphを追加
	複数画像クラス、dxGraphsを追加
	仮想画像クラス、dxImGraphの色/透過度へのアクセス関数を強化
	dxGraphとdxImGraphの相互変換機能追加(いずれもload関数で仕様)
	描画内容指定クラス、dxDMode追加
	描画範囲指定クラス、dxDAreaを追加
*/
#ifndef VER_HMLIB_DXBASIC
#	define VER_HMLIB_DXBASIC 205
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_DXBASIC
#	if USE_HMLIB>=30000
#		define USE_HMLIB_DXBASIC 999
#	elif USE_HMLIB>=20702
#		define USE_HMLIB_DXBASIC 205
#	elif USE_HMLIB>=20600
#		define USE_HMLIB_DXBASIC 204
#	elif USE_HMLIB>=20500
#		define USE_HMLIB_DXBASIC 203
#	elif USE_HMLIB>=20200
#		define USE_HMLIB_DXBASIC 201
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_DXBASIC 200
#	else
#		define USE_HMLIB_DXBASIC 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_DXBASIC
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_DXBASIC 999
#	elif CHG_HMLIB>=20702
#		define CHG_HMLIB_DXBASIC 205
#	elif CHG_HMLIB>=20600
#		define CHG_HMLIB_DXBASIC 204
#	elif CHG_HMLIB>=20500
#		define CHG_HMLIB_DXBASIC 203
#	elif CHG_HMLIB>=20200
#		define CHG_HMLIB_DXBASIC 201
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_DXBASIC 200
#	else
#		define CHG_HMLIB_DXBASIC 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_DXBASIC
#	define INC_USE_HMLIB_DXBASIC
#
#	if USE_HMLIB_DXBASIC>205
#		error hmLib : # error : [dxBasic.hpp] 非対応バージョンです。v2_05より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_DXBASIC==205
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxBasic.hpp] v2_05")
#		endif
#		include "hmLib_v2/dxBasic/dxBasic_v2_05.h"
#	elif USE_HMLIB_DXBASIC==204
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxBasic.hpp] v2_04")
#		endif
#		include "hmLib_v2/dxBasic/dxBasic_v2_04.h"
#	elif USE_HMLIB_DXBASIC==203
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxBasic.hpp] v2_03")
#		endif
#		include "hmLib_v2/dxBasic/dxBasic_v2_03.h"
#	elif USE_HMLIB_DXBASIC==201
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxBasic.hpp] v2_01")
#		endif
#		include "hmLib_v2/dxBasic/dxBasic_v2_01.h"
#	elif USE_HMLIB_DXBASIC==200
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [dxBasic.hpp] v2_00")
#		endif
#		include "hmLib_v2/dxBasic/dxBasic_v2_00.h"
#	else
#		error hmLib : # error : [dxBasic.hpp] 非対応バージョンです。v2_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<USE_HMLIB_DXBASIC
#		pragma message("hmLib : # info  : [dxBasic.hpp] 廃止バージョンです。v2_05より後(v3_00_00以降)は、使用できません。")
#	endif
#	if 1<=OUT_USE_HMLIB && USE_HMLIB_DXBASIC<203
#		pragma message("hmLib : # error : [dxBasic.hpp] dxFont,スクリーン関連のバグが存在します。v2_03(v2_05_00)以上への対応を推奨します。")
#	endif
#	if 1<=OUT_USE_HMLIB && USE_HMLIB_DXBASIC<201
#		pragma message("hmLib : # error : [dxBasic.hpp] スクリーン関連のバグが存在します。v2_01(v2_02_00)以上への対応を推奨します。")
#	endif
#
#	endif
#endif)
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_DXBASIC
#	define INC_CHG_HMLIB_DXBASIC
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_DXBASIC
#		if 1<=OUT_CHG_HMLIB && 205<CHG_HMLIB_DXBASIC
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 非対応バージョンです。v2_05より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<203 && 203<=USE_HMLIB_DXBASIC
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 前方互換性は保証されていません。v2_03(v2_05_00)より前は、dxFont,スクリーン関連のバグが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<201 && 201<=USE_HMLIB_DXBASIC
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 前方互換性は保証されていません。v2_01(v2_02_00)より前は、スクリーン関連のバグが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<200
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 非対応バージョンです。v2_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmTimeとの互換性について*/
#	if 205<=CHG_HMLIB_DXBASIC
#		ifndef CHG_HMLIB_HMTIME
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxBasic.hpp] hmTime.hppに依存しています。CHG_HMLIB用にhmTime.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmTime.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmTime.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXBASIC && CHG_HMLIB_HMTIME<100
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] hmTime.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<100 && 100<=CHG_HMLIB_HMTIME
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] hmTime.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_HMTIME<100 || CHG_HMLIB_HMTIME==999)
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 使用可能なhmTime.hppが存在しません。")
#		endif
#	endif
#	/* winTimeとの互換性について*/
#	if 200<=CHG_HMLIB_DXBASIC && CHG_HMLIB_DXBASIC<205
#		ifndef CHG_HMLIB_WINTIME
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxBasic.hpp] winTime.hppに依存しています。CHG_HMLIB用にwinTime.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/winTime.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/winTime.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXBASIC && CHG_HMLIB_WINTIME<100
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] winTime.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<100 && 100<=CHG_HMLIB_WINTIME
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] winTime.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_WINTIME<100 || CHG_HMLIB_WINTIME==999)
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 使用可能なwinTime.hppが存在しません。")
#		endif
#	endif
#	/* hmBasicとの互換性について*/
#	if 200<=CHG_HMLIB_DXBASIC
#		ifndef CHG_HMLIB_HMBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [dxBasic.hpp] hmBasic.hppに依存しています。CHG_HMLIB用にhmBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmBasic.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_DXBASIC && CHG_HMLIB_HMBASIC<100
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] hmBasic.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_DXBASIC<100 && 100<=CHG_HMLIB_HMBASIC
#//			pragma message("chg_hmLib : # error : [dxBasic.hpp] hmBasic.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_HMBASIC<100 || CHG_HMLIB_HMBASIC==999)
#			pragma message("chg_hmLib : # error : [dxBasic.hpp] 使用可能なhmBasic.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
