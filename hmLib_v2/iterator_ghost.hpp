/*
iterator_ghost_v1_00.hpp
	仮想的に引数として入れられたiteratorとしてふるまう、iteratorの代役
	virtual宣言したいけどiteratorを使いたい場合に使用可能
	通常のtemplate<iterator>に比べて、3～4倍ほど遅い
*/
#ifndef VER_HMLIB_ITERATORGHOST
#	define VER_HMLIB_ITERATORGHOST 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_ITERATORGHOST
#	if USE_HMLIB>=30000
#		define USE_HMLIB_ITERATORGHOST 100
#	else
#		define USE_HMLIB_ITERATORGHOST 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_ITERATORGHOST
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_ITERATORGHOST 100
#	else
#		define CHG_HMLIB_ITERATORGHOST 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_ITERATORGHOST
#	define INC_USE_HMLIB_ITERATORGHOST
#
#	if USE_HMLIB_ITERATORGHOST>100
#		error hmLib : # error : [iterator_ghost.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_ITERATORGHOST==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [iterator_ghost.hpp] v1_00")
#		endif
#		include "hmLib_v2/iterator_ghost/iterator_ghost_v1_00.hpp"
#	else
#		error hmLib : # error : [iterator_ghost.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_ITERATORGHOST && USE_HMLIB_ITERATORGHOST<101
#//		pragma message("hmLib : # error : [iterator_ghost.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_ITERATORGHOST
#	define INC_CHG_HMLIB_ITERATORGHOST
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_ITERATORGHOST
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_ITERATORGHOST
#			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ITERATORGHOST<100 && 100<=USE_HMLIB_ITERATORGHOST
#//			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_ITERATORGHOST<100 && 100<=CHG_HMLIB_ITERATORGHOST
#//			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_ITERATORGHOST<100
#			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* iterator_patternとの互換性について*/
#	if 100<=CHG_HMLIB_ITERATORGHOST
#		ifndef CHG_HMLIB_ITERATORPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [iterator_ghost.hpp] iterator_pattern.hppに依存しています。CHG_HMLIB用にiterator_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/iterator_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/iterator_pattern.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_ITERATORGHOST && CHG_HMLIB_ITERATORPATTERN<100
#//			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] iterator_pattern.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_ITERATORGHOST<100 && 100<=CHG_HMLIB_ITERATORPATTERN
#//			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] iterator_pattern.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_ITERATORPATTERN<100 || CHG_HMLIB_ITERATORPATTERN==999)
#			pragma message("chg_hmLib : # error : [iterator_ghost.hpp] 使用可能なiterator_pattern.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
