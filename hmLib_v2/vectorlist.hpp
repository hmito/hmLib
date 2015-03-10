/*
vectorlist廃止
	より高機能なlist型コンテナ、chainができたため。
	後継はchain.hpp
vectorlist_v1_01
	メモリ確保のふるまいを、vectorに準拠(追加確保は行うが、解放は自動で行わない)
vectorlist_v1_00追加
	STL準拠のlist型vectorクラス
	自ら要素数の調整を行うarraylist
	任意の位置の要素の挿入/削除が高速
*/
#ifndef VER_HMLIB_VECTORLIST
#	define VER_HMLIB_VECTORLIST 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_VECTORLIST
#	if USE_HMLIB>=30000
#		define USE_HMLIB_VECTORLIST 999
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_VECTORLIST 101
#	elif USE_HMLIB>=20900
#		define USE_HMLIB_VECTORLIST 100
#	else
#		define USE_HMLIB_VECTORLIST 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_VECTORLIST
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_VECTORLIST 999
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_VECTORLIST 101
#	elif CHG_HMLIB>=20900
#		define CHG_HMLIB_VECTORLIST 100
#	else
#		define CHG_HMLIB_VECTORLIST 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_VECTORLIST
#	define INC_USE_HMLIB_VECTORLIST
#
#	if USE_HMLIB_VECTORLIST>101
#		error hmLib : # error : [vectorlist.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継にchain.hppが存在します。
#	elif USE_HMLIB_VECTORLIST==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [vectorlist.hpp] v1_01")
#		endif
#		include "hmLib_v2/vectorlist/vectorlist_v1_01.hpp"
#	elif USE_HMLIB_VECTORLIST==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [vectorlist.hpp] v1_00")
#		endif
#		include "hmLib_v2/vectorlist/vectorlist_v1_00.hpp"
#	else
#		error hmLib : # error : [vectorlist.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_VECTORLIST
#	define INC_CHG_HMLIB_VECTORLIST
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_VECTORLIST
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_VECTORLIST
#			pragma message("chg_hmLib : # error : [vectorlist.hpp] 廃止バージョンです。v1_01より後(v3_00_00以降)は、使用できません。後継にchain.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_VECTORLIST<100
#			pragma message("chg_hmLib : # error : [vectorlist.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません")
#		endif
#	endif
#	/* arraylistとの互換性について*/
#	if 100<=CHG_HMLIB_VECTORLIST
#		ifndef CHG_HMLIB_ARRAYLIST
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [vectorlist.hpp] arraylist.hppに依存しています。CHG_HMLIB用にarraylist.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/arraylist.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/arraylist.hpp"
#			endif
#		endif
#
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_VECTORLIST<102 && 104<=CHG_HMLIB_ARRAYLIST
#			pragma message("chg_hmLib : # error : [vectorlist.hpp] arraylist.hpp v1_04(v3_00_00)以降とは、互換性がありません。")
#		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYLIST<104 && 102<=CHG_HMLIB_VECTORLIST
#			pragma message("chg_hmLib : # error : [vectorlist.hpp] arraylist.hpp v1_04(v3_00_00)より前とは、互換性がありません。")
#		endif
#
#	endif
#
#	endif
#endif
