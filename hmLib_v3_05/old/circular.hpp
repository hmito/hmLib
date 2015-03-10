#ifndef HMLIB_CIRCULAR_INC
#define HMLIB_CIRCULAR_INC 100
#ifndef HMLIB_ITERATORPATTERN_INC
#	include "iterator_pattern.hpp"
#endif
#include <queue>
#include <stack>

namespace hmLib{
	template<class T>
	class bicircular{
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
			T* Begin;
			size_type Pos;
			size_type Size;
		protected:
			virtual void increment()/*C++0x_override/**/{Pos=(Pos+1)%Size;}
			virtual void decrement()/*C++0x_override/**/{Pos=(Pos+Size-1)%Size;}
			virtual void forward_add(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size+(num_%Size))%Size;}
			virtual void forward_sub(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size-(num_%Size))%Size;}
			virtual const value_type& refer()const /*C++0x_override/**/{return Begin[Pos];}
			virtual const value_type& index(const difference_type& pos_)const /*C++0x_override/**/{
				pos_=(Pos+Size+(pos_%Size))%Size;
				return Begin[pos_];
			}
			virtual const value_type* access()const /*C++0x_override/**/{return Begin+Pos;}
			virtual bool forward_equal(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos==itr_.Begin+itr_.Pos;}
			virtual bool forward_less(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos<itr_.Begin+itr_.Pos;}
			virtual bool forward_more(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos>itr_.Begin+itr_.Pos;}
			virtual difference_type forward_biex_sub(const my_type& itr_)const /*C++0x_override/**/{return itr_.Begin-Begin+itr_.Pos-Pos;}
		public:
			iterator():Begin(0),Pos(0),Size(0){return;}
			iterator(T* Begin_,size_type Pos_,size_type Size_):Begin(Begin_),Pos(Pos_),Size(Size_){return;}
			iterator(const iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const iterator& operator=(const iterator& itr_){
				if(this!=&itr_){
					Begin=itr_.Begin;
					Pos=itr_.Pos;
					Size=itr_.Size;
				}
				return *this;
			}
		};
		class const_iterator:public pattern::random_access_const_iterator_pattern<iterator,T>{
		private:
			typedef const_iterator my_type;
		private:
			const T* Begin;
			size_type Pos;
			size_type Size;
		protected:
			virtual void increment()/*C++0x_override/**/{Pos=(Pos+1)%Size;}
			virtual void decrement()/*C++0x_override/**/{Pos=(Pos+Size-1)%Size;}
			virtual void forward_add(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size+(num_%Size))%Size;}
			virtual void forward_sub(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size-(num_%Size))%Size;}
			virtual const value_type& const_refer()const /*C++0x_override/**/{return Begin[Pos];}
			virtual const value_type& const_index(const difference_type& pos_)const /*C++0x_override/**/{
				pos_=(Pos+Size+(pos_%Size))%Size;
				return Begin[pos_];
			}
			virtual value_type* access()/*C++0x_override/**/{return Begin+Pos;}
			virtual bool forward_equal(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos==itr_.Begin+itr_.Pos;}
			virtual bool forward_less(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos<itr_.Begin+itr_.Pos;}
			virtual bool forward_more(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos>itr_.Begin+itr_.Pos;}
			virtual difference_type forward_biex_sub(const my_type& itr_)const /*C++0x_override/**/{return itr_.Begin-Begin+itr_.Pos-Pos;}
		public:
			const_iterator():Begin(0),Pos(0),Size(0){return;}
			const_iterator(T* Begin_,size_type Pos_,size_type Size_):Begin(Begin_),Pos(Pos_),Size(Size_){return;}
			const_iterator(const iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const_iterator(const const_iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const const_iterator& operator=(const const_iterator& itr_){
				if(this!=&itr_){
					Begin=itr_.Begin;
					Pos=itr_.Pos;
					Size=itr_.Size;
				}
				return *this;
			}
		};
		class reverse_iterator:public pattern::random_access_iterator_pattern<reverse_iterator,T>{
		private:
			typedef reverse_iterator my_type;
		private:
			T* Begin;
			size_type Pos;
			size_type Size;
		protected:
			virtual void increment()/*C++0x_override/**/{Pos=(Pos+Size-1)%Size;}
			virtual void decrement()/*C++0x_override/**/{Pos=(Pos+1)%Size;}
			virtual void forward_add(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size-(num_%Size))%Size;}
			virtual void forward_sub(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size+(num_%Size))%Size;}
			virtual const value_type& refer()const /*C++0x_override/**/{return Begin[(Pos+Size-1)%Size];}
			virtual const value_type& index(const difference_type& pos_)const /*C++0x_override/**/{
				pos_=(Pos+Size-1-(pos_%Size))%Size;
				return Begin[pos_];
			}
			virtual const value_type* access()const /*C++0x_override/**/{return Begin+Pos-1;}
			virtual bool forward_equal(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos==itr_.Begin+itr_.Pos;}
			virtual bool forward_less(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos>itr_.Begin+itr_.Pos;}
			virtual bool forward_more(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos<itr_.Begin+itr_.Pos;}
			virtual difference_type forward_biex_sub(const my_type& itr_)const /*C++0x_override/**/{return -itr_.Begin+Begin-itr_.Pos+Pos;}
		public:
			reverse_iterator():Begin(0),Pos(0),Size(0){return;}
			reverse_iterator(T* Begin_,size_type Pos_,size_type Size_):Begin(Begin_),Pos(Pos_),Size(Size_){return;}
			reverse_iterator(const reverse_iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const reverse_iterator& operator=(const reverse_iterator& itr_){
				if(this!=&itr_){
					Begin=itr_.Begin;
					Pos=itr_.Pos;
					Size=itr_.Size;
				}
				return *this;
			}
		};
		class const_reverse_iterator:public pattern::random_access_const_iterator_pattern<const_reverse_iterator,T>{
		private:
			typedef reverse_iterator my_type;
		private:
			const T* Begin;
			size_type Pos;
			size_type Size;
		protected:
			virtual void increment()/*C++0x_override/**/{Pos=(Pos+Size-1)%Size;}
			virtual void decrement()/*C++0x_override/**/{Pos=(Pos+1)%Size;}
			virtual void forward_add(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size-(num_%Size))%Size;}
			virtual void forward_sub(const difference_type& num_)/*C++0x_override/**/{Pos=(Pos+Size+(num_%Size))%Size;}
			virtual const value_type& const_refer()const /*C++0x_override/**/{return Begin[(Pos+Size-1)%Size];}
			virtual const value_type& const_index(const difference_type& pos_)const /*C++0x_override/**/{
				pos_=(Pos+Size-1-(pos_%Size))%Size;
				return Begin[pos_];
			}
			virtual const value_type* const_access()const /*C++0x_override/**/{return Begin+Pos-1;}
			virtual bool forward_equal(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos==itr_.Begin+itr_.Pos;}
			virtual bool forward_less(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos>itr_.Begin+itr_.Pos;}
			virtual bool forward_more(const my_type& itr_)const /*C++0x_override/**/{return Begin+Pos<itr_.Begin+itr_.Pos;}
			virtual difference_type forward_biex_sub(const my_type& itr_)const /*C++0x_override/**/{return -itr_.Begin+Begin-itr_.Pos+Pos;}
		public:
			const_reverse_iterator():Begin(0),Pos(0),Size(0){return;}
			const_reverse_iterator(T* Begin_,size_type Pos_,size_type Size_):Begin(Begin_),Pos(Pos_),Size(Size_){return;}
			const_reverse_iterator(const reverse_iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const_reverse_iterator(const const_reverse_iterator& itr_):Begin(itr_.Begin),Pos(itr_.Pos),Size(itr_.Size){return;}
			const const_reverse_iterator& operator=(const const_reverse_iterator& itr_){
				if(this!=&itr_){
					Begin=itr_.Begin;
					Pos=itr_.Pos;
					Size=itr_.Size;
				}
				return *this;
			}
		};
	protected:
		T* Buf;
		size_type BufSize;
		size_type BufPos;
		size_type Size;
	protected:
		void copy(const bicircular<T>& _ar){
			create(_ar.size_create());
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
		bicircular():BufSize(0),BufPos(0),Size(0){return;}
		explicit bicircular(size_type _bufsize):BufSize(0),BufPos(0),Size(0){create(_bufsize);}
		bicircular(size_type _bufsize,size_type _num):BufSize(0),BufPos(0),Size(0){create(_bufsize,_num);}
		bicircular(size_type _bufsize,size_type _num,const T& _val):BufSize(0),BufPos(0),Size(0){create(_bufsize,_num,_val);}
		bicircular(const bicircular<T>& _ar):BufSize(0),BufPos(0),Size(0){copy(_ar);}
		template<class input_iterator>
		bicircular(input_iterator _start,input_iterator _end):BufSize(0),BufPos(0),Size(0){create(_start,_end);}
		template<class input_iterator>
		bicircular(size_type _bufsize,input_iterator _start,input_iterator _end):BufSize(0),BufPos(0),Size(0){create(_bufsize,_start,_end);}
		virtual ~bicircular(){release();}
		const bicircular<T>& operator=(const bicircular<T>& _ar){
			if(this!=&_ar)copy();
			return *this;
		}
		bool operator==(const bicircular<T>& _ar)const{
			if(size()!=_ar.size())return false;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return false;
				++itr1;
				++itr2;
			}
			return true;
		}
		bool operator!=(const bicircular<T>& _ar)const{return !(*this==_ar);}
		T& operator[](size_type _pos){return Buf[(BufPos+_pos)%BufSize];}
		const T& operator[](size_type _pos)const{return Buf[(BufPos+_pos)%BufSize];}
		void create(size_type _bufsize){
			release();
			BufSize=_bufsize+1;
			Buf=new T[BufSize];
			BufPos=0;
			Size=0;
		}
		void create(size_type _bufsize,size_type _num){_create_int(_bufsize,_num);}
		void create(size_type _bufsize,size_type _num,const T& _val){_create_int(_bufsize,_num,_val);}
		template<class input_iterator>
		void create(input_iterator _start,input_iterator _end){_create_itr(_start,_end,std::is_integral<input_iterator>::type());}
		template<class input_iterator>
		void create(size_type _bufsize,input_iterator _start,input_iterator _end){_create_itr(_bufsize,_start,_end,std::is_integral<input_iterator>::type());}
	private:
		void _create_int(size_type _bufsize,size_type _num){
			create(_bufsize);
			assign(_num);
		}
		void _create_int(size_type _bufsize,size_type _num,const T& _val){
			create(_bufsize);
			assign(_num,_val);
		}
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
		template<class input_iterator>
		void _create_itr(input_iterator _start,input_iterator _end,std::true_type){_create_int(_start,_end);}
		template<class input_iterator>
		void _create_itr(size_type _bufsize,input_iterator _start,input_iterator _end,std::false_type){
			create(_bufsize);
			assign(_start,_end);
		}
		template<class input_iterator>
		void _create_itr(size_type _bufsize,input_iterator _start,input_iterator _end,std::true_type){_create_int(_bufsize,_start,_end);}
	public:
		void release(){
			if(BufSize!=0){
				delete[] Buf;
				BufSize=0;
			}
		}
		bool is_create()const{return BufSize==0;}
		size_type size_create()const{return BufSize-1;}
		void swap(bicircular& _ar){
			T* tmp=Buf;
			Buf=_ar.Buf;
			_ar.Buf=tmp;
			size_type tmpS;
			tmpS=BufSize;
			BufSize=_ar.BufSize;
			_ar.BufSize=tmpS;
			tmpS=BufPos;
			BufPos=_ar.BufPos;
			_ar.BufPos=tmpS;
			tmpS=Size;
			Size=_ar.Size;
			_ar.Size=tmpS;
		}
		void assign(size_type _num){
			if(_num>BufSize)throw "bicircular::assign(size_type _num): out of range";
			BufPos=0;
			Size=_num;
		}
		void assign(size_type _num,const T& _val){_assign_int(_num,_val);}
		template<class input_iterator>
		void assign(input_iterator _start,input_iterator _end){_assign_itr(_start,_end,std::is_integral<input_iterator>::type());}
	private:
		void _assign_int(size_type _num,const T& _val){
			assign(_num);
			iterator itr=begin(),enditr=end();
			for(;itr!=enditr;++itr)(*itr)=_val;
		}
		template<class input_iterator>
		void _assign_itr(input_iterator _start,input_iterator _end,std::true_type){
			_assigne_int(_start,_end);
		}
		template<class input_iterator>
		void _assign_itr(input_iterator _start,input_iterator _end,std::false_type){
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
	public:
		T& at(size_type _pos){
			if(_pos>=Size ||_pos<0)throw "bicircular::at(size_type _pos): out of range";
			return Buf[(BufPos+_pos)%BufSize];
		}
		const T& at(size_type _pos)const{
			if(_pos>=Size ||_pos<0)throw "bicircular::at(size_type _pos): out of range";
			return Buf[(BufPos+_pos)%BufSize];
		}
		T& front(){return at(0);}
		const T& front()const{return at(0);}
		T& back(){return at(Size-1);}
		const T& back()const{return at(Size-1);}
		void clear(){
			BufPos=0;
			Size=0;
		}
		bool empty()const{return size()==0;}
		bool full()const{return rest()==0;}
		size_type rest()const{return size_create()-Size;}
		size_type size()const{return Size;}
		iterator begin(){return iterator(Buf,BufPos,BufSize);}
		iterator end(){return iterator(Buf,(BufPos+Size)%BufSize,BufSize);}
		const_iterator begin()const{return const_iterator(Buf,BufPos,BufSize);}
		const_iterator end()const{return const_iterator(Buf,(BufPos+Size)%BufSize,BufSize);}
		reverse_iterator rbegin(){return reverse_iterator(Buf,(BufPos+Size)%BufSize,BufSize);}
		reverse_iterator rend(){return reverse_iterator(Buf,BufPos,BufSize);}
		const_reverse_iterator rbegin()const{return const_reverse_iterator(Buf,(BufPos+Size)%BufSize,BufSize);}
		const_reverse_iterator rend()const{return const_reverse_iterator(Buf,BufPos,BufSize);}
		void pop_back(){
			if(empty())return;
			--Size;
		}
		void pop_front(){
			if(empty())return;
			BufPos=(BufPos+1)%BufSize;
			--Size;
		}
		void push_back(const T& _val){
			if(full())pop_front();
			++Size;
			back()=_val;
		}
		void push_front(const T& _val){
			if(full())pop_back();
			++Size;
			front()=_val;
		}
	};
	template<class T>
	class circular:private bicircular<T>{
		typedef bicircular<T> base_type;
		typedef typename bicircular<T>::size_type size_type;
	public:
		circular():bicircular<T>(){return;}
		explicit circular(size_type _bufsize):bicircular<T>(_bufsize){return;}
		circular(size_type _bufsize,size_type _num):bicircular<T>(_bufsize,_num){return;}
		circular(size_type _bufsize,size_type _num,const T& _val):bicircular<T>(_bufsize,_num,_val){return;}
		circular(const circular<T>& _ar):bicircular<T>(_ar._Get_container){return;}
		template<class input_iterator>
		circular(input_iterator _start,input_iterator _end):bicircular<T>(_start,_end){return;}
		template<class input_iterator>
		circular(size_type _bufsize,input_iterator _start,input_iterator _end):bicircular<T>(_bufsize,_start,_end){return;}
		void create(size_type _bufsize){base_type::create(_bufsize);}
		void create(size_type _bufsize,size_type _num){base_type::create(_bufsize,_num);}
		void create(size_type _bufsize,size_type _num,const T& _val){base_type::create(_bufsize,_num,_val);}
		template<class input_iterator>
		void create(input_iterator _start,input_iterator _end){base_type::create(_start,_end);}
		template<class input_iterator>
		void create(size_type _bufsize,input_iterator _start,input_iterator _end){base_type::create(_bufsize,_start,_end);}
		void release(){base_type::release();}
		bool is_create()const{return base_type::is_create();}
		size_type size_create()const{return base_type::size_create();}
		void push(const T& _val)const{base_type::push_back(_val);}
		void pop()const{base_type::pop_front();}
		T& front(){return base_type::front();}
		T& back(){return base_type::back();}
		const T& front()const{return base_type::front();}
		const T& back()const{return base_type::back();}
		bool empty()const{return base_type::empty();}
		bool full()const{return base_type::full();}
		size_type rest()const{return base_type::rest();}
		size_type size()const{return base_type::size();}
	};
	template<class T>
	class circular_stack:private bicircular<T>{
		typedef bicircular<T> base_type;
		typedef typename bicircular<T>::size_type size_type;
	public:
		circular_stack():bicircular<T>(){return;}
		explicit circular_stack(size_type _bufsize):bicircular<T>(_bufsize){return;}
		circular_stack(size_type _bufsize,size_type _num):bicircular<T>(_bufsize,_num){return;}
		circular_stack(size_type _bufsize,size_type _num,const T& _val):bicircular<T>(_bufsize,_num,_val){return;}
		circular_stack(const circular_stack<T>& _ar):bicircular<T>(_ar._Get_container){return;}
		template<class input_iterator>
		circular_stack(input_iterator _start,input_iterator _end):bicircular<T>(_start,_end){return;}
		template<class input_iterator>
		circular_stack(size_type _bufsize,input_iterator _start,input_iterator _end):bicircular<T>(_bufsize,_start,_end){return;}
		void create(size_type _bufsize){base_type::create(_bufsize);}
		void create(size_type _bufsize,size_type _num){base_type::create(_bufsize,_num);}
		void create(size_type _bufsize,size_type _num,const T& _val){base_type::create(_bufsize,_num,_val);}
		template<class input_iterator>
		void create(input_iterator _start,input_iterator _end){base_type::create(_start,_end);}
		template<class input_iterator>
		void create(size_type _bufsize,input_iterator _start,input_iterator _end){base_type::create(_bufsize,_start,_end);}
		void release(){base_type::release();}
		bool is_create()const{return base_type::is_create();}
		size_type size_create()const{return base_type::size_create();}
		void push(const T& _val)const{base_type::push_back(_val);}
		void pop()const{base_type::pop_back();}
		T& top(){return base_type::back();}
		const T& top()const{return base_type::back();}
		bool empty()const{return base_type::empty();}
		bool full()const{return base_type::full();}
		size_type rest()const{return base_type::rest();}
		size_type size()const{return base_type::size();}
	};
}
#endif
