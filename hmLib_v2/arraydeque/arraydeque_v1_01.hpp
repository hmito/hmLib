#ifndef INC_HMLIB_ARRAYDEQUE
#define INC_HMLIB_ARRAYDEQUE 101
#ifndef INC_HMLIB_ARRAY
#	include "hmLib_v2/array.hpp"
#endif
namespace hmstd{
	template<class T>
	class arraydeque{
	public:
		typedef unsigned int size_type;
		class const_iterator;
		class const_reverse_iterator;
		class iterator{
		private:
			T* pos;
			T* front;
			T* back;
		public:
			iterator(){return;}
			iterator(T* _pos, T* _front, T* _back){
				pos=_pos;
				front=_front;
				back=_back;
			}
			iterator(const iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			bool operator==(const iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const iterator& _itr)const{return pos!=_itr.pos;}
			iterator& operator++(){
				if(pos==back)pos=front;
				else ++pos;
				return *this;
			}
			iterator& operator--(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			iterator operator++(int){
				T* tmp=pos;
				if(pos==back)pos=front;
				else ++pos;
				return iterator(pos,front,back);
			}
			iterator operator--(int){
				T* tmp=pos;
				if(pos==front)pos=back;
				else --pos;
				return iterator(pos,front,back);
			}
			iterator operator+(int _ref){
				iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(back-pos))itr=pos+_ref;
					else itr=front-1+_ref-static_cast<int>(back-pos);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(front-pos))itr=pos+_ref;
					else itr=back+1+_ref-static_cast<int>(front-pos);
				}
				return itr;	
			}
			iterator operator-(int _ref){
				iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(pos-front))itr=pos-_ref;
					else itr=back+1-_ref+static_cast<int>(pos-front);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(pos-back))itr=pos-_ref;
					else itr=front-1-_ref+static_cast<int>(pos-back);
				}
				return itr;	
			}
			T& operator[](int _ref){return *((*this)+_ref);}
			T& operator*()const{return *pos;}
			T* operator->()const{return pos;}
			friend const_iterator;
		};
		class reverse_iterator{
		private:
			T* pos;
			T* front;
			T* back;
		public:
			reverse_iterator(){return;}
			reverse_iterator(T* _pos, T* _front, T* _back){
				pos=_pos;
				front=_front;
				back=_back;
			}
			reverse_iterator(const reverse_iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			bool operator==(const reverse_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const reverse_iterator& _itr)const{return pos!=_itr.pos;}
			reverse_iterator& operator--(){
				if(pos==back)pos=front;
				else ++pos;
				return *this;
			}
			reverse_iterator& operator++(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			reverse_iterator operator--(int){
				T* tmp=pos;
				if(pos==back)pos=front;
				else ++pos;
				return reverse_iterator(pos,front,back);
			}
			reverse_iterator operator++(int){
				T* tmp=pos;
				if(pos==front)pos=back;
				else --pos;
				return reverse_iterator(pos,front,back);
			}
			reverse_iterator operator-(int _ref){
				reverse_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(back-pos))itr=pos+_ref;
					else itr=front-1+_ref-static_cast<int>(back-pos);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(front-pos))itr=pos+_ref;
					else itr=back+1+_ref-static_cast<int>(front-pos);
				}
				return itr;	
			}
			reverse_iterator operator+(int _ref){
				reverse_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(pos-front))itr=pos-_ref;
					else itr=back+1-_ref+static_cast<int>(pos-front);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(pos-back))itr=pos-_ref;
					else itr=front-1-_ref+static_cast<int>(pos-back);
				}
				return itr;	
			}
			T& operator[](int _ref){return *((*this)+_ref);}
			T& operator*()const{return *pos;}
			T* operator->()const{return pos;}
			friend const_reverse_iterator;
		};
		class const_iterator{
		private:
			const T* pos;
			const T* front;
			const T* back;
		public:
			const_iterator(){return;}
			const_iterator(const T* _pos, const T* _front, const T* _back){
				pos=_pos;
				front=_front;
				back=_back;
			}
			const_iterator(const iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			const_iterator(const const_iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			bool operator==(const const_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const const_iterator& _itr)const{return pos!=_itr.pos;}
			const_iterator& operator++(){
				if(pos==back)pos=front;
				else ++pos;
				return *this;
			}
			const_iterator& operator--(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			const_iterator operator++(int){
				const T* tmp=pos;
				if(pos==back)pos=front;
				else ++pos;
				return const_iterator(pos,front,back);
			}
			const_iterator operator--(int){
				const T* tmp=pos;
				if(pos==front)pos=back;
				else --pos;
				return const_iterator(pos,front,back);
			}
			const_iterator operator+(int _ref){
				const_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(back-pos))itr=pos+_ref;
					else itr=front-1+_ref-static_cast<int>(back-pos);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(front-pos))itr=pos+_ref;
					else itr=back+1+_ref-static_cast<int>(front-pos);
				}
				return itr;	
			}
			const_iterator operator-(int _ref){
				const_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(pos-front))itr=pos-_ref;
					else itr=back+1-_ref+static_cast<int>(pos-front);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(pos-back))itr=pos-_ref;
					else itr=front-1-_ref+static_cast<int>(pos-back);
				}
				return itr;	
			}
			const T& operator[](int _ref){return *((*this)+_ref);}
			const T& operator*()const{return *pos;}
			const T* operator->()const{return pos;}
		};
		class const_reverse_iterator{
		private:
			const T* pos;
			const T* front;
			const T* back;
		public:
			const_reverse_iterator(){return;}
			const_reverse_iterator(const T* _pos, const T* _front, const T* _back){
				pos=_pos;
				front=_front;
				back=_back;
			}
			const_reverse_iterator(const reverse_iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			const_reverse_iterator(const const_reverse_iterator& _itr){
				pos=_itr.pos;
				front=_itr.front;
				back=_itr.back;
			}
			bool operator==(const const_reverse_iterator& _itr)const{return pos==_itr.pos;}
			bool operator!=(const const_reverse_iterator& _itr)const{return pos!=_itr.pos;}
			const_reverse_iterator& operator--(){
				if(pos==back)pos=front;
				else ++pos;
				return *this;
			}
			const_reverse_iterator& operator++(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			const_reverse_iterator operator--(int){
				const T* tmp=pos;
				if(pos==back)pos=front;
				else ++pos;
				return const_reverse_iterator(pos,front,back);
			}
			const_reverse_iterator operator++(int){
				const T* tmp=pos;
				if(pos==front)pos=back;
				else --pos;
				return const_reverse_iterator(pos,front,back);
			}
			const_reverse_iterator operator-(int _ref){
				const_reverse_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(back-pos))itr=pos+_ref;
					else itr=front-1+_ref-static_cast<int>(back-pos);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(front-pos))itr=pos+_ref;
					else itr=back+1+_ref-static_cast<int>(front-pos);
				}
				return itr;	
			}
			const_reverse_iterator operator+(int _ref){
				const_reverse_iterator itr(*this);
				if(_ref>0){
					if(_ref<=static_cast<int>(pos-front))itr=pos-_ref;
					else itr=back+1-_ref+static_cast<int>(pos-front);
				}else if(_ref<0){
					if(_ref>=static_cast<int>(pos-back))itr=pos-_ref;
					else itr=front-1-_ref+static_cast<int>(pos-back);
				}
				return itr;	
			}
			const T& operator[](int _ref){return *((*this)+_ref);}
			const T& operator*()const{return *pos;}
			const T* operator->()const{return pos;}
		};
	protected:
		T* Buf;
		size_type BufSize;
		size_type BufPos;
		size_type Size;
	protected:
		void copy(const arraydeque<T>& _ar){
			assignArr(_ar.sizeArr());
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
		arraydeque():BufSize(0),BufPos(0),Size(0){return;}
		arraydeque(size_type _bufsize):BufSize(0),BufPos(0),Size(0){assignArr(_bufsize);}
		arraydeque(size_type _bufsize,size_type _num):BufSize(0),BufPos(0),Size(0){assignArr(_bufsize,_num);}
		arraydeque(size_type _bufsize,size_type _num,const T& _val):BufSize(0),BufPos(0),Size(0){assignArr(_bufsize,_num,_val);}
		arraydeque(const arraydeque& _ar):BufSize(0),BufPos(0),Size(0){copy(_ar);}
		template<class input_iterator>
		arraydeque(input_iterator _start,input_iterator _end):BufSize(0),BufPos(0),Size(0){assignArr(_start,_end);}
		template<class input_iterator>
		arraydeque(size_type _bufsize,input_iterator _start,input_iterator _end):BufSize(0),BufPos(0),Size(0){assignArr(_bufsize,_start,_end);}
		virtual ~arraydeque(){clearArr();}
		const arraydeque<T>& operator=(const arraydeque<T>& _ar){
			if(this!=&_ar)copy();
			return *this;
		}
		bool operator==(const arraydeque& _ar)const{
			if(size()!=_ar.size())return false;
			iterator itr1=begin(),itr2=_ar.begin();
			while(itr1!=end()){
				if((*itr1)!=(*itr2))return false;
				++itr1;
				++itr2;
			}
			return true;
		}
		bool operator!=(const arraydeque& _ar)const{return !(*this!=_ar);}
		T& operator[](size_type _pos){return Buf[(BufPos+_pos)%(BufSize+1)];}
		const T& operator[](size_type _pos)const{return Buf[(BufPos+_pos)%(BufSize+1)];}
		void assignArr(size_type _bufsize){
			clearArr();
			Buf=new T[_bufsize+1];
			BufSize=_bufsize;
			BufPos=0;
			Size=0;
		}
		void assignArr(size_type _bufsize,size_type _num){
			assignArr(_bufsize);
			assign(_type,_num);
		}
		void assignArr(size_type _bufsize,size_type _num,const T& _val){
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
			itr=_start;
			iterator bitr=begin();
			while(itr!=_end){
				(*bitr)=(*itr);
				++itr;
				++bitr;
			}
		}
		template<class input_iterator>
		void assignArr(size_type _bufsize,input_iterator _start,input_iterator _end){
			assignArr(_bufsize);
			assign(_start,_end);
		}
		void clearArr(){
			if(BufSize!=0){
				delete[] Buf;
				BufSize=0;
			}
		}
		bool emptyArr()const{return BufSize==0;}
		size_type sizeArr()const{return BufSize;}
		void swap(arraydeque& _ar){
			T* tmp=buf;
			buf=_ar.buf;
			_ar.buf=tmp;
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
			if(_num>BufSize)throw "arraydeque::assign(size_type _num): out of range";
			BufPos=0;
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
			if(_pos>=Size ||_pos<0)throw "arraydeque::at(size_type _pos): out of range";
			return Buf[(BufPos+_pos)%(BufSize+1)];
		}
		const T& at(size_type _pos)const{
			if(_pos>=Size ||_pos<0)throw "arraydeque::at(size_type _pos): out of range";
			return Buf[(BufPos+_pos)%(BufSize+1)];
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
		bool full()const{return vacancy()==0;}
		size_type vacancy()const{return BufSize-Size;}
		size_type size()const{return Size;}
		iterator begin(){return iterator(Buf+BufPos,Buf,Buf+BufSize);}
		iterator end(){return iterator(Buf+((BufPos+Size)%(BufSize+1)),Buf,Buf+BufSize);}
		reverse_iterator rbegin(){return reverse_iterator(Buf+((BufPos+Size+BufSize)%(BufSize+1)),Buf,Buf+BufSize);}
		reverse_iterator rend(){return reverse_iterator(Buf+((BufPos+BufSize)%(BufSize+1)),Buf,Buf+BufSize);}
		const_iterator begin()const{return iterator(Buf+BufPos,Buf,Buf+BufSize-1);}
		const_iterator end()const{return iterator(Buf+((BufPos+Size)%(BufSize+1)),Buf,Buf+BufSize);}
		const_reverse_iterator rbegin()const{return reverse_iterator(Buf+((BufPos+Size+BufSize)%(BufSize+1)),Buf,Buf+BufSize);}
		const_reverse_iterator rend()const{return reverse_iterator(Buf+((BufPos+BufSize)%(BufSize+1)),Buf,Buf+BufSize);}
		virtual void pop_back(){
			if(empty())throw "arraydeque::push_back(): out of range";
			--Size;
		}
		virtual void pop_front(){
			if(empty())throw "arraydeque::push_back(): out of range";
			BufPos=(BufPos+1)%(BufSize+1);
			--Size;
		}
		virtual void push_back(const T& _val){
			if(full())throw "arraydeque::push_back(): out of range";
			Buf[(BufPos+Size)%(BufSize+1)]=_val;
			++Size;
		}
		virtual void push_front(const T& _val){
			if(full())throw "arraydeque::push_back(): out of range";
			BufPos=(BufPos+BufSize)%(BufSize+1);
			++Size;
			Buf[BufPos]=_val;
		}
	};
}
#endif
