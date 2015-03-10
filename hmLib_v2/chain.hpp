/*
chain_v1_00.hpp
	list型高速割当コンテナ
	位置を選ばない頻繁なinsert&eraseが必要な場合に有効
	listとの相違点は、
		newとdeleteのタイミングがinsert,eraseと異なる
			あらかじめまとめて確保&削除してもメモリを再利用
		chainの一部を別のchainに委ねることができない
			確保位置を配列単位で管理しているので、全て統合する以外に移管する方法がない
*/
#ifndef VER_HMLIB_CHAIN
#	define VER_HMLIB_CHAIN 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_CHAIN
#	if USE_HMLIB>=30000
#		define USE_HMLIB_CHAIN 100
#	else
#		define USE_HMLIB_CHAIN 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_CHAIN
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_CHAIN 100
#	else
#		define CHG_HMLIB_CHAIN 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_CHAIN
#	define INC_USE_HMLIB_CHAIN
#
#	if USE_HMLIB_CHAIN>100
#		error hmLib : # error : [chain.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_CHAIN==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [chain.hpp] v1_00")
#		endif
#		include "hmLib_v2/chain/chain_v1_00.hpp"
#	else
#		error hmLib : # error : [chain.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_CHAIN && USE_HMLIB_CHAIN<101
#//		pragma message("hmLib : # error : [chain.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_CHAIN
#	define INC_CHG_HMLIB_CHAIN
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_CHAIN
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_CHAIN
#			pragma message("chg_hmLib : # error : [chain.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CHAIN<100 && 100<=USE_HMLIB_CHAIN
#//			pragma message("chg_hmLib : # error : [chain.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_CHAIN<100 && 100<=CHG_HMLIB_CHAIN
#//			pragma message("chg_hmLib : # error : [chain.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CHAIN<100
#			pragma message("chg_hmLib : # error : [chain.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* iterator_patternとの互換性について*/
#	if 100<=CHG_HMLIB_CHAIN
#		ifndef CHG_HMLIB_ITERATORPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [chain.hpp] iterator_pattern.hppに依存しています。CHG_HMLIB用にiterator_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/iterator_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/iterator_pattern.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_CHAIN && CHG_HMLIB_ITERATOR_PATTERN<100
#//			pragma message("chg_hmLib : # error : [chain.hpp] iterator_pattern.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_CHAIN<100 && 100<=CHG_HMLIB_ITERATOR_PATTERN
#//			pragma message("chg_hmLib : # error : [chain.hpp] iterator_pattern.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_ITERATORPATTERN<100 || CHG_HMLIB_ITERATORPATTERN==999)
#			pragma message("chg_hmLib : # error : [chain.hpp] 使用可能なiterator_pattern.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
