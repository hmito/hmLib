#ifndef INC_HMLIB_BASICITERATOR
#define INC_HMLIB_BASICITERATOR 100
#include <algorithm>

namespace std{
	class fixed_iterator_tag{};
}
namespace hm{
	inline void showIteratorTag( std::input_iterator_tag p_cTag ){printf( "input_iterator_tag\n" );}
	inline void showIteratorTag( std::output_iterator_tag p_cTag ){printf( "output_iterator_tag\n" );}
	inline void showIteratorTag( std::fixed_iterator_tag p_cTag ){printf( "fixed_iterator_tag\n" );}
	inline void showIteratorTag( std::bidirectional_iterator_tag p_cTag ){printf( "bidirectional_iterator_tag\n" );}
	inline void showIteratorTag( std::forward_iterator_tag p_cTag ){printf( "forward_iterator_tag\n" );}
	inline void showIteratorTag( std::random_access_iterator_tag p_cTag ){printf( "random_access_iterator_tag\n" );}
}
namespace hmstd{
	//accessor_type
	template<class _value_type>
	class accessor_type{
	public:
		typedef _value_type value_type;
	};
	//iterator_manager
	template<class _value_type,class _difference_type=int,class _reference=_value_type&,class _const_reference=const _value_type&,class _pointer=_value_type*,class _const_pointer=const _value_type*>
	class fixed_iterator_manager_type{
	public:
		typedef std::fixed_iterator_tag iterator_tag;
	protected:
		typedef _value_type value_type;
		typedef _difference_type difference_type;
		typedef _reference reference;
		typedef _pointer pointer;
		typedef _const_reference const_reference;
		typedef _const_pointer const_pointer;
//	public:
//		reference ref()=0;
//		pointer ptr()=0;
	};
	template<class _value_type,class _difference_type=int,class _reference=_value_type&,class _const_reference=const _value_type&,class _pointer=_value_type*,class _const_pointer=const _value_type*>
	class forward_iterator_manager_type{
	public:
		typedef std::forward_iterator_tag iterator_tag;
	protected:
		typedef _value_type value_type;
		typedef _difference_type difference_type;
		typedef _reference reference;
		typedef _pointer pointer;
		typedef _const_reference const_reference;
		typedef _const_pointer const_pointer;
//	public:
//		void increment()=0;
//		bool equal(const iterator_manager_type& _itr)const=0;
//		reference ref()=0;
//		pointer ptr()=0;
	};
	template<class _value_type,class _difference_type=int,class _reference=_value_type&,class _const_reference=const _value_type&,class _pointer=_value_type*,class _const_pointer=const _value_type*>
	class bidirectional_iterator_manager_type{
	public:
		typedef std::bidirectional_iterator_tag iterator_tag;
	protected:
		typedef _value_type value_type;
		typedef _difference_type difference_type;
		typedef _reference reference;
		typedef _pointer pointer;
		typedef _const_reference const_reference;
		typedef _const_pointer const_pointer;
//	public:
//		void increment()=0;
//		void decrement()=0;
//		bool equal(const iterator_manager_type& _itr)const=0;
//		reference ref()=0;
//		pointer ptr()=0;
	};
	template<class _value_type,class _difference_type=int,class _reference=_value_type&,class _const_reference=const _value_type&,class _pointer=_value_type*,class _const_pointer=const _value_type*>
	class random_iterator_manager_type{
	public:
		typedef std::random_access_iterator_tag iterator_tag;
	protected:
		typedef _value_type value_type;
		typedef _difference_type difference_type;
		typedef _reference reference;
		typedef _pointer pointer;
		typedef _const_reference const_reference;
		typedef _const_pointer const_pointer;
//	public:
//		void increment()=0;
//		void decrement()=0;
//		void increment(difference_type _num)=0;
//		void decrement(difference_type _num)=0;
//		bool equal(const iterator_manager_type& _itr)const=0;
//		bool less(const iterator_manager_type& _itr)const=0;
//		difference_type difference(const iterator_manager_type& _itr)const=0;
//		reference ref()=0;
//		reference ref(difference_type _num)=0;
//		pointer ptr()=0;
	};
	//iterator
	template <class iterator_type,class iterator_manager_type,class iterator_tag=typename iterator_manager_type::iterator_tag>
	class basic_iterator{};
	template <class iterator_type,class iterator_manager_type>
	class basic_iterator<iterator_type,iterator_manager_type,std::fixed_iterator_tag>:public iterator_manager_type,public std::iterator<std::fixed_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag>:public iterator_manager_type,public std::iterator<std::forward_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		iterator_type operator++(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			increment();
			return ans;
		}
		bool operator==(const basic_iterator<iterator_type,iterator_manager_type>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_iterator<iterator_type,iterator_manager_type>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>:public iterator_manager_type,public std::iterator<std::bidirectional_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		iterator_type operator++(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			increment();
			return ans;
		}
		basic_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator--(){
			decrement();
			return *this;
		}
		iterator_type operator--(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			decrement();
			return ans;
		}
		bool operator==(const basic_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>:public iterator_manager_type,public std::iterator<std::random_access_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		iterator_type operator++(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			increment();
			return ans;
		}
		basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator--(){
			decrement();
			return *this;
		}
		iterator_type operator--(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			decrement();
			return ans;
		}
		basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator+=(typename iterator_manager_type::difference_type _num){
			increment(_num);
			return *this;
		}
		iterator_type operator+(typename iterator_manager_type::difference_type _num)const{
			iterator_type ans(*this);
			ans.increment(_num);
			return ans;
		}
		basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator-=(typename iterator_manager_type::difference_type _num){
			decrement(_num);
			return *this;
		}
		iterator_type operator-(typename iterator_manager_type::difference_type _num)const{
			iterator_type ans(*this);
			ans.decrement(_num);
			return ans;
		}
		typename iterator_manager_type::difference_type operator-(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.difference(*this);}
		bool operator==(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return !equal(_itr);}
		bool operator<(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return less(_itr);}
		bool operator<=(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (less(_itr)||equal(_itr));}
		bool operator>(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.less(*this);}
		bool operator>=(const basic_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::reference operator[](typename iterator_manager_type::difference_type _num){return ref(_num);}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	//const_iterator
	template <class iterator_type,class iterator_manager_type,class iterator_tag=typename iterator_manager_type::iterator_tag>
	class basic_const_iterator{};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_iterator<iterator_type,iterator_manager_type,std::fixed_iterator_tag>:public iterator_manager_type,public std::iterator<std::fixed_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag>:public iterator_manager_type,public std::iterator<std::forward_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_const_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_const_iterator<iterator_type,iterator_manager_type,std::forward_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		bool operator==(const basic_const_iterator<iterator_type,iterator_manager_type>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_const_iterator<iterator_type,iterator_manager_type>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>:public iterator_manager_type,public std::iterator<std::bidirectional_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator--(){
			decrement();
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> operator--(typename iterator_manager_type::difference_type){
			basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> ans(*this);
			decrement();
			return ans;
		}
		bool operator==(const basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_const_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>:public iterator_manager_type,public std::iterator<std::random_access_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator++(){
			increment();
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator--(){
			decrement();
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator--(typename iterator_manager_type::difference_type){
			basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			decrement();
			return ans;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator+=(typename iterator_manager_type::difference_type _num){
			increment(_num);
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator+(typename iterator_manager_type::difference_type _num)const{
			basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.increment(_num);
			return ans;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator-=(typename iterator_manager_type::difference_type _num){
			decrement(_num);
			return *this;
		}
		basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator-(typename iterator_manager_type::difference_type _num)const{
			basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.decrement(_num);
			return ans;
		}
		typename iterator_manager_type::difference_type operator-(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.difference(*this);}
		bool operator==(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return !equal(_itr);}
		bool operator<(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return less(_itr);}
		bool operator<=(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (less(_itr)||equal(_itr));}
		bool operator>(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.less(*this);}
		bool operator>=(const basic_const_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_reference operator[](typename iterator_manager_type::difference_type _num){return ref(_num);}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};

	//reverse_iterator
	template <class iterator_type,class iterator_manager_type,class iterator_tag=typename iterator_manager_type::iterator_tag>
	class basic_reverse_iterator{};
	template <class iterator_type,class iterator_manager_type>
	class basic_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>:public iterator_manager_type,public std::iterator<std::bidirectional_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_reverse_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator++(){
			dencrement();
			return *this;
		}
		iterator_type operator++(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			dencrement();
			return ans;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator--(){
			increment();
			return *this;
		}
		iterator_type operator--(typename iterator_manager_type::difference_type){
			iterator_type ans(*this);
			increment();
			return ans;
		}
		bool operator==(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>:public iterator_manager_type,public std::iterator<std::random_access_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::pointer,typename iterator_manager_type::reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_reverse_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator++(){
			decrement();
			return *this;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			decrement();
			return ans;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator--(){
			increment();
			return *this;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator--(typename iterator_manager_type::difference_type){
			basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator+=(typename iterator_manager_type::difference_type _num){
			decrement(_num);
			return *this;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator+(typename iterator_manager_type::difference_type _num)const{
			basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.decrement(_num);
			return ans;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator-=(typename iterator_manager_type::difference_type _num){
			increment(_num);
			return *this;
		}
		basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator-(typename iterator_manager_type::difference_type _num)const{
			basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.increment(_num);
			return ans;
		}
		typename iterator_manager_type::difference_type operator-(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return -_itr.difference(*this);}
		bool operator==(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return !equal(_itr);}
		bool operator<(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.less(*this);}
		bool operator<=(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		bool operator>(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return less(_itr);}
		bool operator>=(const basic_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (less(_itr)||equal(_itr));}
		typename iterator_manager_type::reference operator*(){return ref();}
		typename iterator_manager_type::reference operator[](typename iterator_manager_type::difference_type _num){return ref(-_num);}
		typename iterator_manager_type::pointer operator->(){return ptr();}
	};
	//const_reverse_iterator
	template <class iterator_type,class iterator_manager_type,class iterator_tag=typename iterator_manager_type::iterator_tag>
	class basic_const_reverse_iterator{};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>:public iterator_manager_type,public std::iterator<std::bidirectional_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_reverse_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator++(){
			dencrement();
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> ans(*this);
			dencrement();
			return ans;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& operator--(){
			increment();
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> operator--(typename iterator_manager_type::difference_type){
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		bool operator==(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::bidirectional_iterator_tag>& _itr)const{return !equal(_itr);}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};
	template <class iterator_type,class iterator_manager_type>
	class basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>:public iterator_manager_type,public std::iterator<std::random_access_iterator_tag,typename iterator_manager_type::value_type,typename iterator_manager_type::difference_type,typename iterator_manager_type::const_pointer,typename iterator_manager_type::const_reference>{
	public:
		typedef typename iterator_manager_type::value_type value_type;
		typedef typename iterator_manager_type::difference_type difference_type;
		typedef typename iterator_manager_type::reference reference;
		typedef typename iterator_manager_type::pointer pointer;
		typedef typename iterator_manager_type::const_reference const_reference;
		typedef typename iterator_manager_type::const_pointer const_pointer;
	public:
		basic_const_reverse_iterator(const iterator_manager_type& _ItrType):iterator_manager_type(_ItrType){return;}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator++(){
			decrement();
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator++(typename iterator_manager_type::difference_type){
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			decrement();
			return ans;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator--(){
			increment();
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator--(typename iterator_manager_type::difference_type){
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			increment();
			return ans;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator+=(typename iterator_manager_type::difference_type _num){
			decrement(_num);
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator+(typename iterator_manager_type::difference_type _num)const{
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.decrement(_num);
			return ans;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& operator-=(typename iterator_manager_type::difference_type _num){
			increment(_num);
			return *this;
		}
		basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> operator-(typename iterator_manager_type::difference_type _num)const{
			basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag> ans(*this);
			ans.increment(_num);
			return ans;
		}
		typename iterator_manager_type::difference_type operator-(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return -_itr.difference(*this);}
		bool operator==(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return equal(_itr);}
		bool operator!=(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return !equal(_itr);}
		bool operator<(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return _itr.less(*this);}
		bool operator<=(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		bool operator>(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return less(_itr);}
		bool operator>=(const basic_const_reverse_iterator<iterator_type,iterator_manager_type,std::random_access_iterator_tag>& _itr)const{return (less(_itr)||equal(_itr));}
		typename iterator_manager_type::const_reference operator*(){return ref();}
		typename iterator_manager_type::const_reference operator[](typename iterator_manager_type::difference_type _num){return ref(-_num);}
		typename iterator_manager_type::const_pointer operator->(){return ptr();}
	};
};
#endif