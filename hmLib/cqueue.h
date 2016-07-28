#ifndef HMLIB_CQUEUE_INC
#define HMLIB_CQUEUE_INC 200
#
/*
===hmLib/cqueue===
スタック機能を提供
cqueue:v2_00/140214 hmIto
	破壊的変更
	constructをstatic/dynamicの差を廃止。malloc/freeで管理するconstructとポインタを与えるplacement_constructに変更
	型名にhmLib_Prefixを追加。ただし、HMLIB_CDEQUE_NOPREFIXでプレフィックス無しバージョンも使える。
cqueue:v1_03/130204
	一部関数の戻り値の型名のバグを修正
		hmLib_boolをhmLib_boolianに訂正
cqueue:v1_02/130105 hmIto
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
#if !(defined(HMLIB_NOLIB) || defined(HMLIB_CQUEUE_NOLIB))
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib/lib/Debug/cqueue.lib")
#	else
#		pragma comment(lib,"hmLib/lib/Release/cqueue.lib")
#	endif
#endif
typedef hmLib_cdeque_iterator hmLib_cqueue_iterator;
typedef hmLib_cdeque_size_t	hmLib_cqueue_size_t;
typedef hmLib_cdeque hmLib_cqueue;
#ifdef __cplusplus
namespace hmLib{
	typedef hmLib_cqueue_iterator cqueue_iterator;
	typedef hmLib_cqueue_size_t cqueue_size_t;
	typedef hmLib_cqueue cqueue;
extern "C"{
#endif
//メモリの初期化
void cqueue_format(hmLib_cqueue* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cqueue_is_construct(hmLib_cqueue* ptr);
//動的確保によってバッファ確保
void cqueue_construct(hmLib_cqueue* pStk, hmLib_cqueue_size_t Size, hmLib_cqueue_size_t ElemSize);
//静的確保によってバッファ確保
void cqueue_placement_construct(hmLib_cqueue* pStk, hmLib_cqueue_size_t Size, hmLib_cqueue_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor);
//バッファ解放 静的/動的は自動判断
void cqueue_destruct(hmLib_cqueue* pStk);
//move
void cqueue_move(hmLib_cqueue* from, hmLib_cqueue* to);
//copy
void cqueue_swap(hmLib_cqueue* ptr1,hmLib_cqueue* ptr2);
//バッファの先頭を取得
void* cqueue_getptr(hmLib_cqueue* pStk);
//バッファに入っているサイズ
hmLib_cqueue_size_t cqueue_size(hmLib_cqueue* pStk);
//バッファに残り入れられるサイズ
hmLib_cqueue_size_t cqueue_rest(hmLib_cqueue* pStk);
//バッファが空かどうか確認
hmLib_boolian cqueue_empty(hmLib_cqueue* pStk);
//バッファがいっぱいかどうか確認
hmLib_boolian cqueue_full(hmLib_cqueue* pStk);
//バッファにデータを挿入
hmLib_boolian cqueue_push(hmLib_cqueue* pStk,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cqueue_pop(hmLib_cqueue* pStk);
//バッファのbegin関数
hmLib_cqueue_iterator cqueue_begin(hmLib_cqueue* pStk);
//バッファのend関数
hmLib_cqueue_iterator cqueue_end(hmLib_cqueue* pStk);
//hmLib_cqueue_iteratorの次のiteratorを与える
hmLib_cqueue_iterator cqueue_next(hmLib_cqueue* pStk,hmLib_cqueue_iterator itr);
//hmLib_cqueue_iteratorの前のiteratorを与える
hmLib_cqueue_iterator cqueue_prev(hmLib_cqueue* pStk,hmLib_cqueue_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#if defined(HMLIB_CQUEUE_NOPREFIX) || defined(HMLIB_NOPREFIX)
typedef hmLib_cqueue cqueue;
typedef hmLib_cqueue_size_t cqueue_size_t;
typedef hmLib_cqueue_iterator cqueue_iterator;
#endif
#
#endif
