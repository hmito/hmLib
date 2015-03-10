/*
hmRSGate.hpp 廃止
	後継にcomgate.hpps
hmRSGate_v1_02.hpp
	ucharが未定義だった問題を修正
hmRSGate_v1_01.hpp
	namespace hmstdに加入
*/
#ifndef VER_HMLIB_HMRSGATE
#	define VER_HMLIB_HMRSGATE 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMRSGATE
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMRSGATE 999
#	elif USE_HMLIB>=20701
#		define USE_HMLIB_HMRSGATE 102
#	elif USE_HMLIB>=20001
#		define USE_HMLIB_HMRSGATE 101
#	else
#		define USE_HMLIB_HMRSGATE 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMRSGATE
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMRSGATE 999
#	elif CHG_HMLIB>=20701
#		define CHG_HMLIB_HMRSGATE 102
#	elif CHG_HMLIB>=20001
#		define CHG_HMLIB_HMRSGATE 101
#	else
#		define CHG_HMLIB_HMRSGATE 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMRSGATE
#	define INC_USE_HMLIB_HMRSGATE
#
#	if USE_HMLIB_HMRSGATE>102
#		error hmLib : # error : [hmRSGate.hpp] 廃止バージョンです。v3_00_00以降は、使用できません。後継にcomgate.hppがあります。
#	elif USE_HMLIB_HMRSGATE==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRSGate.hpp] v1_02")
#		endif
#		include "hmLib_v2/hmRSGate/hmRSGate_v1_02.h"
#	elif USE_HMLIB_HMRSGATE==101
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRSGate.hpp] v1_01")
#		endif
#		include "hmLib_v2/hmRSGate/hmRSGate_v1_01.hpp"
#	else
#		error hmLib : # error : [hmRSGate.hpp] 非対応バージョンです。v1_01(v2_00_01)より前は、使用できません。
#	endif
#
#
#	if 2<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMRSGATE && USE_HMLIB_HMRSGATE<102
#		pragma message("hmLib : #warning: [hmRSGate.hpp] v3_00_00以降は、廃止されます。後継にcomgate.hppがあります。")
#	endif
#
#	if 1<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMRSGATE && USE_HMLIB_HMRSGATE<101
#		pragma message("hmLib : # error : [hmRSGate.hpp] 致命的なバグが存在します。v1_02(v2_07_01)以上への対応を推奨します。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMRSGATE
#	define INC_CHG_HMLIB_HMRSGATE
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMRSGATE
#		if 1<=OUT_CHG_HMLIB && 103<CHG_HMLIB_HMRSGATE
#			pragma message("chg_hmLib : # error : [hmRSGate.hpp] 廃止バージョンです。v3_00_00以降は、使用できません。後継にcomgate.hppがあります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && USE_HMLIB_HMRSGATE<103 && 103<=CHG_HMLIB_HMRSGATE
#			pragma message("chg_hmLib : # error : [hmRSGate.hpp] 後方互換性は保証されていません。v1_03(v3_00_00)以降は、名前空間、クラス名に変更があります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_HMRSGATE<103 && 103<=CHG_HMLIB_HMRSGATE
#			pragma message("chg_hmLib : # error : [hmRSGate.hpp] 前方互換性は保証されていません。v1_03(v3_00_00)より前は、名前空間、クラス名に変更があります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRSGATE<102 && 102<=USE_HMLIB_HMRSGATE
#			pragma message("chg_hmLib : # error : [hmRSGate.hpp] 前方互換性は保証されていません。v1_02(v2_07_01)より前は、致命的なバグが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRSGATE<101
#			pragma message("chg_hmLib : # error : [hmRSGate.hpp] 非対応バージョンです。v1_01(v2_00_01)より前は、使用できません。")
#		endif
#	endif
#	/* gatestreamとの互換性について*/
#	if 100<=CHG_HMLIB_HMRSGATE && CHG_HMLIB_HMRSGATE<=102
#		ifndef CHG_HMLIB_GATESTREAM
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmRSGate.hpp] gatestream.hppに依存しています。CHG_HMLIB用にgatestream.hppをincludeします。")
#			endif
#			ifdef CAN_CHG_HMLIB
#				undef CAN_CHG_HMLIB
#				include "hmLib_v2/gatestream.hpp"
#				define CAN_CHG_HMLIB
#			else
#				include "hmLib_v2/gatestream.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
