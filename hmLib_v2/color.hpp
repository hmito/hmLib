/*
color_v1_00.hpp
	色関係制御用クラス
		class rgb_color;	//RGB型カラー	r:赤強度 g:緑強度 b:青強度 を指定
		class hsv_color;	//HSV型カラー	hue:色相 sat:彩度 vlu:強度 を指定
		class hls_color;	//HLS型カラー	hue:色相 lgt:彩度 sat:明度 を指定
		class rgba_color;	//RGB型カラー	r:赤強度 g:緑強度 b:青強度 a:透過度 を指定
		class hsva_color;	//HSV型カラー	hue:色相 sat:彩度 vlu:強度 a:透過度 を指定
		class hlsa_color;	//HLS型カラー	hue:色相 lgt:彩度 sat:明度 a:透過度 を指定
*/
#ifndef VER_HMLIB_COLOR
#	define VER_HMLIB_COLOR 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_COLOR
#	if USE_HMLIB>=30000
#		define USE_HMLIB_COLOR 100
#	else
#		define USE_HMLIB_COLOR 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_COLOR
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_COLOR 100
#	else
#		define CHG_HMLIB_COLOR 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_COLOR
#	define INC_USE_HMLIB_COLOR
#
#	if USE_HMLIB_COLOR>100
#		error hmLib : # error : [color.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_COLOR==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [color.hpp] v1_00")
#		endif
#		include "hmLib_v2/color/color_v1_00.hpp"
#	else
#		error hmLib : # error : [color.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#//	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_COLOR && USE_HMLIB_COLOR<101
#//		pragma message("hmLib : # error : [color.hpp] ...。v1_01(v3_01_00)以上への対応を推奨します。")
#//	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_COLOR
#	define INC_CHG_HMLIB_COLOR
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_COLOR
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_COLOR
#			pragma message("chg_hmLib : # error : [color.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#//		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_COLOR<100 && 100<=USE_HMLIB_COLOR
#//			pragma message("chg_hmLib : # error : [color.hpp] 前方互換性は保証されていません。v1_00(v3_00_00)より前は、...")
#//		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_COLOR<100 && 100<=CHG_HMLIB_COLOR
#//			pragma message("chg_hmLib : # error : [color.hpp] 後方互換性は保証されていません。v1_00(v3_00_00)以降は、...")
#//		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_COLOR<100
#			pragma message("chg_hmLib : # error : [color.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* bytebase_typeとの互換性について*/
#	if 100<=CHG_HMLIB_COLOR
#		ifndef INC_CHG_HMLIB_BYTEBASETYPE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [color.hpp] bytebase_type.hppに依存しています。CHG_HMLIB用にbytebase_type.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/bytebase_type.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/bytebase_type.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_COLOR && CHG_HMLIB_BYTEBASETYPE<100
#//			pragma message("chg_hmLib : # error : [color.hpp] bytebase_type.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_COLOR<100 && 100<=CHG_HMLIB_BYTEBASETYPE
#//			pragma message("chg_hmLib : # error : [color.hpp] bytebase_type.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_BYTEBASETYPE<100 || CHG_HMLIB_BYTEBASETYPE==999)
#			pragma message("chg_hmLib : # error : [color.hpp] 使用可能なbytebase_type.hppが存在しません。")
#		endif
#	endif
#	/* operator_patternとの互換性について*/
#	if 100<=CHG_HMLIB_COLOR
#		ifndef INC_CHG_HMLIB_OPERATORPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [color.hpp] operator_pattern.hppに依存しています。CHG_HMLIB用にoperator_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/operator_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/operator_pattern.hpp"
#			endif
#		endif
#//		if 1<=OUT_CHG_HMLIB && 100<=CHG_HMLIB_COLOR && CHG_HMLIB_OPERATORPATTERN<100
#//			pragma message("chg_hmLib : # error : [color.hpp] operator_pattern.hpp v1_00(v3_00_00)より前とは、互換性がありません。")
#//		elif 1<=OUT_CHG_HMLIB && CHG_HMLIB_COLOR<100 && 100<=CHG_HMLIB_OPERATORPATTERN
#//			pragma message("chg_hmLib : # error : [color.hpp] operator_pattern.hpp v1_00(v3_00_00)以降とは、互換性がありません。")
#//		endif
#		if 1<=OUT_CHG_HMLIB && ( CHG_HMLIB_OPERATORPATTERN<100 || CHG_HMLIB_OPERATORPATTERN==999)
#			pragma message("chg_hmLib : # error : [color.hpp] 使用可能なoperator_pattern.hppが存在しません。")
#		endif
#	endif
#
#	endif
#endif
