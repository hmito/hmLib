#ifndef HMLIB_CDEQUE_C_INC
#define HMLIB_CDEQUE_C_INC 200
#
#ifndef HMLIB_CDEQUE_INC
#	include <hmLib/cdeque.h>
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
void _cdeque_default_destructor(hmLib_pointer Ptr) { free(Ptr); }
//メモリの初期化
void cdeque_format(hmLib_cdeque* ptr){
	ptr->ElemSize=0;
	ptr->BufBegin=0;
	ptr->BufEnd=0;
	ptr->Begin=0;
	ptr->End=0;
	ptr->Destructor=0;
}
//すでに初期化されたかどうかの確認
hmLib_boolian cdeque_is_construct(hmLib_cdeque* ptr){
	return ptr->BufBegin==0;
}
//動的確保によってバッファ確保
void cdeque_construct(hmLib_cdeque* pDque, hmLib_cdeque_size_t Size, hmLib_cdeque_size_t ElemSize) {
	pDque->ElemSize=ElemSize;
	pDque->BufBegin=malloc(ElemSize*(Size+1));
	pDque->BufEnd=(hmLib_uint8*)(pDque->BufBegin)+ElemSize*(Size+1);
	pDque->Begin=pDque->BufBegin;
	pDque->End=pDque->BufBegin;
	pDque->Destructor=_cdeque_default_destructor;
}
//静的確保によってバッファ確保
void cdeque_placement_construct(hmLib_cdeque* pDque, hmLib_cdeque_size_t Size, hmLib_cdeque_size_t ElemSize, hmLib_pointer Ptr, hmLib_vFp_p Destructor) {
	pDque->ElemSize=ElemSize;
	pDque->BufBegin=Ptr;
	pDque->BufEnd=(hmLib_uint8*)(Ptr)+ElemSize*(Size);
	pDque->Begin=pDque->BufBegin;
	pDque->End=pDque->BufBegin;
	pDque->Destructor=Destructor;
}
//バッファ解放
void cdeque_destruct(hmLib_cdeque* pDque){
	if(pDque->Destructor) {
		pDque->Destructor(pDque->BufBegin);
	}
	pDque->BufBegin=0;
	pDque->BufEnd=0;
	pDque->Begin=0;
	pDque->End=0;
}
//move
void cdeque_move(hmLib_cdeque* from, hmLib_cdeque* to){
	to->Destructor=from->Destructor;
	to->ElemSize=from->ElemSize;
	to->Begin=from->Begin;
	to->End=from->End;
	to->BufBegin=from->BufBegin;
	to->BufEnd=from->BufEnd;
	cdeque_format(from);
}
//copy
void cdeque_swap(hmLib_cdeque* ptr1,hmLib_cdeque* ptr2){
	hmLib_cdeque tmp;
	tmp.Destructor=ptr1->Destructor;
	tmp.ElemSize=ptr1->ElemSize;
	tmp.Begin=ptr1->Begin;
	tmp.End=ptr1->End;
	tmp.BufBegin=ptr1->BufBegin;
	tmp.BufEnd=ptr1->BufEnd;
	ptr1->Destructor=ptr2->Destructor;
	ptr1->ElemSize=ptr2->ElemSize;
	ptr1->Begin=ptr2->Begin;
	ptr1->End=ptr2->End;
	ptr1->BufBegin=ptr2->BufBegin;
	ptr1->BufEnd=ptr2->BufEnd;
	ptr2->Destructor=tmp.Destructor;
	ptr2->ElemSize=tmp.ElemSize;
	ptr2->Begin=tmp.Begin;
	ptr2->End=tmp.End;
	ptr2->BufBegin=tmp.BufBegin;
	ptr2->BufEnd=tmp.BufEnd;
}
//バッファの先頭の要素アドレスを取得
void* cdeque_frontptr(hmLib_cdeque* pDque){return pDque->Begin;}
//バッファの末尾の要素アドレスを取得
void* cdeque_backptr(hmLib_cdeque* pDque){return cdeque_prev(pDque,pDque->End);}
//バッファにデータを挿入
hmLib_boolian cdeque_push_front(hmLib_cdeque* pDque,const void* Ptr){
	hmLib_cdeque_size_t cnt=0;

	if(cdeque_full(pDque))return 1;

	pDque->Begin=cdeque_prev(pDque,pDque->Begin);

	for(cnt=0;cnt<pDque->ElemSize;++cnt){
		*((hmLib_uint8*)(pDque->Begin)+cnt)=*((hmLib_uint8*)(Ptr)+cnt);
	}

	return 0;
}
//バッファの先頭のデータ削除
hmLib_boolian cdeque_pop_front(hmLib_cdeque* pDque){
	if(cdeque_empty(pDque))return 1;

	pDque->Begin=cdeque_next(pDque,pDque->Begin);

	return 0;
}
//バッファの末尾にデータを挿入
hmLib_boolian cdeque_push_back(hmLib_cdeque* pDque,const void* Ptr){
	hmLib_cdeque_size_t cnt=0;

	if(cdeque_full(pDque))return 1;

	for(cnt=0;cnt<pDque->ElemSize;++cnt){
		*((hmLib_uint8*)(pDque->End)+cnt)=*((hmLib_uint8*)(Ptr)+cnt);
	}

	pDque->End=cdeque_next(pDque,pDque->End);

	return 0;
}
//バッファの末尾のデータ削除
hmLib_boolian cdeque_pop_back(hmLib_cdeque* pDque){
	if(cdeque_empty(pDque))return 1;

	pDque->End=cdeque_prev(pDque,pDque->End);

	return 0;
}
//バッファに入っているサイズ
hmLib_cdeque_size_t cdeque_size(hmLib_cdeque* pDque){return (((hmLib_uint8*)(pDque->End)-(hmLib_uint8*)(pDque->Begin)+(hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin))%((hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin)))/pDque->ElemSize;}
//バッファに入っているサイズ
hmLib_cdeque_size_t cdeque_rest(hmLib_cdeque* pDque){return ((hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin))/pDque->ElemSize-cdeque_size(pDque)-1;}
//バッファが空かどうか確認
hmLib_boolian cdeque_empty(hmLib_cdeque* pDque){return (pDque->Begin==pDque->End);}
//バッファがいっぱいかどうか確認
hmLib_boolian cdeque_full(hmLib_cdeque* pDque){return (pDque->Begin==cdeque_next(pDque,pDque->End));}
//バッファのbegin関数
hmLib_cdeque_iterator cdeque_begin(hmLib_cdeque* pDque){return pDque->Begin;}
//バッファのend関数
hmLib_cdeque_iterator cdeque_end(hmLib_cdeque* pDque){return pDque->End;}
//hmLib_cdeque_iteratorの次のhmLib_cdeque_iteratorを返す
hmLib_cdeque_iterator cdeque_next(hmLib_cdeque* pDque,hmLib_cdeque_iterator itr){
	itr=(hmLib_uint8*)(itr)+pDque->ElemSize;
	if(pDque->BufEnd == itr)itr=pDque->BufBegin;
	return itr;
}
//hmLib_cdeque_iteratorの前のhmLib_cdeque_iteratorを返す
hmLib_cdeque_iterator cdeque_prev(hmLib_cdeque* pDque,hmLib_cdeque_iterator itr){
	if(pDque->BufBegin == itr)itr=pDque->BufEnd;
	itr=(hmLib_uint8*)(itr)-pDque->ElemSize;
	return itr;
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
