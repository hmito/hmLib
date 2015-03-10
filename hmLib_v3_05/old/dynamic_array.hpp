/*
STL準拠固定配列型
	関数が同一のものを使える以外、特に変更点なし
*/
#ifndef HMLIB_DYNAMICARRAY_INCy
#define HMLIB_DYNAMICARRAY_INC 100
#include <iostream>
#include <type_traits>
#ifndef HMLIB_ITERATORPATTERN_INC
#	include "iterator_pattern.hpp"
#endif
namespace hmLib{
	template<class T>
	class dynamic_array{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef const T& const_reference;
		typedef const T* const_pointer;
		typedef unsigned int size_type;
		typedef int difference_type;
	public:
		class iterator:public pattern::random_access_iterator_pattern<iterator,T>{
		private:
			typedef iterator my_type;
		private:
			T* Pos;
		protected:
			virtual void increment()/*C++0x_override/**/{++Pos;}
			virtual void decrement()/*C++0x_override/**/{--Pos;}
			virtual void forward_add(const difference_type& sub_)/*C++0x_override/**/{Pos+=sub_;}
			virtual void forward_sub(const difference_type& sub_)/*C++0x_override/**/{Pos-=sub_;}
			virtual const value_type& refer()const /*C++0x_override/**/{return *Pos;}
			virtual const value_type& index(const difference_type& index_)const /*C++0x_override/**/{return Pos[index_];}
			virtual const value_type* access()const /*C++0x_override/**/{return Pos;}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return Pos==val_.Pos;}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return Pos<val_.Pos;}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return Pos>val_.Pos;}
			virtual difference_type forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return Pos-val_.Pos;}
		public:
			iterator():Pos(0){return;}
			iterator(T* Pos_):Pos(Pos_){return;}
			iterator(const iterator& itr_):Pos(itr_.Pos){return;}
			const iterator& operator=(const iterator& itr_){
				if(this!=&itr_)Pos=itr_.Pos;
				return *this;
			}
		};
		class const_iterator: public pattern::random_access_const_iterator_pattern<const_iterator,T>{
		private:
			typedef const_iterator my_type;
		private:
			const T* Pos;
		protected:
			virtual void increment()/*C++0x_override/**/{++Pos;}
			virtual void decrement()/*C++0x_override/**/{--Pos;}
			virtual void forward_add(const difference_type& sub_)/*C++0x_override/**/{Pos+=sub_;}
			virtual void forward_sub(const difference_type& sub_)/*C++0x_override/**/{Pos-=sub_;}
			virtual const value_type& const_refer()const /*C++0x_override/**/{return *Pos;}
			virtual const value_type& const_index(const difference_type& index_)const /*C++0x_override/**/{return Pos[index_];}
			virtual const value_type* const_access()const /*C++0x_override/**/{return Pos;}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return Pos==val_.Pos;}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return Pos<val_.Pos;}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return Pos>val_.Pos;}
			virtual difference_type forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return Pos-val_.Pos;}
		public:
			const_iterator():Pos(0){return;}
			const_iterator(const T* Pos_):Pos(Pos_){return;}
			const_iterator(const iterator& itr_):Pos(itr_.Pos){return;}
			const_iterator(const const_iterator& itr_):Pos(itr_.Pos){return;}
			const const_iterator& operator=(const const_iterator& itr_){
				if(this!=&itr_)Pos=itr_.Pos;
				return *this;
			}
		};
		class reverse_iterator: public pattern::random_access_iterator_pattern<reverse_iterator,T>{
		private:
			typedef reverse_iterator my_type;
		private:
			T* Pos;
		protected:
			virtual void increment()/*C++0x_override/**/{--Pos;}
			virtual void decrement()/*C++0x_override/**/{++Pos;}
			virtual void forward_add(const difference_type& sub_)/*C++0x_override/**/{Pos-=sub_;}
			virtual void forward_sub(const difference_type& sub_)/*C++0x_override/**/{Pos+=sub_;}
			virtual const value_type& refer()const /*C++0x_override/**/{return Pos[-1];}
			virtual const value_type& index(const difference_type& index_)const /*C++0x_override/**/{return Pos[-1-index_];}
			virtual const value_type* access()const /*C++0x_override/**/{return Pos-1;}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return Pos==val_.Pos;}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return Pos>val_.Pos;}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return Pos<val_.Pos;}
			virtual difference_type forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return -Pos+val_.Pos;}
		public:
			reverse_iterator():Pos(0){return;}
			reverse_iterator(T* Pos_):Pos(Pos_){return;}
			reverse_iterator(const reverse_iterator& itr_):Pos(itr_.Pos){return;}
			const reverse_iterator& operator=(const reverse_iterator& itr_){
				if(this!=&itr_)Pos=itr_.Pos;
				return *this;
			}
		};
		class const_reverse_iterator: public pattern::random_access_const_iterator_pattern<const_reverse_iterator,T>{
		private:
			typedef const_reverse_iterator my_type;
		private:
			const T* Pos;
		protected:
			virtual void increment()/*C++0x_override/**/{--Pos;}
			virtual void decrement()/*C++0x_override/**/{++Pos;}
			virtual void forward_add(const difference_type& sub_)/*C++0x_override/**/{Pos-=sub_;}
			virtual void forward_sub(const difference_type& sub_)/*C++0x_override/**/{Pos+=sub_;}
			virtual const value_type& const_refer()const /*C++0x_override/**/{return Pos[-1];}
			virtual const value_type& const_index(const difference_type& index_)const /*C++0x_override/**/{return Pos[-1-index_];}
			virtual const value_type* const_access()const /*C++0x_override/**/{return Pos-1;}
			virtual bool forward_equal(const my_type& val_)const /*C++0x_override/**/{return Pos==val_.Pos;}
			virtual bool forward_less(const my_type& val_)const /*C++0x_override/**/{return Pos>val_.Pos;}
			virtual bool forward_more(const my_type& val_)const /*C++0x_override/**/{return Pos<val_.Pos;}
			virtual difference_type forward_binary_sub(const my_type& val_)const /*C++0x_override/**/{return -Pos+val_.Pos;}
		public:
			const_reverse_iterator():Pos(0){return;}
			const_reverse_iterator(const T* Pos_):Pos(Pos_){return;}
			const_reverse_iterator(const reverse_iterator& itr_):Pos(itr_.Pos){return;}
			const_reverse_iterator(const const_reverse_iterator& itr_):Pos(itr_.Pos){return;}
			const const_reverse_iterator& operator=(const const_reverse_iterator& itr_){
				if(this!=&itr_)Pos=itr_.Pos;
				return *this;
			}
		};
	private:
		T* buf;
		size_type Size;
	protected:
		void copy(const dynamic_array<T>& _ar){
			create(_ar.size());
			const_iterator citr=_ar.begin(),cenditr=_ar.end();
			iterator itr=begin();
			while(citr!=cenditr){
				(*itr)=(*citr);
				++citr;
				++itr;
			}
		}
	public:
		dynamic_array():buf(0),Size(0){return;}
		explicit dynamic_array(size_type _num):buf(0),Size(0){create(_num);}
		dynamic_array(size_type _num,const T& _val):buf(0),Size(0){create(_num,_val);}
		dynamic_array(const dynamic_array& _ar):buf(0),Size(0){copy(_ar);}
		template<class input_iterator>
		dynamic_array(input_iterator _start,input_iterator _end):buf(0),Size(0){create(_start,_end);}
		virtual ~dynamic_array(){release();}
		const dynamic_array<T>& operator=(const dynamic_array<T>& _ar){
			if(this!=&_ar)copy(_ar);
			return *this;
		}
		bool operator==(const dynamic_array& _ar)const{
			if(size()!=_ar.size())return false;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return false;
				++itr1;
				++itr2;
			}
			return true;
		}
		bool operator!=(const dynamic_array& _ar)const{
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
		void create(size_type _num){
			release();
			buf=new T[_num+1];
			Size=_num;
		}
		void create(size_type _num,const T& _val){_create_int(_num,_val);}
		template<class input_iterator>
		void create(input_iterator _start,input_iterator _end){_create_itr(_start,_end,std::is_integral<input_iterator>::type());}
	private:
		void _create_int(size_type _num,const T& _val){
			create(_num);
			iterator itr=begin(),enditr=end();
			for(;itr!=enditr;++itr)(*itr)=_val;
		}
		template<class input_iterator>
		void _create_itr(input_iterator _start,input_iterator _end,std::true_type){_create_int(_start,_end);}
		template<class input_iterator>
		void _create_itr(input_iterator _start,input_iterator _end,std::false_type){
			size_type cnt=0;
			input_iterator itr=_start;
			while(itr!=_end){
				++itr;
				++cnt;
			}
			create(cnt);
			itr=_start;
			iterator bitr=begin();
			while(itr!=_end){
				(*bitr)=(*itr);
				++itr;
				++bitr;
			}
		}
	public:
		bool is_create()const{return buf!=0;}
		size_type size_create()const{return Size;}
		void release(){
			if(is_create()){
				delete[] buf;
				buf=0;
				Size=0;
			}
		}
		T& at(size_type _pos){
			if(_pos>=Size)throw "shm::dynamic_array::at(size_type) out of range";
			return buf[_pos];
		}
		const T& at(size_type _pos)const{
			if(_pos>=Size)throw "shm::dynamic_array::at(size_type) out of range";
			return buf[_pos];
		}
		T& front(){return buf[0];}
		const T& front()const{return buf[0];}
		T& back(){return buf[Size-1];}
		const T& back()const{return buf[Size-1];}
		iterator begin(){return iterator(buf);}
		const_iterator begin()const{return const_iterator(buf);}
		iterator end(){return iterator(buf+Size);}
		const_iterator end()const{return const_iterator(buf+Size);}
		reverse_iterator rbegin(){return reverse_iterator(buf+Size);}
		const_reverse_iterator rbegin()const{return const_reverse_iterator(buf+Size);}
		reverse_iterator rend(){return reverse_iterator(buf);}
		const_reverse_iterator rend()const{return const_reverse_iterator(buf);}
		bool empty()const{return Size==0;}
		size_type size()const{return Size;}
		void swap(dynamic_array& _ar){
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
