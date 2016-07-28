#ifndef HMLIB_CDEQUE_INC
#define HMLIB_CDEQUE_INC 200
#
/*
===hmCLib/hmLib_cdeque===
キュー機能の提供
cdeque:v2_00/140214 hmIto
	破壊的変更
	constructをstatic/dynamicの差を廃止。malloc/freeで管理するconstructとポインタを与えるplacement_constructに変更
	型名にhmLib_Prefixを追加。ただし、HMLIB_CDEQUE_NOPREFIXでプレフィックス無しバージョンも使える。
cdeque:v1_03/130204
	一部関数の戻り値の型名のバグを修正
		hmLib_boolをhmLib_boolianに訂正
	swap関数のバグを修正
		実体に->アクセス演算子を使っていた
cdeque_v1_02/121128 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
cdeque_v1_01/121009 hmIto
	static_construct/destruct追加
		既存の配列を使ってhmLib_cdequeのバッファを作成/解放
	dynamic_construct/destruct追加
		mallocを使ってhmLib_cdequeのバッファを作成/解放
	create/release関数がdynamic_construct/destructに依存するよう変更
		create/release関数は、hmLib_cdeque自信のmallocについて携わる。
*/
//#undef __cplusplus
#ifndef HMLIB_BYTEBASETYPE_INC
#	include <hmLib/type.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#if !(defined(HMLIB_NOLIB) || defined(HMLIB_CDEQUE_NOLIB))
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib/lib/Debug/cdeque.lib")
#	else
#		pragma comment(lib,"hmLib/lib/Release/cdeque.lib")
#	endif
#endif
typedef void*			hmLib_cdeque_iterator;
typedef hmLib_uint16	hmLib_cdeque_size_t;
typedef struct{
	hmLib_vFp_p Destructor;
	hmLib_cdeque_size_t ElemSize;
	void* Begin;
	void* End;
	void* BufBegin;
	void* BufEnd;
}hmLib_cdeque;
#ifdef __cplusplus
namespace hmLib{
extern "C"{
typedef hmLib_cdeque hmLib_cdeque;
typedef hmLib_cdeque_size_t hmLib_cdeque_size_t;
typedef hmLib_cdeque_iterator hmLib_cdeque_iterator;
#endif
//typedef hmLib_boolian	hmLib_boolian;

//メモリの初期化
void cdeque_format(hmLib_cdeque* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cdeque_is_construct(hmLib_cdeque* ptr);
//動的確保によってバッファ確保
void cdeque_construct(hmLib_cdeque* pDque, hmLib_cdeque_size_t Size, hmLib_cdeque_size_t ElemSize);
//静的確保によってバッファ確保
void cdeque_placement_construct(hmLib_cdeque* pDque, hmLib_cdeque_size_t Size, hmLib_cdeque_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor);
//バッファ解放 静的/動的は自動判断
void cdeque_destruct(hmLib_cdeque* pDque);
//move
void cdeque_move(hmLib_cdeque* from, hmLib_cdeque* to);
//copy
void cdeque_swap(hmLib_cdeque* ptr1,hmLib_cdeque* ptr2);
//バッファの先頭を取得
void* cdeque_frontptr(hmLib_cdeque* pDque);
//バッファの先頭を取得
void* cdeque_backptr(hmLib_cdeque* pDque);
//バッファにデータを挿入
hmLib_boolian cdeque_push_front(hmLib_cdeque* pDque,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cdeque_pop_front(hmLib_cdeque* pDque);
//バッファにデータを挿入
hmLib_boolian cdeque_push_back(hmLib_cdeque* pDque,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cdeque_pop_back(hmLib_cdeque* pDque);
//バッファに入っているサイズ
hmLib_cdeque_size_t cdeque_size(hmLib_cdeque* pDque);
//バッファに残り入れられるサイズ
hmLib_cdeque_size_t cdeque_rest(hmLib_cdeque* pDque);
//バッファが空かどうか確認
hmLib_boolian cdeque_empty(hmLib_cdeque* pDque);
//バッファがいっぱいかどうか確認
hmLib_boolian cdeque_full(hmLib_cdeque* pDque);
//バッファのbegin関数
hmLib_cdeque_iterator cdeque_begin(hmLib_cdeque* pDque);
//バッファのend関数
hmLib_cdeque_iterator cdeque_end(hmLib_cdeque* pDque);
//hmLib_cdeque_iteratorの次のiteratorを与える
hmLib_cdeque_iterator cdeque_next(hmLib_cdeque* pDque,hmLib_cdeque_iterator itr);
//hmLib_cdeque_iteratorの前のiteratorを与える
hmLib_cdeque_iterator cdeque_prev(hmLib_cdeque* pDque,hmLib_cdeque_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#if defined(HMLIB_CDEQUE_NOPREFIX) || defined(HMLIB_NOPREFIX)
typedef hmLib_cdeque cdeque;
typedef hmLib_cdeque_size_t cdeque_size_t;
typedef hmLib_cdeque_iterator cdeque_iterator;
#endif
#
#endif
