#ifndef HMLIB_CQUEUE_C_INC
#define HMLIB_CQUEUE_C_INC 200
#
#ifndef HMLIB_CQUEUE_INC
#	include <hmLib/cqueue.h>
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
void cqueue_format(hmLib_cqueue* ptr){
	cdeque_format(ptr);
}
//すでに初期化されたかどうかの確認
hmLib_boolian cqueue_is_construct(hmLib_cqueue* ptr){
	return cdeque_is_construct(ptr);
}
//動的確保によってバッファ確保
void cqueue_construct(hmLib_cqueue* pStk, hmLib_cqueue_size_t Size, hmLib_cqueue_size_t ElemSize) {
	cdeque_construct(pStk, Size, ElemSize);
}
//静的確保によってバッファ確保
void cqueue_placement_construct(hmLib_cqueue* pStk, hmLib_cqueue_size_t Size, hmLib_cqueue_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor) {
	cdeque_placement_construct(pStk, Size, ElemSize, Ptr, Destructor);
}
//バッファ解放 静的/動的は自動判断
void cqueue_destruct(hmLib_cqueue* pStk){
	cdeque_destruct(pStk);
}
//move
void cqueue_move(hmLib_cqueue* from, hmLib_cqueue* to){
	cdeque_move(from,to);
}
//copy
void cqueue_swap(hmLib_cqueue* ptr1,hmLib_cqueue* ptr2){
	cdeque_swap(ptr1,ptr2);
}
//バッファの先頭を取得
void* cqueue_getptr(hmLib_cqueue* pStk){
	return cdeque_frontptr(pStk);
}
//バッファに入っているサイズ
hmLib_cqueue_size_t cqueue_size(hmLib_cqueue* pStk){
	return cdeque_size(pStk);
}
//バッファに残り入れられるサイズ
hmLib_cqueue_size_t cqueue_rest(hmLib_cqueue* pStk){
	return cdeque_rest(pStk);
}
//バッファが空かどうか確認
hmLib_boolian cqueue_empty(hmLib_cqueue* pStk){
	return cdeque_empty(pStk);
}
//バッファがいっぱいかどうか確認
hmLib_boolian cqueue_full(hmLib_cqueue* pStk){
	return cdeque_full(pStk);
}
//バッファにデータを挿入
hmLib_boolian cqueue_push(hmLib_cqueue* pStk,const void* Ptr){
	return cdeque_push_back(pStk,Ptr);
}
//バッファのデータを削除
hmLib_boolian cqueue_pop(hmLib_cqueue* pStk){
	return cdeque_pop_front(pStk);
}
//バッファのbegin関数
hmLib_cqueue_iterator cqueue_begin(hmLib_cqueue* pStk){
	return cdeque_begin(pStk);
}
//バッファのend関数
hmLib_cqueue_iterator cqueue_end(hmLib_cqueue* pStk){
	return cdeque_end(pStk);
}
//hmLib_cqueue_iteratorの次のiteratorを与える
hmLib_cqueue_iterator cqueue_next(hmLib_cqueue* pStk,hmLib_cqueue_iterator itr){
	return cdeque_next(pStk,itr);
}
//hmLib_cqueue_iteratorの前のiteratorを与える
hmLib_cqueue_iterator cqueue_prev(hmLib_cqueue* pStk,hmLib_cqueue_iterator itr){
	return cdeque_prev(pStk,itr);
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
