/*
binary_v1_00.hpp
	hmBasic後継ファイルの一つ
	bit演算用関数群
		bittest:指定bitの値取得
		bitput:指定bitを指定の値に
		bitset:指定bitをon
		bitclear:指定bitをoff
		bitxor:指定bitをxor
*/
#ifndef VER_HMLIB_BINARY
#	define VER_HMLIB_BINARY 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#ifndef USE_HMLIB_BINARY
#	if USE_HMLIB>=30000
#		define USE_HMLIB_BINARY 100
#	else
#		define USE_HMLIB_BINARY 0
#	endif
#endif
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_BINARY
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_BINARY 100
#	else
#		define CHG_HMLIB_BINARY 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_BINARY
#	define INC_USE_HMLIB_BINARY
#
#	if USE_HMLIB_BINARY>100
#		error hmLib : [binary.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_BINARY==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [binary.hpp] v1_00")
#		endif
#		include "hmLib_v2/binary/binary_v1_00.hpp"
#	else
#		error hmLib : [binary.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_BINARY
#	define INC_CHG_HMLIB_BINARY
#
#	if 100<=USE_HMLIB_BINARY
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_BINARY
#			pragma message("chg_hmLib : # error : [binary.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BINARY<100
#			pragma message("chg_hmLib : # error : [binary.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	if 100<=CHG_HMLIB_BINARY
#		ifndef INC_CHG_HMLIB_BYTEBASETYPE
#			if 3<=OUT_CHG_HMLIB
#			 	pragma message("chg_hmLib : # info  : [binary.hpp] bytebase_type.hppに依存しています。CHG_HMLIB用にbytebase_type.hppをincludeします。")
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
