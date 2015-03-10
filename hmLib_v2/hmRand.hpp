/*
hmRand廃止
	多くの機能をstd::randomに移管。デフォルトで初期化されるrandom_engineをrandom.hppに後継として作成。
hmRand_v2_04
	randNormの引数名をvarからsdへと変更
hmRand_v2_03
	hmRandを関数オブジェクトとして使用可能に変更
	hm::getGlobalRand追加
hmRand_v2_02
	hmRand::gen_randNormを追加。正規乱数生成を倍速化。
hmRand_v2_01
	hm::rand(int min,int max)で正常に表示されないバグを修正
hmRand_v2_00
	乱数生成関数を追加
		最小、最大を指定できる乱数
		32bit,64bit両者対応の生の乱数
		最小、最大を指定できる浮動乱数
		正規乱数
	メンバ関数から正規乱数を削除
	hm::randオブジェクトを削除
*/
#ifndef VER_HMLIB_HMRAND
#	define VER_HMLIB_HMRAND 999
#endif
#
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMRAND
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMRAND 999
#	elif USE_HMLIB>=20905
#		define USE_HMLIB_HMRAND 204
#	elif USE_HMLIB>=20902
#		define USE_HMLIB_HMRAND 203
#	elif USE_HMLIB>=20807
#		define USE_HMLIB_HMRAND 202
#	elif USE_HMLIB>=20301
#		define USE_HMLIB_HMRAND 201
#	elif USE_HMLIB>=20300
#		define USE_HMLIB_HMRAND 200
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMRAND 100
#	else
#		define USE_HMLIB_HMRAND 0
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_HMRAND
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMRAND 999
#	elif CHG_HMLIB>=20905
#		define CHG_HMLIB_HMRAND 204
#	elif CHG_HMLIB>=20902
#		define CHG_HMLIB_HMRAND 203
#	elif CHG_HMLIB>=20807
#		define CHG_HMLIB_HMRAND 202
#	elif CHG_HMLIB>=20301
#		define CHG_HMLIB_HMRAND 201
#	elif CHG_HMLIB>=20300
#		define CHG_HMLIB_HMRAND 200
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMRAND 100
#	else
#		define CHG_HMLIB_HMRAND 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMRAND
#	define INC_USE_HMLIB_HMRAND
#
#	if USE_HMLIB_HMRAND>204
#		error hmLib : [hmRand.hpp] 廃止バージョンです。v2_04より後(v3_00_00以降)は、使用できません。後継にrandom.hppが存在します。
#	elif USE_HMLIB_HMRAND==204
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v2_04")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v2_04.h"
#	elif USE_HMLIB_HMRAND==203
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v2_03")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v2_03.h"
#	elif USE_HMLIB_HMRAND==202
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v2_02")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v2_02.h"
#	elif USE_HMLIB_HMRAND==201
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v2_01")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v2_01.h"
#	elif USE_HMLIB_HMRAND==200
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v2_00")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v2_00.h"
#	elif USE_HMLIB_HMRAND==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmRand.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmRand/hmRand_v1_00.h"
#	else
#		error hmLib : [hmRand.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 200<=USE_HMLIB_HMRAND && USE_HMLIB_HMRAND<204
#		pragma message("hmLib : #	info  : [hmRand.hpp] hm::randNormの引数名が正しい意味を示していません。v2_04(v2_09_05)以上への対応を推奨します。")
#	endif
#	if 3<=OUT_USE_HMLIB && 200<=USE_HMLIB_HMRAND && USE_HMLIB_HMRAND<202
#		pragma message("hmLib : #	info  : [hmRand.hpp] 正規分布乱数の生成が倍速化可能です。v2_02(v2_08_07)以上への対応を推奨します。")
#	endif
#	if 2<=OUT_USE_HMLIB && 200<=USE_HMLIB_HMRAND && USE_HMLIB_HMRAND<201
#		pragma message("hmLib : #warning: [hmRand.hpp] hm::randにバグが存在します。v2_00(v2_03_01)以上への対応を推奨します。")
#	endif
#	
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMRAND
#	define INC_CHG_HMLIB_HMRAND
#
#	if 100<=USE_HMLIB_HMRAND
#		if 1<=OUT_CHG_HMLIB && 204<CHG_HMLIB_HMRAND
#			pragma message("chg_hmLib : #	error : [hmRand.hpp] 廃止バージョンです。v2_04より後(v3_00_00以降)は、使用できません。後継にrandom.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRAND<203 && 203<=USE_HMLIB_HMRAND
#			pragma message("chg_hmLib : # error : [hmRand.hpp] 前方互換性は保証されていません。v2_03(v2_09_02)より前は、乱数の関数オブジェクトとしては使えません。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRAND<201 && 201<=USE_HMLIB_HMRAND
#			pragma message("chg_hmLib : # error : [hmRand.hpp] 前方互換性は保証されていません。v2_01(v2_03_01)より前は、hm::randにバグが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRAND<200 && 200<=USE_HMLIB_HMRAND
#			pragma message("chg_hmLib : # error : [hmRand.hpp] 前方互換性は保証されていません。v2_00(v2_03_00)より前は、関数の変更があります。")
#		elif 1<=OUT_CHG_HMLIB && USE_HMLIB_HMRAND<200 && 200<=CHG_HMLIB_HMRAND
#			pragma message("chg_hmLib : # error : [hmRand.hpp] 後方互換性は保証されていません。v2_00(v2_03_00)以降は、関数の変更があります。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMRAND<200
#			pragma message("chg_hmLib : #	error : [hmRand.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	if 100<=CHG_HMLIB_HMRAND && CHG_HMLIB_HMRAND<=204
#		ifndef CHG_HMLIB_HMBASIC
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmRand.hpp] hmBasic.hppに依存しています。CHG_HMLIB用にhmBasic.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmBasic.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmBasic.hpp"
#			endif
#		endif
#	endif
#
#	endif
#endif
