#ifndef HMLIB_CSTRING_C_INC
#define HMLIB_CSTRING_C_INC 103
#ifndef HMLIB_CSTRING_INC
#	include <hmLib_v3_05/cstring.h>
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
void cstring_format(cstring* str){
	str->DynamicFlag=0;
	str->Begin=0;
	str->End=0;
}
hmLib_boolian cstring_is_construct(cstring* str){
	return str->Begin!=0;
}
cstring* cstring_create(cstring_size_t size){
	cstring* str;
	str=(cstring*)malloc(sizeof(cstring));
	cstring_construct_dynamic(str,size);
	return str;
}
void cstring_release(cstring* str){
	cstring_destruct(str);
	free(str);
}
void cstring_construct_dynamic(cstring* str,cstring_size_t size){
	str->Begin=(unsigned char*)malloc(size);
	str->End=str->Begin+size;
	str->DynamicFlag=hmLib_true;
}
void cstring_construct_static(cstring* str,unsigned char* Begin,unsigned char* End){
	str->Begin=Begin;
	str->End=End;
	str->DynamicFlag=hmLib_false;
}
void cstring_destruct(cstring* str){
	if(str->DynamicFlag){
		free(str->Begin);
	}
	str->Begin=0;
	str->End=0;
}
void cstring_move(cstring* from, cstring* to){
	to->DynamicFlag=from->DynamicFlag;
	to->Begin=from->Begin;
	to->End=from->End;
	cstring_format(from);
}
void cstring_swap(cstring* str1, cstring* str2){
	cstring tmp;
	tmp.DynamicFlag=str1->DynamicFlag;
	tmp.Begin=str1->Begin;
	tmp.End=str1->End;
	str1->DynamicFlag=str2->DynamicFlag;
	str1->Begin=str2->Begin;
	str1->End=str2->End;
	str2->DynamicFlag=tmp.DynamicFlag;
	str2->Begin=tmp.Begin;
	str2->End=tmp.End;
}
unsigned char* cstring_ptr(const cstring* str){return str->Begin;}
cstring_size_t cstring_size(const cstring* str){return str->End - str->Begin;}
cstring_iterator cstring_begin(const cstring* str){return str->Begin;}
cstring_iterator cstring_end(const cstring* str){return str->End;}
cstring_iterator cstring_next(const cstring* str,cstring_iterator itr){return ++itr;}
cstring_iterator cstring_prev(const cstring* str,cstring_iterator itr){return --itr;}
void cstring_putc(cstring* str,cstring_size_t pos,unsigned char c){
	if(str->Begin+pos<str->End)str->Begin[pos]=c;
}
void cstring_puts(cstring* str,cstring_size_t pos,const unsigned char* p,cstring_size_t size){
	cstring_size_t cnt;
	for(cnt=0;cnt<size;++cnt){
		if(pos+cnt+str->Begin>=str->End)break;
		str->Begin[pos+cnt]=p[cnt];
	}
}
unsigned char cstring_getc(cstring* str,cstring_size_t pos){
	return str->Begin+pos<str->End ? str->Begin[pos] : 0;
}
unsigned char* cstring_gets(cstring* str, cstring_size_t pos){
	return str->Begin+pos;
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
