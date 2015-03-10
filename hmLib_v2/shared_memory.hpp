/*
shared_memory_v1_00.hpp
	一時的メモリ確保用バッファ
	複数の場所で一時的に巨大なバッファが必要となる場合に、
	各々で確保せず、共通のただ一つのメモリを作り共有する
*/
#ifndef VER_HMLIB_SHAREDMEMORY
#	define VER_HMLIB_SHAREDMEMORY 100
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_SHAREDMEMORY
#	if USE_HMLIB>=30000
#		define USE_HMLIB_SHAREDMEMORY 100
#	else
#		define USE_HMLIB_SHAREDMEMORY 0
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_SHAREDMEMORY
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_SHAREDMEMORY 100
#	else
#		define CHG_HMLIB_SHAREDMEMORY 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_SHAREDMEMORY
#	define INC_USE_HMLIB_SHAREDMEMORY
#
#	if USE_HMLIB_SHAREDMEMORY>100
#		error hmLib : [shared_memory.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_SHAREDMEMORY==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [shared_memory.hpp] v1_00")
#		endif
#		include "hmLib_v2/shared_memory/shared_memory_v1_00.hpp"
#	else
#		error hmLib : [shared_memory.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_SHAREDMEMORY
#		pragma message("hmLib : # info  : [shared_memory.hpp] shm::toStr,shm::toCStr関数はマルチスレッド安全ではありません。")
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_SHAREDMEMORY
#	define INC_CHG_HMLIB_SHAREDMEMORY
#
#	if 100<=USE_HMLIB_SHAREDMEMORY
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_SHAREDMEMORY
#			pragma message("chg_hmLib : # error : [shared_memory.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_SHAREDMEMORY<100
#			pragma message("chg_hmLib : # error : [shared_memory.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	/* exceptionとの互換性について*/
#	if 100<=CHG_HMLIB_SHAREDMEMORY
#		ifndef INC_CHG_HMLIB_EXCEPTION
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [shared_memory.hpp] exception.hppに依存しています。CHG_HMLIB用にexception.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/exception.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/exception.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_SHAREDMEMORY && CHG_HMLIB_EXCEPTION<100
#//			pragma message("chg_hmLib : # error : [shared_memory.hpp] exception.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_SHAREDMEMORY<100 && 100<=CHG_HMLIB_EXCEPTION
#//			pragma message("chg_hmLib : # error : [shared_memory.hpp] exception.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_EXCEPTION<100 || CHG_HMLIB_EXCEPTION==999)
#			pragma message("chg_hmLib : # error : [shared_memory.hpp] 使用可能なexception.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
