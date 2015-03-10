#ifndef HMLIB_CQUEUE_C_INC
#define HMLIB_CQUEUE_C_INC 103
#
#ifndef HMLIB_CQUEUE_INC
#	include <hmLib_v3_04/cqueue.h>
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
void cqueue_format(cqueue* ptr){
	cdeque_format(ptr);
}
//すでに初期化されたかどうかの確認
hmLib_boolian cqueue_is_construct(cqueue* ptr){
	return cdeque_is_construct(ptr);
}
//静的確保によってバッファ確保
void cqueue_construct_static(cqueue* pQue,void* Begin,void* End,cqueue_size_t ElemSize){
	cdeque_construct_static(pQue,Begin,End,ElemSize);
}
//動的確保によってバッファ確保
void cqueue_construct_dynamic(cqueue* pQue,cqueue_size_t Size,cqueue_size_t ElemSize){
	cdeque_construct_dynamic(pQue,Size,ElemSize);
}
//バッファ解放 静的/動的は自動判断
void cqueue_destruct(cqueue* pQue){
	cdeque_destruct(pQue);
}
//cqueueを作成(construct_dynamicを使用)
cqueue* cqueue_create(cqueue_size_t Size,cqueue_size_t ElemSize){
	return cdeque_create(Size,ElemSize);
}
//cqueueを破棄
void cqueue_release(cqueue* pQue){
	cdeque_release(pQue);
}
//move
void cqueue_move(cqueue* from, cqueue* to){
	cdeque_move(from,to);
}
//copy
void cqueue_swap(cqueue* ptr1,cqueue* ptr2){
	cdeque_swap(ptr1,ptr2);
}
//バッファの先頭を取得
void* cqueue_getptr(cqueue* pQue){
	return cdeque_frontptr(pQue);
}
//バッファに入っているサイズ
cqueue_size_t cqueue_size(cqueue* pQue){
	return cdeque_size(pQue);
}
//バッファに残り入れられるサイズ
cqueue_size_t cqueue_rest(cqueue* pQue){
	return cdeque_rest(pQue);
}
//バッファが空かどうか確認
hmLib_boolian cqueue_empty(cqueue* pQue){
	return cdeque_empty(pQue);
}
//バッファがいっぱいかどうか確認
hmLib_boolian cqueue_full(cqueue* pQue){
	return cdeque_full(pQue);
}
//バッファにデータを挿入
hmLib_boolian cqueue_push(cqueue* pQue,const void* Ptr){
	return cdeque_push_back(pQue,Ptr);
}
//バッファのデータを削除
hmLib_boolian cqueue_pop(cqueue* pQue){
	return cdeque_pop_front(pQue);
}
//バッファのbegin関数
cqueue_iterator cqueue_begin(cqueue* pQue){
	return cdeque_begin(pQue);
}
//バッファのend関数
cqueue_iterator cqueue_end(cqueue* pQue){
	return cdeque_end(pQue);
}
//cqueue_iteratorの次のiteratorを与える
cqueue_iterator cqueue_next(cqueue* pQue,cqueue_iterator itr){
	return cdeque_next(pQue,itr);
}
//cqueue_iteratorの前のiteratorを与える
cqueue_iterator cqueue_prev(cqueue* pQue,cqueue_iterator itr){
	return cdeque_prev(pQue,itr);
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
