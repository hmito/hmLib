/*
hmBufGate_v1_03
	他のGateのバッファとして機能するGate
		有限バッファクラス BufGate
		無限バッファクラス InfBufGate
hmBufGate_v1_02
	ibufcltrgateを内部クラスに追加
hmBufGate_v1_01
	getc()実行時にデータがなければ、データが来るまで待機するように変更
	bufのsize取得系関数の追加
*/
#ifndef VER_HMLIB_HMBUFGATE
#	define VER_HMLIB_HMBUFGATE 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMBUFGATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMBUFGATE 999
#	elif USE_HMLIB>=20806
#		define USE_HMLIB_HMBUFGATE 102
#	elif USE_HMLIB>=20003
#		define USE_HMLIB_HMBUFGATE 101
#	elif USE_HMLIB>=20001
#		define USE_HMLIB_HMBUFGATE 100
#	else
#		define USE_HMLIB_HMBUFGATE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMBUFGATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMBUFGATE 999
#	elif CHG_HMLIB>=20806
#		define CHG_HMLIB_HMBUFGATE 102
#	elif CHG_HMLIB>=20003
#		define CHG_HMLIB_HMBUFGATE 101
#	elif CHG_HMLIB>=20001
#		define CHG_HMLIB_HMBUFGATE 100
#	else
#		define CHG_HMLIB_HMBUFGATE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMBUFGATE
#	define INC_USE_HMLIB_HMBUFGATE
#
#	if USE_HMLIB_HMBUFGATE>102
#		error hmLib # error : [hmBufGate.hpp] 廃止バージョンです。v1_02より後(v3_00_00以降)は、使用できません。
#	elif USE_HMLIB_HMBUFGATE==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBufGate.hpp] v1_02")
#		endif
#		include "hmLib_v2/hmBufGate/hmBufGate_v1_02.hpp"
#	elif USE_HMLIB_HMBUFGATE==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBufGate.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmBufGate/hmBufGate_v1_01.hpp"
#	elif USE_HMLIB_HMBUFGATE==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBufGate.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmBufGate/hmBufGate_v1_00.hpp"
#	else
#		error hmLib # error : [hmBufGate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMBUFGATE
#	define INC_CHG_HMLIB_HMBUFGATE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMBUFGATE
#		if 1<=OUT_CHG_HMLIB && 102<CHG_HMLIB_HMBUFGATE
#			pragma message("chg_hmLib # error : [hmBufGate.hpp] 廃止バージョンです。v1_02より後(v3_00_00以降)は、使用できません。")
#		endif
#		if 2<=OUT_CHG_HMLIB && CHG_HMLIB_HMBUFGATE<102 && 102<=USE_HMLIB_HMBUFGATE
#			pragma message("chg_hmLib #warning: [hmBufGate.hpp] 前方互換性は保証されていません。v1_02より前は、ibufctlrgateが使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBUFGATE<101 && 101<=USE_HMLIB_HMBUFGATE
#			pragma message("chg_hmLib # error : [hmBufGate.hpp] 前方互換性は保証されていません。v1_01より前は、クラス名が変更されています。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_HMBUFGATE<101 && 101<=CHG_HMLIB_HMBUFGATE
#			pragma message("chg_hmLib # error : [hmBufGate.hpp] 後方互換性は保証されていません。v1_01以降は、クラス名が変更されています。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBUFGATE<100
#			pragma message("chg_hmLib # error : [hmBufGate.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* gatestreamとの互換性について*/
#	if 100<=CHG_HMLIB_HMBUFGATE && CHG_HMLIB_HMBUFGATE<=102
#		ifndef CHG_HMLIB_GATESTREAM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib # info  : [hmBufGate.hpp] gatestream.hppに依存しています。CHG_HMLIB用にgatestream.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/gatestream.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/gatestream.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
