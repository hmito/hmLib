#ifndef INC_HMLIB_ALGORITHM
#define INC_HMLIB_ALGORITHM 102
#include <algorithm>
#ifndef INC_HMLIB_BASICITERATOR
#	include "hmLib_v2/basic_iterator.hpp"
#endif
#ifndef INC_HMLIB_HMRAND
#	include "hmLib_v2/hmRand.hpp"
#endif

namespace hm{
	template<class Base,class Data>
	class forward_iterator:public std::iterator<std::forward_iterator_tag,Data>{
	protected:
		Base B;
	protected:
		 virtual void move_front(){++B;}
		 virtual bool equal(const forward_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		 virtual Data& access()const{return *B;}
		 virtual Data* accessPtr()const{return B;}
	public:
		forward_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		forward_iterator<Base,Data> operator++(int){
			forward_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		bool operator==(const forward_iterator<Base,Data>& _itr)const{return equal(_itr);}
		bool operator!=(const forward_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		Data& operator*()const{return access();}
		Data* operator->()const{return accessPtr();}
		Base getBase()const{return B;}
	};
	template<class Base,class Data>
	class bidirectional_iterator:public std::iterator<std::bidirectional_iterator_tag,Data>{
	protected:
		Base B;
	protected:
		 virtual void move_front(){++B;}
		 virtual void move_back(){--B;}
		 virtual bool equal(const bidirectional_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		 virtual Data& access()const{return *B;}
		 virtual Data* accessPtr()const{return B;}
	public:
		 bidirectional_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		 bidirectional_iterator<Base,Data> operator++(int){
			bidirectional_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		 bidirectional_iterator<Base,Data>& operator--(){
			move_back();
			return *this;
		}
		 bidirectional_iterator<Base,Data> operator--(int){
			bidirectional_iterator<Base,Data> ans(*this);
			move_back();
			return ans;
		}
		 bool operator==(const bidirectional_iterator<Base,Data>& _itr)const{return equal(_itr);}
		 bool operator!=(const bidirectional_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		 Data& operator*()const{return access();}
		Data* operator->()const{return accessPtr();}
		Base getBase()const{return B;}
	};
	template<class Base,class Data>
	class random_access_iterator:public std::iterator<std::random_access_iterator_tag,Data>{
	public:
		typedef typename Data value_type;
		typedef typename Data& Reference;
		typedef typename Data* Pointer;
	protected:
		Base B;
	protected:
		 virtual void move_front(){++B;}
		 virtual void move_back(){--B;}
		 virtual void move_front(int _num){B+=_num;}
		 virtual void move_back(int _num){B-=_num;}
		 virtual bool equal(const random_access_iterator<Base,Data>& _itr)const{return B==_itr.B;}
		 virtual bool less(const random_access_iterator<Base,Data>& _itr)const{return B<_itr.getBase();}
		 virtual int distance(const random_access_iterator<Base,Data>& _itr)const{return B-_itr.B;}
		 virtual Data& access(){return *B;}
		 virtual Data& access(int _num){return *(B+_num);}
		 virtual Data* accessPtr(){return B;}
	public:
		 random_access_iterator<Base,Data>& operator++(){
			move_front();
			return *this;
		}
		 random_access_iterator<Base,Data> operator++(int){
			random_access_iterator<Base,Data> ans(*this);
			move_front();
			return ans;
		}
		 random_access_iterator<Base,Data>& operator--(){
			move_back();
			return *this;
		}
		 random_access_iterator<Base,Data> operator--(int){
			random_access_iterator<Base,Data> ans(*this);
			move_back();
			return ans;
		}
		 random_access_iterator<Base,Data>& operator+=(int _num){
			move_front(_num);
			return *this;
		}
		 random_access_iterator<Base,Data> operator+(int _num){
			random_access_iterator<Base,Data> ans(*this);
			ans.move_front(_num);
			return ans;
		}
		 random_access_iterator<Base,Data>& operator-=(int _num){
			move_back(_num);
			return *this;
		}
		 random_access_iterator<Base,Data> operator-(int _num){
			random_access_iterator<Base,Data> ans(*this);
			ans.move_back(_num);
			return ans;
		}
		 int operator-(const random_access_iterator<Base,Data>& _itr)const{return distance(_itr);}
		 bool operator==(const random_access_iterator<Base,Data>& _itr)const{return equal(_itr);}
		 bool operator!=(const random_access_iterator<Base,Data>& _itr)const{return !equal(_itr);}
		 bool operator<(const random_access_iterator<Base,Data>& _itr)const{return less(_itr);}
		 bool operator<=(const random_access_iterator<Base,Data>& _itr)const{return (less(_itr)||equal(_itr));}
		 bool operator>(const random_access_iterator<Base,Data>& _itr)const{return _itr.less(*this);}
		 bool operator>=(const random_access_iterator<Base,Data>& _itr)const{return (_itr.less(*this)||_itr.equal(*this));}
		 Reference operator*(){return access();}
		 Reference operator[](int _num){return access(_num);}
		 Pointer operator->(){return accessPtr();}
		const Base& getBase()const{return B;}
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
	template<class Iterator,class Accessor,class IteratorTag=std::iterator_traits<Iterator>::iterator_category(Iterator)>
	class acs_iterator:public Iterator{
	private:
		Accessor acs;
	public:
		typedef typename Iterator::value_type value_type;
		typedef typename Accessor::value_type acs_value_type;
		acs_iterator(Iterator _itr,Accessor _acs=Accessor()):Iterator(_itr),acs(_acs){return;}
		acs_value_type& operator*(){return acs(*static_cast<Iterator>(*this));}
		acs_value_type* operator->(){return &acs(*static_cast<Iterator>(*this));}
	};
	template<class Iterator,class Accessor>
	class acs_iterator<Iterator,Accessor,std::random_access_iterator_tag>:public Iterator{
	private:
		Accessor acs;
	public:
		typedef typename Iterator::value_type value_type;
		typedef typename Accessor::value_type acs_value_type;
		acs_iterator(Iterator _itr,Accessor _acs=Accessor()):Iterator(_itr),acs(_acs){return;}
		acs_value_type& operator*(){return acs(*static_cast<std::random_access_iterator_tag>(*this));}
		acs_value_type& operator[](int _num){return acs(static_cast<std::random_access_iterator_tag>(*this)[_num]);}
		acs_value_type* operator->(){return &acs(*static_cast<std::random_access_iterator_tag>(*this));}
	};
}
#endif
