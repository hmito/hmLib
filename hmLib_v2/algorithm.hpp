/*
algorithm.hpp 廃止
	後継はhmAlgorithm.hppとhmFuncIterator.hppに移行
algorithm_v1_02
	iterator生成用クラス群をbasic_iteratorに統一し、basic_iterator.hppへと移動
algorithm_v1_01追加
	basic_random_access_iteratorにdistanceを追加
algorithm_v1_00追加
	STL系用のalgorithm
*/
#ifndef VER_HMLIB_ALGORITHM
#	define VER_HMLIB_ALGORITHM 103
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_ALGORITHM
#	if USE_HMLIB>=30000
#		define USE_HMLIB_ALGORITHM 103
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_ALGORITHM 102
#	elif USE_HMLIB>=20904
#		define USE_HMLIB_ALGORITHM 101
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_ALGORITHM 100
#	else
#		define USE_HMLIB_ALGORITHM 0
#	endif
#endif
#
#if defined CHG_HMLIB && !defined CHG_HMLIB_ALGORITHM
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_ALGORITHM 103
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_ALGORITHM 102
#	elif CHG_HMLIB>=20904
#		define CHG_HMLIB_ALGORITHM 101
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_ALGORITHM 100
#	else
#		define CHG_HMLIB_ALGORITHM 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_ALGORITHM
#	define INC_USE_HMLIB_ALGORITHM
#
#	if USE_HMLIB_ALGORITHM>103
#		error hmLib : [algorithm.hpp] 非対応バージョンです。v1_03より後は、使用できません。
#	elif USE_HMLIB_ALGORITHM==103
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [algorithm.hpp] v1_03")
#		endif
#		include "hmLib_v2/algorithm/algorithm_v1_03.hpp"
#	elif USE_HMLIB_ALGORITHM==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [algorithm.hpp] v1_02")
#		endif
#		include "hmLib_v2/algorithm/algorithm_v1_02.hpp"
#	elif USE_HMLIB_ALGORITHM==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [algorithm.hpp] v1_01")
#		endif
#		include "hmLib_v2/algorithm/algorithm_v1_01.hpp"
#	elif USE_HMLIB_ALGORITHM==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [algorithm.hpp] v1_00")
#		endif
#		include "hmLib_v2/algorithm/algorithm_v1_00.hpp"
#	else
#		error hmLib : [algorithm.hpp] 非対応バージョンです。v1_00(v2_09_02)より前は、使用できません。
#	endif
#
#	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_ALGORITHM && USE_HMLIB_ALGORITHM<101
#		pragma message("hmLib : #warning: [algorithm.hpp] basic_random_access_iteratorに致命的なバグがあります。v1_01(v2_09_04)以上の使用を推奨します。")
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ALGORITHM
#	define INC_CHG_HMLIB_ALGORITHM
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_ALGORITHM
#		if 1<=OUT_CHG_HMLIB && 103<CHG_HMLIB_ALGORITHM
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 非対応バージョンです。v1_03より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_ALGORITHM<103 && 103<=USE_ALGORITHM
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 前方互換性は保証されていません。v1_03(v3_00_00)より前は、現在のアルゴリズムを含んでいません。")
#		elif 1<=OUT_CHG_HMLIB && USE_ALGORITHM<103 && 103<=CHG_ALGORITHM
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 後方互換性は保証されていません。v1_03(v3_00_00)以降は、特殊イテレータを含んでいません。後継のfunctional_iterator.hppを使用してください。")
#		endif
#		if 2<=OUT_CHG_HMLIB && CHG_HMLIB_ALGORITHM<101 & 101<=USE_HMLIB_ALGORITHM && USE_HMLIB_ALGORITHM<102
#			pragma message("chg_hmLib : #warning: [algorithm.hpp] basic_random_access_iteratorに致命的なバグがあります。v1_01(v2_09_02)以上でしか使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ALGORITHM<100
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 非対応バージョンです。v1_00(v2_09_02)より前は、使用できません。")
#		endif
#	endif
#
#	if 100<=CHG_HMLIB_ALGORITHM && CHG_HMLIB_ALGORITHM<=102
#		ifndef CHG_HMLIB_HMRAND
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [algorithm.hpp] hmRand.hppに依存しています。CHG_HMLIB用にhmRand.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmRand.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmRand.hpp"
#			endif
#		endif
#
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ALGORITHM<102 && 205<=CHG_HMLIB_HMRAND
#			pragma message("chg_hmLib : # error : [algorithm.hpp] hmRand.hpp v2_05(v3_00_00)以降とは互換性がありません。")
#		elif 1<=OUT_CHG_HMLIB && 102<=CHG_HMLIB_ALGORITHM && CHG_HMLIB_HMRAND<205
#			pragma message("chg_hmLib : # error : [algorithm.hpp] hmRand.hpp v2_05(v3_00_00)より前とは互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && (CHG_HMLIB_HMRAND<100 || CHG_HMLIB_HMRAND==999)
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 使用可能なhmRand.hppが存在しません。")
#		endif
#	endif
#
#	if 102<=CHG_HMLIB_ALGORITHM && CHG_HMLIB_ALGORITHM<=102
#		ifndef CHG_HMLIB_BASICITERATOR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [algorithm.hpp] basic_iterator.hppに依存しています。CHG_HMLIB用にbasic_iterator.hppをincludeします。")
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
#		if 1<=OUT_CHG_HMLIB && 101<=CHG_HMLIB_BASIC_ITERATOR
#			pragma message("chg_hmLib : # error : [algorithm.hpp] basic_iterator.hpp v1_01(v3_00_00)以降とは互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BASIC_ITERATOR<100
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 使用可能なbasic_iterator.hppが存在しません。")
#		endif
#	endif
#
#	if 103<=CHG_HMLIB_ALGORITHM
#		ifndef INC_CHG_HMLIB_RANDOM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [algorithm.hpp] random.hppに依存しています。CHG_HMLIB用にrandom.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/random.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/random.hpp"
#			endif
#		endif
#
#		if 1<=OUT_CHG_HMLIB && 101<=CHG_HMLIB_RANDOM
#			pragma message("chg_hmLib : # error : [algorithm.hpp] random.hpp v1_01(v3_00_00)以降とは互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && (CHG_HMLIB_RANDOM<100 || CHG_HMLIB_RANDOM==999)
#			pragma message("chg_hmLib : # error : [algorithm.hpp] 使用可能なrandom.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif

