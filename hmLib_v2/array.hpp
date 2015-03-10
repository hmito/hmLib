/*
array廃止
	std::bitsetとboost::dynamic_bitsetの関係を、boost::arrayと取るため。
array_v1_03
	iteratorにhmstd::basic_iteratorを使用
array_v1_02
	iteratorにhm::basic_random_access_iteratorを使用
array_v1_01
	iteratorをSTL型に準拠
array_v1_00追加
	STL準拠の固定長配列クラス
*/
#ifndef VER_HMLIB_ARRAY
#	define VER_HMLIB_ARRAY 999
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#ifndef USE_HMLIB_ARRAY
#	if USE_HMLIB>=30000
#		define USE_HMLIB_ARRAY 999
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_ARRAY 103
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_ARRAY 102
#	elif USE_HMLIB>=20901
#		define USE_HMLIB_ARRAY 101
#	elif USE_HMLIB>=20900
#		define USE_HMLIB_ARRAY 100
#	else
#		define USE_HMLIB_ARRAY 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_ARRAY
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_ARRAY 999
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_ARRAY 103
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_ARRAY 102
#	elif CHG_HMLIB>=20901
#		define CHG_HMLIB_ARRAY 101
#	elif CHG_HMLIB>=20900
#		define CHG_HMLIB_ARRAY 100
#	else
#		define CHG_HMLIB_ARRAY 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_ARRAY
#	define INC_USE_HMLIB_ARRAY
#
#	if USE_HMLIB_ARRAY>103
#		error hmLib : [array.hpp] 廃止バージョンです。v1_03より後(v3_00_00以降)は、使用できません。後継にdynamic_array.hppが存在します。
#	elif USE_HMLIB_ARRAY==103
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [array.hpp] v1_03")
#		endif
#		include "hmLib_v2/array/array_v1_03.hpp"
#	elif USE_HMLIB_ARRAY==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [array.hpp] v1_02")
#		endif
#		include "hmLib_v2/array/array_v1_02.hpp"
#	elif USE_HMLIB_ARRAY==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [array.hpp] v1_01")
#		endif
#		include "hmLib_v2/array/array_v1_01.hpp"
#	elif USE_HMLIB_ARRAY==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [array.hpp] v1_00")
#		endif
#		include "hmLib_v2/array/array_v1_00.hpp"
#	else
#		error hmLib : [array.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません。
#	endif
#
#	if 2<=OUT_USE_HMLIB && 100<=USE_HMLIB_ARRAY && USE_HMLIB_ARRAY<104
#		pragma message("hmLib : #warning: [array.hpp] 一部アルゴリズムでは、イテレータが正常に作動しない可能性があります。v1_04(v3_00_00)以降で解決されています。")
#	endif
#	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_ARRAY && USE_HMLIB_ARRAY<103
#		pragma message("hmLib : # error : [array.hpp] operator!=に深刻なエラーが存在します。v1_03(v2_09_05)以上への対応を推奨します。")
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ARRAY
#	define INC_CHG_HMLIB_ARRAY
#
#	if 100<=USE_HMLIB_ARRAY
#		if 1<=OUT_CHG_HMLIB && 103<CHG_HMLIB_ARRAY
#			pragma message("chg_hmLib : # error : [array.hpp] 廃止バージョンです。v1_03より後(v3_00_00以降)は、使用できません。後継にdynamic_array.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAY<103 && 103<=USE_HMLIB_ARRAY
#			pragma message("chg_hmLib : # error : [array.hpp] operator!=に深刻なエラーが存在します。v1_03(v2_09_05)以上の使用を推奨します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ARRAY<100
#			pragma message("chg_hmLib : # error : [array.hpp] 非対応バージョンです。v1_00(v2_09_00)より前は、使用できません。")
#		endif
#	endif
#
#	if 102<=CHG_HMLIB_ARRAY && CHG_HMLIB_ARRAY<=102
#		ifndef INC_CHG_HMLIB_ALGORITHM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [array.hpp] algorithm.hppに依存しています。CHG_HMLIB用にalgorithm.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/algorithm.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/algorithm.hpp"
#			endif
#		endif
#		
#		if 1<=OUT_CHG_HMLIB && 102<CHG_HMLIB_ALGORITHM
#			pragma message("chg_hmLib : # error : [array.hpp] algorithm.hpp v1_02(v2_09_05)以降とは、互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ALGORITHM<100
#			pragma message("chg_hmLib : # error : [array.hpp] 使用可能なalgorithm.hppが存在しません。")
#		endif
#	endif
#
#	if 103<=CHG_HMLIB_ARRAY && CHG_HMLIB_ARRAY<=103
#		ifndef INC_CHG_HMLIB_BASICITERATOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [array.hpp] basic_iterator.hppに依存しています。CHG_HMLIB用にbasic_iterator.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/basic_iterator.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/basic_iterator.hpp"
#			endif
#		endif
#		
#		if 1<=OUT_CHG_HMLIB && 102<CHG_HMLIB_BASICITERATOR
#			pragma message("chg_hmLib : # error : [array.hpp] basic_iterator.hpp v1_02(v2_09_05)以降とは、互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && (CHG_HMLIB_BASICITERATOR<100 || CHG_HMLIB_BASICITERATOR==999)
#			pragma message("chg_hmLib : # error : [array.hpp] 使用可能なbasic_iterator.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
