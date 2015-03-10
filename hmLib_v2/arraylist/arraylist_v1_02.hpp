/*
	有限list状配列
	使い勝手はlistと同じ
	配列長を変更する際には、一度内部データの移動が必要
*/
#ifndef INC_HMLIB_ARRAYLIST
#define INC_HMLIB_ARRAYLIST 102
#ifndef INC_HMLIB_ARRAY
#	include "hmLib_v2/array.hpp"
#endif
#include <deque>
#include <sstream>
namespace hmstd{
	template<class T>
	class arraylist{
	protected:
		typedef unsigned int size_type;
		class Datum{
		public:
			Datum* front;
			Datum* back;
			T t;
		public:
			Datum(){
				front=0;
				back=0;
			}
			Datum(const T& _t,Datum* _front,Datum* _back){
				t=_t;
				front=_front;
				back=_back;
			}
			Datum(const Datum& _dat){
				t=_dat.t;
				front=_dat.front;
				back=_dat.back;
			}
			T* operator->(){return &t;}
		};
	public:
		class iterator:public hm::basic_bidirectional_iterator<Datum*,T>{
		protected:
			virtual void move_front(){B=B->front;}
			virtual void move_back(){B=B->back;}
			virtual T& access()const{return B->t;}
			virtual T* accessPtr()const{return &(B->t);}
		public:
			iterator(){B=0;}
			iterator(Datum* _dat){B=_B;}
			iterator(iterator& _itr){B=_itr.B;}
			const iterator& operator=(const iterator& _itr){
				if(this!=&_itr)B=_itr.B;
				return *this;
			}
		};
		class reverse_iterator:public hm::basic_bidirectional_iterator<Datum*,T>{
		protected:
			virtual void move_front(){B=B->back;}
			virtual void move_back(){B=B->front;}
			virtual T& access()const{return B->t;}
			virtual T* accessPtr()const{return &(B->t);}
		public:
			reverse_iterator(){B=0;}
			reverse_iterator(Datum* _dat){B=_B;}
			reverse_iterator(reverse_iterator& _itr){B=_itr.B;}
			const reverse_iterator& operator=(const reverse_iterator& _itr){
				if(this!=&_itr)B=_itr.B;
				return *this;
			}
		};
		class const_iterator:public hm::basic_bidirectional_iterator<Datum*,T>{
		protected:
			virtual void move_front(){B=B->front;}
			virtual void move_back(){B=B->back;}
			virtual T& access()const{return B->t;}
			virtual T* accessPtr()const{return &(B->t);}
		public:
			const_iterator(){B=0;}
			const_iterator(Datum* _dat){B=_B;}
			const_iterator(iterator& _itr){B=_itr.getBase();}
			const_iterator(const_iterator& _itr){B=_itr.B;}
			const const_iterator& operator=(const iterator& _itr){
				B=_itr.getBase();
				return *this;
			}
			const const_iterator& operator=(const const_iterator& _itr){
				if(this!=&_itr)B=_itr.B;
				return *this;
			}
		};
		class const_reverse_iterator:public hm::basic_bidirectional_iterator<Datum*,T>{
		protected:
			virtual void move_front(){B=B->back;}
			virtual void move_back(){B=B->front;}
			virtual T& access()const{return B->t;}
			virtual T* accessPtr()const{return &(B->t);}
		public:
			const_reverse_iterator(){B=0;}
			const_reverse_iterator(Datum* _dat){B=_B;}
			const_reverse_iterator(reverse_iterator& _itr){B=_itr.getBase();}
			const_reverse_iterator(const_reverse_iterator& _itr){B=_itr.B;}
			const const_reverse_iterator& operator=(const reverse_iterator& _itr){
				B=_itr.getBase();
				return *this;
			}
			const const_reverse_iterator& operator=(const const_reverse_iterator& _itr){
				if(this!=&_itr)B=_itr.B;
				return *this;
			}
		};
	protected://164
		array<Datum> Data;
		std::deque<Datum*> Ptrs;
	protected:
		virtual void workInsert(iterator& itr){if(full())throw "arraylist::insert(iterator itr,const T& _val): out of range";}
		virtual void workErase(iterator& itr){return;}
		iterator insert_simple(iterator itr,const T& _val){
			Datum* pos=Ptrs.front();
			Ptrs.pop_front();
			pos->t=_val;
			pos->front=itr.dat;
			pos->back=itr.dat->back;
			pos->back->front=pos;
			pos->front->back=pos;
			return iterator(pos);
		}
		iterator erase_simple(iterator itr){
			Ptrs.push_back(itr.dat);
			itr.dat->front->back=itr.dat->back;
			itr.dat->back->front=itr.dat->front;
			++itr;
			return itr;
		}
		void copy(const arraylist& _ar){assignArr(_ar.sizeArr(),_ar.begin(),_ar.end());}
	public:
		arraylist(){clearArr();}
		arraylist(size_type _arrsize){assignArr(_arrsize);}
		arraylist(size_type _arrsize,size_type _num,const T& _val=T()){assignArr(_arrsize,_num,_val);}
		arraylist(const arraylist& _ar){copy(_ar);}
		template<class input_iterator>
		arraylist(input_iterator _start,input_iterator _end){assignArr(_start,_end);}
		template<class input_iterator>
		arraylist(size_type _arrsize,input_iterator _start,input_iterator _end){assignArr(_arrsize,_start,_end);}
		virtual ~arraylist(){clearArr();}
		void assignArr(size_type _bufsize){
			Data.assign(_bufsize+1);
			clear();
		}
		void assignArr(size_type _bufsize,size_type _num,const T& _val=T()){
			assignArr(_bufsize);
			assign(_num,_val);
		}
		template<class input_iterator>
		void assignArr(input_iterator _start,input_iterator _end){
			size_type cnt=0;
			input_iterator itr=_start;
			while(itr!=_end){
				++itr;
				++cnt;
			}
			assignArr(cnt);
			assign(_start,_end);
		}
		template<class input_iterator>
		void assignArr(size_type _bufsize,input_iterator _start,input_iterator _end){
			assignArr(_bufsize);
			assign(_start,_end);
		}
		void clearArr(){
			Data.assign(1);
			clear();
		}
		bool emptyArr()const{return sizeArr()==0;}
		size_type sizeArr()const{return Data.size();}
		iterator begin(){return iterator(Data[0].front);}
		iterator end(){return iterator(&Data[0]);}
		reverse_iterator rbegin(){return reverse_iterator(Data[0].back);}
		reverse_iterator rend(){return reverse_iterator(&Data[0]);}
		const_iterator begin()const{return const_iterator(Data[0].front);}
		const_iterator end()const{return const_iterator(&Data[0]);}
		const_reverse_iterator rbegin()const{return const_reverse_iterator(Data[0].back);}
		const_reverse_iterator rend()const{return const_reverse_iterator(&Data[0]);}
		bool empty()const{return size()==0;}
		bool full()const{return vacancy()==0;}
		size_type vacancy()const{return Ptrs.size();}
		size_type size()const{return (Data.size()-Ptrs.size()-1);}
		T& front(){return Data[0].front->t;}
		T& back(){return Data[0].back->t;}
		const T& front()const{return Data[0].front->t;}
		const T& back()const{return Data[0].back->t;}
		void push_back(const T& _val){insert(end(),_val);}
		void push_front(const T& _val){insert(begin(),_val);}
		void pop_back(){erase(iterator(Data[0].back));}
		void pop_front(){erase(iterator(Data[0].front));}
		iterator insert(iterator itr,const T& _val){
			workInsert(itr);
			return insert_simple(itr,_val);
		}
		void insert(iterator itr,size_type _num,const T& _val){
			for(size_type i=0;i<_num;++i){
				insert(itr,_val);
			}
		}
		template<class input_iterator>
		void insert(iterator itr,input_iterator _start,input_iterator _end){
			while(_start!=_end){
				insert(itr,(*_start));
				++_start;
			}
		}
		iterator erase(iterator itr){
			workErase(itr);
			return erase_simple(itr);
		}
		iterator erase(iterator _start,iterator _end){
			while(_start!=_end){
				erase(_start);
				++_start;
			}
			return _end;
		}
		void clear(){
			Data[0].front=&Data[0];
			Data[0].back=&Data[0];
			if(Data.size()==1){
				Ptrs.clear();
				return;
			}else{
				Ptrs.assign(Data.size()-1,0);
				std::deque<Datum*>::iterator itr=Ptrs.begin(),enditr=Ptrs.end();
				array<Datum>::iterator ditr=Data.begin();
				++ditr;	//Nullデータ部をよける
				while(itr!=enditr){
					(*itr)=&(*ditr);
					++ditr;
					++itr;
				}
			}
		}
		void assign(size_type _num,const T& _val=T()){
			clear();
			insert(begin(),_num,_val);
		}
		template<class input_iterator>
		void assign(input_iterator _start,input_iterator _last){
			clear();
			insert(begin(),_start,_last);
		}
		void swap(arraylist<T>& _ar){
			Data.swap(_ar.Data);
			Ptrs.swap(_ar.Ptrs);
		}
	};
}
#endif
