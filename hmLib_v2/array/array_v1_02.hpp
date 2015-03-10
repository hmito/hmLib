/*
STL準拠固定配列型
	関数が同一のものを使える以外、特に変更点なし
*/
#ifndef INC_HMLIB_ARRAY
#define INC_HMLIB_ARRAY 102
#include <iostream>
#ifndef INC_HMLIB_ALGORITHM
#	include "hmLib_v2/algorithm.hpp"
#endif
namespace hmstd{
	template<class T>
	class array{
	public:
		typedef unsigned int size_type;
		class iterator:public hm::basic_random_access_iterator<T*,T>{
		public:
			iterator(){return;}
			iterator(T* _pos){B=_pos;}
			iterator(const hm::basic_random_access_iterator<T*,T>& _itr){B=_itr.getBase();}
		};
		class const_iterator:public hm::basic_random_access_iterator<const T*,const T>{
		public:
			const_iterator(){return;}
			const_iterator(const T* _pos){B=_pos;}
			const_iterator(const hm::basic_random_access_iterator<T*,T>& _itr){B=_itr.getBase();}
			const_iterator(const hm::basic_random_access_iterator<const T*,const T>& _itr){B=_itr.getBase();}
		};
		class reverse_iterator:public hm::basic_random_access_iterator<T*,T>{
		protected:
			virtual void move_front(){--B;}
			virtual void move_back(){++B;}
			virtual void move_front(int _num){B-=_num;}
			virtual void move_back(int _num){B+=_num;}
			virtual bool less(const basic_random_access_iterator<T*,T>& _itr)const{return B>_itr.getBase();}
			virtual T& access()const{return *(B-1);}
			virtual T& access(int _num)const{return *(B-_num-1);}
			virtual T* accessPtr()const{return B-1;}
		public:
			reverse_iterator(){return;}
			reverse_iterator(T* _pos){B=_pos;}
			reverse_iterator(const hm::basic_random_access_iterator<T*,T>& _itr){B=_itr.getBase();}
		};
		class const_reverse_iterator:public hm::basic_random_access_iterator<const T*,const T>{
		protected:
			virtual void move_front(){--B;}
			virtual void move_back(){++B;}
			virtual void move_front(int _num){B-=_num;}
			virtual void move_back(int _num){B+=_num;}
			virtual bool less(const basic_random_access_iterator<T*,T>& _itr)const{return B>_itr.getBase();}
			virtual const T& access()const{return *(B-1);}
			virtual const T& access(int _num)const{return *(B-_num-1);}
			virtual const T* accessPtr()const{return B-1;}
		public:
			const_reverse_iterator(){return;}
			const_reverse_iterator(const T* _pos){B=_pos;}
			const_reverse_iterator(const hm::basic_random_access_iterator<T*,T>& _itr){B=_itr.getBase();}
			const_reverse_iterator(const hm::basic_random_access_iterator<const T*,const T>& _itr){B=_itr.getBase();}
		};
		/*		class iterator{

			bool operator==(const iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const iterator& _itr)const{return pos!=_itr.pos;}
			iterator& operator++(){
				++pos;
				return *this;
			}
			iterator& operator--(){
				--pos;
				return *this;
			}
			iterator operator++(int){
				T* tmp=pos;
				++pos;
				return iterator(pos);
			}
			iterator operator--(int){
				T* tmp=pos;
				--pos;
				return iterator(pos);
			}
			iterator operator+(int _ref){
				return iterator(pos+_ref);
			}
			iterator operator-(int _ref){
				return iterator(pos-_ref);
			}
			T& operator[](int _ref){return pos[_ref];}
			T& operator*(){return *pos;}
			T* operator->(){return pos;}
			friend const_iterator;
		};
		class const_iterator{
		private:
			const T* pos;
		public:
			const_iterator(){return;}
			const_iterator(const T* _pos){pos=_pos;}
			const_iterator(const const_iterator& _itr){pos=_itr.pos;}
			const_iterator(const iterator& _itr){pos=_itr.pos;}
			bool operator==(const const_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const const_iterator& _itr)const{return pos!=_itr.pos;}
			const_iterator& operator++(){
				++pos;
				return *this;
			}
			const_iterator& operator--(){
				--pos;
				return *this;
			}
			const_iterator operator++(int){
				const T* tmp=pos;
				++pos;
				return const_iterator(pos);
			}
			const_iterator operator--(int){
				const T* tmp=pos;
				--pos;
				return const_iterator(pos);
			}
			const_iterator operator+(int _ref){
				return const_iterator(pos+_ref);
			}
			const_iterator operator-(int _ref){
				return const_iterator(pos-_ref);
			}
			const T& operator[](int _ref){return pos[_ref];}
			const T& operator*()const{return *pos;}
			const T* operator->()const{return pos;}
		};
		class reverse_iterator{
		private:
			T* pos;
		public:
			reverse_iterator(){return;}
			reverse_iterator(T* _pos){pos=_pos;}
			reverse_iterator(const reverse_iterator& _itr){pos=_itr.pos;}
			bool operator==(const reverse_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const reverse_iterator& _itr)const{return pos!=_itr.pos;}
			reverse_iterator& operator++(){
				--pos;
				return *this;
			}
			reverse_iterator& operator--(){
				++pos;
				return *this;
			}
			reverse_iterator operator++(int){
				T* tmp=pos;
				--pos;
				return reverse_iterator(pos);
			}
			reverse_iterator operator--(int){
				T* tmp=pos;
				++pos;
				return reverse_iterator(pos);
			}
			reverse_iterator operator+(int _ref){
				return reverse_iterator(pos+_ref);
			}
			reverse_iterator operator-(int _ref){
				return reverse_iterator(pos-_ref);
			}
			T& operator[](int _ref){return pos[_ref-1];}
			T& operator*(){return *(pos-1);}
			T* operator->(){return pos-1;}
			friend const_reverse_iterator;
		};
		class const_reverse_iterator{
		private:
			const T* pos;
		public:
			const_reverse_iterator(){return;}
			const_reverse_iterator(const T* _pos){pos=_pos;}
			const_reverse_iterator(const const_reverse_iterator& _itr){pos=_itr.pos;}
			const_reverse_iterator(const reverse_iterator& _itr){pos=_itr.pos;}
			bool operator==(const const_reverse_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const const_reverse_iterator& _itr)const{return pos!=_itr.pos;}
			const_reverse_iterator& operator++(){
				--pos;
				return *this;
			}
			const_reverse_iterator& operator--(){
				++pos;
				return *this;
			}
			const_reverse_iterator operator++(int){
				const T* tmp=pos;
				--pos;
				return const_reverse_iterator(pos);
			}
			const_reverse_iterator operator--(int){
				const T* tmp=pos;
				++pos;
				return const_reverse_iterator(pos);
			}
			const_reverse_iterator operator+(int _ref){
				return const_reverse_iterator(pos+_ref);
			}
			const_reverse_iterator operator-(int _ref){
				return const_reverse_iterator(pos-_ref);
			}
			const T& operator[](int _ref){return pos[_ref-1];}
			const T& operator*()const{return *(pos-1);}
			const T* operator->()const{return pos-1;}
		};*/
	private:
		T* buf;
		size_type Size;
	protected:
		void copy(const array<T>& _ar){
			assign(_ar.size());
			const_iterator citr=_ar.begin(),cenditr=_ar.end();
			iterator itr=begin();
			while(citr!=cenditr){
				(*itr)=(*citr);
				++citr;
				++itr;
			}
		}
	public:
		array():Size(0){return;}
		array(size_type _num):Size(0){assign(_num);}
		array(size_type _num,const T& _val):Size(0){assign(_num,_val);}
		array(const array& _ar):Size(0){copy(_ar);}
		template<class input_iterator>
		array(input_iterator _start,input_iterator _end):Size(0){assign(_start,_end);}
		virtual ~array(){clear();}
		const array<T>& operator=(const array<T>& _ar){
			if(this!=&_ar)copy();
			return *this;
		}
		bool operator==(const array& _ar)const{
			if(size()!=_ar.size())return false;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return false;
				++itr1;
				++itr2;
			}
			return true;
		}
		bool operator!=(const array& _ar)const{return !(*this!=_ar);}
		T& operator[](size_type _pos){return buf[_pos];}
		const T& operator[](size_type _pos)const{return buf[_pos];}
		void assign(size_type _num){
			clear();
			buf=new T[_num+1];
			Size=_num;
		}
		void assign(size_type _num,const T& _val){
			assign(_num);
			iterator itr=begin(),enditr=end();
			for(;itr!=enditr;++itr)(*itr)=_val;
		}
		template<class input_iterator>
		void assign(input_iterator _start,input_iterator _end){
			size_type cnt=0;
			input_iterator itr=_start;
			while(itr!=_end){
				++itr;
				++cnt;
			}
			assign(cnt);
			itr=_start;
			iterator bitr=begin();
			while(itr!=_end){
				(*bitr)=(*itr);
				++itr;
				++bitr;
			}
		}
		T& at(size_type _pos){
			if(_pos>=Size)throw "hmstd::array::at(size_type) out of range";
			return buf[_pos];
		}
		const T& at(size_type _pos)const{
			if(_pos>=Size)throw "hmstd::array::at(size_type) out of range";
			return buf[_pos];
		}
		T& front(){return buf[0];}
		const T& front()const{return buf[0];}
		T& back(){return buf[Size-1];}
		const T& back()const{return buf[Size-1];}
		iterator begin(){return buf;}
		const_iterator begin()const{return buf;}
		iterator end(){return buf+Size;}
		const_iterator end()const{return buf+Size;}
		reverse_iterator rbegin(){return buf+Size;}
		const_reverse_iterator rbegin()const{return buf+Size;}
		reverse_iterator rend(){return buf;}
		const_reverse_iterator rend()const{return buf;}
		void clear(){
			if(Size!=0){
				delete[] buf;
				Size=0;
			}
		}
		bool empty()const{return Size==0;}
		size_type size()const{return Size;}
		void swap(array& _ar){
			T* tmp=buf;
			buf=_ar.buf;
			_ar.buf=tmp;
			size_type tmpS=Size;
			Size=_ar.Size;
			_ar.Size=tmpS;
		}
	};
}
#endif
