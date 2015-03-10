/*
===gate===
入出力管理用クラス
	プログラム外部との通信の出入り口を管理する
	gatestreamを使うことで、streamクラス化可能
////////////////
gate_v1_00.hpp
	gatestreamの後継ファイル
	itf_gateのbasic_nonbuf_streambuf準拠
		gateさえあれば、簡単にstream化可能
*/
#ifndef VER_HMLIB_GATE
#	define VER_HMLIB_GATE 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_GATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_GATE 100
#	else
#		define USE_HMLIB_GATE 0
#	endif
#endif
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_GATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_GATE 100
#	else
#		define CHG_HMLIB_GATE 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_GATE
#	define INC_USE_HMLIB_GATE
#
#	if USE_HMLIB_GATE>100
#		error hmLib : # error : [gate.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_GATE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [gate.hpp] v1_00")
#		endif
#		include "hmLib_v2/gate/gate_v1_00.hpp"
#	else
#		error hmLib : # error : [gate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_GATE
#	define INC_CHG_HMLIB_GATE
#
#	/*自身との互換性について*/
#	if 100<=USE_HMLIB_GATE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_GATE
#			pragma message("chg_hmLib : # error : [gate.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_GATE<100
#			pragma message("chg_hmLib : # error : [gate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	/* streambuf_patternとの互換性について*/
#	if 100<=CHG_HMLIB_GATE
#		ifndef CHG_HMLIB_STREAMBUFPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [gate.hpp] streambuf_pattern.hppに依存しています。CHG_HMLIB用にstreambuf_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/streambuf_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/streambuf_pattern.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
