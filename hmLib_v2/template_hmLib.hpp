/*
xxx_v1_00.hpp
*/
#ifndef VER_HMLIB_XXX
#	define VER_HMLIB_XXX 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_XXX
#	if USE_HMLIB>=30000
#		define USE_HMLIB_XXX 100
#	else
#		define USE_HMLIB_XXX 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_XXX
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_XXX 100
#	else
#		define CHG_HMLIB_XXX 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_XXX
#	define INC_USE_HMLIB_XXX
#
#	if USE_HMLIB_XXX>100
#		error hmLib : # error : [xxx.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_XXX==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [xxx.hpp] v1_00")
#		endif
#		include "hmLib_v2/xxx/xxx_v1_00.hpp"
#	else
#		error hmLib : # error : [xxx.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_XXX && USE_HMLIB_XXX<101
#//		pragma message("hmLib : # error : [xxx.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_XXX
#	define INC_CHG_HMLIB_XXX
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_XXX
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_XXX
#			pragma message("chg_hmLib : # error : [xxx.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_XXX<100 && 100<=USE_HMLIB_XXX
#//			pragma message("chg_hmLib : # error : [xxx.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_XXX<100 && 100<=CHG_HMLIB_XXX
#//			pragma message("chg_hmLib : # error : [xxx.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_XXX<100
#			pragma message("chg_hmLib : # error : [xxx.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* yyyとの互換性について*/
#	if 100<=CHG_HMLIB_XXX
#		ifndef INC_CHG_HMLIB_YYY
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [xxx.hpp] yyy.hppに依存しています。CHG_HMLIB用にyyy.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/yyy.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/yyy.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_XXX && CHG_HMLIB_YYY<100
#//			pragma message("chg_hmLib : # error : [xxx.hpp] yyy.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_XXX<100 && 100<=CHG_HMLIB_YYY
#//			pragma message("chg_hmLib : # error : [xxx.hpp] yyy.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_YYY<100 || CHG_HMLIB_YYY==999)
#			pragma message("chg_hmLib : # error : [xxx.hpp] 使用可能なyyy.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
