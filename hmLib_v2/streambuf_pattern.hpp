/*
streambuf_pattern_v1_00.hpp
	お手軽streambuf作成クラス
	basic_buf_streambuf : バッファ付ストリームバッファ基底クラス
	basic_nonbuf_streambuf : バッファ無ストリームバッファ基底クラス
*/
#ifndef VER_HMLIB_STREAMBUFPATTERN
#	define VER_HMLIB_STREAMBUFPATTERN 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_STREAMBUFPATTERN
#	if USE_HMLIB>=30000
#		define USE_HMLIB_STREAMBUFPATTERN 100
#	else
#		define USE_HMLIB_STREAMBUFPATTERN 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_STREAMBUFPATTERN
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_STREAMBUFPATTERN 100
#	else
#		define CHG_HMLIB_STREAMBUFPATTERN 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_STREAMBUFPATTERN
#	define INC_USE_HMLIB_STREAMBUFPATTERN
#
#	if USE_HMLIB_STREAMBUFPATTERN>100
#		error hmLib : # error : [streambuf_pattern.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_STREAMBUFPATTERN==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [streambuf_pattern.hpp] v1_00")
#		endif
#		include "hmLib_v2/streambuf_pattern/streambuf_pattern_v1_00.hpp"
#	else
#		error hmLib : # error : [streambuf_pattern.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_STREAMBUFPATTERN
#	define INC_CHG_HMLIB_STREAMBUFPATTERN
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_STREAMBUFPATTERN
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_STREAMBUFPATTERN
#			pragma message("chg_hmLib : # error : [streambuf_pattern.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_STREAMBUFPATTERN<100
#			pragma message("chg_hmLib : # error : [streambuf_pattern.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	endif
#endif
