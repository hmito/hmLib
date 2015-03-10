/*
	要素自体はlist状に、
	実体確保はvectorで、
	使用状況はdequeで、
	それぞれ管理する、任意の位置で高速挿入削除可能な配列
*/
#ifndef INC_HMLIB_VECTORLIST
#define INC_HMLIB_VECTORLIST 101
#
#ifndef INC_HMLIB_ARRAYLIST
#	include "hmLib_v2/arraylist.hpp"
#endif
namespace hmstd{
	template<class T>
	class vectorlist:public arraylist<T>{
	protected:
		virtual void workInsert(iterator& _itr){
			if(full()){
				vectorlist<T> tmp;
				swap(tmp);
				assignArr(tmp.sizeArr()*2-1);
				vectorlist<T>::iterator itr=tmp.begin(),enditr=tmp.end(),pitr;
				for(;itr!=enditr;++itr){
					pitr=insert_simple(end(),(*itr));
					if(itr==_itr)_itr=pitr;
				}
				if(enditr==_itr)_itr=end();
			}
		}
		virtual void workErase(iterator& _itr){return;}
/*		virtual void workErase(iterator& _itr){
			if(size()*3-3<=vacancy()){
				vectorlist<T> tmp;
				swap(tmp);
				assignArr(tmp.sizeArr()/2-1);
				vectorlist<T>::iterator itr=tmp.begin(),enditr=tmp.end(),pitr;
				for(;itr!=enditr;++itr){
					pitr=insert_simple(end(),(*itr));
					if(itr==_itr)_itr=pitr;
				}
				if(enditr==_itr)_itr=end();
			}
		}
*/
	public:
		vectorlist():arraylist(){return;}
		vectorlist(size_type _num,const T& _val=T()):arraylist(_num,_num,_val){return;}
		vectorlist(const vectorlist& _ar):arraylist(_ar){return;}
		template<class input_iterator>
		vectorlist(input_iterator _start,input_iterator _end):arraylist(_start,_end){return;}
	};
}
#endif
