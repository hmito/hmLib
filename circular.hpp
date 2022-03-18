#ifndef HMLIB_CIRCULAR_INC
#define HMLIB_CIRCULAR_INC 100
#
#include<iterator>
#include<type_traits>
#include "algorithm/compare.hpp"
#include "exceptions.hpp"
#include "exceptions/access_exceptions.hpp"
namespace hmLib{
	template<std::size_t Size, typename integral_type_ = std::size_t>
	struct circular_integral {
		using this_type = circular_integral<Size, integral_type_>;
		using integral_type = integral_type_;
		using size_type = std::size_t;
		using difference_type = typename std::make_signed<integral_type>::type;
		constexpr static size_type static_size() { return Size; }
	private:
		integral_type idx;
	public:
		circular_integral() = default;
		explicit circular_integral(size_type idx_)noexcept :idx(idx_% static_size()) {}
		circular_integral(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		circular_integral(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		this_type& operator++()noexcept {
			++idx;
			if (idx == static_size())idx = 0;
			return *this;
		}
		this_type operator++(int)noexcept {
			this_type ans = *this;
			operator++();
			return ans;
		}
		this_type& operator--()noexcept {
			if (idx == 0)idx = static_size();
			--idx;
			return *this;
		}
		this_type operator--(int)noexcept {
			auto ans = *this;
			operator--();
			return ans;
		}
		this_type& operator+=(difference_type n)noexcept {
			idx = hmLib::euclidean_mod(idx + n, static_size());
			return *this;
		}
		this_type& operator-=(difference_type n)noexcept {
			idx = hmLib::euclidean_mod(idx - n, static_size());
			return *this;
		}
		integral_type index()const noexcept { return idx; }
		difference_type advance_from(const this_type& origin)const {
			if (origin.idx <= idx)return idx - origin.idx;
			else return static_size() + idx - origin.idx;
		}
		difference_type advance_to(const this_type& target)const {
			return target.advance_from(*this);
		}
		friend bool operator==(const this_type& my1, const this_type& my2) {
			return my1.idx == my2.idx;
		}
		friend bool operator!=(const this_type& my1, const this_type& my2) {
			return my1.idx != my2.idx;
		}
		friend this_type operator+(const this_type& my, difference_type n)noexcept {
			this_type ans = my;
			ans += n;
			return ans;
		}
		friend this_type operator+(difference_type n, const this_type& my)noexcept {
			this_type ans = my;
			ans += n;
			return ans;
		}
		friend this_type operator-(const this_type& my, difference_type n)noexcept {
			this_type ans = my;
			ans -= n;
			return ans;
		}
	};
	template<typename T, std::size_t MaxSize>
	struct circular{
	private:
		using this_type = circular<T,MaxSize>;
	public:
		using value_type = T;
		using reference = typename std::add_lvalue_reference<T>::type;
		using rval_reference = typename std::add_rvalue_reference<T>::type;
		using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
		using pointer = typename std::add_pointer<T>::type;
		using const_pointer = typename std::add_pointer<typename std::add_const<T>::type>::type;
		using index_type =  circular_integral<MaxSize+1>;
		using size_type =  std::size_t;
		using difference_type = typename index_type::difference_type;
		constexpr static size_type max_size(){ return MaxSize; }
	public:
		struct const_iterator{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = typename this_type::value_type;
			using reference = typename this_type::const_reference;
			using pointer = typename this_type::const_pointer;
			using index_type =  typename this_type::index_type;
			using difference_type = typename this_type::difference_type;
		private:
			index_type Idx;
			const this_type* This;
		public:
			const_iterator()=default;
			const_iterator(index_type Idx_, const this_type* This_): Idx(Idx_), This(This_){}
		public:
			index_type index()const{return Idx;}
			const_iterator& operator++(){
				++Idx;
				return *this;
			}
			const_iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			const_iterator& operator--(){
				--Idx;
				return *this;
			}
			const_iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			const_iterator& operator+=(difference_type n){
				Idx+=n;
				return *this;
			}
			this_type& operator-=(difference_type n){
				Idx-=n;
				return *this;
			}
			reference operator*(){ return *(This->ptr(Idx)); }
			pointer operator->(){ return This->ptr(Idx); }
			reference operator[](difference_type n){return *(This->ptr(Idx+n));}
			friend const_iterator operator+(const const_iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend const_iterator operator+(difference_type n, const const_iterator& Itr){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend const_iterator operator-(const const_iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans -= n;
				return Ans;
			}
			difference_type operator-(const const_iterator& itr)const {
				return This->BegIdx.advance_to(Idx) - itr.This->BegIdx.advance_to(itr.Idx);
			}
			friend bool operator==(const const_iterator& itr1, const const_iterator& itr2){
				return itr1.Idx == itr2.Idx;
			}
			friend bool operator!=(const const_iterator& itr1, const const_iterator& itr2){
				return !(itr1.Idx == itr2.Idx);
			}
			friend bool operator<(const const_iterator& itr1, const const_iterator& itr2){
				return (itr1 - itr2) < 0;
			}
			friend bool operator>(const const_iterator& itr1, const const_iterator& itr2){
				return (itr1 - itr2) > 0;
			}
			friend bool operator<=(const const_iterator& itr1, const const_iterator& itr2){
				return (itr1 - itr2) <= 0;
			}
			friend bool operator>=(const const_iterator& itr1, const const_iterator& itr2){
				return (itr1 - itr2) >= 0;
			}
		};
		struct iterator{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = typename this_type::value_type;
			using reference = typename this_type::reference;
			using pointer = typename this_type::pointer;
			using index_type =  typename this_type::index_type;
			using difference_type = typename this_type::difference_type;
		private:
			index_type Idx;
			this_type* This;
		public:
			iterator()=default;
			iterator(index_type Idx_, this_type* This_): Idx(Idx_), This(This_){}
			operator const_iterator()const{return const_iterator(Idx,This);}
		public:
			index_type index()const{return Idx;}
			iterator& operator++(){
				++Idx;
				return *this;
			}
			iterator operator++(int){
				auto Prv = *this;
				operator++();
				return Prv;
			}
			iterator& operator--(){
				--Idx;
				return *this;
			}
			iterator operator--(int){
				auto Prv = *this;
				operator--();
				return Prv;
			}
			iterator& operator+=(difference_type n){
				Idx+=n;
				return *this;
			}
			iterator& operator-=(difference_type n){
				Idx-=n;
				return *this;
			}
			reference operator*(){ return *(This->ptr(Idx)); }
			pointer operator->(){ return This->ptr(Idx); }
			reference operator[](difference_type n){return *(This->ptr(Idx+n));}
			friend iterator operator+(const iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend iterator operator+(difference_type n, const iterator& Itr){
				auto Ans = Itr;
				Ans += n;
				return Ans;
			}
			friend iterator operator-(const iterator& Itr, difference_type n){
				auto Ans = Itr;
				Ans -= n;
				return Ans;
			}
			difference_type operator-(const iterator& itr)const{
				return This->BegIdx.advance_to(Idx) - itr.This->BegIdx.advance_to(itr.Idx);
			}
			friend bool operator==(const iterator& itr1, const iterator& itr2){
				return itr1.Idx == itr2.Idx;
			}
			friend bool operator!=(const iterator& itr1, const iterator& itr2){
				return !(itr1.Idx == itr2.Idx);
			}
			friend bool operator<(const iterator& itr1, const iterator& itr2){
				return (itr1 - itr2) < 0;
			}
			friend bool operator>(const iterator& itr1, const iterator& itr2){
				return (itr1 - itr2) > 0;
			}
			friend bool operator<=(const iterator& itr1, const iterator& itr2){
				return (itr1 - itr2) <= 0;
			}
			friend bool operator>=(const iterator& itr1, const iterator& itr2){
				return (itr1 - itr2) >= 0;
			}
		};
	public:
		circular():Buf(),BegIdx(0),EndIdx(0){}
		circular(size_type n, const_reference val):circular(){
			assign(n,val);
		}
		template<typename input_iterator>
		circular(input_iterator first, input_iterator last):circular(){
			assign(first,last);
		}
		circular(const this_type& my):circular(){
			assign(my.begin(),my.end());
		}
		circular<T,MaxSize>& operator=(const this_type& my){
			if(this!=&my){
				assign(my.begin(),my.end());
			}
			return *this;
		}
		circular(circular&& my):circular(){
			assign(my.begin(),my.end());
		}
		circular<T,MaxSize>& operator=(this_type&& my){
			if(this!=&my){
				assign(my.begin(),my.end());
			}
			return *this;
		}
		~circular(){clear();}
	private:
		pointer ptr(index_type idx){ return static_cast<pointer>(static_cast<void*>(Buf))+idx.index(); }
		const_pointer ptr(index_type idx)const{ return static_cast<const_pointer>(static_cast<const void*>(Buf))+idx.index(); }
		bool is_valid(index_type idx)const{return idx.advance_from(BegIdx) < size();}
		void push_back_impl(const_reference val){
			::new(ptr(EndIdx)) value_type(val);
			++EndIdx;
		}
		void push_back_impl(rval_reference val){
			::new(ptr(EndIdx)) value_type(std::move(val));
			++EndIdx;
		}
		void push_front_impl(const_reference val){
			--BegIdx;
			::new(ptr(BegIdx)) value_type(val);
		}
		void push_front_impl(rval_reference val){
			--BegIdx;
			::new(ptr(BegIdx)) value_type(std::move(val));
		}
		void pop_back_impl(){
			--EndIdx;
			ptr(EndIdx)->~value_type();
		}
		void pop_front_impl(){
			ptr(BegIdx)->~value_type();
			++BegIdx;
		}
	public:
		iterator begin(){return iterator(BegIdx,this);}
		iterator end(){return iterator(EndIdx,this);}
		const_iterator begin()const{return const_iterator(BegIdx,this);}
		const_iterator end()const{return const_iterator(EndIdx,this);}
		const_iterator cbegin()const{return const_iterator(BegIdx,this);}
		const_iterator cend()const{return const_iterator(EndIdx,this);}
		reference front(){return *ptr(BegIdx);}
		const_reference front()const{return *ptr(BegIdx);}
		reference back(){return *ptr(EndIdx-1);}
		const_reference back()const{return *ptr(EndIdx-1);}
		reference at(difference_type pos){
			hmLib_assert(0<=pos && pos< static_cast<difference_type>(size()),hmLib::access_exceptions::out_of_range_access,"out of access into circular.");
			return *ptr(BegIdx+pos);
		}
		const_reference at(difference_type pos)const{
			hmLib_assert(0<=pos && pos<size(),hmLib::access_exceptions::out_of_range_access,"out of access into circular.");
			return *ptr(BegIdx+pos);
		}
		reference operator[](difference_type pos){return *ptr(BegIdx+pos);}
		const_reference operator[](difference_type pos)const{return *ptr(BegIdx+pos);}
		bool empty()const{ return BegIdx == EndIdx; }
		bool full()const{ return size() == max_size(); }
		size_type size()const{ return EndIdx.advance_from(BegIdx); }
		size_type remain()const{ return max_size() - size(); }
		bool assign(size_type n, const_reference val){
			if(n>max_size())return true;

			auto Itr = begin();
			auto End = end();
			for(;Itr!=End; ++Itr){
				if(n==0){
					erase(Itr,End);
					return false;
				}
				*Itr = val;
				--n;
			}
			insert(End,n,val);

			return false;
		}
		template<typename input_iterator>
		bool assign(input_iterator first, input_iterator last){
			auto n = static_cast<size_type>(std::distance(first,last));
			if( n > max_size())return true;

			auto Itr = begin();
			auto End = end();
			for(;Itr!=End; ++Itr){
				if(first==last){
					erase(Itr,End);
					return false;
				}
				*Itr = *first;
				++first;
			}
			insert(End,first,last);

			return false;
		}
		void clear(){
			erase(begin(),end());
		}
		bool push_back(const_reference val){
			if(full())return true;
			push_back_impl(val);
			return false;
		}
		bool push_back(rval_reference val){
			if(full())return true;
			push_back_impl(std::move(val));
			return false;
		}
		bool push_front(const_reference val){
			if(full())return true;
			push_front_impl(val);
			return false;
		}
		bool push_front(rval_reference val){
			if(full())return true;
			push_front_impl(std::move(val));
			return false;
		}
		void pop_back(){
			if(empty())return;
			pop_back_impl();
		}
		void pop_front(){
			if(empty())return;
			pop_front_impl();
		}
		bool rotete_back(const_reference val) {
			if(full()) {
				pop_front_impl();
				push_back_impl(val);
				return true;
			} else {
				push_back_impl(val);
				return false;
			}
		}
		bool rotete_back(rval_reference val) {
			if(full()) {
				pop_front_impl();
				push_back_impl(std::move(val));
				return true;
			} else {
				push_back_impl(std::move(val));
				return false;
			}
		}
		bool rotete_front(const_reference val) {
			if(full()) {
				pop_back_impl();
				push_front_impl(val);
				return true;
			} else {
				push_front_impl(val);
				return false;
			}
		}
		bool rotete_front(rval_reference val) {
			if(full()) {
				pop_back_impl();
				push_front_impl(std::move(val));
				return true;
			} else {
				push_front_impl(std::move(val));
				return false;
			}
		}
		iterator insert(const_iterator itr, const_reference val){
			if(full())return end();

			auto To = EndIdx;
			auto From = EndIdx;
			--From;
			++EndIdx;

			if(itr.index()==To){
				::new(ptr(To)) value_type(val);
			}else{
				::new(ptr(To)) value_type(std::move(*ptr(From)));
				To = From;
				--From;

				while(itr.index()!=To){
					*(ptr(To)) = std::move(*ptr(From));
					To = From;
					--From;
				}

				*(ptr(To)) = val;
			}
			return iterator(itr.index(),this);
		}
		iterator insert(const_iterator itr, rval_reference val){
			if(full())return end();

			auto To = EndIdx;
			auto From = EndIdx;
			--From;
			++EndIdx;

			if(itr.index()==To){
				::new(ptr(To)) value_type(val);
			}else{
				::new(ptr(To)) value_type(std::move(*ptr(From)));
				To = From;
				--From;

				while(itr.index()!=To){
					*(ptr(To)) = std::move(*ptr(From));
					To = From;
					--From;
				}

				*(ptr(To)) = std::move(val);
			}
			return iterator(itr.index(),this);
		}
		iterator insert(const_iterator itr, size_type n, const_reference val){
			auto Num = n;
			if(remain() < Num) return end();

			auto From = EndIdx;
			--From;
			EndIdx += Num;
			auto To = EndIdx;
			--To;

			auto LasIdx = itr.index() + (n-1);
			auto NewNum = Num;
			while(To!=LasIdx){
				if(NewNum>0){
					::new(ptr(To)) value_type(std::move(*ptr(From)));
					--NewNum;
				}else{
					*(ptr(To)) = std::move(*ptr(From));
				}
				--To;
				--From;
			}

			To = itr.index();
			for(auto CpyNum = Num - NewNum; CpyNum>0; --CpyNum){
				*(ptr(To)) = val;
				++To;
			}
			for(; NewNum>0; --NewNum){
				::new(ptr(To)) value_type(val);
				++To;
			}

			return iterator(itr.index(),this);
		}
		template<typename input_iterator>
		iterator insert(const_iterator itr, input_iterator first, input_iterator last){
			auto Num = std::distance(first,last);
			if(static_cast<decltype(Num)>(remain()) < Num) return end();

			auto From = EndIdx;
			--From;
			EndIdx += Num;
			auto To = EndIdx;
			--To;

			auto LasIdx = itr.index() + (Num-1);
			auto NewNum = Num;
			while(To!=LasIdx){
				if(NewNum>0){
					::new(ptr(To)) value_type(std::move(*ptr(From)));
					--NewNum;
				}else{
					*(ptr(To)) = std::move(*ptr(From));
				}
				--To;
				--From;
			}

			To = itr.index();
			for(auto CpyNum = Num - NewNum; CpyNum>0; --CpyNum){
				*(ptr(To)) = *first;
				++To;
				++first;
			}
			for(; NewNum>0; --NewNum){
				::new(ptr(To)) value_type(*first);
				++To;
				++first;
			}

			return iterator(itr.index(),this);
		}
		iterator erase(const_iterator itr){
			auto To = itr.index();
			auto From = itr.index();
			++From;

			while(From != EndIdx){
				*ptr(To) = std::move(*ptr(From));
				To = From;
				++From;
			}
			ptr(EndIdx)->~value_type();
			--EndIdx;

			return iterator( itr.index(), this);
		}
		iterator erase(const_iterator first, const_iterator last){
			auto Num = (last-first);
			auto To = first.index();
			auto From = last.index();
			auto NewEnd = EndIdx - Num;

			while(To != NewEnd){
				*ptr(To) = std::move(*ptr(From));
				++To;
				++From;
			}

			for(decltype(Num) Cnt = 0; Cnt < Num; ++Cnt){
				ptr(To)->~value_type();
				++To;
			}
			EndIdx = NewEnd;

			return iterator( first.index(), this);
		}
	private:
		alignas(alignof(T)) unsigned char Buf[sizeof(T)*index_type::static_size()];
		index_type BegIdx;
		index_type EndIdx;
	};

	template<typename container>
	struct back_rotate_iterator{
		using this_type = back_rotate_iterator<container>;
		using container_type = container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = void;
		using iterator_category = std::output_iterator_tag;
	public:
		back_rotate_iterator() = delete;
		explicit back_rotate_iterator(container& x):ptr(std::addressof(x)){}
		this_type& operator*(){return *this;}
		this_type& operator=(const typename container::value_type& value){
			ptr->rotate_back(value);
			return *this;
		}
		this_type& operator=(typename container::value_type&& value){
			ptr->rotate_back(std::move(value));
			return *this;
		}
		this_type& operator++(){return *this;}
		this_type& operator++(int){return *this;}
	private:
		container* ptr;
	};
	template<typename container>
	struct forward_rotate_iterator{
		using this_type = forward_rotate_iterator<container>;
		using container_type = container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = void;
		using iterator_category = std::output_iterator_tag;
	public:
		forward_rotate_iterator() = delete;
		explicit forward_rotate_iterator(container& x):ptr(std::addressof(x)){}
		this_type& operator*(){return *this;}
		this_type& operator=(const typename container::value_type& value){
			ptr->rotate_forward(value);
			return *this;
		}
		this_type& operator=(typename container::value_type&& value){
			ptr->rotate_forward(std::move(value));
			return *this;
		}
		this_type& operator++(){return *this;}
		this_type& operator++(int){return *this;}
	private:
		container* ptr;
	};
	template<typename container>
	back_rotate_iterator<container> back_rotator(container& x){return back_rotate_iterator<container>(x);}
	template<typename container>
	forward_rotate_iterator<container> forward_rotator(container& x){return forward_rotate_iterator<container>(x);}
}
#
#endif
