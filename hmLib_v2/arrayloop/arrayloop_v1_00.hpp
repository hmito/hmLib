/*
	â~èÛÇÃå≈íËí∑îzóÒ
	ä÷êîå`ÇÕdequeÇ…èÄãí
	è„å¿Çí¥Ç¶ÇƒÇÃë}ì¸ÇÕ
*/
#ifndef INC_HMLIB_ARRAYLOOP
#define INC_HMLIB_ARRAYLOOP 100
#ifndef INC_HMLIB_ARRAYDEQUE
#	include "hmLib_v2/arraydeque.hpp"
#endif
namespace hmstd{
	template<class T>
	class arrayloop:public arraydeque<T>{
	public:
		arrayloop():arraydeque<T>(){return;}
		arrayloop(size_type _bufsize):arraydeque<T>(_bufsize){return;}
		arrayloop(size_type _bufsize,size_type _num):arraydeque<T>(_bufsize,_num){return;}
		arrayloop(size_type _bufsize,size_type _num,const T& _val):arraydeque<T>(_bufsize,_num,_val){return;}
		arrayloop(const arrayloop& _ar):arraydeque<T>(_ar){return;}
		template<class input_iterator>
		arrayloop(input_iterator _start,input_iterator _end):arraydeque<T>(_start,_end){return;}
		template<class input_iterator>
		arrayloop(size_type _bufsize,input_iterator _start,input_iterator _end):arraydeque<T>(_bufsize,_start,_end){return;}
		virtual ~arrayloop(){clearArr();}
		const arrayloop<T>& operator=(const arrayloop<T>& _ar){
			if(this!=&_ar)copy();
			return *this;
		}
		bool operator==(const arrayloop& _ar)const{
			if(size()!=_ar.size())return false;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return false;
				++itr1;
				++itr2;
			}
			return true;
		}
		bool operator!=(const arrayloop& _ar)const{return !(*this!=_ar);}
		T& operator[](size_type _pos){return Buf[(BufPos+_pos)%(BufSize+1)];}
		const T& operator[](size_type _pos)const{return Buf[(BufPos+_pos)%(BufSize+1)];}
		virtual void push_back(const T& _val){
			if(full())pop_front();
			Buf[(BufPos+Size)%(BufSize+1)]=_val;
			++Size;
		}
		virtual void push_front(const T& _val){
			if(full())pop_back();
			BufPos=(BufPos+BufSize)%(BufSize+1);
			++Size;
			Buf[BufPos]=_val;
		}
	};
}
#endif
