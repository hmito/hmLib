/*
circular_v1_00.hpp
	循環バッファ用コンテナ
		bicircular
			前後から挿入、削除ができるタイプ
		circular
			後込め、先出し式循環バッファ
		circular_stack
			先込め、先出し式循環バッファ
*/
#ifndef VER_HMLIB_CIRCULAR
#	define VER_HMLIB_CIRCULAR 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_CIRCULAR
#	if USE_HMLIB>=30000
#		define USE_HMLIB_CIRCULAR 100
#	else
#		define USE_HMLIB_CIRCULAR 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_CIRCULAR
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_CIRCULAR 100
#	else
#		define CHG_HMLIB_CIRCULAR 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_CIRCULAR
#	define INC_USE_HMLIB_CIRCULAR
#
#	if USE_HMLIB_CIRCULAR>100
#		error hmLib : # error : [circular.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_CIRCULAR==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [circular.hpp] v1_00")
#		endif
#		include "hmLib_v2/circular/circular_v1_00.hpp"
#	else
#		error hmLib : # error : [circular.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_CIRCULAR
#	define INC_CHG_HMLIB_CIRCULAR
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_CIRCULAR
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_CIRCULAR
#			pragma message("chg_hmLib : # error : [circular.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_CIRCULAR<100
#			pragma message("chg_hmLib : # error : [circular.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* iterator_patternとの互換性について*/
#	if 100<=CHG_HMLIB_CIRCULAR
#		ifndef CHG_HMLIB_ITERATORPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [circular.hpp] iterator_pattern.hppに依存しています。CHG_HMLIB用にiterator_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/iterator_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/iterator_pattern.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
