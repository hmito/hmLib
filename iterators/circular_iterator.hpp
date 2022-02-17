#ifndef HMLIB_ITERATORS_CIRCULARITERATOR_INC
#define HMLIB_ITERATORS_CIRCULARITERATOR_INC 100
#
#include<iterator>
#include<type_traits>
#include<algorithm>
namespace hmLib{
	template<typename base_itr_
		, typename base_iterator_tag =  typename std::iterator_traits<base_itr_>::iterator_category
		, bool is_const_itr = std::is_const<typename std::iterator_traits<base_itr_>::value_type>::value>
	struct circular_iterator{};
	template<typename base_itr_, bool is_const_itr>
	struct circular_iterator<base_itr_, std::forward_iterator_tag, is_const_itr>{
    private:
        using this_type = circular_iterator<base_itr_, std::forward_iterator_tag, is_const_itr>;
	public:
		using base_iterator = base_itr_;
		using iterator_category = std::forward_iterator_tag;
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
	private:
		base_iterator Cur;
		base_iterator Beg;
		base_iterator End;
	public:
		circular_iterator()=default;
		circular_iterator(base_iterator Cur_,base_iterator Beg_, base_iterator End_):Cur(Cur_), Beg(Beg_), End(End_){}
		template<typename other_base_itr, typename other_base_iterator_tag, bool other_is_const_itr>
		circular_iterator(const circular_iterator<other_base_itr, other_base_iterator_tag, other_is_const_itr>& other)
			:circular_iterator(other.Cur,other.Beg,other.End){
		}
		base_iterator base(){return Cur;}
		base_iterator base_begin(){return Beg;}
		base_iterator base_end(){return End;}
	public:
		this_type& operator++(){
			++Cur;
			if(Cur == End)Cur = Beg;
			return *this;
		}
		this_type operator++(int){
			auto Prv = *this;
			operator++();
			return Prv;
		}
		reference operator*(){ return *Cur; }
		pointer operator->(){ return Cur; }
		friend bool operator==(const this_type& Itr1, const this_type& Itr2){
			return Itr1.Cur == Itr2.Cur;
		}
		friend bool operator!=(const this_type& Itr1, const this_type& Itr2){
			return !(Itr1 == Itr2);
		}
	};
	template<typename base_itr_, bool is_const_itr>
	struct circular_iterator<base_itr_, std::bidirectional_iterator_tag, is_const_itr>{
    private:
        using this_type = circular_iterator<base_itr_, std::forward_iterator_tag, is_const_itr>;
	public:
		using base_iterator = base_itr_;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
	private:
		base_iterator Cur;
		base_iterator Beg;
		base_iterator End;
	public:
		circular_iterator()=default;
		circular_iterator(base_iterator Cur_,base_iterator Beg_, base_iterator End_):Cur(Cur_), Beg(Beg_), End(End_){}
		template<typename other_base_itr, typename other_base_iterator_tag, bool other_is_const_itr>
		circular_iterator(const circular_iterator<other_base_itr, other_base_iterator_tag, other_is_const_itr>& other)
			:circular_iterator(other.Cur,other.Beg,other.End){
		}
		base_iterator base(){return Cur;}
		base_iterator base_begin(){return Beg;}
		base_iterator base_end(){return End;}
	public:
		this_type& operator++(){
			++Cur;
			if(Cur == End)Cur = Beg;
			return *this;
		}
		this_type operator++(int){
			auto Prv = *this;
			operator++();
			return Prv;
		}
		this_type& operator--(){
			if(Cur == Beg)Cur = End;
			--Cur;
			return *this;
		}
		this_type operator--(int){
			auto Prv = *this;
			operator--();
			return Prv;
		}
		reference operator*(){ return *Cur; }
		pointer operator->(){ return Cur; }
		friend bool operator==(const this_type& Itr1, const this_type& Itr2){
			return Itr1.Cur == Itr2.Cur;
		}
		friend bool operator!=(const this_type& Itr1, const this_type& Itr2){
			return !(Itr1 == Itr2);
		}
	};
	template<typename base_itr_, bool is_const_itr>
	struct circular_iterator<base_itr_, std::random_access_iterator_tag, is_const_itr>{
    private:
        using this_type = circular_iterator<base_itr_, std::forward_iterator_tag, is_const_itr>;
	public:
		using base_iterator = base_itr_;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
	private:
		base_iterator Cur;
		base_iterator Beg;
		base_iterator End;
	public:
		circular_iterator()=default;
		circular_iterator(base_iterator Cur_,base_iterator Beg_, base_iterator End_):Cur(Cur_), Beg(Beg_), End(End_){}
		template<typename other_base_itr, typename other_base_iterator_tag, bool other_is_const_itr>
		circular_iterator(const circular_iterator<other_base_itr, other_base_iterator_tag, other_is_const_itr>& other)
			:circular_iterator(other.Cur,other.Beg,other.End){
		}
		base_iterator base(){return Cur;}
		base_iterator base_begin(){return Beg;}
		base_iterator base_end(){return End;}
	public:
		this_type& operator++(){
			++Cur;
			if(Cur == End)Cur = Beg;
			return *this;
		}
		this_type operator++(int){
			auto Prv = *this;
			operator++();
			return Prv;
		}
		this_type& operator--(){
			if(Cur == Beg)Cur = End;
			--Cur;
			return *this;
		}
		this_type operator--(int){
			auto Prv = *this;
			operator--();
			return Prv;
		}
		reference operator*(){ return *Cur; }
		pointer operator->(){ return Cur; }
		friend bool operator==(const this_type& Itr1, const this_type& Itr2){
			return Itr1.Cur == Itr2.Cur;
		}
		friend bool operator!=(const this_type& Itr1, const this_type& Itr2){
			return !(Itr1 == Itr2);
		}
		void advance(difference_type pos){
			pos = hmLib::euclidean_mod(pos+std::distance(Beg,Cur), std::distance(Beg,End));
			Cur = std::next(Beg, pos);
		}
		this_type next(difference_type pos){
			this_type ans = *this;
			ans.advance(pos);
			return ans;
		}
		reference at(difference_type pos){
			return Beg[hmLib::euclidean_mod(pos+std::distance(Beg,Cur), std::distance(Beg,End))];
		}
	};
}
#
#endif
