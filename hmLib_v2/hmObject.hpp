/*
hmObject_v1_00.hpp
*/
#ifndef VER_HMLIB_HMOBJECT
#	define VER_HMLIB_HMOBJECT 999
#endif
#ifndef INC_HMLIB
#	include "hmLib_v2/hmLib.h"
#endif
#
#ifndef USE_HMLIB_HMOBJECT
#	if USE_HMLIB>=30000
#		define USE_HMLIB_HMOBJECT 999
#	elif USE_HMLIB>=20000
#		define USE_HMLIB_HMOBJECT 200
#	else
#		define USE_HMLIB_HMOBJECT 0
#	endif
#endif
#if defined CHG_HMLIB && !defined CHG_HMLIB_HMOBJECT
#	if CHG_HMLIB>=30000
#		define CHG_HMLIB_HMOBJECT 999
#	elif CHG_HMLIB>=20000
#		define CHG_HMLIB_HMOBJECT 200
#	else
#		define CHG_HMLIB_HMOBJECT 0
#	endif
#endif
#ifdef CAN_USE_HMLIB
#	ifndef INC_USE_HMLIB_HMOBJECT
#	define INC_USE_HMLIB_HMOBJECT
#
#	if USE_HMLIB_HMOBJECT>200
#		error hmLib : # error : [hmObject.hpp] 廃止バージョンです。v2_00より後(v3_00_00以降)は、使用できません。後継に、body.hppが存在します。
#	elif USE_HMLIB_HMOBJECT==200
#		if 1<=OUT_USE_HMLIB
#			pragma message("hmLib : [hmObject.hpp] v2_00")
#		endif
#		include "hmLib_v2/hmObject/hmObject_v2_00.hpp"
#	else
#		error hmLib : # error : [hmObject.hpp] 非対応バージョンです。v1_00(v3_00_00)より前は、使用できません。
#	endif
#
#	if 3<=OUT_USE_HMLIB
#		pragma message("hmLib : # info  : [hmObject.hpp] v2_00より後(v3_00_00以降)は、廃止されます。後継に、hmBody.hppが存在します。
#	endif
#
#	endif
#endif
#ifdef CAN_CHG_HMLIB
#	ifndef INC_CHG_HMLIB_HMOBJECT
#	define INC_CHG_HMLIB_HMOBJECT
#
#	/*自身との互換性*/
#	if 200<=USE_HMLIB_HMOBJECT
#		if 1<=OUT_CHG_HMLIB && 200<CHG_HMLIB_HMOBJECT
#			pragma message("chg_hmLib : # error : [hmObject.hpp] 廃止バージョンです。v2_00より後(v3_00_00以降)は、使用できません。後継に、body.hppが存在します。")
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMOBJECT<200
#			pragma message("chg_hmLib : # error : [hmObject.hpp] 非対応バージョンです。v2_00(v2_00_00)より前は、使用できません。")
#		endif
#	endif
#	/* hmRectとの互換性について*/
#	if 200<=CHG_HMLIB_HMOBJECT
#		ifndef CHG_HMLIB_HMRECT
#			if 3<=OUT_CHG_HMLIB
#				pragma message("chg_hmLib : # info  : [hmObject.hpp] hmRect.hppに依存しています。CHG_HMLIB用にhmRect.hppをincludeします。")
#			endif
#			ifdef CAN_USE_HMLIB
#				undef CAN_USE_HMLIB
#				include "hmLib_v2/hmRect.hpp"
#				define CAN_USE_HMLIB
#			else
#				include "hmLib_v2/hmRect.hpp"
#			endif
#		endif
#		if 1<=OUT_CHG_HMLIB && CHG_HMLIB_HMOBJECT<=200 && 102<=CHG_HMLIB_HMRECT
#			pragma message("chg_hmLib : # error : [hmObject.hpp] hmRect.hpp v1_02以降とは、互換性がありません。")
#		endif
#
#	endif
#
#	endif
#endif
