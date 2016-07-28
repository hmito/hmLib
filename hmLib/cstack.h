#ifndef HMLIB_CSTACK_INC
#define HMLIB_CSTACK_INC 200
#
/*
===hmLib/cstack===
スタック機能を提供
cstack:v2_00/140214 hmIto
	破壊的変更
	constructをstatic/dynamicの差を廃止。malloc/freeで管理するconstructとポインタを与えるplacement_constructに変更
	型名にhmLib_Prefixを追加。ただし、HMLIB_CDEQUE_NOPREFIXでプレフィックス無しバージョンも使える。
cstack:v1_03/130204
	一部関数の戻り値の型名のバグを修正
		hmLib_boolをhmLib_boolianに訂正
cstack:v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
*/
//#undef __cplusplus
#ifndef HMLIB_CDEQUE_INC
#	include <hmLib/cdeque.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#if !(defined(HMLIB_NOLIB) || defined(HMLIB_CSTACK_NOLIB))
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib/lib/Debug/cstack.lib")
#	else
#		pragma comment(lib,"hmLib/lib/Release/cstack.lib")
#	endif
#endif
typedef hmLib_cdeque_iterator hmLib_cstack_iterator;
typedef hmLib_cdeque_size_t	hmLib_cstack_size_t;
typedef hmLib_cdeque hmLib_cstack;
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
//メモリの初期化
void cstack_format(hmLib_cstack* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cstack_is_construct(hmLib_cstack* ptr);
//動的確保によってバッファ確保
void cstack_construct(hmLib_cstack* pStk, hmLib_cstack_size_t Size, hmLib_cstack_size_t ElemSize);
//静的確保によってバッファ確保
void cstack_placement_construct(hmLib_cstack* pStk, hmLib_cstack_size_t Size, hmLib_cstack_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor);
//バッファ解放 静的/動的は自動判断
void cstack_destruct(hmLib_cstack* pStk);
//move
void cstack_move(hmLib_cstack* from, hmLib_cstack* to);
//copy
void cstack_swap(hmLib_cstack* ptr1,hmLib_cstack* ptr2);
//バッファの先頭を取得
void* cstack_getptr(hmLib_cstack* pStk);
//バッファに入っているサイズ
hmLib_cstack_size_t cstack_size(hmLib_cstack* pStk);
//バッファに残り入れられるサイズ
hmLib_cstack_size_t cstack_rest(hmLib_cstack* pStk);
//バッファが空かどうか確認
hmLib_boolian cstack_empty(hmLib_cstack* pStk);
//バッファがいっぱいかどうか確認
hmLib_boolian cstack_full(hmLib_cstack* pStk);
//バッファにデータを挿入
hmLib_boolian cstack_push(hmLib_cstack* pStk,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cstack_pop(hmLib_cstack* pStk);
//バッファのbegin関数
hmLib_cstack_iterator cstack_begin(hmLib_cstack* pStk);
//バッファのend関数
hmLib_cstack_iterator cstack_end(hmLib_cstack* pStk);
//hmLib_cstack_iteratorの次のiteratorを与える
hmLib_cstack_iterator cstack_next(hmLib_cstack* pStk,hmLib_cstack_iterator itr);
//hmLib_cstack_iteratorの前のiteratorを与える
hmLib_cstack_iterator cstack_prev(hmLib_cstack* pStk,hmLib_cstack_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#if defined(HMLIB_CSTACK_NOPREFIX) || defined(HMLIB_NOPREFIX)
typedef hmLib_cstack cstack;
typedef hmLib_cstack_size_t cstack_size_t;
typedef hmLib_cstack_iterator cstack_iterator;
#endif
#
#endif
