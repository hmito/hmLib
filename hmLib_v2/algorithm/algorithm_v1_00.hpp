#ifndef INC_HMLIB_ALGORITHM
#define INC_HMLIB_ALGORITHM 100
#include <algorithm>
#ifndef INC_HMLIB_HMRAND
#	include "hmLib_v2/hmRand.hpp"
#endif
namespace hm{
	inline void showIteratorTag( std::input_iterator_tag p_cTag ){printf( "input_iterator_tag\n" );}
	inline void showIteratorTag( std::output_iterator_tag p_cTag ){printf( "output_iterator_tag\n" );}
	inline void showIteratorTag( std::bidirectional_iterator_tag p_cTag ){printf( "bidirectional_iterator_tag\n" );}
	inline void showIteratorTag( std::forward_iterator_tag p_cTag ){printf( "forward_iterator_tag\n" );}
	inline void showIteratorTag( std::random_access_iterator_tag p_cTag ){printf( "random_access_iterator_tag\n" );}
/*	template<class T>
	class pointer{
		T* t;
	protected:
		virtual void copy(const pointer<T>& _ptr){t=_ptr.t;}
		virtual bool equal(const pointer<T>& _ptr)const{return t==_ptr.t;}
		virtual bool less(const pointer<T>& _ptr)const{return t<_ptr.t;}
	public:
		pointer(){t=0;}
		pointer(const T* _t){t=_t;}
		pointer(const pointer<T> _ptr){copy(_ptr);}
		const pointer<T>& operator=(const T* _t){
			t=_t;
			return *this;
		}
		const pointer<T>& operator=(const pointer<T> _ptr){
			if(this!=&_ptr)copy(_ptr);
			return *this;
		}
		bool operator==(const pointer<T> _ptr)const{return equal(_ptr);}
		bool operator!=(const pointer<T> _ptr)const{return !equal(_ptr);}
		bool operator<(const pointer<T> _ptr)const{return less(_ptr);}
		bool operator<=(const pointer<T> _ptr)const{return (less(_ptr)||equal(_ptr));}
		bool operator>(const pointer<T> _ptr)const{return _ptr.less(*this);}
		bool operator>=(const pointer<T> _ptr)const{return (_ptr.less(*this)||_ptr.equal(*this));}
		pointer<T>& operator++(){
			++t;
			return *this;
		}
	};*/
	template<class Base,class Data>
	class basic_forward_iterator:public std::iterator<std::forward_iterator_tag,Data>{
	protected:
		Base B;
	protected:
		virtual void move_front(){++B;}
		virtual bool equal(const basic_forward_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		virtual Data& access()const{return *B;}
		virtual Data* accessPtr()const{return B;}
	public:
		virtual basic_forward_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		virtual basic_forward_iterator<Base,Data> operator++(int){
			basic_forward_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		virtual bool operator==(const basic_forward_iterator<Base,Data>& _itr)const{return equal(_itr);}
		virtual bool operator!=(const basic_forward_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		virtual Data& operator*()const{return access();}
		virtual Base operator->()const{return accessPtr();}
		Base getBase()const{return B;}
	};
	template<class Base,class Data>
	class basic_bidirectional_iterator:public std::iterator<std::bidirectional_iterator_tag,Data>{
	protected:
		Base B;
	protected:
		virtual void move_front(){++B;}
		virtual void move_back(){--B;}
		virtual bool equal(const basic_bidirectional_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		virtual Data& access()const{return *B;}
		virtual Data* accessPtr()const{return B;}
	public:
		virtual basic_bidirectional_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		virtual basic_bidirectional_iterator<Base,Data> operator++(int){
			basic_bidirectional_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		virtual basic_bidirectional_iterator<Base,Data>& operator--(){
			move_back();
			return *this;
		}
		virtual basic_bidirectional_iterator<Base,Data> operator--(int){
			basic_bidirectional_iterator<Base,Data> ans(*this);
			move_back();
			return ans;
		}
		virtual bool operator==(const basic_bidirectional_iterator<Base,Data>& _itr)const{return equal(_itr);}
		virtual bool operator!=(const basic_bidirectional_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		virtual Data& operator*()const{return access();}
		virtual Base operator->()const{return accessPtr();}
		Base getBase()const{return B;}
	};
	template<class Base,class Data>
	class basic_random_access_iterator:public std::iterator<std::random_access_iterator_tag,Data>{
	protected:
		Base B;
	protected:
		virtual void move_front(){++B;}
		virtual void move_back(){--B;}
		virtual void move_front(int _num){B+=_num;}
		virtual void move_back(int _num){B-=_num;}
		virtual bool equal(const basic_random_access_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		virtual bool less(const basic_random_access_iterator<Base,Data>& _itr)const{return B<_itr.getBase();}
		virtual Data& access()const{return *B;}
		virtual Data& access(int _num)const{return *(B+_num);}
		virtual Data* accessPtr()const{return B;}
	public:
		virtual basic_random_access_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		virtual basic_random_access_iterator<Base,Data> operator++(int){
			basic_random_access_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		virtual basic_random_access_iterator<Base,Data>& operator--(){
			move_back();
			return *this;
		}
		virtual basic_random_access_iterator<Base,Data> operator--(int){
			basic_random_access_iterator<Base,Data> ans(*this);
			move_back();
			return ans;
		}
		virtual basic_random_access_iterator<Base,Data>& operator+=(int _num){
			move_front(_num);
			return *this;
		}
		virtual basic_random_access_iterator<Base,Data> operator+(int _num){
			basic_random_access_iterator<Base,Data> ans(*this);
			ans.move_front(_num);
			return ans;
		}
		virtual basic_random_access_iterator<Base,Data>& operator-=(int _num){
			move_back(_num);
			return *this;
		}
		virtual basic_random_access_iterator<Base,Data> operator-(int _num){
			basic_random_access_iterator<Base,Data> ans(*this);
			ans.move_back(_num);
			return ans;
		}
		virtual bool operator==(const basic_random_access_iterator<Base,Data>& _itr)const{return equal(_itr);}
		virtual bool operator!=(const basic_random_access_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		virtual bool operator<(const basic_random_access_iterator<Base,Data>& _itr)const{return less(_itr);}
		virtual bool operator<=(const basic_random_access_iterator<Base,Data>& _itr)const{return (less(_itr)||equal(_itr));}
		virtual bool operator>(const basic_random_access_iterator<Base,Data>& _itr)const{return _itr.less(*this);}
		virtual bool operator>=(const basic_random_access_iterator<Base,Data>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		virtual Data& operator*()const{return access();}
		virtual Data& operator[](int _num)const{return access(_num);}
		virtual Base operator->()const{return accessPtr();}
		Base getBase()const{return B;}
	};
}
namespace hmstd{
	template<class Iterator>
	class self_iterator:public Iterator{
	public:
		self_iterator():Iterator(){return;}
		self_iterator(Iterator _itr):Iterator(_itr){return;}
		Iterator& operator*(){return *this;}
		Iterator* operator->(){return this;}
	};
	template<class T>
	class const_value_iterator:public hm::basic_random_access_iterator<const T*,const T>{
	private:
		T Val;
	protected:
		virtual void move_front(){return;}
		virtual void move_back(){return;}
		virtual void move_front(int _num){return;}
		virtual void move_back(int _num){return;}
		virtual bool equal(const basic_random_access_iterator<T,T>& _itr)const{return true;}
		virtual bool less(const basic_random_access_iterator<T,T>& _itr)const{return false;}
		virtual const T& access()const{return Val;}
		virtual const T& access(int _num)const{return Val;}
		virtual const T* accessPtr()const{return &Val;}
	public:
		const_value_iterator():hm::basic_random_access_iterator<const T*,const T>(){Val=T();}
		const_value_iterator(const T& _t):hm::basic_random_access_iterator<const T*,const T>(){Val=_t;}
		const_value_iterator(const const_value_iterator<T>& _itr){Val=_itr.Val;}
	};
	template<class T>
	class linear_value_iterator:public hm::basic_random_access_iterator<T,T>{
	private:
		T Slope;
	protected:
		virtual void move_front(){B+=Slope;}
		virtual void move_back(){B-=Slope;}
		virtual void move_front(int _num){B+=Slope*_num;}
		virtual void move_back(int _num){B-=Slope*_num;}
		virtual bool equal(const basic_random_access_iterator<T,T>& _itr)const{return B==_itr.getBase();}
		virtual bool less(const basic_random_access_iterator<T,T>& _itr)const{return B<_itr.getBase();}
		virtual T& access()const{return B;}
		virtual T& access(int _num)const{return B+Slope*_num;}
		virtual T* accessPtr()const{return &B;}
	public:
		linear_value_iterator(){B=T();}
		linear_value_iterator(const T& _Base,const T& _Slope){
			B=_Base;
			Slope=_Slope;
		}
	};
	template<class InputIterator,class Accesser>
	InputIterator roulette(InputIterator itr,InputIterator enditr,Accesser weight_acs,hmRand& Rand=hm::getGlobalRand()){
		InputIterator1 ditr=itr;
		double tmp=0;
		for(;ditr!=enditr;++ditr)tmp+=weight_acs(itr);
		tmp=hm::randf_eu(0.,tmp);
		for(;itr!=enditr;++itr){
			tmp-=weight_acs(itr);
			if(tmp<0)return itr;
		}
		return enditr;
	}
}
#endif
