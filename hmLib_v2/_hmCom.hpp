/*
hmCom_v1_00.hpp
	通信処理用の抽象クラス
		各種Gateを使って、制御可能
			無限バッファ付hmInfBufComGate
			有限バッファ付hmBufComGate
		スレッド対応
	boost/thread依存ファイル		
*/
#ifndef VER_HMCOM
#	define VER_HMCOM 100
#endif
#ifndef INC_HMLIB
#	include "hmLib.h"
#endif
#
#ifndef USE_HMCOM
#	if USE_HMLIB>=30000
#		define USE_HMCOM 100
#	else
#		define USE_HMCOM 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMCOM
#	if CHG_HMLIB>=30000
#		define CHG_HMCOM 100
#	else
#		define CHG_HMCOM 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMCOM
#	define INC_USE_HMCOM
#
#	if USE_HMCOM>100
#		error hmLib # error : [hmCom.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMCOM==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmCom.hpp] v1_00")
#		endif
#		include "hmCom/hmCom_v1_00.hpp"
#	else
#		error hmLib # error : [hmCom.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMCOM
#		pragma message("hmLib # info  : [hmCom.hpp] boost依存ファイルです。boost/threadを使用しています。")
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMCOM
#	define INC_CHG_HMCOM
#
#	/*自身との互換性*/
#	if 100<=USE_HMCOM
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMCOM
#			pragma message("chg_hmLib # error : [hmCom.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMCOM<100
#			pragma message("chg_hmLib # error : [hmCom.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmGateとの互換性について*/
#	if 100<=CHG_HMCOM
#		ifndef CHG_HMGATE
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib # info  : [hmCom.hpp] hmGate.hppに依存しています。CHG_HMLIB用にhmGate.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmGate.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmGate.hpp"
#			endif
#		endif
#	endif
#	/* circularとの互換性について*/
#	if 100<=CHG_HMCOM
#		ifndef CHG_CIRCULAR
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib # info  : [hmCom.hpp] circular.hppに依存しています。CHG_HMLIB用にcircular.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "circular.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "circular.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
