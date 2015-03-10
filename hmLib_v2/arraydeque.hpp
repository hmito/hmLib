/*
arraydeque廃止
	dequeの名が誤解を招き、arrayloopの機能も持つcircular.hppが出来たため、廃止。
	circular.hppのbicircularが同様の機能を持つ。
arraydeque_v1_03
	iteratorにhmstd::basic_iteratorを使用
arraydeque_v1_02
	iteratorにhm::basic_random_access_iteratorを使用
	iteratorがバッファ上のデータの先頭のアドレスを保持するように変更
arraydeque_v1_01
	iteratorをSTL型に準拠
arraydeque_v1_00追加
	STL準拠のdeque型固定長配列クラス
	メモリの自己確保を行わないため、高速
*/
#ifndef VER_HMLIB_ARRAYDEQUE
#	define VER_HMLIB_ARRAYDEQUE 999
#endif
#ifndef INC_HMLIB
#	include <hmLib.h>
#endif
#ifndef USE_HMLIB_ARRAYDEQUE
#	ifdef USE_HMLIB>=30000
#		define USE_HMLIB_ARRAYDEQUE 999
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_ARRAYDEQUE 103
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_ARRAYDEQUE 102
#	elif USE_HMLIB>=20901
#		define USE_HMLIB_ARRAYDEQUE 101
#	elif USE_HMLIB>=20900
#		define USE_HMLIB_ARRAYDEQUE 100
#	else
#		define USE_HMLIB_ARRAYDEQUE 100
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_ARRAYDEQUE
#	ifdef CHG_HMLIB>=30000
#		define CHG_HMLIB_ARRAYDEQUE 999
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_ARRAYDEQUE 103
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_ARRAYDEQUE 102
#	elif CHG_HMLIB>=20901
#		define CHG_HMLIB_ARRAYDEQUE 101
#	elif CHG_HMLIB>=20900
#		define CHG_HMLIB_ARRAYDEQUE 100
#	else
#		define CHG_HMLIB_ARRAYDEQUE 100
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_ARRAYDEQUE
#	define INC_USE_HMLIB_ARRAYDEQUE
#
#	ifdef USE_HMLIB_ARRAYDEQUE>103
#		error hmLib : [arraydeque.hpp] 廃止バージョンです。v1_03より後(v3_00_00以降)は、使用できません。circular.hppを使用してください。
#	elif USE_HMLIB_ARRAYDEQUE==103
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraydeque.hpp] v1_03");
#		endif
#		include "hmLib_v2/arraydeque/arraydeque_v1_03.hpp"
#	elif USE_HMLIB_ARRAYDEQUE==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraydeque.hpp] v1_02");
#		endif
#		include "hmLib_v2/arraydeque/arraydeque_v1_02.hpp"
#	elif USE_HMLIB_ARRAYDEQUE==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraydeque.hpp] v1_01");
#		endif
#		include "hmLib_v2/arraydeque/arraydeque_v1_01.hpp"
#	elif USE_HMLIB_ARRAYDEQUE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [arraydeque.hpp] v1_00");
#		endif
#		include "hmLib_v2/arraydeque/arraydeque_v1_00.hpp"
#	else
#		error hmLib : [arraydeque.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB
#		pragma message("hmLib : # info  : [arraydeque.hpp] v1_03より後(v3_00_00以降)、廃止されます。後継にcircular.hppが存在します。");
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ARRAYDEQUE
#	define INC_CHG_HMLIB_ARRAYDEQUE
#
#	if 100<=USE_HMLIB_ARRAYDEQUE
#		if 1<=OUT_CHG_HMLIB && 103<CHG_HMLIB_ARRAYDEQUE
#			pragma message("chg_hmLib : # error : [arraydeque.hpp] 廃止バージョンです。v1_03より後(v3_00_00以降)は、使用できません。circular.hppを使用してください。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYDEQUE<100
#			pragma message("chg_hmLib : # error : [arraydeque.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません。")
#		endif
#	endif
#
#	if 100<=CHG_HMLIB_ARRAYDEQUE
#		ifndef INC_CHG_HMLIB_ARRAY
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [arraydeque.hpp] array.hppに依存しています。CHG_HMLIB用にarray.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/array.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/array.hpp"
#			endif
#		endif
#
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAYDEQUE<=103 && 103<CHG_HMLIB_ARRAY
#			pragma message("chg_hmLib : # error : [arraydeque.hpp] array.hpp v1_04(v3_00_00)以降とは互換性がありません。")
#		endif
#	endif
#
#	endif
#endif
