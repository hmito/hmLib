#ifndef HMLIB_CIRCULAR_INC
#define HMLIB_CIRCULAR_INC 100
#
#include<iterator>
#include<type_traits>
#include "aligned_heap.hpp"
#include "algorithm/compare.hpp"
#include "exception.hpp"
#include "exceptions/access_exceptions.hpp"
namespace hmLib{
	template<typename T, unsigned int MaxSize>
	struct circular{
	private:
		using this_type = circular<T, MaxSize> ;
	public:
		using value_type = T ;
		using reference = typename std::add_lvalue_reference<T>::type;
		using rval_reference = typename std::add_rvalue_reference<T>::type;
		using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
		using pointer = typename std::add_pointer<T>::type;
		using const_pointer = typename std::add_pointer<typename std::add_const<T>::type>::type;
		using size_type =  std::size_t;
		using difference_type = int;
	private:
		struct base_type{
		public:
			using value_type = typename this_type::value_type;
			using reference = typename this_type::reference;
			using rval_reference = typename this_type::rval_reference;
			using const_reference =typename this_type::const_reference;
			using pointer = typename this_type::pointer;
			using const_pointer = typename this_type::const_pointer;
			using size_type =  typename this_type::size_type;
			using difference_type = typename this_type::difference_type;
		public:
			base_type():BegPos(0),LasPos(0),Size(0){}
			base_type(const base_type&) = delete;
			const base_type& operator=(const base_type&) = delete;
			base_type(base_type&&) = delete;
			const base_type& operator=(base_type&&) = delete;
			~base_type(){clear();}
		private:
			constexpr static size_type buffer_size(){return MaxSize+1;}
			pointer buffer_begin(){return static_cast<pointer>(static_cast<void*>(Buf));}
			const_pointer buffer_begin()const{return static_cast<pointer>(static_cast<void*>(Buf));}
			pointer buffer_end(){return buffer_begin()+buffer_size();}
			const_pointer buffer_end()const{return buffer_begin()+buffer_size();}
		public:
			constexpr static void increment_pos(size_type& Pos){
				++Pos;
				if(Pos==buffer_size())Pos = 0;
			}
			constexpr static void decrement_pos(size_type& Pos){
				if(Pos==0)Pos = buffer_size();
				--Pos
			}
			constexpr static void void advance_pos(size_type& Pos,difference_type n){Pos = hmLib::euclidean_mod(Pos+n,MaxSize+1);}
			constexpr static void size_type next_pos(size_type Pos,difference_type n){return hmLib::euclidean_mod(Pos+n,MaxSize+1);}
			pointer ptr(size_type Pos){ return buffer_begin()+Pos; }
			const_pointer ptr(size_type Pos)const{ return buffer_begin()+Pos; }
			size_type index_to_pos(difference_type n)const{return hmLib::euclidean_mod(BegPos+n,MaxSize+1);}
			difference_type pos_to_index(size_type Pos)const{
				if(BegPos<=Pos)return Pos-BegPos;
				else Pos + max_size() - BegPos;
			}
			bool is_valid_pos(size_type Pos)const{return Pos < buffer_size() & pos_to_index(Pos)<size();}
			size_type front_pos()const{ return BegPos; }
			size_type back_pos()const{ return LasPos; }
			reference front(){ return buffer_begin()[BegPos]; }
			const_reference front()const{ return buffer_begin()[BegPos]; }
			reference back(){ return buffer_begin()[LasPos]; }
			const_reference back()const{ return buffer_begin()[LasPos]; }
			bool empty()const{ return Size == 0; }
			bool full()const{ return Size == max_size(); }
			size_type size()const{ return Size; }
			static constexpr size_type max_size()const { return MaxSize; }
			size_type remain()const{ return max_size() - size(); }
		public:
			void clear(){
				while(Size>0){
					ptr(LasPos)->~value_type();
					decrement_pos(LasPos);
					--Size;
				}
			}
			bool push_back(const_reference Value){
				if(full())return true;
				increment_pos(LasPos);
				++Size;
				::new(ptr(LasPos)) value_type(Value);
				return false;
			}
			bool push_back(rval_reference Value){
				if(full())return true;
				increment_pos(LasPos);
				++Size;
				::new(ptr(LasPos)) value_type(std::move(Value));
				return false;
			}
			bool push_front(const_reference Value){
				if(full())return true;
				decrement_pos(BegPos);
				++Size;
				::new(ptr(BegPos)) value_type(Value);
				return false;
			}
			bool push_front(rval_reference Value){
				if(full())return true;
				decrement_pos(BegPos);
				++Size;
				::new(ptr(BegPos)) value_type(std::move(Value));
				return false;
			}
			void pop_back(){
				if(empty())return;
				ptr(LasPos)->~value_type();
				decrement_pos(LasPos);
				--Size;
			}
			void pop_front(){
				if(empty())return;
				ptr(BegPos)->~value_type();
				increment_pos(BegPos);
				--Size;
			}
			bool rotete_back(const_reference Value) {
				if(full()) {
					ptr(BegPos)->~value_type();
					increment_pos(BegPos);
					increment_pos(LasPos);
					::new(ptr(LasPos)) value_type(Value);
					return true;
				} else {
					increment_pos(LasPos);
					++Size;
					::new(ptr(LasPos)) value_type(Value);
					return false;
				}
			}
			bool rotete_back(rval_reference Value) {
				if(full()) {
					ptr(BegPos)->~value_type();
					increment_pos(BegPos);
					increment_pos(LasPos);
					::new(ptr(LasPos)) value_type(std::move(Value));
					return true;
				} else {
					increment_pos(LasPos);
					++Size;
					::new(ptr(LasPos)) value_type(std::move(Value));
					return false;
				}
			}
			bool rotete_front(const_reference Value) {
				if(full()) {
					ptr(LasPos)->~value_type();
					decrement_pos(LasPos);
					decrement_pos(BegPos);
					::new(ptr(BegPos)) value_type(Value);
					return true;
				} else {
					if(BegPos==0)BegPos=max_size();
					decrement_pos(BegPos);
					++Size;
					::new(ptr(BegPos)) value_type(Value);
					return false;
				}
			}
			bool rotete_front(rval_reference Value) {
				if(full()) {
					ptr(LasPos)->~value_type();
					decrement_pos(LasPos);
					decrement_pos(BegPos);
					::new(ptr(BegPos)) value_type(std::move(Value));
					return true;
				} else {
					if(BegPos==0)BegPos=max_size();
					decrement_pos(BegPos);
					++Size;
					::new(ptr(BegPos)) value_type(std::move(Value));
					return false;
				}
			}
			size_type insert_by_pos(size_type Pos, const_reference Value){
				if(full())return buffer_size();

				auto FromPos = LasPos;
				increment_pos(LasPos);
				++Size;
				auto ToPos = LasPos;

				if(Pos==ToPos){
					::new(ptr(Pos)) value_type(Value);
				}else{
					::new(ptr(ToPos)) value_type(std::move(*ptr(FromPos)));
					ToPos = FromPos;
					derement_pos(FromPos);

					while(Pos!=ToPos){
						*(ptr(ToPos)) = std::move(*ptr(FromPos));
						ToPos = FromPos;
						derement_pos(FromPos);
					}

					*(ptr(Pos)) = Value;
				}
				return Pos;
			}
			size_type insert_by_pos(size_type Pos, rval_reference Value){
				if(full())return buffer_size();

				auto FromPos = LasPos;
				increment_pos(LasPos);
				++Size;
				auto ToPos = LasPos;

				if(Pos==ToPos){
					::new(ptr(Pos)) value_type(std::move(Value));
				}else{
					::new(ptr(ToPos)) value_type(std::move(*ptr(FromPos)));
					ToPos = FromPos;
					derement_pos(FromPos);

					while(Pos!=ToPos){
						*(ptr(ToPos)) = std::move(*ptr(FromPos));
						ToPos = FromPos;
						derement_pos(FromPos);
					}

					*(ptr(ToPos)) = std::move(Value);
				}
				return Pos;
			}
			template<typename input_iterator>
			size_type insert_by_pos(size_type Pos, input_iterator Beg, input_iterator End){
				auto n = std::distance(Beg,End);
				if(n==0 || n>remain())return buffer_size();

				auto EndPos = next_pos(Pos,n-1);
				auto FromPos = LasPos;
				advance_pos(LasPos,n);
				Size += n;
				auto ToPos = LasPos;

				auto New = n;
				while(EndPos!=ToPos){
					if(New>0){
						::new(ptr(ToPos)) value_type(std::move(*ptr(FromPos)));
						--New;
					}else{
						*(ptr(ToPos)) = std::move(*ptr(FromPos));
					}
					decrement_pos(ToPos);
					decrement_pos(FromPos);
				}

				auto Cpy = n - New;
				ToPos = Pos;
				while(Cpy>0){
						*(ptr(ToPos)) = *Beg;
						increment_pos(ToPos);
						++Beg;
						--Cpy;
				}
				while(New>0){
						::new(ptr(ToPos)) value_type(*Beg);
						increment_pos(ToPos);
						++Beg;
						--New;
				}

				return Pos;
			}
			size_type erase_by_pos(size_type Pos){
				auto ToPos = Pos;
				auto FromPos = Pos;
				increment_pos(FromPos);

				while(ToPos != LasPos){
					*ptr(ToPos) = std::move(*ptr(FromPos));

					ToPos = FromPos;
					increment_pos(FromPos);
				}
				ptr(LasPos)->~value_type();
				decrement_pos(LasPos);
				--Size;

				return Pos;
			}
			size_type erase_by_pos(size_type Beg, size_type End){
				auto n = std::distance(Beg,End);
				if(n==0)return Beg;

				auto ToPos = Beg;
				auto FromPos = End;
				auto NewEndPos = next_pos(LasPos,-n+1);

				while(ToPos != NewEndPos){
					*ptr(ToPos) = std::move(*ptr(FromPos));
					increment_pos(ToPos);
					increment_pos(FromPos);
				}

				auto DstNum = n;
				while(DstNum>0){
					ptr(ToPos)->~value_type();
					increment_pos(ToPos);
					--DstNum;
				}
				advance_pos(LasPos,-n);
				Size -= n;

				return Beg;
			}
		private:
			alignas(alignof(T)) unsigned char Buf[sizeof(T)*buffer_size()];
			size_type BegPos;
			size_type LasPos;
			unsigned int Size;
		};
	public:
		struct const_iterator{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = typename this_type::value_type;
			using difference_type = typename this_type::difference_type;
			using pointer = typename this_type::const_pointer;
			using reference = typename this_type::const_reference;
		private:
			size_type Pos;
			const base_type* Base;
		public:
			const_iterator()=default;
			const_iterator(size_type Pos_, const base_type* Base_): Pos(Pos_), Base(Base_){}
		public:
			const_iterator& operator++(){
				Base->increment_pos(Pos);
				return *this;
			}
			const_iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			const_iterator& operator--(){
				Base->decrement_pos(Pos);
				return *this;
			}
			const_iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			const_iterator& operator+=(difference_type n){
				Base->advance_pos(Pos,n);
				return *this;
			}
			const_iterator& operator-=(difference_type n){
				Base->advance_pos(Pos,-n);
				return *this;
			}
			reference operator*(){ return *(Base->ptr(Pos)); }
			pointer operator->(){ return Base->ptr(Pos); }
			reference operator[](difference_type n){return *(Base->ptr(Base->next_pos(Pos,n)));}
			friend const_iterator operator+(const const_iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend const_iterator operator+(difference_type n, const const_iterator& Itr){
				return operator+(Itr, n);
			}
			friend const_iterator operator-(const const_iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans -= n;
				return Ans;
			}
			friend difference_type operator-(const const_iterator& itr1, const_iterator itr2){
				difference_type pos1 = itr1.Base->pos_to_index(itr1.Pos);
				difference_type pos2 = itr2.Base->pos_to_index(itr2.Pos);
				return pos1 - pos2;
			}
			friend bool operator==(const this_type& itr1, const this_type& itr2){
				return itr1.Pos == itr2.Pos;
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2){
				return !(itr1.Pos == itr2.Pos);
			}
			friend bool operator<(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 < 0
			}
			friend bool operator>(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 > 0
			}
			friend bool operator<=(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 <= 0
			}
			friend bool operator>=(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 >= 0
			}
		};
		struct iterator{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = typename this_type::value_type;
			using difference_type = typename this_type::difference_type;
			using pointer = typename this_type::pointer;
			using reference = typename this_type::reference;
		private:
			size_type Pos;
			base_type* Base;
		public:
			iterator()=default;
			iterator(size_type Pos_, base_type* Base_): Pos(Pos_), Base(Base_){}
			operator const_iterator()const{return const_iterator(Pos,Base);}
		public:
			iterator& operator++(){
				Base->increment_pos(Pos);
				return *this;
			}
			iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			iterator& operator--(){
				Base->decrement_pos(Pos);
				return *this;
			}
			iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			iterator& operator+=(difference_type n){
				Base->advance_pos(Pos,n);
				return *this;
			}
			iterator& operator-=(difference_type n){
				Base->advance_pos(Pos,-n);
				return *this;
			}
			reference operator*(){ return Base->reference_by_pos(Pos); }
			pointer operator->(){ return Base->ptr(Pos); }
			reference operator[](difference_type n){return *(Base->ptr(Base->next_pos(Pos,n)));}
			friend iterator operator+(const iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend iterator operator+(difference_type n, const iterator& Itr){
				return operator+(Itr, n);
			}
			friend iterator operator-(const iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans -= n;
				return Ans;
			}
			friend difference_type operator-(const iterator& itr1, iterator itr2){
				difference_type pos1 = itr1.Base->pos_to_index(itr1.Pos);
				difference_type pos2 = itr2.Base->pos_to_index(itr2.Pos);
				return pos1 - pos2;
			}
			friend bool operator==(const this_type& itr1, const this_type& itr2){
				return itr1.Pos == itr2.Pos;
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2){
				return !(itr1.Pos == itr2.Pos);
			}
			friend bool operator<(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 < 0
			}
			friend bool operator>(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 > 0
			}
			friend bool operator<=(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 <= 0
			}
			friend bool operator>=(const this_type& itr1, const this_type& itr2){
				return itr1 - itr2 >= 0
			}
		};
	public:
		circular():Base(std::make_unique<base_type>()){}
	public:
		reference at(difference_type n){ 
			hmLib_assert(0<=n && n<size(), hmLib::exceptions::access::out_of_range_access, "circular access error: out of range.");
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
		const_reference at(difference_type n)const{
			hmLib_assert(0<=n && n<size(), hmLib::exceptions::access::out_of_range_access, "circular access error: out of range.");
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
		reference operator[](difference_type pos)noexcept{return *(Base->ptr(Base->index_to_pos(n))); }
		const_reference operator[](difference_type pos)const noexcept{return *(Base->ptr(Base->index_to_pos(n))); }
		reference front(){ return Base->front(); }
		const_reference front()const{ return Base->front(); }
		reference back(){ return Base->back(); }
		const_reference back()const{ return Base->back(); }
		bool empty()const{ return (!static_cast<bool>(Base)) || Base->empty();}
		bool full()const{ return static_cast<bool>(Base) && Base->full(); }
		size_type size()const{ if(!static_cast<bool>(Base)) return 0; return Base->size(); }
		static constexpr size_type max_size() { return MaxSize; }
		static constexpr size_type static_size() { return MaxSize; }
		size_type remain()const{ return max_size() - size(); }
	public:
		iterator begin(){ 
			return iterator(Base->front_pos(),Base.get()); 
		}
		iterator end(){ 
			auto EndPos = Base->back_pos();
			Base->increment_pos(EndPos);
			return iterator(EndPos,Base.get()); 
		}
		const_iterator begin()const{ return cbegin(); }
		const_iterator end()const{ return cend(); }
		const_iterator cbegin()const{ 
			return iterator(Base->front_pos(),Base.get()); 
		}
		const_iterator cend()const{ 
			auto EndPos = Base->back_pos();
			Base->increment_pos(EndPos);
			return iterator(EndPos,Base.get()); 
		}
	public:
		bool push_back(const_reference Value){ return Base->push_back(Value); }
		bool push_back(rval_reference Value){ return Base->push_back(std::move(Value));}
		bool push_front(const_reference Value){return Base->push_front(Value);}
		bool push_front(rval_reference Value){return Base->push_back(std::move(Value));}
		void pop_back(){Base->pop_back(Value);}
		void pop_front(){Base->pop_back(Value);}
		bool rotete_back(const_reference Value) {return Base->rotete_back(Value);}
		bool rotete_back(rval_reference Value) {return Base->rotete_back(std::move(Value));}
		bool rotete_front(const_reference Value) {return Base->rotete_front(Value);}
		bool rotete_front(rval_reference Value) {return Base->rotete_front(std::move(Value));}
		void clear(){Base->clear();}
	public:
		iterator insert(const_iterator Itr, const_reference Value){
			if(!Base){
				Base = std::make_unique<base_type>();

			}
			Base->insert_by_pos(Itr.Pos,Value);
		}
		template<typename input_iterator>
		iterator insert(const_iterator Itr, input_iterator Begin, input_iterator End){
			if(!Base)Base = std::make_unique<base_type>();
			Base->insert_by_pos(Itr.Pos,Begin,End);
		}
		iterator erase(const_iterator Itr){

		}
		iterator erase(const_iterator Begin_, const_iterator End_){
			int N = std::distance(Begin_, End_);

			if(std::distance<const_iterator>(Beg, Begin_) <= std::distance<const_iterator>(End_, End)){
				//erase_front
				iterator From(Begin_.base(),*this);
				iterator To(End_.base(),*this);

				--From;
				--To;

				bool IsDestruct = (Begin_ == Beg);
				while(true){
					if(IsDestruct){
						To->~value_type();
					} else{
						*To = *From;
					}

					if(From == Beg) IsDestruct = true;

					if(To == Beg)break;

					--From;
					--To;
				}

				Beg += N;
				Size -= N;

				return iterator(End_.base(), *this) + 1;
			} else{
				//erase_back
				iterator From(End_.base(),*this);
				iterator To(Begin_.base(), *this);

				bool IsDestruct = (From == End);
				while(true){
					if(IsDestruct){
						To->~value_type();
					} else{
						*To = *From;
					}

					++From;
					++To;

					if(To == End)break;
					if(From == End) IsDestruct = true;
				}

				End -= N;
				Size -= N;

				return iterator(Begin_.base(), *this) + 1;
			}
		}
	private:
		std::unique_ptr<base_type> Base;
	};
}
#
#endif
