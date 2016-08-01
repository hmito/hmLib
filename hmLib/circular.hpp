#ifndef HMLIB_CIRCULAR_INC
#define HMLIB_CIRCULAR_INC 100
#
#include<iterator>
namespace hmLib{
	namespace{
		template<typename T, unsigned int Size_>
		struct aligned_array{
			typedef T& reference;
			typedef T* pointer;
			typedef pointer iterator;
			typedef const pointer const_iterator;
		private:
			unsigned char Array[sizeof(T)*(Size_ + 1)];
			unsigned char* Begin;
			unsigned char* End;
		public:
			aligned_array()
				: Begin(Array + ((sizeof(T) - static_cast<unsigned int>(reinterpret_cast<unsigned long long>(Array) % sizeof(T))) % sizeof(T)))
				, End(Begin + sizeof(T)*Size_){}
		private:
			aligned_array(const aligned_array<T, Size_>&);
			aligned_array<T, Size_>& operator=(const aligned_array<T, Size_>&);
		public:
			unsigned int size(){ return Size_; }
			reference operator[](int pos){ return begin()[pos]; }
			reference at(int pos){ return begin()[pos]; }
			iterator begin(){ return reinterpret_cast<pointer>(Begin); }
			iterator end(){ return reinterpret_cast<pointer>(End); }
			const reference operator[](int pos)const{ return begin()[pos]; }
			const reference at(int pos)const{ return begin()[pos]; }
			const_iterator begin()const{ return reinterpret_cast<const pointer>(Begin); }
			const_iterator end()const{ return reinterpret_cast<const pointer>(End); }
		};
		template<typename this_circular>
		struct circular_const_iterator;

		template<typename this_circular>
		struct circular_iterator : public std::iterator<std::random_access_iterator_tag, typename this_circular::value_type, typename this_circular::difference_type, typename this_circular::pointer, typename this_circular::reference>{
			friend struct circular_const_iterator<this_circular>;
			typedef circular_iterator<this_circular> this_type;
			typedef typename this_circular::base_iterator base_iterator;
			typedef const reference const_reference;
			typedef const pointer const_pointer;
		private:
			base_iterator Cur;
			const this_circular* Ptr;
		public:
			circular_iterator()
				: Cur()
				, Ptr(nullptr){}
			circular_iterator(base_iterator Cur_, const this_circular& Ref_)
				: Cur(Cur_)
				, Ptr(&Ref_){}
		public:
			base_iterator base()const{ return Cur; }
			this_type& operator++(){
				if(Cur == Ptr->buffer_last())Cur = Ptr->buffer_first();
				else ++Cur;
				return *this;
			}
			this_type operator++(int){
				auto Prev = *this;
				operator++();
				return Prev;
			}
			this_type& operator--(){
				if(Cur == Ptr->buffer_first())Cur = Ptr->buffer_last();
				else --Cur;
				return *this;
			}
			this_type operator--(int){
				auto Prev = *this;
				operator--();
				return Prev;
			}
			this_type& operator+=(difference_type dif){
				if(dif < 0){
					if(std::distance(Ptr->buffer_first(), Cur) < -dif){
						Cur = Ptr->buffer_last() - (-dif - std::distance(Ptr->buffer_first(), Cur) - 1);
					} else{
						Cur += dif;
					}
				} else{
					if(std::distance(Cur, Ptr->buffer_last()) < dif){
						Cur = Ptr->buffer_first() + (dif - std::distance(Cur, Ptr->buffer_last()) - 1);
					} else{
						Cur += dif;
					}
				}
				return *this;
			}
			this_type& operator-=(difference_type dif){
				return operator+=(-dif);
			}
			reference operator*(){ return *Cur; }
			const_reference operator*()const{ return *Cur; }
			pointer operator->(){ return Cur; }
			const_pointer operator->()const{ return Cur; }
			reference operator[](difference_type dif){
				if(dif < 0){
					if(std::distance(Ptr->buffer_first(), Cur) < -dif){
						return Ptr->buffer_last() [- (-dif - std::distance(Ptr->buffer_first(), Cur) - 1)];
					} else{
						return Cur[dif];
					}
				} else{
					if(std::distance(Cur, Ptr->buffer_last()) < dif){
						return Ptr->buffer_first()[ + (dif - std::distance(Cur, Ptr->buffer_last()) - 1)];
					} else{
						return Cur[dif];
					}
				}
			}
			const_reference operator[](difference_type dif)const{
				if(dif < 0){
					if(std::distance(Ptr->buffer_first(), Cur) < -dif){
						return Cur[Ptr->buffer_last() - (-dif - std::distance(Ptr->buffer_first(), Cur) - 1)];
					} else{
						return Cur[dif];
					}
				} else{
					if(std::distance(Cur, Ptr->buffer_last()) < dif){
						return Cur[Ptr->buffer_first() + (dif - std::distance(Cur, Ptr->buffer_last()) - 1)];
					} else{
						return Cur[dif];
					}
				}
			}
			friend this_type operator+(this_type Itr, difference_type dif){
				auto Ans = Itr;
				Ans += dif;
				return Ans;
			}
			friend this_type operator+(difference_type dif, this_type Itr){
				return operator+(Itr, dif);
			}
			friend this_type operator-(this_type Itr, difference_type dif){
				return operator+(Itr, -dif);
			}
			friend difference_type operator-(this_type Itr1, this_type Itr2){
				if(Itr1.Cur <= Itr1.Ptr->end().base() && Itr2.Ptr->end().base() < Itr2.Cur){
					//1<=E<2
					return (Itr2.Ptr->buffer_last() - Itr2.Cur) + (Itr1.Cur - Itr1.Ptr->buffer_first())+1;
				} else if(Itr2.Cur <= Itr2.Ptr->end().base() && Itr1.Ptr->end().base() < Itr1.Cur){
					//2<=E<1
					return - (Itr1.Ptr->buffer_last() - Itr1.Cur) - (Itr2.Cur - Itr2.Ptr->buffer_first())-1;
				}
				return Itr1.Cur - Itr2.Cur;
			}
			friend bool operator==(this_type Itr1, this_type Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(this_type Itr1, this_type Itr2){
				return !(Itr1 == Itr2);
			}
			friend bool operator<(this_type Itr1, this_type Itr2){
				if(Itr1.Cur <= Itr1.Ptr->end().base() && Itr2.Ptr->end().base() < Itr2.Cur){
					//1<=E<2
					return false;
				} else if(Itr2.Cur < Itr2.Ptr->end().base() && Itr1.Ptr->end().base() < Itr1.Cur){
					//2<=E<1
					return true;
				}
				return Itr1.Cur < Itr2.Cur;
			}
			friend bool operator>(this_type Itr1, this_type Itr2){
				return Itr2 < Itr1;
			}
			friend bool operator<=(this_type Itr1, this_type Itr2){
				return Itr1 == Itr2 || Itr1 < Itr2;
			}
			friend bool operator>=(this_type Itr1, this_type Itr2){
				return Itr1 == Itr2 || Itr1 > Itr2;
			}
		};
		template<typename this_circular>
		struct circular_const_iterator : public std::iterator<std::random_access_iterator_tag, typename this_circular::value_type, typename this_circular::difference_type, typename this_circular::pointer, typename this_circular::reference>{
			typedef circular_const_iterator<this_circular> this_type;
			typedef typename this_circular::base_const_iterator base_const_iterator;
		private:
			base_const_iterator Cur;
			const this_circular* Ptr;
		public:
			circular_const_iterator()
				: Cur()
				, Ptr(nullptr){}
			circular_const_iterator(base_const_iterator Cur_, const this_circular& Ref_)
				: Cur(Cur_)
				, Ptr(&Ref_){}
			circular_const_iterator(circular_iterator<this_circular> Itr_)
				: Cur(Itr_.Cur)
				, Ptr(Itr_.Ptr){}
		public:
			base_const_iterator base()const{ return Cur; }
			this_type& operator++(){
				if(Cur == Ptr->buffer_last())Cur = Ptr->buffer_first();
				else ++Cur;
			}
			this_type operator++(int){
				auto Prev = *this;
				operator++();
				return Prev;
			}
			this_type& operator--(){
				if(Cur == Ptr->buffer_first())Cur = Ptr->buffer_last();
				else --Cur;
			}
			this_type operator--(int){
				auto Prev = *this;
				operator--();
				return Prev;
			}
			this_type& operator+=(difference_type dif){
				if(dif < 0){
					if(std::distance(Ptr->buffer_first(), Cur) < -dif){
						Cur = Ptr->buffer_last() - (-dif - std::distance(Ptr->buffer_first(), Cur) - 1);
					} else{
						Cur += dif;
					}
				} else{
					if(std::distance(Cur, Ptr->buffer_last()) < dif){
						Cur = Ptr->buffer_first() + (dif - std::distance(Cur, Ptr->buffer_last()) - 1);
					} else{
						Cur += dif;
					}
				}
				return *this;
			}
			this_type& operator-=(difference_type dif){
				return operator+=(-dif);
			}
			reference operator*()const{ return Cur.operator*(); }
			pointer operator->()const{ return Cur.operator->(); }
			reference operator[](difference_type dif)const{
				if(dif < 0){
					if(std::distance(Ptr->buffer_first(), Cur) < -dif){
						return Cur[Ptr->buffer_last() - (-dif - std::distance(Ptr->buffer_first(), Cur) - 1)];
					} else{
						return Cur[dif];
					}
				} else{
					if(std::distance(Cur, Ptr->buffer_last()) < dif){
						return Cur[Ptr->buffer_first() + (dif - std::distance(Cur, Ptr->buffer_last()) - 1)];
					} else{
						return Cur[dif];
					}
				}
			}
			friend this_type operator+(this_type Itr, difference_type dif){
				auto Ans = Itr;
				Ans += dif;
				return Ans;
			}
			friend this_type operator+(difference_type dif, this_type Itr){
				return operator+(Itr, dif);
			}
			friend this_type operator-(this_type Itr, difference_type dif){
				return operator+(Itr, -dif);
			}
			friend difference_type operator-(this_type Itr1, this_type Itr2){
				if(Itr1.Cur <= Itr1.Ptr->end().base() && Itr2.Ptr->end().base() < Itr2.Cur){
					//1<=E<2
					return (Itr2.Ptr->buffer_last() - Itr2.Cur) + (Itr1.Cur - Itr1.Ptr->buffer_first()) + 1;
				} else if(Itr2.Cur < Itr2.Ptr->end().base() && Itr1.Ptr->end().base() < Itr1.Cur){
					//2<=E<1
					return -(Itr1.Ptr->buffer_last() - Itr1.Cur) - (Itr2.Cur - Itr2.Ptr->buffer_first()) - 1;
				}
				return Itr1.Cur - Itr2.Cur;
			}
			friend bool operator==(this_type Itr1, this_type Itr2){
				return Itr1.Cur == Itr2.Cur;
			}
			friend bool operator!=(this_type Itr1, this_type Itr2){
				return !(Itr1 == Itr2);
			}
			friend bool operator<(this_type Itr1, this_type Itr2){
				if(Itr1.Cur <= Itr1.Ptr->end().base() && Itr2.Ptr->end().base() < Itr2.Cur){
					//1<=E<2
					return false;
				} else if(Itr2.Cur < Itr2.Ptr->end().base() && Itr1.Ptr->end().base() < Itr1.Cur){
					//2<=E<1
					return true;
				}
				return Itr1.Cur < Itr2.Cur;
			}
			friend bool operator>(this_type Itr1, this_type Itr2){
				return Itr2 < Itr1;
			}
			friend bool operator<=(this_type Itr1, this_type Itr2){
				return Itr1 == Itr2 || Itr1 < Itr2;
			}
			friend bool operator>=(this_type Itr1, this_type Itr2){
				return Itr1 == Itr2 || Itr1 > Itr2;
			}
		};
	}
	template<typename T, unsigned int Size_>
	struct circular{
		typedef circular<T, Size_> this_type;
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef unsigned int size_type;
		typedef signed int difference_type;
	public:
		static size_type max_size(){ return Size_; }
	private:
		using base_array = aligned_array<T, Size_ + 1>;
	public:
		using base_iterator = typename base_array::iterator;
		using base_const_iterator = typename base_array::const_iterator;
		typedef circular_iterator<this_type> iterator;
		typedef circular_const_iterator<this_type> const_iterator;
	private:
		base_array Array;
		iterator Beg;
		iterator End;
		unsigned int Size;
	public:
		base_const_iterator buffer_first()const{ return Array.begin(); }
		base_const_iterator buffer_last()const{ return Array.end() - 1; }
	public:
		circular()
			: Beg(Array.begin(), *this)
			, End(Array.begin(), *this)
			, Size(0){
		}
		circular(const this_type& Other)
			: Beg(Array.begin(), *this)
			, End(Array.begin(), *this)
			, Size(0){
			insert(begin(), Other.cbegin(), Other.cend());
		}
		this_type& operator=(const this_type& Other){
			if(&Other != this){
				clear();
				insert(begin(), Other.cbegin(), Other.cend());
			}
			return *this;
		}
		template<typename input_iterator>
		circular(input_iterator Begin_, input_iterator End_)
			: Beg(Array.begin(), *this)
			, End(Array.begin(), *this)
			, Size(0){
			insert(begin(), Begin_, End_);
		}
		~circular(){
			clear();
		}
	public:
		reference at(difference_type pos){ return Beg[pos]; }
		const_reference at(difference_type pos)const{ return Beg[pos]; }
		reference operator[](difference_type pos){ return Beg[pos]; }
		const_reference operator[](difference_type pos)const{ return Beg[pos]; }
		reference front(){ return *Beg; }
		const_reference front()const{ return *Beg; }
		reference back(){ return End[-1]; }
		const_reference back()const{ return End[-1]; }
	public:
		bool push_back(const_reference Value){
			if(full())return true;
			::new(End.base()) value_type(Value);
			++End;
			++Size;
			return false;
		}
		void pop_back(){
			if(empty())return;
			End[-1].~value_type();
			--End;
			--Size;
		}
		bool push_front(const_reference Value){
			if(full())return true;
			::new((Beg - 1).base()) value_type(Value);
			--Beg;
			++Size;
			return false;
		}
		void pop_front(){
			if(empty())return;
			(*End).~value_type();
			++Beg;
			--Size;
		}
		void clear(){
			iterator Itr = Beg;
			while(Itr != End){
				pointer Ptr = &(*Itr);
				Ptr->~value_type();
				++Itr;
			}
			Beg = End;
			Size = 0;
		}
		iterator insert(const_iterator Itr, const_reference Value){
			if(full())return End;

			if(std::distance<const_iterator>(Beg, Itr) < static_cast<int>(size() / 2)){
				//insert_front
				iterator From = Beg;
				iterator To = Beg - 1;

				if(Beg == Itr){
					::new(To.base()) value_type(Value);
				} else{
					::new(To.base()) value_type(*From);

					++From;
					++To;

					while(To != Itr){
						*To = *From;

						++From;
						++To;
					}

					*To = Value;
				}

				--Beg;
				++Size;
				return To;
			} else{
				//insert_back
				iterator From = End - 1;
				iterator To = End;

				if(Itr == End){
					::new(To.base()) value_type(Value);
				} else{
					::new(To.base()) value_type(*From);

					--From;
					--To;

					while(To != Itr){
						*To = *From;

						--From;
						--To;
					}

					*To = Value;
				}

				++End;
				++Size;
				return To;
			}
		}
		template<typename input_iterator>
		iterator insert(const_iterator Itr, input_iterator Begin_, input_iterator End_){
			int N = std::distance(Begin_, End_);
			if(remain() < N || N == 0)return End;

			if(std::distance<const_iterator>(Beg, Itr) < static_cast<int>(size() / 2)){
				//insert_front
				iterator From = Beg;
				iterator To = Beg - N;

				int n = N;
				while(To != Itr){
					if(n > 0){
						::new(To.base()) value_type(*From);
						--n;
					} else{
						*To = *From;
					}

					++From;
					++To;
				}

				To = From;
				for(; Begin_ != End_; ++Begin_){
					if(n > 0){
						::new(To.base()) value_type(*Begin);
						--n;
					} else{
						*(To++) = *Begin_;
					}
				}

				Beg -= N;
				Size += N;

				return From;
			} else{
				//insert_back
				iterator From = End - 1;
				iterator To = From + N;

				int n = N;
				while(true){
					if(n > 0){
						::new(To.base()) value_type(*From);
						--n;
					} else{
						*To = *From;
					}
					if(From == Itr)break;

					--From;
					--To;
				}

				n = N - n;
				To = From;
				for(; Begin_ != End_; ++Begin_){
					if(n > 0){
						*To = *Begin_;
						--n;
					} else{
						::new(To.base()) value_type(*Begin_);
					}
				}

				End += N;
				Size += N;

				return From;
			}
		}
		iterator erase(const_iterator Itr){
			if(empty())return End;
			if(Itr == cend())return End;

			if(std::distance<const_iterator>(Beg, Itr) < static_cast<int>(size() / 2)){
				//erase_front
				iterator To(Itr.base(), *this);
				iterator From = To - 1;

				while(To != Beg){
					*To = *From;

					--From;
					--To;
				}

				Beg->~value_type();
				++Beg;
				--Size;
				return iterator(Itr.base(), *this) + 1;
			} else{
				//erase_back
				iterator To(Itr.base(), *this);
				iterator From = To + 1;

				while(From != End){
					*To = *From;

					++From;
					++To;
				}

				To->~value_type();
				--End;
				--Size;
				return iterator(Itr.base(), *this);
			}
		}
		iterator erase(const_iterator Begin_, const_iterator End_){
			if(std::distance<const_iterator>(Beg, Begin_) <= std::distance<const_iterator>(End_, End)){
				//erase_front
				iterator From(Begin_);
				iterator To(End_);

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

				int N = std::distance(Begin_, End_);
				Beg += N;
				Size -= N;

				return iterator(End_.base(), *this) + 1;
			} else{
				//erase_back
				iterator From(End_);
				iterator To(Begin_);

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

				int N = std::distance(Begin_, End_);
				End -= N;
				Size -= N;

				return iterator(Begin_.base(), *this) + 1;
			}

		}
	public:
		iterator begin(){ return Beg; }
		iterator end(){ return End; }
		const_iterator begin()const{ return Beg; }
		const_iterator end()const{ return End; }
		const_iterator cbegin()const{ return Beg; }
		const_iterator cend()const{ return End; }
	public:
		bool empty()const{ return Size == 0; }
		bool full()const{ return Size == max_size(); }
		size_type size()const{ return Size; }
		size_type remain()const{ return max_size() - size(); }
	};
}
#
#endif
