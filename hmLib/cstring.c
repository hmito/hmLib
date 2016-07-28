#ifndef HMLIB_CSTRING_C_INC
#define HMLIB_CSTRING_C_INC 201
#ifndef HMLIB_CSTRING_INC
#	include <hmLib/cstring.h>
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
void _cstring_default_destructor(hmLib_pointer Ptr) { free(Ptr); }
void cstring_format(hmLib_cstring* str){
	str->Destructor=0;
	str->Begin=0;
	str->End=0;
}
hmLib_boolian cstring_is_construct(hmLib_cstring* str){
	return str->Begin!=0;
}
void cstring_construct(hmLib_cstring* str, hmLib_cstring_size_t size) {
	if(size==0) {
		cstring_format(str);
		return;
	}
	str->Begin=(unsigned char*)malloc(size);
	if(str->Begin==0) {
		cstring_format(str);
		return;
	}
	str->End=str->Begin+size;
	str->Destructor=_cstring_default_destructor;
}
void cstring_placement_construct(hmLib_cstring* str, hmLib_cstring_size_t size, hmLib_pointer ptr, hmLib_vFp_p destructor){
	if(size==0) {
		cstring_format(str);
		return;
	}
	if(ptr==0) {
		cstring_format(str);
		return;
	}
	str->Begin=(unsigned char*)(ptr);
	str->End=(unsigned char*)(ptr)+size;
	str->Destructor=destructor;
}
void cstring_destruct(hmLib_cstring* str){
	if(str->Destructor) {
		str->Destructor(str->Begin);
	}
	str->Begin=0;
	str->End=0;
}
void cstring_move(hmLib_cstring* from, hmLib_cstring* to){
	to->Destructor=from->Destructor;
	to->Begin=from->Begin;
	to->End=from->End;
	cstring_format(from);
}
void cstring_swap(hmLib_cstring* str1, hmLib_cstring* str2){
	hmLib_cstring tmp;
	tmp.Destructor=str1->Destructor;
	tmp.Begin=str1->Begin;
	tmp.End=str1->End;
	str1->Destructor=str2->Destructor;
	str1->Begin=str2->Begin;
	str1->End=str2->End;
	str2->Destructor=tmp.Destructor;
	str2->Begin=tmp.Begin;
	str2->End=tmp.End;
}
unsigned char* cstring_ptr(const hmLib_cstring* str){return str->Begin;}
hmLib_cstring_size_t cstring_size(const hmLib_cstring* str){return str->End - str->Begin;}
hmLib_cstring_iterator cstring_begin(const hmLib_cstring* str){return str->Begin;}
hmLib_cstring_iterator cstring_end(const hmLib_cstring* str){return str->End;}
hmLib_cstring_iterator cstring_next(const hmLib_cstring* str,hmLib_cstring_iterator itr){return ++itr;}
hmLib_cstring_iterator cstring_prev(const hmLib_cstring* str,hmLib_cstring_iterator itr){return --itr;}
void cstring_putc(hmLib_cstring* str,hmLib_cstring_size_t pos,unsigned char c){
	if(str->Begin+pos<str->End)str->Begin[pos]=c;
}
void cstring_puts(hmLib_cstring* str,hmLib_cstring_size_t pos,const unsigned char* p,hmLib_cstring_size_t size){
	hmLib_cstring_size_t cnt;
	for(cnt=0;cnt<size;++cnt){
		if(pos+cnt+str->Begin>=str->End)break;
		str->Begin[pos+cnt]=p[cnt];
	}
}
unsigned char cstring_getc(hmLib_cstring* str,hmLib_cstring_size_t pos){
	return str->Begin+pos<str->End ? str->Begin[pos] : 0;
}
unsigned char* cstring_gets(hmLib_cstring* str, hmLib_cstring_size_t pos){
	return str->Begin+pos;
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
