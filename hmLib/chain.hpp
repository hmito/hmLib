#include<functional>
namespace hmLib{
	template<typename T>
	struct chain{
	public:
		using this_type = chain<T>;
		using size_type = unsigned int;
	public:
		struct element{
			friend struct chain<T>;
		private:
			T value;
			element *prev;
			element *next;
		public:
			element() :prev(0), next(0){}
			explicit element(const T& value_) :value(value_), prev(0), next(0){}
			element(const element& Elm) :value(Elm.value), prev(0), next(0){}
			element& operator=(const element& Elm){
				if(this != &Elm){
					value = Elm.value;
				}
				return *this;
			}
		public:
			operator bool(){ return next == 0; }
			T& operator*(){ return value; }
			const T& operator*()const{ return value; }
			T* operator->(){ return &value; }
			const T* operator->()const{ return &value; }
		public:
			static void connect(element& Prev, element& Next){
				Prev.next = &Next;
				Next.prev = &Prev;
			}
		};
	public:
		struct iterator:public std::iterator<std::bidirectional_iterator_tag,T>{
		private:
			element* Cur;
		public:
			iterator():Cur(0){}
			explicit iterator(element* Cur_):Cur(Cur_){}
			iterator(const iterator& Other):Cur(Other.Cur){}
			iterator& operator=(const iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			iterator& operator--(){
				Cur = Cur->prev;
				return *this;
			}
			iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			T& operator*(){ return Cur->operator*(); }
			const T& operator*()const{ return Cur->operator*(); }
			T* operator->(){ return Cur->operator->(); }
			const T* operator->()const{ return Cur->operator->(); }
			friend bool operator==(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current(){ return Cur; }
		};
		struct const_iterator :public std::iterator<std::bidirectional_iterator_tag, const T>{
		private:
			element* Cur;
		public:
			const_iterator() :Cur(0){}
			explicit const_iterator(element* Cur_) :Cur(Cur_){}
			const_iterator(const iterator& Other) :Cur(Other.current()){}
			const_iterator(const const_iterator& Other) :Cur(Other.Cur){}
			const_iterator& operator=(const iterator& Other){
				if(this != &Other){
					Cur = Other.Cur;
				}
				return *this;
			}
		public:
			const_iterator& operator++(){
				Cur = Cur->next;
				return *this;
			}
			const_iterator operator++(int){
				iterator Ans(*this);
				this->operator++();
				return Ans;
			}
			const_iterator& operator--(){
				Cur = Cur->prev;
				return *this;
			}
			const_iterator operator--(int){
				iterator Ans(*this);
				this->operator--();
				return Ans;
			}
			const T& operator*()const{ return Cur->operator*(); }
			const T* operator->()const{ return Cur->operator->(); }
			friend bool operator==(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(const iterator& Itr1, const iterator& Itr2){
				return Itr1.Cur != Itr2.Cur;
			}
		public:
			element* current(){ return Cur; }
		};
	private:
		element Sentinel;
		size_type Size;
	public:
		chain():Size(0){
			Sentinel.next = &Sentinel;
			Sentinel.prev= &Sentinel;
		}
	public:
		iterator begin(){ return iterator(Sentinel.next); }
		iterator end(){ return iterator(&Sentinel); }
		const_iterator cbegin()const{ return const_iterator(Sentinel.next); }
		const_iterator cend()const{ return const_iterator(&Sentinel); }
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty(){ return Size==0; }
		size_type size(){ return Size; }
		T& front(){ return *(Sentinel.next); }
		T& back(){ return *(Sentinel.prev); }
		const T& front()const{ return *(Sentinel.next); }
		const T& back()const{ return *(Sentinel.prev); }
		void push_back(element& Elem){
			element& Prev = *(Sentinel.prev);
			element::connect(Prev, Elem);
			element::connect(Elem, Sentinel);
		}
		void pop_back(){
			element& Prev = *(Sentinel.prev);
			element::connect(*(Prev.prev), Sentinel);
			Prev.next = 0;
			Prev.prev = 0;
		}
		void push_front(element& Elem){
			element& Next = *(Sentinel.next);
			element::connect(Sentinel, Elem);
			element::connect(Elem, Next);
		}
		void pop_front(){
			element& Next = *(Sentinel.next);
			element::connect(Sentinel, *(Next.next));
			Next.next = 0;
			Next.prev = 0;
		}
		iterator insert(const_iterator pos, element& Elem){
			element& Curr = pos.current();
			element::connect(*(Curr.prev), Elem);
			element::connect(Elem, Curr);
		}
		iterator erase(const_iterator pos){
			element& Curr = pos.current();
			element::connect(*(Curr.prev), *(Curr.next));
			Curr.next = 0;
			Curr.prev = 0;
		}
		iterator erase(const_iterator pos, const_iterator last){
			element& Prev = *(pos.current().prev);

			for(; pos != last; ++pos){
				element& Curr = pos.current();
				Curr.next = 0;
				Curr.prev = 0;
			}

			element::connect(Prev, last.current());
		}
		void swap(this_type& other){
			std::swap(Sentinel.prev, other.Sentinel.prev);
			std::swap(Sentinel.next, other.Sentinel.next);
		}
		void clear(){
			erase(begin(), end());
		}
		void splice(const_iterator pos, this_type& other){
			if(other.empty())return;

			element& Prev = *(pos.current().prev);
			element& Curr = *(pos.current());

			element::connect(Prev, *(other.Sentinel.next));
			element::connect(*(other.Sentinel.prev), Curr);
			other.Sentinel.next = &(other.Sentinel);
			other.Sentinel.prev = &(other.Sentinel);
		}
	};
	template<typename T, typename Comp = std::less<T>>
	struct priority_chain{
	public:
		using this_type = chain<T>;
		using size_type = unsigned int;
	public:
		struct element{
		private:
			T value;
			element *next;
		public:
			operator bool(){ return next != 0; }
			T& operator*(){ return value; }
			const T& operator*()const{ return value; }
			T* operator->(){ return &value; }
			const T* operator->()const{ return &value; }
		};
		struct iterator{};
		struct const_iterator{};
	private:
		element Sentinel;
	public:
		iterator begin();
		iterator end();
		const_iterator cbegin()const;
		const_iterator cend()const;
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
	public:
		bool empty();
		T& front();
		const T& front()const;
		void push(element& Elem);
		void pop();
		void swap(this_type& other);
		void clear();
	};
}
