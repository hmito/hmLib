﻿#ifndef HMLIB_CONFIGVC_INC
#define HMLIB_CONFIGVC_INC 101
#
/*===config_vc===
VisualStudio C/C++の設定用マクロ

config_vc_v1_01/121204 hmIto
	拡張子をhppからhに変更
	インクルードガードマクロの不要なアンダーバーを消去
*/
#ifdef _MSC_VER
#	//windows.hのmin,maxを使わせない
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	//古いunsafeな関数群使用による警告回避
#	define _CRT_SECURE_NO_WARNINGS
#	define _AFX_SECURE_NO_WARNINGS
#	define _ATL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#	define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#	define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#endif
#
#endif
