/*
lociset_v1_00.hpp
*/
#ifndef VER_HMLIB_LOCISET
#	define VER_HMLIB_LOCISET 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_LOCISET
#	if USE_HMLIB>=30000
#		define USE_HMLIB_LOCISET 100
#	else
#		define USE_HMLIB_LOCISET 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_LOCISET
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_LOCISET 100
#	else
#		define CHG_HMLIB_LOCISET 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_LOCISET
#	define INC_USE_HMLIB_LOCISET
#
#	if USE_HMLIB_LOCISET>100
#		error hmLib : # error : [lociset.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_LOCISET==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [lociset.hpp] v1_00")
#		endif
#		include "hmLib_v2/lociset/lociset_v1_00.hpp"
#	else
#		error hmLib : # error : [lociset.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_LOCISET && USE_HMLIB_LOCISET<101
#//		pragma message("hmLib : # error : [lociset.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_LOCISET
#	define INC_CHG_HMLIB_LOCISET
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_LOCISET
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_LOCISET
#			pragma message("chg_hmLib : # error : [lociset.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_LOCISET<100 && 100<=USE_HMLIB_LOCISET
#//			pragma message("chg_hmLib : # error : [lociset.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_LOCISET<100 && 100<=CHG_HMLIB_LOCISET
#//			pragma message("chg_hmLib : # error : [lociset.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_LOCISET<100
#			pragma message("chg_hmLib : # error : [lociset.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* randomとの互換性について*/
#	if 100<=CHG_HMLIB_LOCISET
#		ifndef INC_CHG_HMLIB_RANDOM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [lociset.hpp] random.hppに依存しています。CHG_HMLIB用にrandom.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/random.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/random.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_LOCISET && CHG_HMLIB_RANDOM<100
#//			pragma message("chg_hmLib : # error : [lociset.hpp] random.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_LOCISET<100 && 100<=CHG_HMLIB_RANDOM
#//			pragma message("chg_hmLib : # error : [lociset.hpp] random.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_RANDOM<100 || CHG_HMLIB_RANDOM==999)
#			pragma message("chg_hmLib : # error : [lociset.hpp] 使用可能なrandom.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
