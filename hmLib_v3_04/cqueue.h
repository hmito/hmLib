#ifndef HMLIB_CQUEUE_INC
#define HMLIB_CQUEUE_INC 103
#
/*
===hmCLib/cqueue===
リスト機能を提供

cqueue:v1_03/130204 hmIto
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
#	include <hmLib_v3_04/cdeque.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/cqueue.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/cqueue.lib")
#	endif
#endif
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
//typedef cdeque_boolian	hmLib_boolian;
typedef cdeque_iterator cqueue_iterator;
typedef cdeque_size_t	cqueue_size_t;
typedef cdeque cqueue;
//メモリの初期化
void cqueue_format(cqueue* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cqueue_is_construct(cqueue* ptr);
//静的確保によってバッファ確保
void cqueue_construct_static(cqueue* pQue,void* Begin,void* End,cqueue_size_t ElemSize);
//動的確保によってバッファ確保
void cqueue_construct_dynamic(cqueue* pQue,cqueue_size_t Size,cqueue_size_t ElemSize);
//バッファ解放 静的/動的は自動判断
void cqueue_destruct(cqueue* pQue);
//cqueueを作成(construct_dynamicを使用)
cqueue* cqueue_create(cqueue_size_t Size,cqueue_size_t ElemSize);
//cqueueを破棄
void cqueue_release(cqueue* pQue);
//move
void cqueue_move(cqueue* from, cqueue* to);
//copy
void cqueue_swap(cqueue* ptr1,cqueue* ptr2);
//バッファの先頭を取得
void* cqueue_getptr(cqueue* pQue);
//バッファに入っているサイズ
cqueue_size_t cqueue_size(cqueue* pQue);
//バッファに残り入れられるサイズ
cqueue_size_t cqueue_rest(cqueue* pQue);
//バッファが空かどうか確認
hmLib_boolian cqueue_empty(cqueue* pQue);
//バッファがいっぱいかどうか確認
hmLib_boolian cqueue_full(cqueue* pQue);
//バッファにデータを挿入
hmLib_boolian cqueue_push(cqueue* pQue,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cqueue_pop(cqueue* pQue);
//バッファのbegin関数
cqueue_iterator cqueue_begin(cqueue* pQue);
//バッファのend関数
cqueue_iterator cqueue_end(cqueue* pQue);
//cqueue_iteratorの次のiteratorを与える
cqueue_iterator cqueue_next(cqueue* pQue,cqueue_iterator itr);
//cqueue_iteratorの前のiteratorを与える
cqueue_iterator cqueue_prev(cqueue* pQue,cqueue_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
