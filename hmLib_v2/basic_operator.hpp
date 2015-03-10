/*
basic_operator 廃止
	機能群整理のため
	後継にoperator_pattern.hpp
basic_operator_v1_00追加
	operator基本作成キット
*/
#ifndef VER_HMLIB_BASICOPERATOR
#define VER_HMLIB_BASICOPERATOR 999
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#ifndef USE_HMLIB_BASICOPERATOR
#	if USE_HMLIB>=30000
#		define USE_HMLIB_BASICOPERATOR 999
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_BASICOPERATOR 100
#	else
#		define USE_HMLIB_BASICOPERATOR 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_BASICOPERATOR
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_BASICOPERATOR 999
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_BASICOPERATOR 100
#	else
#		define CHG_HMLIB_BASICOPERATOR 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_BASICOPERATOR
#	define INC_USE_HMLIB_BASICOPERATOR
#
#	if USE_HMLIB_BASICOPERATOR>100
#		error hmLib : [basic_operator.hpp] 廃止バージョンです。v1_00より後(v3_00_00以降)は、使用できません。後継にoperator_pattern.hppが存在します。
#	elif USE_HMLIB_BASICOPERATOR==100
#		include "hmLib_v2/basic_operator/basic_operator_v1_00.hpp"
#	else
#		error hmLib : [basic_operator.hpp] 非対応バージョンです。v1_00(v2_09_05)より前は、使用できません。
#	endif
#
#	if 1<=OUT_USE_HMLIB
#		pragma message("hmLib : [basic_operator.hpp] v1_01")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_BASICOPERATOR
#	define INC_CHG_HMLIB_BASICOPERATOR
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_BASICOPERATOR
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_BASICOPERATOR
#			pragma message("chg_hmLib : # error : [basic_operator.hpp] 廃止バージョンです。v1_00より後(v3_00_00以降)は、使用できません。後継にoperator_pattern.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BASICOPERATOR<100
#			pragma message("chg_hmLib : # error : [basic_operator.hpp] 非対応バージョンです。v1_00(v2_09_05)より前は、使用できません。")
#		endif
#	endif
#	
#	endif
#endif
#
#endif
