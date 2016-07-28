#ifndef HMLIB_CSTACK_C_INC
#define HMLIB_CSTACK_C_INC 200
#
#ifndef HMLIB_CSTACK_INC
#	include <hmLib/cstack.h>
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
void cstack_format(hmLib_cstack* ptr){
	cdeque_format(ptr);
}
//すでに初期化されたかどうかの確認
hmLib_boolian cstack_is_construct(hmLib_cstack* ptr){
	return cdeque_is_construct(ptr);
}
//動的確保によってバッファ確保
void cstack_construct(hmLib_cstack* pStk, hmLib_cstack_size_t Size, hmLib_cstack_size_t ElemSize) {
	cdeque_construct(pStk, Size, ElemSize);
}
//静的確保によってバッファ確保
void cstack_placement_construct(hmLib_cstack* pStk, hmLib_cstack_size_t Size, hmLib_cstack_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor) {
	cdeque_placement_construct(pStk, Size, ElemSize, Ptr, Destructor);
}
//バッファ解放 静的/動的は自動判断
void cstack_destruct(hmLib_cstack* pStk){
	cdeque_destruct(pStk);
}
//move
void cstack_move(hmLib_cstack* from, hmLib_cstack* to){
	cdeque_move(from,to);
}
//copy
void cstack_swap(hmLib_cstack* ptr1,hmLib_cstack* ptr2){
	cdeque_swap(ptr1,ptr2);
}
//バッファの先頭を取得
void* cstack_getptr(hmLib_cstack* pStk){
	return cdeque_backptr(pStk);
}
//バッファに入っているサイズ
hmLib_cstack_size_t cstack_size(hmLib_cstack* pStk){
	return cdeque_size(pStk);
}
//バッファに残り入れられるサイズ
hmLib_cstack_size_t cstack_rest(hmLib_cstack* pStk){
	return cdeque_rest(pStk);
}
//バッファが空かどうか確認
hmLib_boolian cstack_empty(hmLib_cstack* pStk){
	return cdeque_empty(pStk);
}
//バッファがいっぱいかどうか確認
hmLib_boolian cstack_full(hmLib_cstack* pStk){
	return cdeque_full(pStk);
}
//バッファにデータを挿入
hmLib_boolian cstack_push(hmLib_cstack* pStk,const void* Ptr){
	return cdeque_push_back(pStk,Ptr);
}
//バッファのデータを削除
hmLib_boolian cstack_pop(hmLib_cstack* pStk){
	return cdeque_pop_back(pStk);
}
//バッファのbegin関数
hmLib_cstack_iterator cstack_begin(hmLib_cstack* pStk){
	return cdeque_begin(pStk);
}
//バッファのend関数
hmLib_cstack_iterator cstack_end(hmLib_cstack* pStk){
	return cdeque_end(pStk);
}
//hmLib_cstack_iteratorの次のiteratorを与える
hmLib_cstack_iterator cstack_next(hmLib_cstack* pStk,hmLib_cstack_iterator itr){
	return cdeque_next(pStk,itr);
}
//hmLib_cstack_iteratorの前のiteratorを与える
hmLib_cstack_iterator cstack_prev(hmLib_cstack* pStk,hmLib_cstack_iterator itr){
	return cdeque_prev(pStk,itr);
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
