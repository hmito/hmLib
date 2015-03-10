/*
hmBasic.hpp廃止 v3_00_00
	基本となるファイルを統合してしまうと、一部の変更がライブラリ全体に及ぼす影響が大きいため
	提供されていた大部分の機能は、複数の後継ファイルへ継承される
		hmStr 文字列関連
		hmAlgorithm 数値処理関連
		hmType 型関連
		hmPrcTime 時間関連
		hmBit bit処理関連
hmBasic_v1_07
	system関数を追加
hmBasic_v1_06
	modの致命的なバグを修正
hmBasic_v1_05
	str,c_strをhmに追加
hmBasic_v1_04
	mod追加
hmBasic_v1_03
	char* getTmpStrAdress()をhmに移動
	int getTmpStrAdressSize()を追加
hmBasic_v1_02
	bitxorの追加
hmBasic_v1_01
	min3値,max3値,med3の追加
*/
#ifndef VER_HMLIB_HMBASIC
#	define VER_HMLIB_HMBASIC 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMBAISC
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMBASIC 999
#	elif USE_HMLIB>=20903
#		define USE_HMLIB_HMBASIC 107
#	elif USE_HMLIB>=20807
#		define USE_HMLIB_HMBASIC 106
#	elif USE_HMLIB>=20805
#		define USE_HMLIB_HMBASIC 105
#	elif USE_HMLIB>=20702
#		define USE_HMLIB_HMBASIC 104
#	elif USE_HMLIB>=20502
#		define USE_HMLIB_HMBASIC 103
#	elif USE_HMLIB>=20500
#		define USE_HMLIB_HMBASIC 102
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMBASIC 100
#	else
#		define USE_HMLIB_HMBASIC 0
#	endif
#endif
#
#if defined CHG_HMLIB &&  !defined CHG_HMLIB_HMBAISC
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMBASIC 999
#	elif CHG_HMLIB>=20903
#		define CHG_HMLIB_HMBASIC 107
#	elif CHG_HMLIB>=20807
#		define CHG_HMLIB_HMBASIC 106
#	elif CHG_HMLIB>=20805
#		define CHG_HMLIB_HMBASIC 105
#	elif CHG_HMLIB>=20702
#		define CHG_HMLIB_HMBASIC 104
#	elif CHG_HMLIB>=20502
#		define CHG_HMLIB_HMBASIC 103
#	elif CHG_HMLIB>=20500
#		define CHG_HMLIB_HMBASIC 102
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMBASIC 100
#	else
#		define CHG_HMLIB_HMBASIC 0
#	endif
#endif
#
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMBASIC
#	define iNC_USE_HMLIB_HMBASIC
#
#	if USE_HMLIB_HMBASIC>107
#		error hmLib : [hmBasic.hpp] 廃止バージョンです。v1_07より後(v3_00_00以降)は使用できません。移行先の各ヘッダファイルを使用してください。
#	elif USE_HMLIB_HMBASIC==107
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_07")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_07.h"
#	elif USE_HMLIB_HMBASIC==106
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_06")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_06.h"
#	elif USE_HMLIB_HMBASIC==105
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_05")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_05.h"
#	elif USE_HMLIB_HMBASIC==104
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_04")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_04.h"
#	elif USE_HMLIB_HMBASIC==103
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_03")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_03.h"
#	elif USE_HMLIB_HMBASIC==102
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_02")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_02.h"
#	elif USE_HMLIB_HMBASIC==100
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmBasic.hpp] v1_00")
#		endif
#		include "hmLib_v2/hmBasic/hmBasic_v1_00.h"
#	else
#		error hmLib : [hmBasic.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB && 100<=USE_HMLIB_HMBASIC
#		pragma message("hmLib : # info  : [hmBasic.hpp] v3_00_00以上では、hmBasic.hppは複数のファイルに解体されます。")
#	endif
#	if 2<=OUT_USE_HMLIB && 104<=USE_HMLIB_HMBASIC && USE_HMLIB_HMBASIC<106
#		pragma message("hmLib : #warning: [hmBasic.hpp] hmstd::modに致命的なバグが存在します。v1_06(v2_08_07)以上への更新を推奨します。")
#	endif
#
#	endif
#endif
#
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMBASIC
#	define INC_CHG_HMLIB_HMBASIC
#
#	/*自身との互換性*/
#	if 100<=USE_HMLIB_HMBASIC
#		if 1<=OUT_CHG_HMLIB && 107<CHG_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 廃止バージョンです。v1_07より後(v3_00_00以降)は使用できません。移行先の各ヘッダファイルを使用してください。")
#		endif
#		if 1<=OUT_CHG_HMLIB && 104<=CHG_HMLIB_HMBASIC && CHG_HMLIB_HMBASIC<107 && 107<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 前方互換性は保証されていません。v1_07(v2_09_03)以上でしかsystem関数は使用できません。")
#		elif 104<=USE_HMLIB_HMBASIC && USE_HMLIB_HMBASIC<107 && 107<=CHG_HMLIB_HMBASIC
#		endif
#		if 1<=OUT_CHG_HMLIB && 104<=CHG_HMLIB_HMBASIC && CHG_HMLIB_HMBASIC<106 && 106<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 前方互換性は保証されていません。hmstd::modに致命的なバグが存在します。v1_06(v2_08_07)以上の利用を推奨します。")
#		elif 104<=USE_HMLIB_HMBASIC && USE_HMLIB_HMBASIC<106 && 106<=CHG_HMLIB_HMBASIC
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBASIC<105 && 105<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 前方互換性は保証されていません。v1_05(v2_08_05)より前では、str,c_strが使用不能になります。")
#		elif USE_HMLIB_HMBASIC<105 && 105<=CHG_HMLIB_HMBASIC
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBASIC<104 && 104<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 前方互換性は保証されていません。v1_04(v2_07_02)より前では、modが使用不能になります。")
#		elif USE_HMLIB_HMBASIC<104 && 104<=CHG_HMLIB_HMBASIC
#		endif
#		if 2<=OUT_CHG_HMLIB && CHG_HMLIB_HMBASIC<103 && 103<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : #warning: [hmBasic.hpp] 一部名前空間における仕様変更が存在します。")
#		elif 2<=OUT_CHG_HMLIB && USE_HMLIB_HMBASIC<103 && 103<=CHG_HMLIB_HMBASIC
#			pragma message("chg_hmLib : #warning: [hmBasic.hpp] 一部名前空間における仕様変更が存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBASIC<102 && 102<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 前方互換性は保証されていません。v1_02(v2_05_00)より前では、bit関連の関数が使用不能になります。")
#		elif USE_HMLIB_HMBASIC<102 && 102<=CHG_HMLIB_HMBASIC
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMBASIC<100 && 100<=USE_HMLIB_HMBASIC
#			pragma message("chg_hmLib : # error : [hmBasic.hpp] 非対応バージョンです。v1_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#
#	endif
#endif
