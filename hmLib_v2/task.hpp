/*
task_v1_00.hpp
	TaskSystemを再現するクラス task
		複層タスク
			createChild/releaseChild関数で、子タスクの制御可能
		優先度付実行機能
			Priorityを設定することで、優先実行順位を決定可能
		条件削除機能
			Priorityの幅と、登録したTask自体の2方法で削除可能
		自己終了機能
			自身の終了もexit関数で親タスクへ通告可能
		姉妹タスク作成
			自身の姉妹タスクとなるタスク作成を、親タスクに要求可能
		スリープ機能
			sleep関数で、指定回数だけ関数の実行をスキップ
*/
#ifndef VER_HMLIB_TASK
#	define VER_HMLIB_TASK 100
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_TASK
#	if USE_HMLIB>=30000
#		define USE_HMLIB_TASK 100
#	else
#		define USE_HMLIB_TASK 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_TASK
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_TASK 100
#	else
#		define CHG_HMLIB_TASK 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_TASK
#	define INC_USE_HMLIB_TASK
#
#	if USE_HMLIB_TASK>100
#		error hmLib : # error : [task.hpp] 非対応バージョンです。v1_00より後は、使用できません。
#	elif USE_HMLIB_TASK==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [task.hpp] v1_00")
#		endif
#		include "hmLib_v2/task/task_v1_00.hpp"
#	else
#		error hmLib : # error : [task.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_TASK
#	define INC_CHG_HMLIB_TASK
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_TASK
#		if 1<=OUT_CHG_HMLIB && 100<CHG_HMLIB_TASK
#			pragma message("chg_hmLib : # error : [task.hpp] 非対応バージョンです。v1_00より後は、使用できません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_TASK<100
#			pragma message("chg_hmLib : # error : [task.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* iterator_patternとの互換性について*/
#	if 100<=CHG_HMLIB_TASK
#		ifndef CHG_HMLIB_ITERATORPATTERN
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [task.hpp] iterator_pattern.hppに依存しています。CHG_HMLIB用にiterator_pattern.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/iterator_pattern.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/iterator_pattern.hpp"
#			endif
#		endif
#
#		if 100<=CHG_HMLIB_TASK
#			pragma message("chg_hmLib : # error : [task.hpp] iterator_pattern v1_01(v3_00_00)より前とは、互換性がありません。")
#		endif
#		
#	endif
#
#	endif
#endif
