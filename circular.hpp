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
		public:
			constexpr static size_type buffer_size(){return MaxSize+1;}
			pointer buffer_begin(){return static_cast<pointer>(static_cast<void*>(Buf));}
			const_pointer buffer_begin()const{return static_cast<pointer>(static_cast<void*>(Buf));}
			pointer buffer_end(){return buffer_begin()+buffer_size();}
			const_pointer buffer_end()const{return buffer_begin()+buffer_size();}
		public:
			size_type index_to_pos(difference_type n){return hmLib::euclidean_mod(BegPos+n,MaxSize+1);}
			reference reference_by_pos(size_type Pos){ return buffer_begin()[Pos]; }
			const_reference reference_by_pos(size_type Pos)const{ return buffer_begin()[Pos]; }
			pointer pointer_by_pos(size_type Pos){ return buffer_begin()+Pos; }
			const_pointer pointer_by_pos(size_type Pos)const{ return buffer_begin()+Pos; }
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
				while(!empty())pop_back();
			}	
			bool push_back(const_reference Value){
				if(full())return true;
				++LasPos;
				if(LasPos==max_size())LasPos=0;
				++Size;
				::new(buffer_begin()+LasPos) value_type(Value);
				return false;
			}
			bool push_front(const_reference Value){
				if(full())return true;
				if(BegPos==0)BegPos=max_size();
				--BegPos;
				++Size;
				::new(buffer_begin()+BegPos) value_type(Value);
				return false;
			}
			void pop_back(){
				if(empty())return;
				(buffer_begin()+LasPos)->~value_type();
				if(LasPos==0)LasPos=max_size();
				--LasPos;
				--Size;
			}
			void pop_front(){
				if(empty())return;
				(buffer_begin()+BegPos)->~value_type();
				++BegPos;
				if(BegPos==max_size())BegPos=0;
				--Size;
			}
			bool rotete_back(const_reference Value) {
				if(full()) {
					(buffer_begin()+BegPos)->~value_type();
					++BegPos;
					if(BegPos==max_size())BegPos=0;
					++LasPos;
					if(LasPos==max_size())LasPos=0;
					::new(buffer_begin()+LasPos) value_type(Value);
					return true;
				} else {
					++LasPos;
					if(LasPos==max_size())LasPos=0;
					::new(buffer_begin()+LasPos) value_type(Value);
					return false;
				}
			}
			bool rotete_front(const_reference Value) {
				if(full()) {
					(buffer_begin()+LasPos)->~value_type();
					if(LasPos==0)LasPos=max_size();
					--LasPos;
					if(BegPos==0)BegPos=max_size();
					--BegPos;
					::new(buffer_begin()+BegPos) value_type(Value);
					return true;
				} else {
					if(BegPos==0)BegPos=max_size();
					--BegPos;
					++Size;
					::new(buffer_begin()+BegPos) value_type(Value);
					return false;
				}
			}
			bool insert_by_pos(size_type Pos, const_reference Value){
				if(full())return true;

				auto FromPos = LasPos;
				++LasPos;
				if(LasPos==max_size())LasPos=0;
				++Size;
				auto ToPos = LasPos;

				if(Pos==ToPos){
					::new(buffer_begin()+ToPos) value_type(Value);
				}else{
					::new(buffer_begin()+ToPos) value_type(std::move(*(buffer_begin()+FromPos)));

					while(Pos!=ToPos){
						ToPos = FromPos;
						if(FromPos==0)FromPos = max_size();
						--FromPos;
						*(buffer_begin()+ToPos) = std::move(*(buffer_begin()+FromPos));
					}

					*(buffer_begin()+ToPos) = Value;
				}
				return false;
			}
			void erase_by_pos(size_type Pos){
				if(empty())return;

				auto ToPos = Pos;
				++Pos;
				if(Pos==max_size())Pos=0;
				auto FromPos = Pos;

				while(ToPos != LasPos){
					*(buffer_begin()+ToPos) = std::move(*(buffer_begin()+FromPos));

					ToPos = FromPos;
					++FromPos;
					if(FromPos==max_size())FromPos=0;
				}
				(buffer_begin()+LasPos)->~value_type();
				if(LasPos==0)LasPos=max_size();
				--LasPos;
				--Size;
			}
		private:
			alignas(alignof(T)) unsigned char Buf[sizeof(T)*(MaxSize+1)];
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
				++Pos;
				if(Pos == this_type::max_size()) Pos = 0;
				return *this;
			}
			const_iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			const_iterator& operator--(){
				if(Pos == 0) Pos = this_type::max_size();
				--Pos;
				return *this;
			}
			const_iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			const_iterator& operator+=(difference_type n){
				Pos = hmLib::euclidean_mod(Pos + n, Base->buffer_size());
				return *this;
			}
			const_iterator& operator-=(difference_type n){
				Pos = hmLib::euclidean_mod(Pos - n, Base->buffer_size());
				return *this;
			}
			reference operator*(){ return Base->reference_by_pos(Pos); }
			pointer operator->(){ return Base->pointer_by_pos(Pos); }
			reference operator[](difference_type n){
				return Base->reference_by_pos(hmLib::euclidean_mod(Pos + n, Base->buffer_size()));
			}
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
				difference_type pos1 = itr1.Pos + (*(itr1.pBegPos) > itr1.Pos ? (itr1.End-itr1.Beg) : 0 ) - *(itr1.pBegPos);
				difference_type pos2 = itr2.Pos + (*(itr2.pBegPos) > itr2.Pos ? (itr2.End-itr2.Beg) : 0 ) - *(itr2.pBegPos);
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
				++Pos;
				if(Pos == this_type::max_size()) Pos = 0;
				return *this;
			}
			iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			iterator& operator--(){
				if(Pos == 0) Pos = this_type::max_size();
				--Pos;
				return *this;
			}
			iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			iterator& operator+=(difference_type n){
				Pos = hmLib::euclidean_mod(Pos + n, Base->buffer_size());
				return *this;
			}
			iterator& operator-=(difference_type n){
				Pos = hmLib::euclidean_mod(Pos - n, Base->buffer_size());
				return *this;
			}
			reference operator*(){ return Base->reference_by_pos(Pos); }
			pointer operator->(){ return Base->pointer_by_pos(Pos); }
			reference operator[](difference_type n){
				return Base->reference_by_pos(hmLib::euclidean_mod(Pos + n, Base->buffer_size()));
			}
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
				difference_type pos1 = itr1.Pos + (*(itr1.pBegPos) > itr1.Pos ? (itr1.End-itr1.Beg) : 0 ) - *(itr1.pBegPos);
				difference_type pos2 = itr2.Pos + (*(itr2.pBegPos) > itr2.Pos ? (itr2.End-itr2.Beg) : 0 ) - *(itr2.pBegPos);
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
	private:
		std::unique_ptr<base_type> Base;
	public:
		reference at(difference_type n){ 
			hmLib_assert(n<size(), hmLib::exceptions::access::out_of_range_access, "circular access error: out of range.");
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
		const_reference at(difference_type n)const{
			hmLib_assert(n<size(), hmLib::exceptions::access::out_of_range_access, "circular access error: out of range.");
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
		reference operator[](difference_type pos)noexcept{
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
		const_reference operator[](difference_type pos)const noexcept{
			return Base->reference_by_pos(Base->index_to_pos(n)); 
		}
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
		bool push_back(const_reference Value){
			if(!Base)Base = std::make_unique<base_type>();
			return Base->push_back(Value);
		}
		bool push_front(const_reference Value){
			if(!Base)Base = std::make_unique<base_type>();
			return Base->push_front(Value);
		}
		void pop_back(){
			if(Base)Base->pop_back(Value);
		}
		void pop_front(){
			if(Base)Base->pop_back(Value);
		}
		bool rotete_back(const_reference Value) {
			if(!Base)Base = std::make_unique<base_type>();
			return Base->rotete_back(Value);
		}
		bool rotete_front(const_reference Value) {
			if(!Base)Base = std::make_unique<base_type>();
			return Base->rotete_front(Value);
		}
		void clear(){
			if(Base)Base->clear();
		}
	public:
		iterator begin(){ return Beg; }
		iterator end(){ return End; }
		const_iterator begin()const{ return Beg; }
		const_iterator end()const{ return End; }
		const_iterator cbegin()const{ return Beg; }
		const_iterator cend()const{ return End; }
	public:
		iterator insert(const_iterator Itr, const_reference Value){
			if(!Base)Base = std::make_unique<base_type>();
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
	};
}
#
#endif
