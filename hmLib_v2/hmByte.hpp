/*
hmByte_v1_01
	Bytes::getのキャスト演算子がconst変数を使えなかったバグを修正
	ostreamへの出力を、0x表示から単純bit挿入に変更
	0x表示を出力するstr()関数追加
hmByte_v1_00
	ビット演算専用クラス、bytetype,Byte,Bytes追加
*/
#ifndef VER_HMLIB_HMBYTE
#	define VER_HMLIB_HMBYTE 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMBYTE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMBYTE 999
#	elif USE_HMLIB>=20803
#		define USE_HMLIB_HMBYTE 101
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMBYTE 100
#	else
#		define USE_HMLIB_HMBYTE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMBYTE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMBYTE 999
#	elif CHG_HMLIB>=20803
#		define CHG_HMLIB_HMBYTE 101
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMBYTE 100
#	else
#		define CHG_HMLIB_HMBYTE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMBYTE
#	define INC_USE_HMLIB_HMBYTE
#
#	if USE_HMLIB_HMBYTE>101
#		error hmLib : # error : [hmByte.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_HMBYTE==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmByte.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmByte/hmByte_v1_01.hpp"
#	elif USE_HMLIB_HMBYTE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmByte.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmByte/hmByte_v1_00.hpp"
#	else
#		error hmLib : # error : [hmByte.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 2<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMBYTE && USE_HMLIB_HMBYTE<101
#		pragma message("hmLib : #warning: [hmByte.hpp] Bytes::get関数にバグが存在します。const変数を使えません。v1_01(v3_01_00)以上への対応を推奨します。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMBYTE
#	define INC_CHG_HMLIB_HMBYTE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMBYTE
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_HMBYTE
#			pragma message("chg_hmLib : # error : [hmByte.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBYTE<101 && 101<=USE_HMLIB_HMBYTE
#			pragma message("chg_hmLib : # error : [hmByte.hpp] 前方互換性は保証されていません。v1_01(v2_08_03)より前は、Bytes::get関数にバグが存在します。")
#		elif 2<=OUT_CHG_HMLIB && CHG_HMLIB_HMBYTE<101 && 101<=USE_HMLIB_HMBYTE
#			pragma message("chg_hmLib : #warning: [hmByte.hpp] 前方互換性は保証されていません。v1_01(v2_08_03)より前は、ostream出力内容に変更があります。")
#		elif 2<=OUT_CHG_HMLIB && USE_HMLIB_HMBYTE<101 && 101<=CHG_HMLIB_HMBYTE
#			pragma message("chg_hmLib : #warning: [hmByte.hpp] 後方互換性は保証されていません。v1_01(v2_08_03)以降は、ostream出力内容に変更があります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBYTE<100
#			pragma message("chg_hmLib : # error : [hmByte.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmBasicとの互換性について*/
#	if 100<=CHG_HMLIB_HMBYTE
#		ifndef CHG_HMLIB_HMBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmByte.hpp] hmBasic.hppに依存しています。CHG_HMLIB用にhmBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmBasic.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_HMBYTE && CHG_HMLIB_HMBASIC<100
#//			pragma message("chg_hmLib : # error : [hmByte.hpp] hmBasic.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBYTE<100 && 100<=CHG_HMLIB_HMBASIC
#//			pragma message("chg_hmLib : # error : [hmByte.hpp] hmBasic.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && (CHG_HMLIB_HMBASIC<100 || CHG_HMLIB_HMBASIC==999)
#			pragma message("chg_hmLib : # error : [hmByte.hpp] 使用可能なhmBasic.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
