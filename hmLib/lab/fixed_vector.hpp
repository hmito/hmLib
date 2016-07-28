#ifndef HMLIB_FIXEDVECTOR_INC
#define HMLIB_FIXEDVECTOR_INC 100
/*=== fixed_vector ===
固定長の簡易配列

=== fixed_vector ===
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
	class fixed_vector {
		typedef fixed_vector<T> my_type;
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
		public:
			iterator() :Ptr(0) {}
			iterator(T* Ptr_) :Ptr(Ptr_) {}
			iterator(const iterator& Itr_) :Ptr(Itr_.Ptr) {}
			const iterator& operator=(const iterator& Itr_) {
				if(this==&Itr_)return *this;
				Ptr=Itr_.Ptr;
				return *this;
			}
		public:
			const iterator& operator++() { 
				++Ptr;
				return *this;
			}
			const iterator& operator--() {
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
		private:
			const T* Ptr;
		public:
			const_iterator() :Ptr(0) {}
			const_iterator(const T* Ptr_) :Ptr(Ptr_) {}
			const_iterator(const iterator& Itr_) :Ptr(Itr_.Ptr) {}
			const_iterator(const const_iterator& Itr_) :Ptr(Itr_.Ptr) {}
			const const_iterator& operator=(const const_iterator& Itr_) {
				if(this==&Itr_)return *this;
				Ptr=Itr_.Ptr;
				return *this;
			}
		public:
			const const_iterator& operator++() {
				++Ptr;
				return *this;
			}
			const const_iterator& operator--() {
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
		T* End;
		destructor Destructor;
	public:
		//コンストラクタ
		fixed_vector()
			: BufBegin(0)
			, BufEnd(0)
			, End(0)
			, Destructor(0){
		}
		fixed_vector(unsigned int Size_)
			: BufBegin(0)
			, BufEnd(0)
			, End(0)
			, Destructor(0) {
			if(Size_!=0) {
				BufBegin=new T[Size_];
				BufEnd=BufBegin+Size_;
				End=BufBegin;
				Destructor=default_destructor;
			}
		}
		fixed_vector(T* BufBegin_, T* BufEnd_, destructor Destructor_)
			: BufBegin(0)
			, BufEnd(0)
			, End(0)
			, Destructor(0){
			if(Begin!=0) {
				BufBegin=BufBegin_;
				BufEnd=BufEnd_;
				End=BufBegin;
				Destructor=Destructor_;
			}
		}
		~fixed_vector() {
			if(BufBegin && Destructor)Destructor(BufBegin);
		}
	private:
		//コピー禁止
		fixed_vector(const my_type& My_);
		const my_type& operator=(const my_type& My_);
	public:
		//バッファを再確保
		void reset(unsigned int Size_) {
			this->~fixed_vector();
			this->fixed_vector(Size_);
		}
		void reset(T* BufBegin_, T* BufEnd_, destructor Destructor_) {
			this->~fixed_vector();
			this->fixed_vector(BufBegin_,BufEnd_,Destructor_);
		}
		//バッファが空か
		bool empty()const { return BufBegin==End; }
		//バッファがいっぱいか
		bool full()const { return BufEnd==End; }
		//バッファサイズ
		unsigned int size()const {
			return End-BufBegin;
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
		void swap(fixed_vector& My_) {
			T* tmpBufBegin=BufBegin;
			T* tmpBufEnd=BufEnd;
			T* tmpEnd=End;
			destructor tmpDestructor=Destructor;

			BufBegin=My_.BufBegin;
			BufEnd=My_.BufEnd;
			End=My_.End;
			Destructor=My_.Destructor;

			My_.BufBegin=tmpBufBegin;
			My_.BufEnd=tmpBufEnd;
			My_.End=tmpEnd;
			My_.Destructor=tmpDestructor;
		}
		//クリア
		void clear() {
			End=BufBegin;
		}
	public:
		void push_back(const T& val_) {
			hmLib_assert_any(!full(), "");
			*End = val_;
			++End;
		}
		void pop_back() {
			hmLib_assert_any(!empty(), "");
			--End;
		}
	public:
		//先頭アドレスを取得
		T* data() { return BufBegin; }
		const T* data() const{ return BufBegin; }
		//指定位置のアドレスを取得
		T* get(unsigned int Pos_=0) {
			hmLib_assert_any(End-BufBegin>Pos_, "out of range");
			return BufBegin+Pos_;
		}
		const T* get(unsigned int Pos_=0)const {
			hmLib_assert_any(End-BufBegin>Pos_, "out of range");
			return BufBegin+Pos_;
		}
		//指定位置のデータを取得
		T& at(unsigned int Pos_) {
			hmLib_assert_any(End-BufBegin>Pos_, "out of range");
			return BufBegin[Pos_];
		}
		const T& at(unsigned int Pos_)const {
			hmLib_assert_any(End-BufBegin>Pos_, "out of range");
			return BufBegin[Pos_];
		}
		T& operator[](unsigned int Pos_) {
			return at(Pos_);
		}
		const T& operator[](unsigned int Pos_)const {
			return at(Pos_);
		}
		//末尾のデータを取得
		T& back() {
			hmLib_assert_any(End>BufBegin, "out of range");
			return End[-1];
		}
		const T& back()const {
			hmLib_assert_any(End>BufBegin, "out of range");
			return End[-1];
		}
		//イテレータを取得
		iterator begin() {
			return iterator(BufBegin);
		}
		iterator end() {
			return iterator(End);
		}
		const_iterator begin()const {
			return const_iterator(BufBegin);
		}
		const_iterator end()const {
			return const_iterator(End);
		}
	};
}
#
#endif
