/*
csvflow_v1_01
	ocsvflow,icsvflowのコンストラクタの記述バグを修正
csvflow_v1_00
*/
#ifndef VER_HMLIB_CSVFLOW
#	define VER_HMLIB_CSVFLOW 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_CSVFLOW
#	if USE_HMLIB>=30000
#		define USE_HMLIB_CSVFLOW 999
#	elif USE_HMLIB>=20802
#		define USE_HMLIB_CSVFLOW 101
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_CSVFLOW 100
#	else
#		define USE_HMLIB_CSVFLOW 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_CSVFLOW
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_CSVFLOW 999
#	elif CHG_HMLIB>=20802
#		define CHG_HMLIB_CSVFLOW 101
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_CSVFLOW 100
#	else
#		define CHG_HMLIB_CSVFLOW 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_CSVFLOW
#	define INC_USE_HMLIB_CSVFLOW
#
#	if USE_HMLIB_CSVFLOW>101
#		error hmLib : # error : [csvflow.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_CSVFLOW==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [csvflow.hpp] v1_01")
#		endif
#		include "hmLib_v2/csvflow/csvflow_v1_01.hpp"
#	elif USE_HMLIB_CSVFLOW==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [csvflow.hpp] v1_00")
#		endif
#		include "hmLib_v2/csvflow/csvflow_v1_00.hpp"
#	else
#		error hmLib : # error : [csvflow.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_CSVFLOW && USE_HMLIB_CSVFLOW<101
#		pragma message("hmLib : # error : [csvflow.hpp] ocsvflow,icsvflowのコンストラクタに致命的なバグが存在します。v1_01(v2_08_02)以上への対応を推奨します。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_CSVFLOW
#	define INC_CHG_HMLIB_CSVFLOW
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_CSVFLOW
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_CSVFLOW
#			pragma message("chg_hmLib : # error : [csvflow.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CSVFLOW<101 && 101<=USE_HMLIB_CSVFLOW
#			pragma message("chg_hmLib : # error : [csvflow.hpp] 前方互換性は保証されていません。v1_01(v2_08_05)より前は、ocsvflow,icsvflowのコンストラクタに致命的なバグが存在します。")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_CSVFLOW<100 && 100<=CHG_HMLIB_CSVFLOW
#//			pragma message("chg_hmLib : # error : [csvflow.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CSVFLOW<100
#			pragma message("chg_hmLib : # error : [csvflow.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmPosとの互換性について*/
#	if 100<=CHG_HMLIB_CSVFLOW
#		ifndef INC_CHG_HMLIB_HMPOS
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [csvflow.hpp] hmPos.hppに依存しています。CHG_HMLIB用にhmPos.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmPos.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmPos.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_CSVFLOW && CHG_HMLIB_HMPOS<302
#//			pragma message("chg_hmLib : # error : [csvflow.hpp] hmPos.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CSVFLOW<=101 && 302<=CHG_HMLIB_HMPOS
#			pragma message("chg_hmLib : # error : [csvflow.hpp] hmPos.hpp v3_02(v3_00_00)以降とは、互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_HMPOS<100 || CHG_HMLIB_HMPOS==999)
#			pragma message("chg_hmLib : # error : [csvflow.hpp] 使用可能なhmPos.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
