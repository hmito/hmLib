#ifndef HMLIB_CSTRING_INC
#define HMLIB_CSTRING_INC 201
/*
===cstring===
文字列を扱うstringクラスの疑似構造体

cstring:v2_01/140319 hmIto
	コンストラクタ時の安全性チェックを追加
		指定サイズが0の時、mallocを呼ばないように修正
		渡されたポインタがnullptrの時無視するように修正
		mallocが0を返した時に、formatして返すように修正
cstring:v2_00/131217 hmIto
	破壊的変更
		constructをstatic/dynamicの差を廃止。malloc/freeで管理するconstructとポインタを与えるplacement_constructに変更
		型名にhmLib_Prefixを追加。ただし、HMLIB_CSTRING_NOPREFIXでプレフィックス無しバージョンも使える。
cstring:v1_03/130204 hmIto
	一部関数の戻り値の型名のバグを修正
		hmLib_boolをhmLib_boolian二訂正
	swap関数のバグを修正
		実体に->アクセス演算子を使っていた
cstring:v1_02/130105 hmIto
	format関数を追加
		変数を初期化できる destructで必要な処理を行わない
	is_construct関数を追加
		constructされているかどうかを判定できる
		formatされていることが条件
	move関数を追加
		一方のポインタからもう一方のポインタにオブジェクト自体を移動する
	swap関数を追加
		二つのポインタの中身を入れ替える
cstring:v1_01/121128 hmIto
	next/prev関数を追加
	C++に対応
	dynamic/staticをフラグで覚えておくように変更
		destructはフラグに基づいて判断
cstring:v1_00/121013 hmIto
	cstringを作成
*/
#ifndef HMLIB_TYPE_INC
#	include <hmLib/type.h>
#endif
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib/lib/Debug/cstring.lib")
#	else
#		pragma comment(lib,"hmLib/lib/Release/cstring.lib")
#	endif
#endif
typedef hmLib_uint16 hmLib_cstring_size_t;
typedef unsigned char* hmLib_cstring_iterator;
typedef struct {
	hmLib_vFp_p Destructor;
	unsigned char* Begin;
	unsigned char* End;
}hmLib_cstring;
#ifdef __cplusplus
namespace hmLib{
extern "C"{
typedef hmLib_cstring cstring;
typedef hmLib_cstring_size_t cstring_size_t;
typedef hmLib_cstring_iterator cstring_iterator;
#endif

void cstring_format(hmLib_cstring* str);
void cstring_construct(hmLib_cstring* str, hmLib_cstring_size_t size);
void cstring_placement_construct(hmLib_cstring* str, hmLib_cstring_size_t size, hmLib_pointer ptr, hmLib_vFp_p destructor);
hmLib_boolian cstring_is_construct(hmLib_cstring* str);
void cstring_destruct(hmLib_cstring* str);
void cstring_move(hmLib_cstring* from, hmLib_cstring* to);
void cstring_swap(hmLib_cstring* str1, hmLib_cstring* str2);
unsigned char* cstring_ptr(const hmLib_cstring* str);
hmLib_cstring_size_t cstring_size(const hmLib_cstring* str);
hmLib_cstring_iterator cstring_begin(const hmLib_cstring* str);
hmLib_cstring_iterator cstring_end(const hmLib_cstring* str);
hmLib_cstring_iterator cstring_next(const hmLib_cstring* str, hmLib_cstring_iterator itr);
hmLib_cstring_iterator cstring_prev(const hmLib_cstring* str, hmLib_cstring_iterator itr);
void cstring_putc(hmLib_cstring* str, hmLib_cstring_size_t pos, unsigned char c);
void cstring_puts(hmLib_cstring* str, hmLib_cstring_size_t pos, const unsigned char* p, hmLib_cstring_size_t size);
unsigned char cstring_getc(hmLib_cstring* str, hmLib_cstring_size_t pos);
unsigned char* cstring_gets(hmLib_cstring* str, hmLib_cstring_size_t pos);
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#ifdef HMLIB_CSTRING_NOPREFIX
typedef hmLib_cstring cstring;
typedef hmLib_cstring_size_t cstring_size_t;
typedef hmLib_cstring_iterator cstring_iterator;
#endif
#
#endif
