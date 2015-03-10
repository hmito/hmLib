/*
bytebase_type_v1_00.hpp
	hmBasic後継ファイルの一つ
	環境非依存のタイプが使用可能
		プレフィックス
			s_ signed
			u_ unsigned
			f_ float
		typename
			byte 8bit
			short 16bit
			int 32bit
			long 64bit
			llong 128bit
			float 32bit
			double 64bit
			ldouble 128bit
	PIの宣言
	使用には、環境の型サイズを記録したbytebase_typeSize.hppが必要
		make_bytebase_typeSize.c(exeファイル有)の実行で自動生成可能
*/

#ifndef VER_HMLIB_BYTEBASETYPE
#	define VER_HMLIB_BYTEBASETYPE 100
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_BYTEBASETYPE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_BYTEBASETYPE 100
#	else
#		define USE_HMLIB_BYTEBASETYPE 0 
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_BYTEBASETYPE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_BYTEBASETYPE 100
#	else
#		define CHG_HMLIB_BYTEBASETYPE 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_BYTEBASETYPE
#	define INC_USE_HMLIB_BYTEBASETYPE
#
#	if USE_HMLIB_BYTEBASETYPE>100
#		error hmLib : [bytebase_type.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_BYTEBASETYPE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [bytebase_type.hpp] v1_00")
#		endif
#		include "hmLib_v2/bytebase_type/bytebase_type_v1_00.hpp"
#	else
#		error hmLib : [bytebase_type.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_BYTEBASETYPE
#	define INC_CHG_HMLIB_BYTEBASETYPE
#
#	if 100<=USE_HMLIB_BYTEBASETYPE
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_BYTEBASETYPE
#			pragma message("chg_hmLib : # error : [bytebase_type.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_BYTEBASETYPE<100
#			pragma message("chg_hmLib : # error : [bytebase_type.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	endif
#endif
