#ifndef HMLIB_CSTACK_INC
#define HMLIB_CSTACK_INC 103
#
/*
===hmCLib/cstack===
スタック機能を提供

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
#	include <hmLib_v3_04/cdeque.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/cstack.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/cstack.lib")
#	endif
#endif
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
typedef cdeque_iterator cstack_iterator;
typedef cdeque_size_t	cstack_size_t;
typedef cdeque cstack;
//メモリの初期化
void cstack_format(cstack* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cstack_is_construct(cstack* ptr);
//静的確保によってバッファ確保
void cstack_construct_static(cstack* pStk,void* Begin,void* End,cstack_size_t ElemSize);
//動的確保によってバッファ確保
void cstack_construct_dynamic(cstack* pStk,cstack_size_t Size,cstack_size_t ElemSize);
//バッファ解放 静的/動的は自動判断
void cstack_destruct(cstack* pStk);
//cstackを作成(construct_dynamicを使用)
cstack* cstack_create(cstack_size_t Size,cstack_size_t ElemSize);
//cstackを破棄
void cstack_release(cstack* pStk);
//move
void cstack_move(cstack* from, cstack* to);
//copy
void cstack_swap(cstack* ptr1,cstack* ptr2);
//バッファの先頭を取得
void* cstack_getptr(cstack* pStk);
//バッファに入っているサイズ
cstack_size_t cstack_size(cstack* pStk);
//バッファに残り入れられるサイズ
cstack_size_t cstack_rest(cstack* pStk);
//バッファが空かどうか確認
hmLib_boolian cstack_empty(cstack* pStk);
//バッファがいっぱいかどうか確認
hmLib_boolian cstack_full(cstack* pStk);
//バッファにデータを挿入
hmLib_boolian cstack_push(cstack* pStk,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cstack_pop(cstack* pStk);
//バッファのbegin関数
cstack_iterator cstack_begin(cstack* pStk);
//バッファのend関数
cstack_iterator cstack_end(cstack* pStk);
//cstack_iteratorの次のiteratorを与える
cstack_iterator cstack_next(cstack* pStk,cstack_iterator itr);
//cstack_iteratorの前のiteratorを与える
cstack_iterator cstack_prev(cstack* pStk,cstack_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
