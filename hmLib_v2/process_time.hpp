/*
process_time_v1_00.hpp
	hmBasic後継ファイルの一つ
	時間制御関数群
		プロセスタイムを取得
		inline u_int prctime();
		ストップウォッチを動かす Bufを変えることで、異なるストップウォッチとなる。
		inline u_int stopwatch(u_int &Buf);
		一時停止用関数
		inline void delay(u_int ms);
*/
#ifndef VER_HMLIB_PROCESSTIME
#	define VER_HMLIB_PROCESSTIME 100
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_PROCESSTIME
#	if USE_HMLIB>=30000
#		define USE_HMLIB_PROCESSTIME 100
#	else
#		define USE_HMLIB_PROCESSTIME 0
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_PROCESSTIME
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_PROCESSTIME 100
#	else
#		define CHG_HMLIB_PROCESSTIME 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_PROCESSTIME
#	define INC_USE_HMLIB_PROCESSTIME
#
#	if USE_HMLIB_PROCESSTIME>100
#		error hmLib : # error : [process_time.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_PROCESSTIME==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [process_time.hpp] v1_00")
#		endif
#		include "hmLib_v2/process_time/process_time_v1_00.hpp"
#	else
#		error hmLib : # error : [process_time.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB_PROCESSTIME
#	ifndef INC_CHG_HMLIB_PROCESSTIME
#	define INC_CHG_HMLIB_PROCESSTIME
#
#	if 100<=USE_HMLIB_PROCESSTIME
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_PROCESSTIME
#			pragma message("chg_hmLib : # error : [process_time.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_PROCESSTIME<100
#			pragma message("chg_hmLib : # error : [process_time.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* bytebase_typeとの互換性について*/
#	if 100<=CHG_HMLIB_PROCESSTIME
#		ifndef CHG_HMLIB_BYTEBASETYPE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [process_time.hpp] bytebase_type.hppに依存しています。CHG_HMLIB用にbytebase_type.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/bytebase_type.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/bytebase_type.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
