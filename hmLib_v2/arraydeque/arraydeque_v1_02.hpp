#ifndef INC_HMLIB_ARRAYDEQUE
#define INC_HMLIB_ARRAYDEQUE 102
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
		class iterator:public hm::basic_random_access_iterator<T*,T>{
		private:
			T* first;
			T* front;
			T* back;
		protected:
			virtual void move_front(){
				if(B==back)B=front;
				else ++B;
			}
			virtual void move_front(int _num){
				if(_num>0){
					if(B+_num<=back)B+=_num;
					else B+=-(back-front+1)+_num;
				}else if(_num<0){
					if(B+_num>=front)B+=_num;
					else B+=(back-front+1)+_num;
				}
			}
			virtual void move_back(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			virtual void move_back(int _num){
				if(_num>0){
					if(B-_num>=front)B-=_num;
					else B+=(back-front+1)-_num;
				}else if(_num<0){
					if(B-_num<=back)B-=_num;
					else B+=-(back-front+1)-_num;
				}
			}
			virtual bool less(const basic_random_access_iterator<T*,T>& _itr)const{
				if(first<=B){
					if(first<=_itr.getBase()){
						return B<_itr.getBase();
					}else{
						return true;
					}
				}else{
					if(first<=_itr.getBase()){
						return false;
					}else{
						return B<_itr.getBase();
					}
				}
			}
			virtual T& access(int _num)const{
				if(_num>0){
					if(B+_num<=back)return *(B+_num);
					else return *(B-(back-front+1)+_num);
				}else if(_num<0){
					if(B+_num>=front)return *(B+_num);
					else return *(B+(back-front+1)+_num);
				}
				return *B;
			}
		public:
			iterator(){return;}
			iterator(T* _pos, T* _front, T* _back,T* _first){
				B=_pos;
				front=_front;
				back=_back;
				first=_first;
			}
			iterator(const iterator& _itr){
				B=_itr.B;
				front=_itr.front;
				back=_itr.back;
				first=_itr.first;
			}
			const iterator& operator=(const iterator& _itr){
				if(this!=&_itr){
					B=_itr.B;
					front=_itr.front;
					back=_itr.back;
					first=_itr.first;
				}
				return *this;
			}
		};
		class const_iterator:public hm::basic_random_access_iterator<const T*,const T>{
		private:
			const T* first;
			const T* front;
			const T* back;
		protected:
			virtual void move_front(){
				if(B==back)B=front;
				else ++B;
			}
			virtual void move_front(int _num){
				if(_num>0){
					if(B+_num<=back)B+=_num;
					else B+=-(back-front+1)+_num;
				}else if(_num<0){
					if(B+_num>=front)B+=_num;
					else B+=(back-front+1)+_num;
				}
			}
			virtual void move_back(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			virtual void move_back(int _num){
				if(_num>0){
					if(B-_num>=front)B-=_num;
					else B+=(back-front+1)-_num;
				}else if(_num<0){
					if(B-_num<=back)B-=_num;
					else B+=-(back-front+1)-_num;
				}
			}
			virtual bool less(const basic_random_access_iterator<const T*,const T>& _itr)const{
				if(first<=B){
					if(first<=_itr.getBase()){
						return B<_itr.getBase();
					}else{
						return true;
					}
				}else{
					if(first<=_itr.getBase()){
						return false;
					}else{
						return B<_itr.getBase();
					}
				}
			}
			virtual const T& access(int _num)const{
				if(_num>0){
					if(B+_num<=back)return *(B+_num);
					else return *(B-(back-front+1)+_num);
				}else if(_num<0){
					if(B+_num>=front)return *(B+_num);
					else return *(B+(back-front+1)+_num);
				}
				return *B;
			}
		public:
			const_iterator(){return;}
			const_iterator(const T* _pos, const T* _front, const T* _back,const T* _first){
				B=_pos;
				front=_front;
				back=_back;
				first=_first;
			}
			const_iterator(const const_iterator& _itr){
				B=_itr.B;
				front=_itr.front;
				back=_itr.back;
				first=_itr.first;
			}
			const const_iterator& operator=(const const_iterator& _itr){
				if(this!=&_itr){
					B=_itr.B;
					front=_itr.front;
					back=_itr.back;
					first=_itr.first;
				}
				return *this;
			}
		};
		class reverse_iterator:public hm::basic_random_access_iterator<T*,T>{
		private:
			T* first;
			T* front;
			T* back;
		protected:
			virtual void move_back(){
				if(B==back)B=front;
				else ++B;
			}
			virtual void move_back(int _num){
				if(_num>0){
					if(B+_num<=back)B+=_num;
					else B+=-(back-front+1)+_num;
				}else if(_num<0){
					if(B+_num>=front)B+=_num;
					else B+=(back-front+1)+_num;
				}
			}
			virtual void move_front(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			virtual void move_front(int _num){
				if(_num>0){
					if(B-_num>=front)B-=_num;
					else B+=(back-front+1)-_num;
				}else if(_num<0){
					if(B-_num<=back)B-=_num;
					else B+=-(back-front+1)-_num;
				}
			}
			virtual bool less(const basic_random_access_iterator<T*,T>& _itr)const{
				if(first<=B){
					if(first<=_itr.getBase()){
						return B>_itr.getBase();
					}else{
						return false;
					}
				}else{
					if(first<=_itr.getBase()){
						return true;
					}else{
						return B>_itr.getBase();
					}
				}
			}
			virtual T& access(int _num)const{
				if(_num>0){
					if(B-_num>=front)return *(B-_num);
					else return *(B+(back-front+1)-_num);
				}else if(_num<0){
					if(B-_num<=back)return *(B-_num);
					else return *(B-(back-front+1)-_num);
				}
				return *B;
			}
		public:
			reverse_iterator(){return;}
			reverse_iterator(T* _pos, T* _front, T* _back,T* _first){
				B=_pos;
				front=_front;
				back=_back;
				first=_first;
			}
			reverse_iterator(const reverse_iterator& _itr){
				B=_itr.B;
				front=_itr.front;
				back=_itr.back;
				first=_itr.first;
			}
			const reverse_iterator& operator=(const reverse_iterator& _itr){
				if(this!=&_itr){
					B=_itr.B;
					front=_itr.front;
					back=_itr.back;
					first=_itr.first;
				}
				return *this;
			}
		};
		class const_reverse_iterator:public hm::basic_random_access_iterator<const T*,const T>{
		private:
			const T* first;
			const T* front;
			const T* back;
		protected:
			virtual void move_back(){
				if(B==back)B=front;
				else ++B;
			}
			virtual void move_back(int _num){
				if(_num>0){
					if(B+_num<=back)B+=_num;
					else B+=-(back-front+1)+_num;
				}else if(_num<0){
					if(B+_num>=front)B+=_num;
					else B+=(back-front+1)+_num;
				}
			}
			virtual void move_front(){
				if(pos==front)pos=back;
				else --pos;
				return *this;
			}
			virtual void move_front(int _num){
				if(_num>0){
					if(B-_num>=front)B-=_num;
					else B+=(back-front+1)-_num;
				}else if(_num<0){
					if(B-_num<=back)B-=_num;
					else B+=-(back-front+1)-_num;
				}
			}
			virtual bool less(const basic_random_access_iterator<const T*,const T>& _itr)const{
				if(first<=B){
					if(first<=_itr.getBase()){
						return B>_itr.getBase();
					}else{
						return false;
					}
				}else{
					if(first<=_itr.getBase()){
						return true;
					}else{
						return B>_itr.getBase();
					}
				}
			}
			virtual const T& access(int _num)const{
				if(_num>0){
					if(B-_num>=front)return *(B-_num);
					else return *(B+(back-front+1)-_num);
				}else if(_num<0){
					if(B-_num<=back)return *(B-_num);
					else return *(B-(back-front+1)-_num);
				}
				return *B;
			}
		public:
			const_reverse_iterator(){return;}
			const_reverse_iterator(const T* _pos, const T* _front, const T* _back,const T* _first){
				B=_pos;
				front=_front;
				back=_back;
				first=_first;
			}
			const_reverse_iterator(const const_reverse_iterator& _itr){
				B=_itr.B;
				front=_itr.front;
				back=_itr.back;
				first=_itr.first;
			}
			const const_reverse_iterator& operator=(const const_reverse_iterator& _itr){
				if(this!=&_itr){
					B=_itr.B;
					front=_itr.front;
					back=_itr.back;
					first=_itr.first;
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
		iterator begin(){return iterator(Buf+BufPos,Buf,Buf+BufSize-1,Buf+BufPos);}
		iterator end(){return iterator(Buf+((BufPos+Size)%(BufSize+1)),Buf,Buf+BufSize-1,Buf+BufPos);}
		reverse_iterator rbegin(){return reverse_iterator(Buf+((BufPos+Size+BufSize)%(BufSize+1)),Buf,Buf+BufSize-1,Buf+((BufPos+Size+BufSize)%(BufSize+1)));}
		reverse_iterator rend(){return reverse_iterator(Buf+((BufPos+BufSize)%(BufSize+1)),Buf,Buf+BufSize-1,Buf+((BufPos+Size+BufSize)%(BufSize+1)));}
		const_iterator begin()const{return iterator(Buf+BufPos,Buf,Buf+BufSize-1,Buf+BufPos);}
		const_iterator end()const{return iterator(Buf+((BufPos+Size)%(BufSize+1)),Buf,Buf+BufSize-1,Buf+BufPos);}
		const_reverse_iterator rbegin()const{return reverse_iterator(Buf+((BufPos+Size+BufSize)%(BufSize+1)),Buf,Buf+BufSize,Buf+((BufPos+Size+BufSize)%(BufSize+1)));}
		const_reverse_iterator rend()const{return reverse_iterator(Buf+((BufPos+BufSize)%(BufSize+1)),Buf,Buf+BufSize,Buf+((BufPos+Size+BufSize)%(BufSize+1)));}
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
