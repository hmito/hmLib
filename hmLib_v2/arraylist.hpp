/*
arraylist廃止
	後継は可変長list型配列のchain　ポインタが要素の追加削除時も保障されるようになる
arraylist_v1_03
	iteratorにhmstd::basic_iteratorを使用
arraylist_v1_02
	iteratorにhm::basic_bidirectional_iteratorを使用
arraylist_v1_01
	iteratorをSTL型に準拠
arraylist_v1_00追加
	STL準拠のlist型arrayクラス
	毎回メモリ確保を行わず、固定配列上にデータを置
	任意の位置の要素の挿入/削除が高速
*/
#ifndef VER_HMLIB_ARRAYLIST
#	define VER_HMLIB_ARRAYLIST 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_ARRAYLIST
#	if USE_HMLIB>=30000
#		define USE_HMLIB_ARRAYLIST 999
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_ARRAYLIST 103
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_ARRAYLIST 102
#	elif USE_HMLIB>=20901
#		define USE_HMLIB_ARRAYLIST 101
#	elif USE_HMLIB>=20900
#		define USE_HMLIB_ARRAYLIST 100
#	else
#		define USE_HMLIB_ARRAYLIST 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_ARRAYLIST
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_ARRAYLIST 999
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_ARRAYLIST 103
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_ARRAYLIST 102
#	elif CHG_HMLIB>=20901
#		define CHG_HMLIB_ARRAYLIST 101
#	elif CHG_HMLIB>=20900
#		define CHG_HMLIB_ARRAYLIST 100
#	else
#		define CHG_HMLIB_ARRAYLIST 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_ARRAYLIST
#	define INC_USE_HMLIB_ARRAYLIST
#
#	if USE_HMLIB_ARRAYLIST>103
#		error hmLib : # error : [arraylist.hpp] 廃止バージョンです。v1_03(v3_00_00)より後は、使用できません。後継にchain.hppが存在します。
#	elif USE_HMLIB_ARRAYLIST==103
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraylist.hpp] v1_03")
#		endif
#		include "hmLib_v2/arraylist/arraylist_v1_03.hpp"
#	elif USE_HMLIB_ARRAYLIST==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraylist.hpp] v1_02")
#		endif
#		include "hmLib_v2/arraylist/arraylist_v1_02.hpp"
#	elif USE_HMLIB_ARRAYLIST==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraylist.hpp] v1_01")
#		endif
#		include "hmLib_v2/arraylist/arraylist_v1_01.hpp"
#	elif USE_HMLIB_ARRAYLIST==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraylist.hpp] v1_00")
#		endif
#		include "hmLib_v2/arraylist/arraylist_v1_00.hpp"
#	else
#		error hmLib : # error : [arraylist.hpp] 非対応バージョンです。v1_00(2_09_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ARRAYLIST
#	define INC_CHG_HMLIB_ARRAYLIST
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_ARRAYLIST
#		if 1<=OUT_CHG_HMLIB && 103<CHG_HMLIB_ARRAYLIST
#			pragma message("chg_hmLib : # error : [arraylist.hpp] 廃止バージョンです。v1_03(v3_00_00)より後は、使用できません。後継にchain.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYLIST<100
#			pragma message("chg_hmLib : # error : [arraylist.hpp] 非対応バージョンです。v1_00(2_09_00)より前は、使用できません。")
#		endif
#	endif
#	/* arrayとの互換性について*/
#	if 100<=CHG_HMLIB_ARRAYLIST
#		ifndef INC_CHG_HMLIB_ARRAY
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [arraylist.hpp] array.hppに依存しています。CHG_HMLIB用にarray.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/array.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/array.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYLIST<104 && 104<=CHG_HMLIB_ARRAY
#			pragma message("chg_hmLib : # error : [arraylist.hpp] array.hpp v1_04(v3_00_00)以降とは、互換性がありません。")
#		endif
#	endif
#
#	endif
#endif
