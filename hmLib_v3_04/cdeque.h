#ifndef HMLIB_CDEQUE_INC
#define HMLIB_CDEQUE_INC 103
#
/*
===hmCLib/cdeque===
キュー機能の提供

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
		既存の配列を使ってcdequeのバッファを作成/解放
	dynamic_construct/destruct追加
		mallocを使ってcdequeのバッファを作成/解放
	create/release関数がdynamic_construct/destructに依存するよう変更
		create/release関数は、cdeque自信のmallocについて携わる。
*/
//#undef __cplusplus
#ifndef HMLIB_BYTEBASETYPE_INC
#	include <hmLib_v3_04/bytebase_type.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/cdeque.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/cdeque.lib")
#	endif
#endif
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
//typedef hmLib_boolian	hmLib_boolian;
typedef void*			cdeque_iterator;
typedef hmLib_uint16	cdeque_size_t;
typedef struct{
	hmLib_boolian DynamicFlag;
	cdeque_size_t ElemSize;
	void* Begin;
	void* End;
	void* BufBegin;
	void* BufEnd;
}cdeque;
//メモリの初期化
void cdeque_format(cdeque* ptr);
//すでに初期化されたかどうかの確認
hmLib_boolian cdeque_is_construct(cdeque* ptr);
//静的確保によってバッファ確保
void cdeque_construct_static(cdeque* pDque,void* Begin,void* End,cdeque_size_t ElemSize);
//動的確保によってバッファ確保
void cdeque_construct_dynamic(cdeque* pDque,cdeque_size_t Size,cdeque_size_t ElemSize);
//バッファ解放 静的/動的は自動判断
void cdeque_destruct(cdeque* pDque);
//cdequeを作成(construct_dynamicを使用)
cdeque* cdeque_create(cdeque_size_t Size,cdeque_size_t ElemSize);
//cdequeを破棄
void cdeque_release(cdeque* pDque);
//move
void cdeque_move(cdeque* from, cdeque* to);
//copy
void cdeque_swap(cdeque* ptr1,cdeque* ptr2);
//バッファの先頭を取得
void* cdeque_frontptr(cdeque* pDque);
//バッファの先頭を取得
void* cdeque_backptr(cdeque* pDque);
//バッファにデータを挿入
hmLib_boolian cdeque_push_front(cdeque* pDque,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cdeque_pop_front(cdeque* pDque);
//バッファにデータを挿入
hmLib_boolian cdeque_push_back(cdeque* pDque,const void* Ptr);
//バッファのデータを削除
hmLib_boolian cdeque_pop_back(cdeque* pDque);
//バッファに入っているサイズ
cdeque_size_t cdeque_size(cdeque* pDque);
//バッファに残り入れられるサイズ
cdeque_size_t cdeque_rest(cdeque* pDque);
//バッファが空かどうか確認
hmLib_boolian cdeque_empty(cdeque* pDque);
//バッファがいっぱいかどうか確認
hmLib_boolian cdeque_full(cdeque* pDque);
//バッファのbegin関数
cdeque_iterator cdeque_begin(cdeque* pDque);
//バッファのend関数
cdeque_iterator cdeque_end(cdeque* pDque);
//cdeque_iteratorの次のiteratorを与える
cdeque_iterator cdeque_next(cdeque* pDque,cdeque_iterator itr);
//cdeque_iteratorの前のiteratorを与える
cdeque_iterator cdeque_prev(cdeque* pDque,cdeque_iterator itr);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
