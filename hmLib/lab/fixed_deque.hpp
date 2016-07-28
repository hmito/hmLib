#ifndef HMLIB_FIXEDDEQUE_INC
#define HMLIB_FIXEDDEQUE_INC 100
/*=== fixed_deque ===
固定長の簡易配列

=== fixed_deque ===
v1_00/140329 hmIto
作成
*/
#ifndef HMLIB_TYPE_INC
#	include <hmLib/type.h>
#endif
#ifndef HMLIB_EXCEPTIONS_INC
#	include <hmLib/exceptions.hpp>
#endif
namespace hmLib {
	template<typename T>
	class fixed_deque {
		typedef fixed_deque<T> my_type;
	public:
		typedef void(*destructor)(T*);
	private:
		static void default_destructor(T* Ptr) { delete[] Ptr; }
	public:
		//イテレータ
		struct iterator {
			friend struct const_iterator;
		private:
			T* Ptr;
			T* BufBegin;
			T* BufEnd;
		public:
			iterator() :Ptr(0) {}
			iterator(T* Ptr_, T* BufBegin_, T* BufEnd_) :Ptr(Ptr_), BufBegin(BufBegin_), BufEnd(BufEnd_){}
			iterator(const iterator& Itr_) :Ptr(Itr_.Ptr) {}
			const iterator& operator=(const iterator& Itr_) {
				if(this==&Itr_)return *this;
				Ptr=Itr_.Ptr;
				BufBegin=Itr_.BufBegin;
				BufEnd=Itr_.BufEnd;
				return *this;
			}
		public:
			const iterator& operator++() {
				++Ptr;
				if(Ptr==BufEnd)Ptr=BufBegin;
				return *this;
			}
			const iterator& operator--() {
				if(Ptr==BufBegin)Ptr=BufEnd;
				--Ptr;
				return *this;
			}
			iterator operator++(int) {
				iterator ans(*this);
				++(*this);
				return ans;
			}
			iterator operator--(int) {
				iterator ans(*this);
				--(*this);
				return ans;
			}
			T& operator*() {
				return *Ptr;
			}
			T* operator->() {
				return Ptr;
			}
			bool operator!=(const iterator& My_)const {
				return Ptr!=My_.Ptr;
			}
			bool operator==(const iterator& My_)const {
				return Ptr==My_.Ptr;
			}
		};
		//constイテレータ
		struct const_iterator {
			friend struct const_const_iterator;
		private:
			const T* Ptr;
			const T* BufBegin;
			const T* BufEnd;
		public:
			const_iterator() :Ptr(0) {}
			const_iterator(const T* Ptr_, const T* BufBegin_, const T* BufEnd_) :Ptr(Ptr_), BufBegin(BufBegin_), BufEnd(BufEnd_) {}
			const_iterator(const const_iterator& Itr_) :Ptr(Itr_.Ptr) {}
			const const_iterator& operator=(const const_iterator& Itr_) {
				if(this==&Itr_)return *this;
				Ptr=Itr_.Ptr;
				BufBegin=Itr_.BufBegin;
				BufEnd=Itr_.BufEnd;
				return *this;
			}
		public:
			const const_iterator& operator++() {
				++Ptr;
				if(Ptr==BufEnd)Ptr=BufBegin;
				return *this;
			}
			const const_iterator& operator--() {
				if(Ptr==BufBegin)Ptr=BufEnd;
				--Ptr;
				return *this;
			}
			const const_iterator& operator++(int) {
				const_iterator ans(*this);
				++(*this);
				return ans;
			}
			const const_iterator& operator--(int) {
				const_iterator ans(*this);
				--(*this);
				return ans;
			}
			const T& operator*() {
				return *Ptr;
			}
			const T* operator->() {
				return Ptr;
			}
			bool operator!=(const const_iterator& My_)const {
				return Ptr!=My_.Ptr;
			}
			bool operator==(const const_iterator& My_)const {
				return Ptr==My_.Ptr;
			}
		};
	private:
		T* BufBegin;
		T* BufEnd;
		T* Begin;
		T* End;
		unsigned int Size;
		destructor Destructor;
	public:
		//コンストラクタ
		fixed_deque()
			: BufBegin(0)
			, BufEnd(0)
			, Begin(0)
			, End(0)
			, Size(0)
			, Destructor(0) {
		}
		fixed_deque(unsigned int Size_)
			: BufBegin(0)
			, BufEnd(0)
			, Begin(0)
			, End(0)
			, Size(0)
			, Destructor(0) {
			if(Size_!=0) {
				BufBegin=new T[Size_+1];
				BufEnd=BufBegin+Size_;
				Begin=BufBegin;
				End=BufBegin;
				Destructor=default_destructor;
			}
		}
		fixed_deque(T* BufBegin_, T* BufEnd_, destructor Destructor_)
			: BufBegin(0)
			, BufEnd(0)
			, Begin(0)
			, End(0)
			, Size(0)
			, Destructor(0) {
			if(Begin!=0) {
				BufBegin=BufBegin_;
				BufEnd=BufEnd_;
				Begin=BufBegin;
				End=BufBegin;
				Destructor=Destructor_;
			}
		}
		~fixed_deque() {
			if(BufBegin && Destructor)Destructor(BufBegin);
		}
	private:
		//コピー禁止
		fixed_deque(const my_type& My_);
		const my_type& operator=(const my_type& My_);
	public:
		//バッファを再確保
		void reset(unsigned int Size_) {
			this->~fixed_deque();
			this->fixed_deque(Size_);
		}
		void reset(T* BufBegin_, T* BufEnd_, destructor Destructor_) {
			this->~fixed_deque();
			this->fixed_deque(BufBegin_, BufEnd_, Destructor_);
		}
		//バッファが空か
		bool empty()const { return Begin==End; }
		//バッファがいっぱいか
		bool full()const { return begin()==(++end());}
		//バッファサイズ
		unsigned int size()const {
			return Size;
		}
		//最大サイズ
		unsigned int maxsize()const {
			return BufEnd-BufBegin;
		}
		//バッファの確保の有無
		operator bool()const {
			return BufBegin!=0;
		}
		//スワップ
		void swap(fixed_deque& My_) {
			T* tmpBufBegin=BufBegin;
			T* tmpBufEnd=BufEnd;
			T* tmpBegin=Begin;
			T* tmpEnd=End;
			destructor tmpDestructor=Destructor;

			BufBegin=My_.BufBegin;
			BufEnd=My_.BufEnd;
			Begin=My_.Begin;
			End=My_.End;
			Destructor=My_.Destructor;

			My_.BufBegin=tmpBufBegin;
			My_.BufEnd=tmpBufEnd;
			My_.Begin=tmpBegin;
			My_.End=tmpEnd;
			My_.Destructor=tmpDestructor;
		}
		//クリア
		void clear() {
			Begin=BufBegin;
			End=BufBegin;
		}
	public:
		void push_front(const T& val_) {
			hmLib_assert_any(!full(), "");
			if(Begin==BufBegin)Begin=BufEnd;
			--Begin;
			*Begin = val_;
			++Size;
		}
		void pop_front() {
			hmLib_assert_any(!empty(), "");
			++Begin;
			if(Begin==BufEnd)Begin=BufBegin;
			--Size;
		}
		void push_back(const T& val_) {
			hmLib_assert_any(!full(), "");
			*End = val_;
			++End;
			if(End==BufEnd)End=BufBegin;
			++Size;
		}
		void pop_back() {
			hmLib_assert_any(!empty(), "");
			if(End==BufBegin)End=BufEnd;
			--End;
			--Size;
		}
	public:
		//先頭アドレスを取得
		T* data() { return BufBegin; }
		//先頭のデータを取得
		T& front() {
			hmLib_assert_any(!empty(), "out of range");
			return *Begin;
		}
		const T& front()const {
			hmLib_assert_any(!empty(), "out of range");
			return *Begin;
		}
		//末尾のデータを取得
		T& back() {
			hmLib_assert_any(End>BufBegin, "out of range");
			return *(--end());
		}
		const T& back()const {
			hmLib_assert_any(End>BufBegin, "out of range");
			return *(--end());
		}
		//イテレータを取得
		iterator begin() {
			return iterator(Begin,BufBegin,BufEnd);
		}
		iterator end() {
			return iterator(End, BufBegin, BufEnd);
		}
		const_iterator begin()const {
			return const_iterator(Begin, BufBegin, BufEnd);
		}
		const_iterator end()const {
			return const_iterator(End, BufBegin, BufEnd);
		}
	};
}
#
#endif
