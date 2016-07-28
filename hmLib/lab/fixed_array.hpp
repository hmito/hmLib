#ifndef HMLIB_FIXEDARRAY_INC
#define HMLIB_FIXEDARRAY_INC 100
/*=== fixed_array ===
固定長の簡易配列

=== fixed_array ===
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
	class fixed_array {
		typedef fixed_array<T> my_type;
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
		T* Begin;
		T* End;
		destructor Destructor;
	public:
		//コンストラクタ
		fixed_array()
			: Begin(0)
			, End(0)
			, Destructor(0){
		}
		fixed_array(unsigned int Size_)
			: Begin(0)
			, End(0)
			, Destructor(0) {
			if(Size_!=0) {
				Begin=new T[Size_];
				End=Begin+Size_;
				Destructor=default_destructor;
			}
		}
		fixed_array(T* Begin_, T* End_, destructor Destructor_)
			: Begin(0)
			, End(0)
			, Destructor(0){
			if(Begin!=0) {
				Begin=Begin_;
				End=End_;
				Destructor=Destructor_;
			}
		}
		~fixed_array() {
			if(Begin && Destructor)Destructor(Begin);
		}
	private:
		//コピー禁止
		fixed_array(const my_type& My_);
		const my_type& operator=(const my_type& My_);
	public:
		//バッファを再確保
		void reset(unsigned int Size_) {
			this->~fixed_array();
			this->fixed_array(Size_);
		}
		void reset(T* Begin_, T* End_, destructor Destructor_) {
			this->~fixed_array();
			this->fixed_array(Begin_,End_,Destructor_);
		}
		//バッファサイズ
		unsigned int size()const {
			return End-Begin;
		}
		//バッファの確保の有無
		operator bool()const {
			return Begin!=0;
		}
		//スワップ
		void swap(fixed_array& My_) {
			T* tmpBegin=Begin;
			T* tmpEnd=End;
			destructor tmpDestructor=Destructor;

			Begin=My_.Begin;
			End=My_.End;
			Destructor=My_.Destructor;

			My_.Begin=tmpBegin;
			My_.End=tmpEnd;
			My_.Destructor=tmpDestructor;
		}
	public:
		//指定位置のアドレスを取得
		T* get(unsigned int Pos_=0) {
			hmLib_assert_any(End-Begin>Pos_, "out of range");
			return Begin+Pos_;
		}
		const T* get(unsigned int Pos_=0)const {
			hmLib_assert_any(End-Begin>Pos_, "out of range");
			return Begin+Pos_;
		}
		//指定位置のデータを取得
		T& at(unsigned int Pos_) {
			hmLib_assert_any(End-Begin>Pos_, "out of range");
			return Begin[Pos_];
		}
		const T& at(unsigned int Pos_)const {
			hmLib_assert_any(End-Begin>Pos_, "out of range");
			return Begin[Pos_];
		}
		T& operator[](unsigned int Pos_) {
			return at(Pos_);
		}
		const T& operator[](unsigned int Pos_)const {
			return at(Pos_);
		}
		//イテレータを取得
		iterator begin() {
			return iterator(Begin);
		}
		iterator end() {
			return iterator(End);
		}
		const_iterator begin()const {
			return const_iterator(Begin);
		}
		const_iterator end()const {
			return const_iterator(End);
		}
	};
}
#
#endif
