/*
STL準拠固定配列型
	関数が同一のものを使える以外、特に変更点なし
*/
#ifndef INC_HMLIB_ARRAY
#define INC_HMLIB_ARRAY 103
#include <iostream>
#ifndef INC_HMLIB_BASICITERATOR
#	include "hmLib_v2/basic_iterator.hpp"
#endif
//#include <algorithm.hpp>
//#include <boost/iterator_adaptors.hpp>
namespace hmstd{
	template<class T>
	class array{
	private:
		class basic_iterator_manager_type:public hmstd::random_iterator_manager_type<T>{
		protected:
			T* Ptr;
		public:
			basic_iterator_manager_type(T* _Ptr){Ptr=_Ptr;}
			void increment(){++Ptr;}
			void decrement(){--Ptr;}
			void increment(difference_type _num){Ptr+=_num;}
			void decrement(difference_type _num){Ptr-=_num;}
			bool equal(const basic_iterator_manager_type& _itr)const{return Ptr==_itr.Ptr;}
		};
		class iterator_manager_type:public basic_iterator_manager_type{
		public:
			iterator_manager_type():basic_iterator_manager_type(0){return;}
			iterator_manager_type(T* _Ptr):basic_iterator_manager_type(_Ptr){return;}
			bool less(const iterator_manager_type& _itr)const{return Ptr< _itr.Ptr;}
			difference_type difference(const iterator_manager_type& _itr)const{return _itr.Ptr-Ptr;}
			reference ref(){return *Ptr;}
			reference ref(difference_type _num){return *(Ptr+_num);}
			pointer ptr(){return Ptr;}
		};
		class sub_iterator_manager_type:public basic_iterator_manager_type{
		public:
			sub_iterator_manager_type():basic_iterator_manager_type(0){return;}
			sub_iterator_manager_type(T* _Ptr):basic_iterator_manager_type(_Ptr){return;}
			bool less(const iterator_manager_type& _itr)const{return Ptr > _itr.Ptr;}
			difference_type difference(const iterator_manager_type& _itr)const{return Ptr-_itr.Ptr;}
			reference ref(){return *(Ptr-1);}
			reference ref(difference_type _num){return *(Ptr-_num-1);}
			pointer ptr(){return Ptr-1;}
		};
	public:
		typedef unsigned int size_type;
		class iterator:public basic_iterator<iterator,iterator_manager_type>{
		public:
			iterator():basic_iterator<iterator,iterator_manager_type>(iterator_manager_type()){return;}
			iterator(T* _Ptr):basic_iterator<iterator,iterator_manager_type>(iterator_manager_type(_Ptr)){return;}
			iterator(const basic_iterator<iterator,iterator_manager_type>& _bitr):basic_iterator<iterator,iterator_manager_type>(_bitr){return;}
			iterator operator-(int _num)const{
				iterator ans(*this);
				ans.decrement(_num);
				return ans;
			}
			int operator-(const iterator& itr)const{
				return itr.difference(*this);
			}
		};
		class const_iterator:public basic_const_iterator<const_iterator,iterator_manager_type>{
		public:
			const_iterator():basic_const_iterator<const_iterator,iterator_manager_type>(iterator_manager_type()){return;}
			const_iterator(T* _Ptr):basic_const_iterator<const_iterator,iterator_manager_type>(iterator_manager_type(_Ptr)){return;}
			const_iterator(const basic_const_iterator<const_iterator,iterator_manager_type>& _bitr):basic_const_iterator<const_iterator,iterator_manager_type>(_bitr){return;}
		};
		class reverse_iterator:public basic_reverse_iterator<reverse_iterator,sub_iterator_manager_type>{
		public:
			reverse_iterator():basic_reverse_iterator<sub_iterator_manager_type>(sub_iterator_manager_type()){return;}
			reverse_iterator(T* _Ptr):basic_reverse_iterator<sub_iterator_manager_type>(sub_iterator_manager_type(_Ptr)){return;}
			reverse_iterator(const basic_reverse_iterator<reverse_iterator,iterator_manager_type>& _bitr):basic_reverse_iterator<reverse_iterator,iterator_manager_type>(_bitr){return;}
		};
		class const_reverse_iterator:public basic_const_reverse_iterator<const_reverse_iterator,sub_iterator_manager_type>{
		public:
			const_reverse_iterator():basic_const_reverse_iterator<sub_iterator_manager_type>(sub_iterator_manager_type()){return;}
			const_reverse_iterator(T* _Ptr):basic_const_reverse_iterator<sub_iterator_manager_type>(sub_iterator_manager_type(_Ptr)){return;}
			const_reverse_iterator(const basic_const_reverse_iterator<const_reverse_iterator,iterator_manager_type>& _bitr):basic_const_reverse_iterator<const_reverse_iterator,iterator_manager_type>(_bitr){return;}
		};
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
		bool operator!=(const array& _ar)const{
			if(size()!=_ar.size())return true;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return true;
				++itr1;
				++itr2;
			}
			return false;
		}
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
