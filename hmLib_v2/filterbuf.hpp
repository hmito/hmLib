/*
filterbuf_v1_00.hpp
*/
#ifndef VER_HMLIB_FILTERBUF
#	define VER_HMLIB_FILTERBUF 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_FILTERBUF
#	if USE_HMLIB>=30000
#		define USE_HMLIB_FILTERBUF 100
#	else
#		define USE_HMLIB_FILTERBUF 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_FILTERBUF
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_FILTERBUF 100
#	else
#		define CHG_HMLIB_FILTERBUF 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_FILTERBUF
#	define INC_USE_HMLIB_FILTERBUF
#
#	if USE_HMLIB_FILTERBUF>100
#		error hmLib : # error : [filterbuf.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_FILTERBUF==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [filterbuf.hpp] v1_00")
#		endif
#		include "hmLib_v2/filterbuf/filterbuf_v1_00.hpp"
#	else
#		error hmLib : # error : [filterbuf.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_FILTERBUF && USE_HMLIB_FILTERBUF<101
#//		pragma message("hmLib : # error : [filterbuf.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_FILTERBUF
#	define INC_CHG_HMLIB_FILTERBUF
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_FILTERBUF
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_FILTERBUF
#			pragma message("chg_hmLib : # error : [filterbuf.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_FILTERBUF<100 && 100<=USE_HMLIB_FILTERBUF
#//			pragma message("chg_hmLib : # error : [filterbuf.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_FILTERBUF<100 && 100<=CHG_HMLIB_FILTERBUF
#//			pragma message("chg_hmLib : # error : [filterbuf.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_FILTERBUF<100
#			pragma message("chg_hmLib : # error : [filterbuf.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	endif
#endif
