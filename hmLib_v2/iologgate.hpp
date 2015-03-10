/*
iologate hmLibv3未対応
iologgate_v1_01.hpp
	hmTime_v1_00.hppに対応
*/
#ifndef VER_HMLIB_IOLOGGATE
#	define VER_HMLIB_IOLOGGATE 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#ifndef USE_HMLIB_IOLOGGATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_IOLOGGATE 999
#	elif USE_HMLIB>=20702
#		define USE_HMLIB_IOLOGGATE 101
#	elif USE_HMLIB>=20701
#		define USE_HMLIB_IOLOGGATE 100
#	else
#		define USE_HMLIB_IOLOGGATE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_IOLOGGATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_IOLOGGATE 999
#	elif CHG_HMLIB>=20702
#		define CHG_HMLIB_IOLOGGATE 101
#	else
#		define CHG_HMLIB_IOLOGGATE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_IOLOGGATE
#	define INC_USE_HMLIB_IOLOGGATE
#
#	if USE_HMLIB_IOLOGGATE>101
#		error hmLib : # error : [iologgate.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_IOLOGGATE==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [iologgate.hpp] v1_01")
#		endif
#		include "hmLib_v2/iologgate/iologgate_v1_01.hpp"
#	else
#		error hmLib : # error : [iologgate.hpp] 非対応バージョンです。v1_01(v2_07_02)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_IOLOGGATE && USE_HMLIB_IOLOGGATE<101
#//		pragma message("hmLib : # error : [iologgate.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_IOLOGGATE
#	define INC_CHG_HMLIB_IOLOGGATE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_IOLOGGATE
#		if 1<=OUT_CHG_HMLIB && 101<CHG_HMLIB_IOLOGGATE
#			pragma message("chg_hmLib : # error : [iologgate.hpp] 非対応バージョンです。v1_01より後(v3_00_00以降)は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_IOLOGGATE<200 && 200<=USE_HMLIB_IOLOGGATE
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] 前方互換性は保証されていません。v1_01(v2_07_02)より前は、iologHMTimeが使用できなくなります。")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_IOLOGGATE<200 && 200<=CHG_HMLIB_IOLOGGATE
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] 後方互換性は保証されていません。v1_01(v2_07_02)以降は、iologWinTimeが使用できなくなります。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_IOLOGGATE<101
#			pragma message("chg_hmLib : # error : [iologgate.hpp] 非対応バージョンです。v1_01(v2_07_02)より前は、使用できません。")
#		endif
#	endif
#	/* gatestreamとの互換性について*/
#	if 100<=CHG_HMLIB_IOLOGGATE
#		ifndef CHG_HMLIB_GATESTREAM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [iologgate.hpp] gatestream.hppに依存しています。CHG_HMLIB用にgatestream.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/gatestream.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/gatestream.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_IOLOGGATE && CHG_HMLIB_GATESTREAM<100
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] gatestream.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_IOLOGGATE<100 && 100<=CHG_HMLIB_GATESTREAM
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] gatestream.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_GATESTREAM<100
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] 使用可能なgatestream.hppが存在しません。")
#//		endif
#	endif
#	/* hmTimeとの互換性について*/
#	if 100<=CHG_HMLIB_IOLOGGATE
#		ifndef CHG_HMLIB_HMTIME
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [iologgate.hpp] hmTime.hppに依存しています。CHG_HMLIB用にhmTime.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmTime.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmTime.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_IOLOGGATE && CHG_HMLIB_HMTIME<100
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] hmTime.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_IOLOGGATE<100 && 100<=CHG_HMLIB_HMTIME
#//			pragma message("chg_hmLib : # error : [iologgate.hpp] hmTime.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_IOLOGGATE<102 && 102<=CHG_HMLIB_HMTIME
#			pragma message("chg_hmLib : # error : [iologgate.hpp] hmTime.hpp v1_02(v3_00_00)以降とは、互換性がありません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMTIME<100
#			pragma message("chg_hmLib : # error : [iologgate.hpp] 使用可能なhmTime.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
