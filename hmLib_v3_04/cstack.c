#ifndef HMLIB_CSTACK_C_INC
#define HMLIB_CSTACK_C_INC 103
#
#ifndef HMLIB_CSTACK_INC
#	include <hmLib_v3_04/cstack.h>
#endif
#ifdef __cplusplus
extern "C"{
#endif
#include <stdlib.h>
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
//メモリの初期化
void cstack_format(cstack* ptr){
	cdeque_format(ptr);
}
//すでに初期化されたかどうかの確認
hmLib_boolian cstack_is_construct(cstack* ptr){
	return cdeque_is_construct(ptr);
}
//静的確保によってバッファ確保
void cstack_construct_static(cstack* pStk,void* Begin,void* End,cstack_size_t ElemSize){
	cdeque_construct_static(pStk,Begin,End,ElemSize);
}
//動的確保によってバッファ確保
void cstack_construct_dynamic(cstack* pStk,cstack_size_t Size,cstack_size_t ElemSize){
	cdeque_construct_dynamic(pStk,Size,ElemSize);
}
//バッファ解放 静的/動的は自動判断
void cstack_destruct(cstack* pStk){
	cdeque_destruct(pStk);
}
//cstackを作成(construct_dynamicを使用)
cstack* cstack_create(cstack_size_t Size,cstack_size_t ElemSize){
	return cdeque_create(Size,ElemSize);
}
//cstackを破棄
void cstack_release(cstack* pStk){
	cdeque_release(pStk);
}
//move
void cstack_move(cstack* from, cstack* to){
	cdeque_move(from,to);
}
//copy
void cstack_swap(cstack* ptr1,cstack* ptr2){
	cdeque_swap(ptr1,ptr2);
}
//バッファの先頭を取得
void* cstack_getptr(cstack* pStk){
	return cdeque_backptr(pStk);
}
//バッファに入っているサイズ
cstack_size_t cstack_size(cstack* pStk){
	return cdeque_size(pStk);
}
//バッファに残り入れられるサイズ
cstack_size_t cstack_rest(cstack* pStk){
	return cdeque_rest(pStk);
}
//バッファが空かどうか確認
hmLib_boolian cstack_empty(cstack* pStk){
	return cdeque_empty(pStk);
}
//バッファがいっぱいかどうか確認
hmLib_boolian cstack_full(cstack* pStk){
	return cdeque_full(pStk);
}
//バッファにデータを挿入
hmLib_boolian cstack_push(cstack* pStk,const void* Ptr){
	return cdeque_push_back(pStk,Ptr);
}
//バッファのデータを削除
hmLib_boolian cstack_pop(cstack* pStk){
	return cdeque_pop_back(pStk);
}
//バッファのbegin関数
cstack_iterator cstack_begin(cstack* pStk){
	return cdeque_begin(pStk);
}
//バッファのend関数
cstack_iterator cstack_end(cstack* pStk){
	return cdeque_end(pStk);
}
//cstack_iteratorの次のiteratorを与える
cstack_iterator cstack_next(cstack* pStk,cstack_iterator itr){
	return cdeque_next(pStk,itr);
}
//cstack_iteratorの前のiteratorを与える
cstack_iterator cstack_prev(cstack* pStk,cstack_iterator itr){
	return cdeque_prev(pStk,itr);
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
